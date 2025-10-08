#pragma once

#include "physics/body.h"
#include <raylib.h>

class DrawableBody : public physics::Body {

public:
    explicit DrawableBody(glm::vec2 pos, glm::vec2 vel, float mass, float radius, Color color);
    virtual ~DrawableBody();

    virtual void draw();
    float getRadius() const { return m_radius; }
protected:
    Color m_color {WHITE};
    float m_radius {1.0f};

};
