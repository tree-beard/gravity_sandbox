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
template<typename T>
class QuadtreeNode {
public:
    
    QuadtreeNode(const AABB& boundary) 
        : m_boundary(boundary), m_divided(false) {}
        
    AABB m_boundary;
    glm::vec2 m_point;
    std::shared_ptr<T> m_data;
    bool m_divided;
    
    // Children: NW, NE, SW, SE
    std::unique_ptr<QuadtreeNode<T>> m_nw, m_ne, m_sw, m_se;

    virtual std::unique_ptr<QuadtreeNode<T>> createChildNode(const AABB& boundary) {
        return std::make_unique<QuadtreeNode<T>>(boundary);
    }
    
    virtual bool insert(const glm::vec2& point, std::shared_ptr<T> data) {
        // Point is not in this node's boundary
        if (!m_boundary.containsPoint(point)) {
            return false;
        }
        
        // If there's space and not divided, add the point
        if (!m_data && !m_divided) {
            m_point = point;
            m_data = data;
            return true;
        }
        
        // Need to subdivide if not already divided
        if (!m_divided) {
            subdivide();

            // Redistribute existing point to children
            if (m_data) {
                insert(m_point, m_data);
                m_point = {};
                m_data = nullptr;
            }
        }

        // Insert the new point into the appropriate child
        return insertToChild(point, data);
    }

        // Insert point to appropriate child
    virtual bool insertToChild(const glm::vec2& point, std::shared_ptr<T> data) {
        if (m_nw->insert(point, data)) return true;
        if (m_ne->insert(point, data)) return true;
        if (m_sw->insert(point, data)) return true;
        if (m_se->insert(point, data)) return true;
        return false;
    }
    
    virtual bool remove(const glm::vec2& point) {
        if (!m_boundary.containsPoint(point)) {
            return false;
        }
        
        // Try to remove from this node's point
        if (m_point == point) {
            m_point = {};
            m_data = nullptr;
            return true;
        }
        
        // If no children, point not found
        if (!m_divided) {
            return false;
        }
        
        // Search in children
        return (m_nw->remove(point) || m_ne->remove(point) ||
                m_sw->remove(point) || m_se->remove(point));
    }

    void subdivide() {
        auto childHalfDim = m_boundary.halfDimension / 2.0;
        
        glm::vec2 nwCenter(m_boundary.center.x - childHalfDim, m_boundary.center.y - childHalfDim);
        glm::vec2 neCenter(m_boundary.center.x + childHalfDim, m_boundary.center.y - childHalfDim);
        glm::vec2 swCenter(m_boundary.center.x - childHalfDim, m_boundary.center.y + childHalfDim);
        glm::vec2 seCenter(m_boundary.center.x + childHalfDim, m_boundary.center.y + childHalfDim);
        
        m_nw = createChildNode(AABB(nwCenter, childHalfDim));
        m_ne = createChildNode(AABB(neCenter, childHalfDim));
        m_sw = createChildNode(AABB(swCenter, childHalfDim));
        m_se = createChildNode(AABB(seCenter, childHalfDim));
        
        m_divided = true;
    }

    std::vector<std::shared_ptr<T>> queryRange(const AABB& range) const {
        std::vector<std::shared_ptr<T>> found;
        queryRange(range, found);
        return found;
    }
    
    void queryRange(const AABB& range, std::vector<std::shared_ptr<T>>& found) const {
        // This node doesn't intersect the query range
        if (!m_boundary.intersects(range)) {
            return;
        }
        
        // Check point in this node
        if (range.containsPoint(m_point)) {
                found.push_back(m_data);
        }
        
        // No children, return
        if (!m_divided) {
            return;
        }
        
        // Check children
        m_nw->queryRange(range, found);
        m_ne->queryRange(range, found);
        m_sw->queryRange(range, found);
        m_se->queryRange(range, found);
    }
};