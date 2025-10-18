#pragma once

#include <memory>
#include <vector>

class DrawableBody;

class BodiesGenerator {

public:
    BodiesGenerator() = delete;
    static std::vector<std::shared_ptr<DrawableBody>> generateRandomBodies(size_t number, float areaWidth, float areaHeight);
};
