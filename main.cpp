#include "raylib.h"
#include "raymath.h"

#include "grid.hpp"
#include "cell.hpp"

int main() {

    Grid grid = Grid(); 

    InitWindow(SCREEN_W, SCREEN_H, "asdasd");
    SetTargetFPS(60);

    ParticleVariant selectedP = ParticleVariant::Sand;

    while (!WindowShouldClose()) {
        BeginDrawing();
            grid.mouseInput(selectedP);
            grid.updateGrid();
            grid.renderGrid();
        EndDrawing();
    }

    CloseWindow();

}
