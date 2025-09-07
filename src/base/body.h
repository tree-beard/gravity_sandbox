#pragma once

#include <glm/vec2.hpp>
#include <shared_mutex>

namespace physics {

class Body {

public:
    explicit Body(glm::vec2 _pos, glm::vec2 _vel, float _mass);
    virtual ~Body() = default;
    virtual void update(float dt);

    glm::vec2 position() const;
    glm::vec2 force() const;
    float mass() const;
    void addForce(const glm::vec2& _force);

protected:
    glm::vec2 m_position;
    glm::vec2 m_velocity; 
    glm::vec2 m_acceleration;
    glm::vec2 m_force;
    float m_mass;
    mutable std::shared_timed_mutex m_mutex;
};

}
