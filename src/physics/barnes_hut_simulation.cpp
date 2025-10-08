#include "barnes_hut_simulation.h"

namespace physics {

    const float G_CONST = 1.0f;//6.67430e-11;        // Gravitational constant
    const float THETA = 1.0f;            // Barnes-Hut opening angle
    const float SOFT_FACTOR = 0.5f;   // Softening parameter to avoid singularities
    const float AREA_PADDING = 10.0f;

    // BHQuadtreeNode
    //--------------------------------------------------------------------------------------
    BHQuadtreeNode::BHQuadtreeNode(const AABB& boundary) 
        : QuadtreeNode(boundary) {

    }

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
        } else {
            float newTotalMass = m_totalMass + newBody.mass();
            m_centerOfMass = ((m_centerOfMass * m_totalMass) + newBody.position() * newBody.mass()) / newTotalMass;
            m_totalMass = newTotalMass;
        }
    }

    void BHQuadtreeNode::computeForce(Body& target, float theta, float G) const {
        if (!m_divided) {
            if (!m_data) {
                return;
            }
            // Leaf node with single body (avoid self-interaction)
            if (m_data && *m_data == target) {
                return;
            }
            addDirectForce(target, *m_data, G);
            return;
        }
        
        const auto r = m_centerOfMass - target.position();
        const auto distance = std::sqrt(r.x * r.x + r.y * r.y);
        
        // Barnes-Hut criterion: s/d < theta
        if (m_boundary.getWidth() / distance < theta) {
            // Treat cell as a single mass
            if (m_totalMass > 0) {
                addApproximateForce(target, m_centerOfMass, m_totalMass, distance, G);
            }
        } else {
            // Recursively process children
            static_cast<BHQuadtreeNode*>(m_nw.get())->computeForce(target, theta, G);
            static_cast<BHQuadtreeNode*>(m_ne.get())->computeForce(target, theta, G);
            static_cast<BHQuadtreeNode*>(m_sw.get())->computeForce(target, theta, G);
            static_cast<BHQuadtreeNode*>(m_se.get())->computeForce(target, theta, G);
        }
    }

    void BHQuadtreeNode::addDirectForce(Body& target, const Body& source, float G) const {
        const auto r = source.position() - target.position();
        const auto distanceSq = r.x * r.x + r.y * r.y;
        
        // Avoid division by zero and self-interaction
        if (distanceSq == 0) return;
        
        const float distance = std::sqrt(distanceSq + SOFT_FACTOR);
        const auto forceMag = G * target.mass() * source.mass() / distanceSq;
        const auto forceVec = forceMag * r / distance;
        target.addForce(forceVec);
    }

    void BHQuadtreeNode::addApproximateForce(Body& target, const glm::vec2& comPos, float mass, float distance, float G) const {
        const auto r = comPos - target.position();
        const auto forceMag = G * target.mass() * mass / (distance * distance);
        const auto forceVec = forceMag * r / distance;
        target.addForce(forceVec);
    }



    // BarnesHutSimulation
    //--------------------------------------------------------------------------------------
    BarnesHutSimulation::BarnesHutSimulation(glm::vec2 visualArea)
        : m_root(std::make_unique<BHQuadtreeNode>(
            AABB(glm::vec2(visualArea.x / 2, visualArea.y / 2), std::max(visualArea.x / 2, visualArea.y / 2) + AREA_PADDING))
            ) {

    }

    void BarnesHutSimulation::setBodies(std::vector<std::shared_ptr<Body>>& bodies) {
        m_bodies = bodies;
    }

    void BarnesHutSimulation::addBodie(std::shared_ptr<Body> body) {
        m_bodies.push_back(body);
    }
    
    // Perform one simulation step
    void BarnesHutSimulation::step(float dt) {
        // Compute forces using Barnes-Hut algorithm
        for (auto& body : m_bodies) {
            m_root->computeForce(*body, THETA, G_CONST);
        }
        
        // Update positions and velocities using Leapfrog integration
        for (auto bodyIter = m_bodies.begin(); bodyIter != m_bodies.end(); /* no increment here */) {

            // Update velocity and position
            (*bodyIter)->update(dt);
            
            // Check boundaries (simple reflection)
            const auto bottomRight = m_root->getBoundary().center + m_root->getBoundary().halfDimension;
            const auto topLeft = m_root->getBoundary().center - m_root->getBoundary().halfDimension;
            if ((*bodyIter)->position().x > bottomRight.x || (*bodyIter)->position().y > bottomRight.y
                || (*bodyIter)->position().x < topLeft.x|| (*bodyIter)->position().y < topLeft.y) {
                bodyIter = m_bodies.erase(bodyIter);
            }
            else {
                ++bodyIter;
            }
        }
        
        // Rebuild tree with new positions
        rebuildTree();
    }

            // Rebuild the quadtree with current bodies
    void BarnesHutSimulation::rebuildTree() {
        const auto halfDimension = m_root->getBoundary().halfDimension;
        const auto center =m_root->getBoundary().center;
        m_root = std::make_unique<BHQuadtreeNode>(AABB(center, halfDimension));
        
        for (auto& body : m_bodies) {
            m_root->insert(body);
        }
    }
}
