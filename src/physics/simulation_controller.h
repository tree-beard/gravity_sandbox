#pragma once

#include "body.h"
#include "barnes_hut_simulation.h"

#include <vector>
#include <memory>
#include <thread>
#include <atomic>

namespace physics {

class SimulationController {

public:
    explicit SimulationController(glm::vec2 visualArea, std::vector<std::shared_ptr<Body>>& bodies);
    SimulationController(const SimulationController&) = delete;
    SimulationController& operator=(const SimulationController&) = delete;
    ~SimulationController() = default;

    void start(float dt = 0.0f);
    void stop();

private:
    std::unique_ptr<BarnesHutSimulation> m_simulation;
    std::atomic_bool m_running {false};
    std::thread m_workerThread;

    void update(float dt);
};

}
