#include "bodies_holder.h"
#include "drawable_body.h"
#include <random>

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

BodiesHolder::BodiesHolder() {

}

void BodiesHolder::addBody(std::shared_ptr<physics::Body> _body) {
    m_bodies.push_back(std::move(_body));
}

const std::vector<std::shared_ptr<physics::Body>>& BodiesHolder::getBodies() const {
    return m_bodies;
}

void BodiesHolder::generateRandomBodies(size_t number) {
    for (auto i = 0; i < number; ++i) {
        const auto time = static_cast<int>(std::chrono::system_clock::now().time_since_epoch().count());
        const auto pos = getRandomVec2(time, 0, 1280);
        const auto vel = getRandomVec2(time, -10, 10);
        const auto mass = getRandomFloat(time, 10, 100);
        const auto radius = getRandomFloat(time, 1, 3);
        auto color = getRandomColor(time);
        addBody(std::make_shared<DrawableBody>(pos, vel, mass, color, radius));
    }
}
