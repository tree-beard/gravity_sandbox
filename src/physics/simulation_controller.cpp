#include "simulation_controller.h"
#include <cmath>
#include <limits>
#include <algorithm>
#include <iostream>

namespace physics {

SimulationController::SimulationController(glm::vec2 visualArea, std::vector<std::shared_ptr<Body>>& bodies)
    : m_simulation(std::make_unique<BarnesHutSimulation>(visualArea)) {
    m_simulation->setBodies(bodies);
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



// Direct O(N^2) algorithm
//--------------------------------------------------------------------------------------
// void Simulation::update(float dt) {

//     for( auto i = 0; i < m_bodies.size(); ++i) {
//         const auto pos1 = m_bodies[i]->position();
//         const auto mass1 = m_bodies[i]->mass();

//         for(auto j = i + 1; j < m_bodies.size(); ++j) {
//             const auto pos2 = m_bodies[j]->position();
//             const auto mass2 = m_bodies[j]->mass();

//             const auto r = pos2 - pos1;
//             const float dist_sqr = r.x * r.x + r.y * r.y;
//             const float dist = std::sqrt(dist_sqr + 0.0001f);
//             const float forceMagnitude = mass1 * mass2 / (dist * dist);
//             const auto forceVec = forceMagnitude * r / dist;

//             m_bodies[i]->addForce(forceVec); // Apply force vector for m_bodies[i]
//             m_bodies[j]->addForce( glm::vec2(0.0f,0.0f) - forceVec); // Apply negtive force vector for m_bodies[j]
//         }
//     }

//     for(auto body : m_bodies) {
//         body->update(dt);
//     }
// }

}
