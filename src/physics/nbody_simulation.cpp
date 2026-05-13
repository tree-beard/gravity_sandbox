#include "nbody_simulation.h"
#include <future>

namespace physics {

    const float G = 1.0f;//6.67430e-11;         // Gravitational constant
    const float THETA = 1.0f;                   // Barnes-Hut opening angle
    const float SOFT_FACTOR = 0.5f;             // Softening parameter to avoid singularities
    const float AREA_PADDING = 10.0f;           // Compute area internal padding

    // BHQuadtreeNode
    //--------------------------------------------------------------------------------------

    bool BHQuadtreeNode::insert(std::shared_ptr<Body> body) {
        return insert(body->position(), body);
    }

    bool BHQuadtreeNode::insert(const glm::vec2& point, std::shared_ptr<Body> data) {
        if (QuadtreeNode::insert(point, data)) {
            updateMassProperties(*data);
            return true;
        }
        return false;
    }

    void BHQuadtreeNode::updateMassProperties(const Body& newBody) {
        if (m_totalMass == 0) {
            m_totalMass = newBody.mass();
            m_centerOfMass = newBody.position();
        } 
        else {
            float newTotalMass = m_totalMass + newBody.mass();
            m_centerOfMass = ((m_centerOfMass * m_totalMass) + newBody.position() * newBody.mass()) / newTotalMass;
            m_totalMass = newTotalMass;
        }
    }

    void BHQuadtreeNode::computeForce(Body& target) const {
        if (!m_divided) {
            if (!m_data) {
                return;
            }
            // Leaf node with single body (avoid self-interaction)
            if (m_data && *m_data == target) {
                return;
            }
            addDirectForce(target, *m_data);
            return;
        }
        
        const auto r = m_centerOfMass - target.position();
        const auto distance = std::sqrt(r.x * r.x + r.y * r.y);
        
        // Barnes-Hut criterion: s/d < theta
        if (m_boundary.getWidth() / distance < THETA) {
            // Treat cell as a single mass
            if (m_totalMass > 0) {
                addApproximateForce(target, m_centerOfMass, m_totalMass, distance);
            }
        } 
        else {
            // Recursively process children
            static_cast<BHQuadtreeNode*>(m_nw.get())->computeForce(target);
            static_cast<BHQuadtreeNode*>(m_ne.get())->computeForce(target);
            static_cast<BHQuadtreeNode*>(m_sw.get())->computeForce(target);
            static_cast<BHQuadtreeNode*>(m_se.get())->computeForce(target);
        }
    }

    void BHQuadtreeNode::addDirectForce(Body& target, const Body& source) const {
        const auto r = source.position() - target.position();
        const auto distanceSq = r.x * r.x + r.y * r.y;
        
        // Avoid division by zero and self-interaction
        if (distanceSq == 0) return;
        
        const float distance = std::sqrt(distanceSq + SOFT_FACTOR);
        const auto forceMag = G * target.mass() * source.mass() / distanceSq;
        const auto forceVec = forceMag * r / distance;
        target.addForce(forceVec);
    }

    void BHQuadtreeNode::addApproximateForce(Body& target, const glm::vec2& comPos, float mass, float distance) const {
        const auto r = comPos - target.position();
        const auto forceMag = G * target.mass() * mass / (distance * distance);
        const auto forceVec = forceMag * r / distance;
        target.addForce(forceVec);
    }



    // BarnesHutSimulation
    //--------------------------------------------------------------------------------------
    NBodySimulation::NBodySimulation(glm::vec2 visualArea)
        : m_root(std::make_unique<BHQuadtreeNode>(
            AABB(glm::vec2(visualArea.x / 2, visualArea.y / 2), std::max(visualArea.x / 2, visualArea.y / 2) + AREA_PADDING))
            ) {

    }

    void NBodySimulation::setBodies(std::vector<std::shared_ptr<Body>>& bodies) {
        m_bodies = bodies;
    }

    void NBodySimulation::addBodie(std::shared_ptr<Body> body) {
        m_bodies.push_back(body);
    }
    
    // Perform one simulation step
    void NBodySimulation::step(float dt) {
        computeForces();
        updatePositions(dt);
        rebuildTree();
    }

    void NBodySimulation::computeForces() {
        auto chunks = getChunks(m_bodies, std::thread::hardware_concurrency(), 100);
        std::vector<std::future<void>> tasks;
        for (auto& chunk : chunks) {
             tasks.emplace_back(std::async([this, &chunk]() ->void {
                    for(auto it = chunk.first; it < chunk.second; ++it) {
                        m_root->computeForce(**it);
                    }
                })
            );
        }

        // Wait for all async tasks to complete
        for (auto& task : tasks) {
            task.get();
        }
    }

    void NBodySimulation::updatePositions(float dt) {
        for (auto bodyIter = m_bodies.begin(); bodyIter != m_bodies.end(); /* no increment here */) {
            // Update velocity and position
            (*bodyIter)->update(dt);
            
            // Check boundaries (simple reflection)
            if (!m_root->getBoundary().containsPoint((*bodyIter)->position())) {
                (*bodyIter)->setActive(false);
                bodyIter = m_bodies.erase(bodyIter);
            }
            else {
                ++bodyIter;
            }
        }
    }

    void NBodySimulation::rebuildTree() {
        const auto halfDimension = m_root->getBoundary().halfDimension;
        const auto center =m_root->getBoundary().center;
        m_root = std::make_unique<BHQuadtreeNode>(AABB(center, halfDimension));
        
        for (auto& body : m_bodies) {
            m_root->insert(body);
        }
    }

    auto NBodySimulation::getChunks(std::vector<std::shared_ptr<Body>>& vec
                                                                    , size_t numCores
                                                                    , size_t minChunkSize) -> std::vector<Chunk> const {
        std::vector<Chunk> chunks;
        if (vec.empty() || numCores == 0 || minChunkSize == 0) {
            return chunks;
        }

        const auto totalSize = vec.size();
        auto chunkSize = std::max(minChunkSize, (totalSize + numCores - 1) / numCores);
        chunkSize = std::max(chunkSize, minChunkSize);
        const auto numChunks = (totalSize + chunkSize - 1) / chunkSize;

        auto start = vec.begin();
        for (size_t i = 0; i < numChunks; ++i) {
            auto end = start;
            auto remaining = totalSize - (start - vec.begin());
            auto currentChunkSize = std::min(chunkSize, remaining);

            if (currentChunkSize < minChunkSize && i < numChunks - 1) {
                currentChunkSize = minChunkSize;
            }

            std::advance(end, currentChunkSize);
            chunks.emplace_back(start, end);
            start = end;

            if (start == vec.end()) break;
        }
        return chunks;
    }
}
