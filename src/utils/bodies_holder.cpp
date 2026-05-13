#include "bodies_holder.h"
#include "graphics/drawable_body.h"

BodiesHolder::BodiesHolder(std::vector<std::shared_ptr<DrawableBody>>&& bodies) : m_bodies(std::move(bodies)) {
    
}