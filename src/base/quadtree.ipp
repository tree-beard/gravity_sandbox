#pragma once

template<typename T, typename Derived>
QuadtreeNode<T, Derived>::QuadtreeNode(const AABB& boundary) : m_boundary(boundary), m_divided(false) {}

template<typename T, typename Derived>
bool QuadtreeNode<T, Derived>::insert(const glm::vec2& point, std::shared_ptr<T> data) {
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

template<typename T, typename Derived>
bool QuadtreeNode<T, Derived>::insertToChild(const glm::vec2& point, std::shared_ptr<T> data) {
    if (m_nw->insert(point, data)) return true;
    if (m_ne->insert(point, data)) return true;
    if (m_sw->insert(point, data)) return true;
    if (m_se->insert(point, data)) return true;
    return false;
}

template<typename T, typename Derived>
bool QuadtreeNode<T, Derived>::remove(const glm::vec2& point) {
    if (!m_boundary.containsPoint(point)) {
        return false;
    }

    // Try to remove from this node's point
    if (m_data && m_point == point) {
        m_point = {};
        m_data = nullptr;
        return true;
    }

    // If no children, point not found
    if (!m_divided) {
        return false;
    }

    // Search in children
    return (m_nw->remove(point) || m_ne->remove(point) || m_sw->remove(point) || m_se->remove(point));
}

template<typename T, typename Derived>
void QuadtreeNode<T, Derived>::subdivide() {
    auto childHalfDim = m_boundary.halfDimension / 2.0f;

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

template<typename T, typename Derived>
std::unique_ptr<QuadtreeNode<T, Derived>> QuadtreeNode<T, Derived>::createChildNode(const AABB& boundary) {
    return std::make_unique<Derived>(boundary);
}

template<typename T, typename Derived>
std::vector<std::shared_ptr<T>> QuadtreeNode<T, Derived>::queryRange(const AABB& range) const {
    std::vector<std::shared_ptr<T>> found;
    queryRange(range, found);
    return found;
}

template<typename T, typename Derived>
void QuadtreeNode<T, Derived>::queryRange(const AABB& range, std::vector<std::shared_ptr<T>>& found) const {
    // This node doesn't intersect the query range
    if (!m_boundary.intersects(range)) {
        return;
    }

    // Check point in this node
    if (m_data && range.containsPoint(m_point)) {
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
