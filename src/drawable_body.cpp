#include "drawable_body.h"
#include <algorithm>

DrawableBody::DrawableBody(glm::vec2 _pos, glm::vec2 _vel, float _mass, Color _color, float _radius)
    : physics::Body(_pos, _vel, _mass)
    , m_color(_color)
    , m_radius(_radius) {
}

DrawableBody::~DrawableBody() {

}
    
void DrawableBody::draw() {
    std::shared_lock<std::shared_timed_mutex> lock(m_mutex);
    DrawCircleV({m_position.x, m_position.y}, m_radius, m_color);
}
