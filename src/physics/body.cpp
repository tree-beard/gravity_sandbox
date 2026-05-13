#include "body.h"
#include <random>

namespace physics {

Body::Body(glm::vec2 _pos, glm::vec2 _vel, float _mass) 
        : m_position(_pos)
        , m_velocity(_vel)
        , m_mass(_mass)
        , m_active(true) {

}

void Body::setPosition(glm::vec2 position) {
    m_position = position;
}

void Body::setVelocity(glm::vec2 velocity) {
    m_velocity = velocity;
}

void Body::setActive(bool active) {
    m_active = active;
}

glm::vec2 Body::position() const {
    return m_position;
}

glm::vec2 Body::force() const { 
    return m_force;
}

void Body::addForce(const glm::vec2& _force) {
    m_force += _force;
}

float Body::mass() const {
   return m_mass;
}

bool Body::isActive() const {
    return m_active;
}

void Body::update(float dt) {
    m_position += m_velocity * dt;
    m_velocity += (m_force / m_mass) * dt;
    m_force = {0.0f, 0.0f};
}

}
