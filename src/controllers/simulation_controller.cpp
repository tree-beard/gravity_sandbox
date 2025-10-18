#include "simulation_controller.h"
#include "physics/nbody_simulation.h"
#include "graphics/drawable_body.h"

SimulationController::SimulationController(glm::vec2 visualArea, std::vector<std::shared_ptr<DrawableBody>>& bodies)
    : m_simulation(std::make_unique<physics::NBodySimulation>(visualArea))
    , m_bodies(std::move(bodies)) {
    for (auto body : m_bodies) {
        m_simulation->addBodie(std::static_pointer_cast<physics::Body>(body));
    }
}

void SimulationController::start(float dt) {
    m_running = true;
    m_workerThread = std::thread([this, dt]() mutable {
        std::chrono::high_resolution_clock::time_point lastFrameTimePoint = std::chrono::high_resolution_clock::now();
        float realDt = dt;
        while (m_running) {
            // Update
            const auto now = std::chrono::high_resolution_clock::now();
	        realDt = static_cast<float>(std::chrono::duration_cast<std::chrono::milliseconds>(now - lastFrameTimePoint).count()) / 1000;

            if (realDt >= dt) {
                lastFrameTimePoint = now;
                update(dt);
            }
        }
    });
}

void SimulationController::stop() {
    m_running = false;
    if(m_workerThread.joinable()) {
        m_workerThread.join();
    }
}

void SimulationController::update(float dt) {
    m_simulation->step(dt);
}

void SimulationController::render() {
    ClearBackground(BLACK);

    for (auto bodyIter = m_bodies.begin(); bodyIter != m_bodies.end(); /* no increment here */) {
        if ((*bodyIter)->isActive()) {
            (*bodyIter)->draw();
            ++bodyIter;
        }
        else {
            bodyIter = m_bodies.erase(bodyIter);
        }
    }
    std::string header = "Gravity simulation for " + std::to_string(m_bodies.size()) + " bodies";
    DrawText(header.c_str(), 10, 10, 20, GREEN);
}