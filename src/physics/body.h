#pragma once

#include <glm/vec2.hpp>
#include <shared_mutex>

namespace physics {

class Body {

public:
    explicit Body(glm::vec2 pos, glm::vec2 vel, float mass);
    virtual ~Body() = default;
    virtual void update(float dt);

    void setPosition(glm::vec2 position);
    void setVelocity(glm::vec2 velocity);
    glm::vec2 position() const;
    glm::vec2 force() const;
    float mass() const;
    void addForce(const glm::vec2& force);
    bool operator==(const Body& other) const {
        return m_position == other.m_position
            && m_mass == other.m_mass;
    }

protected:
    glm::vec2 m_position;
    glm::vec2 m_velocity; 
    glm::vec2 m_acceleration;
    glm::vec2 m_force;
    float m_mass;
    mutable std::shared_timed_mutex m_mutex;
};

}
