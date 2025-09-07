#include "simulation.h"
#include <cmath>
#include <limits>
#include <algorithm>
#include <iostream>

namespace physics {

constexpr float MIN_FLOAT_VAL = std::numeric_limits<float>::lowest();

Simulation::Simulation(std::vector<std::shared_ptr<Body>>& _bodies) : m_bodies(_bodies) {

}

Simulation::Simulation(Simulation&& other) noexcept : m_bodies(std::move(other.m_bodies)) {

}

void Simulation::start(float dt) {
    m_running = true;
    m_workerThread = std::thread([this, dt]() mutable {
        std::chrono::high_resolution_clock::time_point lastFrameTimePoint = std::chrono::high_resolution_clock::now();
        float finalDt = dt;
        while (m_running) {
            // Update
            if(dt == 0.0f) {
                const auto now = std::chrono::high_resolution_clock::now();
	            finalDt = static_cast<float>(std::chrono::duration_cast<std::chrono::milliseconds>(now - lastFrameTimePoint).count()) / 1000;
                lastFrameTimePoint = now;
            }
	        update(finalDt);
        }
    });
}

void Simulation::stop() {
    m_running = false;
    if(m_workerThread.joinable()) {
        m_workerThread.join();
    }
}

void Simulation::update(float dt) {

    for( auto i = 0; i < m_bodies.size(); ++i) {
        const auto pos1 = m_bodies[i]->position();
        const auto mass1 = m_bodies[i]->mass();

        for(auto j = i + 1; j < m_bodies.size(); ++j) {
            const auto pos2 = m_bodies[j]->position();
            const auto mass2 = m_bodies[j]->mass();

            const auto r = pos2 - pos1;
            const float dist_sqr = r.x * r.x + r.y * r.y;
            const float dist = std::sqrt(dist_sqr + 0.0001f);
            const float forceMagnitude = mass1 * mass2 / (dist * dist);
            const auto forceVec = forceMagnitude * r / dist;

            m_bodies[i]->addForce(forceVec); // Apply force vector for m_bodies[i]
            m_bodies[j]->addForce( glm::vec2(0.0f,0.0f) - forceVec); // Apply negtive force vector for m_bodies[j]
        }
    }

    for(auto body : m_bodies) {
        body->update(dt);
    }
}

}
