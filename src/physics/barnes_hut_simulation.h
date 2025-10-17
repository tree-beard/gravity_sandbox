#pragma once

#include "body.h"
#include "base/quadtree.h"

#include <vector>
#include <memory>

namespace physics {

// Quadtree Node for Barnes-Hut algorithm
class BHQuadtreeNode : public QuadtreeNode<Body, BHQuadtreeNode> {

public:
    explicit BHQuadtreeNode(const AABB& boundary);
    
    const AABB& getBoundary() const { return m_boundary; }
    bool isDivided() const { return m_divided; }
    bool insert(std::shared_ptr<Body> body);
    bool hasBodyData() const { return !!m_data; }
    const std::shared_ptr<Body> getBody() const { return std::static_pointer_cast<Body>(m_data); }
    float getTotalMass() const { return m_totalMass; }
    void getCenterOfMass(float& x, float& y) const { x = m_centerOfMass.x; y = m_centerOfMass.y; }
    // Barnes-Hut force calculation
    void computeForce(Body& target, float theta, float G) const;

private:
    // Mass properties for Barnes-Hut approximation
    float m_totalMass {0.0};
    glm::vec2 m_centerOfMass {0.0, 0.0};

    // Insert a body and update mass properties
    virtual bool insert(const glm::vec2& point, std::shared_ptr<Body> data) override;

    // Update mass properties when inserting a body
    void updateMassProperties(const Body& newBody);

    // Add direct force between two bodies
    void addDirectForce(Body& target, const Body& source, float G) const;
    
    // Add approximate force from a cell
    void addApproximateForce(Body& target, const glm::vec2& comPos, float mass, 
                           float distance, float G) const;
};

class BarnesHutSimulation {

public:
    explicit BarnesHutSimulation(glm::vec2 visualArea);
    BarnesHutSimulation(BarnesHutSimulation&& other) noexcept;
    BarnesHutSimulation(const BarnesHutSimulation&) = delete;
    BarnesHutSimulation& operator=(const BarnesHutSimulation&) = delete;
    ~BarnesHutSimulation() = default;

    void setBodies(std::vector<std::shared_ptr<Body>>& bodies);
    void addBodie(std::shared_ptr<Body> body);
    void step(float dt);

private:
    void rebuildTree();

    std::unique_ptr<BHQuadtreeNode> m_root;
    std::vector<std::shared_ptr<Body>> m_bodies;
    
};

}
