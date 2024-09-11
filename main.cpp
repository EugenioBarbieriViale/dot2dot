#include <iostream>
#include <vector>
#include <raylib.h>

const int X = 1000;
const int Y = 800;

const int R = 40;

std::vector<Vector2> dots;

std::vector<Vector2> red;
std::vector<Vector2> blue;

void init_points() {
    int n = 6;

    for (int j=0; j<6; j++) {
        for (int i=0; i<n; i++) {
            float x = 300 + (2*R+20)*i;
            float y = 170 + (2*R+10)*j;

            dots.push_back((Vector2){x,y});
        }
        n--;
    }
}

void draw_points() {
    for (int i=0; i<21; i++) {
        DrawCircle(dots[i].x, dots[i].y, R, (Color){255,255,0,90});
        DrawText(TextFormat("%d", i), dots[i].x - 6, dots[i].y - 10, 20, WHITE);
    }
}

bool clicked(Vector2 mouse_pos, Vector2 current_pos) {
    if (CheckCollisionPointCircle(mouse_pos, current_pos, R) && IsMouseButtonPressed(MOUSE_BUTTON_LEFT))
        return true;
    return false;
}

void update_turn(int *turn, int *temp) {
    if (*temp % 2 == 1)
        (*turn)++;
    (*temp)++;
}

void erase(Vector2 mouse_pos, int *turn, int *temp) {
    for (int i=0; i<21; i++) {
        if (clicked(mouse_pos, dots[i])) {

            if (*turn % 2 == 0) {
                red.push_back(dots[i]);
            } 
            else {
                blue.push_back(dots[i]);
            }

            update_turn(turn, temp);
        }
    }
}

float shift_lines(float x1, float x2) {
    float shift = x1 - x2;
    shift /= abs(shift);
    shift *= 3*R/4;
    return shift;
}

void draw_red_lines() {
    for (int i = 0; i < red.size(); i++) {
        Vector2 curr = red[i];
        Vector2 prev = red[i-1];

        if (curr.y == prev.y && i % 2 == 1) {
            float shift = shift_lines(curr.x, prev.x);

            curr.x += shift; 
            prev.x -= shift; 

            DrawLineEx(curr, prev, 5, RED);
        }
    }
}

void draw_blue_lines() {
    for (int i = 0; i < blue.size(); i++) {
        Vector2 curr = blue[i];
        Vector2 prev = blue[i-1];

        if (curr.y == prev.y && i % 2 == 1) {
            float shift = shift_lines(curr.x, prev.x);

            curr.x += shift; 
            prev.x -= shift; 

            DrawLineEx(curr, prev, 5, BLUE);
        }
    }
}

void print_v() {
    for (auto r : red)
        std::cout << "red: " << r.x << ", " << r.y << '\n';
    for (auto b : blue)
        std::cout << "blue: " << b.x << ", " << b.y << '\n';
}


int main() {
    InitWindow(X, Y, "dot2dot");
    SetTargetFPS(30);

    int turn = 0;
    int temp = 0;

    init_points();

    while (!WindowShouldClose()) {
        Vector2 mouse_pos = GetMousePosition();

        erase(mouse_pos, &turn, &temp);

        BeginDrawing();
        ClearBackground(GRAY);

        draw_points();

        draw_red_lines();
        draw_blue_lines();

        EndDrawing();
    }

    return 0;
}
