#pragma once

#include <glm/vec2.hpp>
#include <vector>
#include <memory>
#include <algorithm>

// Axis-Aligned Bounding Box
struct AABB {
    glm::vec2 center;
    float halfDimension;
    
    AABB(glm::vec2 center, float halfDimension) 
        : center(center), halfDimension(halfDimension) {}
    
    bool containsPoint(const glm::vec2& point) const {
        return (point.x >= center.x - halfDimension &&
                point.x <= center.x + halfDimension &&
                point.y >= center.y - halfDimension &&
                point.y <= center.y + halfDimension);
    }
    
    bool intersects(const AABB& other) const {
        return !(other.center.x - other.halfDimension > center.x + halfDimension ||
                 other.center.x + other.halfDimension < center.x - halfDimension ||
                 other.center.y - other.halfDimension > center.y + halfDimension ||
                 other.center.y + other.halfDimension < center.y - halfDimension);
    }

    float getWidth() const { return halfDimension * 2; }
    float getMinX() const { return center.x - halfDimension; }
    float getMaxX() const { return center.x + halfDimension; }
    float getMinY() const { return center.y - halfDimension; }
    float getMaxY() const { return center.y + halfDimension; }
};

// Quadtree Node
template<typename T, typename Derived>
class QuadtreeNode {
public:
    QuadtreeNode(const AABB& boundary);

    virtual bool insert(const glm::vec2& point, std::shared_ptr<T> data);
    virtual bool insertToChild(const glm::vec2& point, std::shared_ptr<T> data);
    virtual bool remove(const glm::vec2& point);

    std::unique_ptr<QuadtreeNode> createChildNode(const AABB& boundary);
    std::vector<std::shared_ptr<T>> queryRange(const AABB& range) const;
    void queryRange(const AABB& range, std::vector<std::shared_ptr<T>>& found) const;
    void subdivide();

protected:
    AABB m_boundary;
    glm::vec2 m_point;
    std::shared_ptr<T> m_data;
    bool m_divided;

    // Children: NW, NE, SW, SE
    std::unique_ptr<QuadtreeNode> m_nw, m_ne, m_sw, m_se;
};

#include "quadtree.ipp"
