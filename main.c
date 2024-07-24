/*
 * TODO:
 * - collision at start
 * + collision sometimes doesn't work properly
 * + blue lines often too long
*/


#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <time.h>
#include <raylib.h>


#define X 1000
#define Y 800

# define R 40

int n_erased;

int red_nodes = 0;
int blue_nodes = 0;

int turn = 0;
int temp = 0;

typedef struct {
    int x;
    int y;
    bool erased;
} Dot;

bool who_won() {
    if (n_erased >= 21) {
        if (turn % 2 == 0)
            printf("HUMAN WON\n");
        else
            printf("MACHINE WON\n");

        return true;
    }
    return false;
}

bool button_pressed(Vector2 mouse_pos, Rectangle button) {
	if (CheckCollisionPointRec(mouse_pos, button) && IsMouseButtonPressed(MOUSE_BUTTON_LEFT))
        return true;
    return false;
}

void init_points(Dot dots[21][2]) {
    int n = 6;
    int count = 0;

    for (int j=0; j<6; j++) {
        for (int i=0; i<n; i++) {
            Dot current = dots[i];

            int x = 300 + (2*R+20)*i;
            int y = 170 + (2*R+10)*j;

            current.x = x;
            current.y = y;
            current.erased = false;

            count++;
        }
        n--;
    }
}

void draw_points(Dot dots[21][2]) {
    for (int i=0; i<21; i++) {
        Dot current = dots[i];

        DrawCircle(current.x, current.y, R, (Color){255,255,0,90});
        DrawText(TextFormat("%d", i), current.x - 6, current.y - 10, 20, WHITE);
    }
}

int count_erased(float a, float b) {
    return (abs(((a - b)/(2*R+20))) + 1);
}

bool clicked(Vector2 mouse_pos, int x, int y) {
    Vector2 current_pos = {x,y};
    if (CheckCollisionPointCircle(mouse_pos, current_pos, R) && IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) return true;
    else return false;
}

int random_int(int range) {
    return (rand() % range);
}

int get_y(Dot dots[21]) {
    for (int i=0; i<50; i++) {
        int ans = dots[random_int(21)].x;
        if (!ans.erased)
            return ans;
    }
    return 0;
}

int get_x(int y, Dot dots[21]) {
    int c = 0;
    int ans[6];

    for (int i=0; i<21; i++) {
        if (dots[i].y == y) {
            ans[c] = dots[i].x;
            c++;
        }
    }
    return ans[random_int(c)];
}

void update_turn(void) {
    temp++;
    if (temp % 2 == 0) turn++;
}

void red_erase(int x, int y, Dot dots[21], int red_erased[21][2], int blue_erased[21][2]) {
    if (turn % 2 == 0) {
        red_erased[red_nodes][0] = x;
        red_erased[red_nodes][1] = y;

        red_nodes++;

        update_turn();
    }
}

void blue_erase(Dot dots[21], int red_erased[21][2], int blue_erased[21][2]) {
    if (turn % 2 == 1) {

        int rand_y = get_y(dots);

        if (blue_nodes % 2 == 0)
            blue_erased[blue_nodes][1] = rand_y;
        else
            blue_erased[blue_nodes][1] = blue_erased[blue_nodes - 1][1];

        int rand_x = get_x(rand_y, dots);
        blue_erased[blue_nodes][0] = rand_x;

        blue_nodes++;

        update_turn();
    }
}

void erase(Vector2 mouse_pos, Rectangle button, Dot dots[21], int red_erased[21][2], int blue_erased[21][2]) {
    for (int i=0; i<21; i++) {
        int x = dots[i].x;
        int y = dots[i].y;

        if (clicked(mouse_pos, x, y)) {
            red_erase(x, y, dots, red_erased, blue_erased, possible_pos);
            dots[i].erased = true;
        }
    }

    if (button_pressed(mouse_pos, button)) {
        blue_erase(dots, red_erased, blue_erased, possible_pos);
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
    srand(time(NULL));

    InitWindow(X,Y, "dot2dot");
    SetTargetFPS(60);       

    Dot dots[21];

    int blue_erased[21][2];
    int red_erased[21][2];

    init_points(dots, possible_pos);

	Rectangle button = {945, Y - 50, 15, 15};

    bool end_game = false;

    while (!WindowShouldClose() && !end_game) {
        Vector2 mouse_pos = GetMousePosition();

        if (who_won())
            end_game = true;

        n_erased = 0;
        erase(mouse_pos, button, dots, red_erased, blue_erased, possible_pos);


        BeginDrawing();
        ClearBackground(GRAY);
        DrawText("dot2dot game - erase dots in rows!", 50, 10, 50, BLACK);

        draw_points(dots);

        red_lines(red_erased);
        blue_lines(blue_erased);
        
		DrawRectangleRounded(button, 0.8, 40, RED);

        EndDrawing();
    }
    CloseWindow();

    return 0;
}
