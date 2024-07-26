#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <raylib.h>

#define X 1000
#define Y 800

# define R 40

typedef struct {
    int x;
    int y;
    bool erased;
} Dot;

int n_erased;

int red_nodes = 0;
int blue_nodes = 0;

int turn = 0;
int temp = 0;

bool end_game = false;

bool who_won() {
    if (n_erased >= 21) {
        if (turn % 2 == 0)
            printf("RED WON\n");
        else
            printf("BLUE WON\n");

        return true;
    }
    return false;
}

void init_points(Dot dots[21]) {
    int n = 6;
    int count = 0;

    for (int j=0; j<6; j++) {
        for (int i=0; i<n; i++) {
            int x = 300 + (2*R+20)*i;
            int y = 170 + (2*R+10)*j;

            dots[count].x = x;
            dots[count].y = y;
            dots[count].erased = false;

            count++;
        }
        n--;
    }
}

void draw_points(Dot dots[21]) {
    for (int i=0; i<21; i++) {
        DrawCircle(dots[i].x, dots[i].y, R, (Color){255,255,0,90});
        DrawText(TextFormat("%d", i), dots[i].x - 6, dots[i].y - 10, 20, WHITE);
    }
}

int count_erased(float a, float b) {
    return (abs(((a - b)/(2*R+20))) + 1);
}

bool clicked(int x, int y) {
    Vector2 mouse_pos = GetMousePosition();
    Vector2 current_pos = {x,y};
    if (CheckCollisionPointCircle(mouse_pos, current_pos, R) && IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) return true;
    else return false;
}

void update_dot(Dot dots[21], int ci, int pi) {
    if (temp % 2 == 1) {
        if (ci < pi) {
            int now = pi;
            pi = ci;
            ci = now;
        }

        for (int j = pi; j <= ci; j++)
            dots[j].erased = true;
    }
}

void erase(Dot dots[21], int red_erased[21][2], int blue_erased[21][2], int x_erased[21], int indexes[21]) {
    for (int i=0; i<21; i++) {
        int x = dots[i].x;
        int y = dots[i].y;

        bool same_col = false;
        if (clicked(x,y)) {

            if (dots[i].erased && (x != x_erased[temp - 1]))
                same_col = true;

            indexes[temp] = i;
            x_erased[temp] = x;

            if (turn % 2 == 0) {
                if (dots[i].erased && (x != red_erased[red_nodes - 1][0] || same_col)) {
                    end_game = true;
                }

                red_erased[red_nodes][0] = x;
                red_erased[red_nodes][1] = y;
                red_nodes++;
            }

            else {
                if (dots[i].erased && (x != blue_erased[blue_nodes - 1][0] || same_col)) {
                    end_game = true;
                }

                blue_erased[blue_nodes][0] = x;
                blue_erased[blue_nodes][1] = y;
                blue_nodes++;
            }

            update_dot(dots, i, indexes[temp - 1]);

            temp++;
            if (temp % 2 == 0) turn++;
        }
    }
}

void red_lines(int red_erased[21][2]) {
    for (int i=1; i<red_nodes; i++)   {
        Vector2 p1 = {red_erased[i-1][0], red_erased[i-1][1]};
        Vector2 p2 = {red_erased[i][0], red_erased[i][1]};

        if (p1.x > p2.x) {
            p1.x += 3*R/4;
            p2.x -= 3*R/4;
        } else {
            p1.x -= 3*R/4;
            p2.x += 3*R/4;
        }

        if (p1.y == p2.y && i%2==1) {
            DrawLineEx(p1, p2, 5, RED);
            n_erased += count_erased(p1.x, p2.x);
        }
    }

}

void blue_lines(int blue_erased[21][2]) {
    for (int i=1; i<blue_nodes; i++) {
        Vector2 p1 = {blue_erased[i-1][0], blue_erased[i-1][1]};
        Vector2 p2 = {blue_erased[i][0], blue_erased[i][1]};

        if (p1.x > p2.x) {
            p1.x += 3*R/4;
            p2.x -= 3*R/4;
        } else {
            p1.x -= 3*R/4;
            p2.x += 3*R/4;
        }

        if (p1.y == p2.y && i%2==1) {
            DrawLineEx(p1, p2, 5, BLUE);
            n_erased += count_erased(p1.x, p2.x);
        }
    }
}

int main() {
    InitWindow(X,Y, "dot2dot");
    SetTargetFPS(60);       
    
    Dot dots[21];

    int red_erased[21][2];
    int blue_erased[21][2];

    int x_erased[21];
    int indexes[21];

    init_points(dots);

    while (!WindowShouldClose() && !end_game) {
        if (who_won())
            end_game = true;

        n_erased = 0;
        erase(dots, red_erased, blue_erased, x_erased, indexes);


        BeginDrawing();
        ClearBackground(GRAY);
        DrawText("dot2dot game - erase dots in rows!", 50, 10, 50, BLACK);

        draw_points(dots);
        red_lines(red_erased);
        blue_lines(blue_erased);

        EndDrawing();
    }
    CloseWindow();
    return 0;
}
