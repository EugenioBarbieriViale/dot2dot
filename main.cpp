#include <iostream>
#include <raylib.h>

const int X = 1000;
const int Y = 800;

int main() {
    InitWindow(X, Y, "dot2dot");
    SetTargetFPS(30);
    
    while (!WindowShouldClose()) {
        BeginDrawing();
        ClearBackground(GRAY);


        EndDrawing();
    }

    return 0;
}
