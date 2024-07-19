#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <raylib.h>

int random_int(int range) {
    return (rand() % range);
}

int get_y(int pos[][2]) {
    return pos[random_int(21)][1];
}

int get_x(int y, int pos[][2]) {
    for (int i=0; i<100; i++) {
        int index = random_int(21);
        if (pos[index][1] == y)
            return pos[index][0];
    }
    return 0;
}

int main() {
    srand(time(NULL));
    InitWindow(1000, 800, "xiao");
    SetTargetFPS(30);

    int pos[][2] = {
        {300, 170},
        {400, 170},
        {500, 170},
        {600, 170},
        {700, 170},
        {800, 170},
        {300, 260},
        {400, 260},
        {500, 260},
        {600, 260},
        {700, 260},
        {300, 350},
        {400, 350},
        {500, 350},
        {600, 350},
        {300, 440},
        {400, 440},
        {500, 440},
        {300, 530},
        {400, 530},
        {300, 620}
    };

    int to_draw[21][2];
    size_t N = 10;

    while (!WindowShouldClose()) {
        BeginDrawing();
        ClearBackground(BLACK);

        for (int i=0; i<1; i++) {
            int y = get_y(pos);
            to_draw[i][1] = y;

            for (int i = 0; i<2; i++) {
                int x = get_x(y, pos);
                to_draw[i][0] = x;
            }
        }

        for (int i=0; i<1; i++) {
            DrawLine(to_draw[i-1][0], to_draw[i-1][1], to_draw[i][0], to_draw[i][1], RED);
        }

        /* int y =  get_y(pos); */
        /* int x1 = get_x(y, pos); */
        /* int x2 = get_x(y, pos); */
        /* DrawLine(x1, y, x2, y, RED); */

        EndDrawing();
    }

    return 0;
}
