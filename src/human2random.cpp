#include <iostream>
#include <vector>
#include <raylib.h>

#define X 1000
#define Y 800

#define R 40

void init_points(std::vector<Vector2>& dots, std::vector<bool>& erased_dots) {
    int n = 6;

    for (int j=0; j<6; j++) {
        for (int i=0; i<n; i++) {
            float x = 300 + (2*R+20)*i;
            float y = 170 + (2*R+10)*j;

            dots.push_back((Vector2){x,y});
            erased_dots.push_back(false);
        }
        n--;
    }
}

void draw_points(const std::vector<Vector2>& dots, Color c) {
    for (int i=0; i<21; i++) {
        DrawCircle(dots[i].x, dots[i].y, R, c);
        DrawText(TextFormat("%d", i), dots[i].x - 6, dots[i].y - 10, 20, WHITE);
    }
}

bool clicked(Vector2 mouse_pos, Vector2 current_pos) {
    if (CheckCollisionPointCircle(mouse_pos, current_pos, R) && IsMouseButtonPressed(MOUSE_BUTTON_LEFT))
        return true;
    return false;
}

void update_turn(int& turn, int& temp) {
    if (temp % 2 == 1)
        turn++;
    temp++;
}

void update_dot(std::vector<bool>& erased_dots, int temp, int ci, int pi, bool& coll) {
    if (temp % 2 == 1) {
        if (ci < pi) {
            int now = pi;
            pi = ci;
            ci = now;
        }

        for (int j = pi; j <= ci; j++) {
            if (!erased_dots[j])
                erased_dots[j] = true;
            else
                coll = true;
        }
    }
}

int count_erased(const std::vector<bool>& erased_dots) {
    int count = 0;
    for (auto x:erased_dots) {
        if (x) {
            count++;
        }
    }
    return count;
}

float shift_lines(float x1, float x2) {
    float shift = x1 - x2;

    if (shift != 0)
        shift /= abs(shift);
    else
        return 3*R/4;

    return shift * 3*R/4;
}

void draw_red_lines(const std::vector<Vector2>& red) {
    for (int i = 1; i < red.size(); i++) {
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

void draw_blue_lines(const std::vector<Vector2>& blue) {
    for (int i = 1; i < blue.size(); i++) {
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

std::vector<int> generate(const std::vector<bool>& erased_dots) {
    std::vector<int> idxs;

    int i1, i2;
    int c = 0;
    bool go = false;

    std::cout << "-----------" << "\n";
    do {
        go = false;
        int col = std::rand() % 6;

        i1 = (std::rand() % (6 - col));
        i2 = (std::rand() % (6 - col));

        for (int i=0; i<col; i++) {
            i1 += (6 - i);
            i2 += (6 - i);
        }

        if (i1 > i2) {
            int now = i2;
            i2 = i1;
            i1 = now;
        }

        // std::cout << i1 << " " << i2 << "\n";

        for (int j = i1; j <= i2; j++) {
            if (erased_dots[j])
                go = true;
        }

        c++;
        std::cout << c << "\n";

    } while ((erased_dots[i1] || erased_dots[i2] || go) && c <= 1000);

    idxs.push_back(i1);
    idxs.push_back(i2);

    return idxs;
}



int main() {
    std::srand(time(0));

    InitWindow(X, Y, "dot2dot");
    SetTargetFPS(30);

    std::vector<Vector2> dots;
    std::vector<bool> erased_dots;

    std::vector<Vector2> red;
    std::vector<Vector2> blue;

    Color c = ORANGE;
    Color bg_c = BROWN;

    int turn = 0;
    int temp = 0;
    int indexes[2];

    bool collision = false;

    init_points(dots, erased_dots);

    while (!WindowShouldClose()) {
        int n_erased = count_erased(erased_dots);

        Vector2 mouse_pos = GetMousePosition();

        if (turn % 2 == 1 && n_erased < 21) {
            std::vector<int> idxs = generate(erased_dots);

            blue.push_back(dots[idxs[0]]);
            blue.push_back(dots[idxs[1]]);

            temp++;
            update_dot(erased_dots, temp, idxs[0], idxs[1], collision);

            update_turn(turn, temp);
        }

        for (int i=0; i<21; i++) {
            if (clicked(mouse_pos, dots[i])) {
                if (turn % 2 == 0) {
                    indexes[temp % 2] = i;

                    red.push_back(dots[i]);

                    update_dot(erased_dots, temp, i, indexes[(temp % 2) - 1], collision);
                    update_turn(turn, temp);
                }
            }
        }

        BeginDrawing();
        ClearBackground(bg_c);

        if (n_erased >= 21) {
            WaitTime(2);
            DrawRectangle(0, 0, X, Y, bg_c);

            if (turn % 2 == 0)
                DrawText("RED WON", 175, Y/2-60, 150, RED);
            else
                DrawText("BLUE WON", 120, Y/2-60, 150, BLUE);
        }

        else if (collision) {
            WaitTime(2);
            DrawRectangle(0, 0, X, Y, bg_c);
            DrawText("COLLISION", 110, Y/2-60, 150, c);
        }

        else {
            draw_points(dots, c);

            draw_blue_lines(blue);
            draw_red_lines(red);
        }

        EndDrawing();
    }

    return 0;
}
