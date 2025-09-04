#pragma once

#include <glm/vec2.hpp>

namespace physics {

class Body {

public:
    explicit Body(glm::vec2 _pos, glm::vec2 _vel, glm::vec2 _acc, float _mass);

private:
    glm::vec2 m_position;
    glm::vec2 m_velocity; 
    glm::vec2 m_acceleration;
    float m_mass;
};

}