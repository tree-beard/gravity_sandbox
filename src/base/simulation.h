#pragma once

#include "body.h"

#include <vector>
#include <memory>
#include <thread>
#include <atomic>

namespace physics {

class Simulation {

public:
    explicit Simulation(std::vector<std::shared_ptr<Body>>& _bodies);
    Simulation(Simulation&& other) noexcept;
    Simulation(const Simulation&) = delete;
    Simulation& operator=(const Simulation&) = delete;
    ~Simulation() = default;

    void start(float dt = 0.0f);
    void stop();

private:
    std::vector<std::shared_ptr<Body>> m_bodies;
    std::atomic_bool m_running {false};
    std::thread m_workerThread;

    void update(float dt);
};

}
