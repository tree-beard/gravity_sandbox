#pragma once

#include <glm/vec2.hpp>
#include <memory>
#include <vector>

namespace physics {
    class Body;
}

class BodiesHolder {

public:
    explicit BodiesHolder();

    void generateRandomBodies(size_t number);
    void addBody(std::shared_ptr<physics::Body> _body);
    const std::vector<std::shared_ptr<physics::Body>>& getBodies() const;

    template<typename T>
    std::vector<std::shared_ptr<T>> getBodiesAs() const {
        std::vector<std::shared_ptr<T>> resultVector;
        for (const auto& ptr : m_bodies) {
            auto desiredPtr = std::dynamic_pointer_cast<T>(ptr);
            if (desiredPtr != nullptr) {
                resultVector.push_back(desiredPtr);
            }
        }
        return resultVector;
    }

private:
    std::vector<std::shared_ptr<physics::Body>> m_bodies;
};
