#include "bodies_generator.h"
#include "graphics/drawable_body.h"

#include <random>
#include <glm/gtc/random.hpp>

glm::vec2 getRandomVec2(unsigned int seed, float min, float max) {
    static std::mt19937 gen(seed);
    std::uniform_real_distribution<float> dis(min, max);
    return glm::vec2(dis(gen), dis(gen));
}

Color getRandomColor(unsigned int seed) {
    static std::mt19937 gen(seed);
    std::uniform_int_distribution<uint8_t> dis(0, 255);
    return Color {dis(gen), dis(gen), dis(gen), 255};
}

float getRandomFloat(unsigned int seed, float min, float max) {
    static std::mt19937 gen(seed);
    std::uniform_real_distribution<float> dis(min, max);
    return dis(gen);
}

std::vector<std::shared_ptr<DrawableBody>> BodiesGenerator::generateRandomBodies(size_t number, float areaWidth, float areaHeight) {
    std::vector<std::shared_ptr<DrawableBody>> bodies;
    bodies.reserve(number);
    for (auto i = 0; i < number; ++i) {
        const auto time = static_cast<int>(std::chrono::system_clock::now().time_since_epoch().count());
        const auto pos = glm::diskRand(areaHeight / 2.0f) + glm::vec2(areaWidth, areaHeight) / 2.0f;
        const auto vel = getRandomVec2(time, -10, 10);
        const auto mass = getRandomFloat(time, 10, 100);
        const auto radius = getRandomFloat(time, 0.1, 1);
        auto color = WHITE;
        bodies.emplace_back(std::make_shared<DrawableBody>(pos, vel, mass, radius, color));
    }
    return bodies;
}
