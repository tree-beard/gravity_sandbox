#pragma once

#include <memory>
#include <vector>

class DrawableBody;

class BodiesHolder {

public:
    BodiesHolder(std::vector<std::shared_ptr<DrawableBody>>&& bodies);
    
private:
    std::vector<std::shared_ptr<DrawableBody>> m_bodies;
};
