#pragma once

#include "base/body.h"
#include <raylib.h>

class DrawableBody : public physics::Body {

public:
    explicit DrawableBody(glm::vec2 _pos, glm::vec2 _vel, float _mass, Color _color, float _radius = 1.0f);
    virtual ~DrawableBody();

    virtual void draw();
    float getRadius() const { return m_radius; }
protected:
    Color m_color {RED};
    float m_radius {1.0f};

};
