#include <raylib.h>
#include "bodies_holder.h"
#include "base/simulation.h"
#include "drawable_body.h"

//------------------------------------------------------------------------------------
// Program main entry point
//------------------------------------------------------------------------------------
int main(void)
{
    // Initialization
    //--------------------------------------------------------------------------------------
    const int screenWidth = 1280;
    const int screenHeight = 720;

    SetConfigFlags(FLAG_WINDOW_HIGHDPI);
    InitWindow(screenWidth, screenHeight, "gravity sandbox");

    SetTargetFPS(60);               // Set our game to run at 60 frames-per-second

    // Setup
    //--------------------------------------------------------------------------------------
    auto bodiesHolder = std::make_unique<BodiesHolder>();
    bodiesHolder->generateRandomBodies(1000);

    // Add a couple of heave bodies
    auto bigBody = std::make_shared<DrawableBody>(glm::vec2(0.5 * screenWidth, 0.5 * screenHeight), glm::vec2(0,0), 100000, BLACK, 10);
    auto anotherBigBody = std::make_shared<DrawableBody>(glm::vec2(0.5 * screenWidth + 200, 0.5 * screenHeight + 200), glm::vec2(0,0), 50000, DARKGRAY, 8);
    bodiesHolder->addBody(anotherBigBody);
    bodiesHolder->addBody(bigBody);

    // Start simulation
    auto bodies = bodiesHolder->getBodies();
    auto simulation = std::make_unique<physics::Simulation>(bodies);
    simulation->start();

    // Main game loop
    //--------------------------------------------------------------------------------------
    while (!WindowShouldClose())    // Detect window close button or ESC key
    {
        // Draw
        //--------------------------------------------------------------------------------------
        BeginDrawing();

        ClearBackground(RAYWHITE);

        for(auto body : bodies) {
            std::static_pointer_cast<DrawableBody>(body)->draw();
        }
        std::string header = "Gravity simulation for " + std::to_string(bodies.size()) + " bodies";
        DrawText(header.c_str(), 10, 10, 20, DARKGRAY);
        EndDrawing();
    }

    simulation->stop();

    // De-Initialization
    //--------------------------------------------------------------------------------------
    CloseWindow();        // Close window and OpenGL context

    return 0;
}
