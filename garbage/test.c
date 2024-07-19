#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#include <raylib.h>

#define X 1000
#define Y 800

int human_erased[21][2];
int machine_erased[21][2];

int human_nodes = 0;
int machine_nodes = 0;

int turn = 0;

int temp = 0;

int random_x(int range) {
    return (rand() % range);
}

int random_y(int range) {
    return (rand() % range);
}

void erase(void) {
    Vector2 mouse = GetMousePosition();
    if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) {
        if (temp % 2 == 0) {
            human_erased[human_nodes][0] = mouse.x;
            human_erased[human_nodes][1] = mouse.y;
            human_nodes++;
        }
        temp++;
        if (temp % 2 == 0) turn++;
    }
    else if (IsMouseButtonReleased(MOUSE_BUTTON_LEFT)) {
        if (temp % 2 == 1) {
            machine_erased[machine_nodes][0] = random_x(X);
            machine_erased[machine_nodes][1] = random_y(Y);

            machine_nodes++;
        }

        /* temp++; */
        /* if (temp % 2 == 1) turn++; */
    }

}

void draw(void) {
    for (int i=0; i<human_nodes; i++) {
		DrawLine(human_erased[i-1][0], human_erased[i-1][1], human_erased[i][0], human_erased[i][1], RED);
    }
    for (int i=0; i<machine_nodes; i++) {
		DrawLine(machine_erased[i-1][0], machine_erased[i-1][1], machine_erased[i][0], machine_erased[i][1], BLUE);
    }
}

int main() {
    srand(time(NULL));

    InitWindow(X, Y, "dhfl");
    SetTargetFPS(60);

    while (!WindowShouldClose()) {
        erase();

        BeginDrawing();
        ClearBackground(GRAY);

        draw();

        EndDrawing();
    }
    CloseWindow();

    return 0;
}
