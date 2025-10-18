#pragma once

#include "body.h"
#include "base/quadtree.h"

#include <vector>
#include <memory>

namespace physics {

// Quadtree Node for Barnes-Hut algorithm
class BHQuadtreeNode : public QuadtreeNode<Body, BHQuadtreeNode> {

public:
    using QuadtreeNode::QuadtreeNode;
    
    const AABB& getBoundary() const { return m_boundary; }
    bool isDivided() const { return m_divided; }
    bool insert(std::shared_ptr<Body> body);
    bool hasBodyData() const { return !!m_data; }
    const std::shared_ptr<Body> getBody() const { return std::static_pointer_cast<Body>(m_data); }
    float getTotalMass() const { return m_totalMass; }
    void getCenterOfMass(float& x, float& y) const { x = m_centerOfMass.x; y = m_centerOfMass.y; }
    void computeForce(Body& target) const;

private:
    virtual bool insert(const glm::vec2& point, std::shared_ptr<Body> data) override;
    void updateMassProperties(const Body& newBody);
    void addDirectForce(Body& target, const Body& source) const;
    void addApproximateForce(Body& target, const glm::vec2& comPos, float mass, float distance) const;

    float m_totalMass {0.0};
    glm::vec2 m_centerOfMass {0.0, 0.0};
};

class NBodySimulation {

public:
    explicit NBodySimulation(glm::vec2 visualArea);
    NBodySimulation(NBodySimulation&& other) noexcept;
    NBodySimulation(const NBodySimulation&) = delete;
    NBodySimulation& operator=(const NBodySimulation&) = delete;
    ~NBodySimulation() = default;

    void setBodies(std::vector<std::shared_ptr<Body>>& bodies);
    void addBodie(std::shared_ptr<Body> body);
    void step(float dt);

private:
    void rebuildTree();

    std::unique_ptr<BHQuadtreeNode> m_root;
    std::vector<std::shared_ptr<Body>> m_bodies;
};

}
