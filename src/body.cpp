#include "body.h"

namespace physics {

Body::Body(glm::vec2 _pos, glm::vec2 _vel, glm::vec2 _acc, float _mass) 
        : m_position(_pos)
        , m_velocity(_vel)
        , m_acceleration(_acc)
        , m_mass(_mass) {

    }
}