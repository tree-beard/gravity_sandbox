#include "drawable_body.h"
#include <algorithm>

DrawableBody::DrawableBody(glm::vec2 pos, glm::vec2 vel, float mass, float radius, Color color)
    : physics::Body(pos, vel, mass)
    , m_radius(radius)
    , m_color(color) {
}

DrawableBody::~DrawableBody() {

}
    
void DrawableBody::draw() {
    std::shared_lock<std::shared_timed_mutex> lock(m_mutex);
    DrawCircleV({m_position.x, m_position.y}, m_radius, m_color);
}
