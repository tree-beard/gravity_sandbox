#pragma once

#include "physics/nbody_simulation.h"

#include <glm/vec2.hpp>
#include <vector>
#include <memory>
#include <thread>
#include <atomic>

class DrawableBody;

class SimulationController {

public:
    explicit SimulationController(glm::vec2 visualArea, std::vector<std::shared_ptr<DrawableBody>>& bodies);
    SimulationController(const SimulationController&) = delete;
    SimulationController& operator=(const SimulationController&) = delete;
    ~SimulationController() = default;

    void start(float dt = 0.0f);
    void stop();
    void render();

private:
    void update(float dt);

    std::unique_ptr<physics::NBodySimulation> m_simulation;
    std::vector<std::shared_ptr<DrawableBody>> m_bodies;
    std::atomic_bool m_running {false};
    std::thread m_workerThread;
};
