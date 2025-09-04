#pragma once

#include "body.h"

#include <vector>
#include <memory>

namespace physics {

class Simulation {

public:
    explicit Simulation(uint32_t _bodies_num);
    void update(float dt);

private:
    std::vector<std::shared_ptr<Body>> m_bodies;
};

}