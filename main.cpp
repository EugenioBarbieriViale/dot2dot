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

    if (shift != 0)
        shift /= abs(shift);
    else
        return 3*R/4;

    return shift * 3*R/4;
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

bool is_intersecting(Vector2 start1, Vector2 end1, Vector2 start2, Vector2 end2) {
	float denominator = ((end1.x - start1.x) * (end2.y - start2.y)) - ((end1.y - start1.y) * (end2.x - start2.x));
    float numerator1 = ((start1.y - start2.y) * (end2.x - start2.x)) - ((start1.x - start2.x) * (end2.y - start2.y));
    float numerator2 = ((start1.y - start2.y) * (end1.x - start1.x)) - ((start1.x - start2.x) * (end1.y - start1.y));

    if (denominator == 0)
        return (numerator1 == 0 && numerator2 == 0);
    
    float r = numerator1 / denominator;
    float s = numerator2 / denominator;

    return (r > 0 && r < 1) && (s > 0 && s < 1);
}

bool check_line_collsion() {
    bool coll = false;

    for (int r = 0; r < red.size(); r += 2) {
        for (int b = 0; b < blue.size(); b += 2) {

            if (blue.size() != 0 && red.size() != 0 && blue.size() % 2 == 0 && red.size() % 2 == 0)
                // coll = is_intersecting(red[r], red[r+1], blue[b], blue[b+1]);
                coll = is_intersecting(blue[b], blue[b+1], red[r], red[r+1]);

            if (red[r].x == red[r+1].x || blue[b].x == blue[b+1].x)
                coll = false;
        }
    }
    return coll;
}

void print_v() {
    std::cout << "----------------\n";
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

    while (!WindowShouldClose() && !check_line_collsion()) {
    // while (!WindowShouldClose()) {
        Vector2 mouse_pos = GetMousePosition();

        // print_v();
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
