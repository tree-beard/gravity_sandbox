#include <raylib.h>
#include "utils/bodies_generator.h"
#include "controllers/simulation_controller.h"
#include "graphics/drawable_body.h"

//------------------------------------------------------------------------------------
// Program main entry point
//------------------------------------------------------------------------------------
int main(void)
{
    // Initialization
    //--------------------------------------------------------------------------------------
    const int screenWidth = 1010;
    const int screenHeight = 660;

    SetConfigFlags(FLAG_WINDOW_HIGHDPI);
    InitWindow(screenWidth, screenHeight, "gravity sandbox");

    SetTargetFPS(60);               // Set our game to run at 60 frames-per-second

    // Setup
    //--------------------------------------------------------------------------------------
    auto bodies = BodiesGenerator::generateRandomBodies(10000, screenWidth, screenHeight);

    // Add a couple of heave bodies
    auto bigBody = std::make_shared<DrawableBody>(glm::vec2(0.5 * screenWidth, 0.5 * screenHeight), glm::vec2(0,0), 100000, 7, GOLD);
    auto anotherBigBody = std::make_shared<DrawableBody>(glm::vec2(0.5 * screenWidth + 100, 0.5 * screenHeight + 100), glm::vec2(0,0), 10000, 4, RED);
    bodies.push_back(anotherBigBody);
    bodies.push_back(bigBody);

    // Inintialize and start simulation
    auto simulation = std::make_unique<SimulationController>(glm::vec2(screenWidth, screenHeight), bodies);
    simulation->start(0.01);

    // Main game loop
    //--------------------------------------------------------------------------------------
    while (!WindowShouldClose())    // Detect window close button or ESC key
    {
        // Draw
        //--------------------------------------------------------------------------------------
        BeginDrawing();
        simulation->render();
        EndDrawing();
    }

    simulation->stop();

    // De-Initialization
    //--------------------------------------------------------------------------------------
    CloseWindow();        // Close window and OpenGL context

    return 0;
}
