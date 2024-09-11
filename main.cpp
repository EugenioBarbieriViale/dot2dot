#include <iostream>
#include <vector>
#include <raylib.h>

const int X = 1000;
const int Y = 800;

const int R = 40;

typedef struct {
    Vector2 pos;
    bool erased;
} Dot;

void init_points(Dot dots[21]) {
    int n = 6;
    int count = 0;

    for (int j=0; j<6; j++) {
        for (int i=0; i<n; i++) {
            int x = 300 + (2*R+20)*i;
            int y = 170 + (2*R+10)*j;

            dots[count].pos.x = x;
            dots[count].pos.y = y;
            dots[count].erased = false;

            count++;
        }
        n--;
    }
}

void draw_points(Dot dots[21]) {
    for (int i=0; i<21; i++) {
        DrawCircle(dots[i].pos.x, dots[i].pos.y, R, (Color){255,255,0,90});
        DrawText(TextFormat("%d", i), dots[i].pos.x - 6, dots[i].pos.y - 10, 20, WHITE);
    }
}

bool clicked(Vector2 mouse_pos, Vector2 current_pos) {
    if (CheckCollisionPointCircle(mouse_pos, current_pos, R) && IsMouseButtonPressed(MOUSE_BUTTON_LEFT))
        return true;
    return false;
}

void decide_turn(int *turn, int *temp) {
    if (*temp % 2 == 1)
        (*turn)++;
    (*temp)++;
}

void erase(Dot dots[21], Vector2 mouse_pos, std::vector<Vector2> red, std::vector<Vector2> blue, int *turn, int *temp) {
    for (int i=0; i<21; i++) {
        if (clicked(mouse_pos, dots[i].pos)) {

            if (*turn % 2 == 1) {
                red.push_back(dots[i].pos);
            } 
            else {
                blue.push_back(dots[i].pos);
            }

            decide_turn(turn, temp);
        }
    }
}

void draw_lines(std::vector<Vector2> red, std::vector<Vector2> blue) {
    for (int i = 0; i < red.size(); i++)
        DrawLineEx(red[i-1], red[i], 5, RED);

    for (int i = 0; i < blue.size(); i++)
        DrawLineEx(blue[i-1], blue[i], 5, BLUE);
}

void print_v(std::vector<Vector2> red, std::vector<Vector2> blue) {
    for (auto r : red)
        std::cout << "red: " << r.x << ", " << r.y << '\n';
    for (auto b : blue)
        std::cout << "blue: " << b.x << ", " << b.y << '\n';
}


int main() {
    InitWindow(X, Y, "dot2dot");
    SetTargetFPS(30);

    Dot dots[21];

    std::vector<Vector2> red;
    std::vector<Vector2> blue;

    int turn = 0;
    int temp = 0;

    init_points(dots);

    while (!WindowShouldClose()) {
        Vector2 mouse_pos = GetMousePosition();

        erase(dots, mouse_pos, red, blue, &turn, &temp);

        std::cout << temp << "--" << turn << std::endl;
        print_v(red, blue);

        BeginDrawing();
        ClearBackground(GRAY);

        draw_points(dots);
        draw_lines(red, blue);

        EndDrawing();
    }

    return 0;
}
