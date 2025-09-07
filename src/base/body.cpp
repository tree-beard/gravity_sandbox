#include "body.h"
#include <random>

namespace physics {

Body::Body(glm::vec2 _pos, glm::vec2 _vel, float _mass) 
        : m_position(_pos)
        , m_velocity(_vel)
        , m_acceleration({})
        , m_mass(_mass) {

}

glm::vec2 Body::position() const {
    std::shared_lock<std::shared_timed_mutex> lock(m_mutex);
    return m_position;
}

glm::vec2 Body::force() const { 
    std::shared_lock<std::shared_timed_mutex> lock(m_mutex);
    return m_force;
}

void Body::addForce(const glm::vec2& _force) {
    std::lock_guard<std::shared_timed_mutex> lock(m_mutex);
    m_force += _force;
}

float Body::mass() const {
   return m_mass;
}

void Body::update(float dt) {
    std::lock_guard<std::shared_timed_mutex> lock(m_mutex);
    m_acceleration = m_force / m_mass;
    m_position += m_velocity * dt;
    m_velocity += m_acceleration * dt;
    m_force = {0.0f, 0.0f};
    m_acceleration = {0.0f, 0.0f};
}

}
