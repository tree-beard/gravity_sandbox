#pragma once

#include <memory>
#include <vector>

namespace physics {
    class Body;
}

class BodiesGenerator {

public:
    BodiesGenerator() = delete;
    static std::vector<std::shared_ptr<physics::Body>> generateRandomBodies(size_t number, float areaWidth, float areaHeight);
};
