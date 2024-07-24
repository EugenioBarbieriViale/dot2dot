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

void init_points(int pos[21][2], int possible_pos[21][2]) {
    int n = 6;
    int count = 0;

    for (int j=0; j<6; j++) {
        for (int i=0; i<n; i++) {
            int x = 300 + (2*R+20)*i;
            int y = 170 + (2*R+10)*j;

            pos[count][0] = x;
            pos[count][1] = y;

            possible_pos[count][0] = x;
            possible_pos[count][1] = y;

            count++;
        }
        n--;
    }
}

void alloc_erased(int red_erased[21][2], int blue_erased[21][2]) {
    for (int i=0; i<21; i++) {
        red_erased[i][0] = 0;
        red_erased[i][1] = 0;

        blue_erased[i][0] = 0;
        blue_erased[i][1] = 0;
    }
}


void draw_points(int pos[21][2]) {
    for (int i=0; i<21; i++) {
        int x = pos[i][0];
        int y = pos[i][1];

        DrawCircle(x, y, R, (Color){255,255,0,90});
        DrawText(TextFormat("%d", i), x - 6, y - 10, 20,WHITE);
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

bool collision(int x1, int x2, int x3, int x4) {
    if (x1 < x2)
        if ((x3 >= x1 && x3 <= x2) || (x4 >= x1 && x4 <= x2))
            return true;
    else 
        if ((x3 >= x2 && x3 <= x1) || (x4 >= x2 && x4 <= x1))
            return true;
    return false;
}

bool already_erased(int red_erased[21][2], int blue_erased[21][2]) {
    for (int i=0; i<21; i+=2) {
        for (int j=0; j<21; j+=2) {
            if (red_erased[i][1] != 0 && blue_erased[i][1] != 0 && blue_erased[j+1][1] != 0) {
            /* if (red_nodes > 0 && blue_nodes > 0) { */
                if (blue_erased[i][1] == red_erased[j][1]) {
                    /* if (collision(blue_erased[i][0], blue_erased[i+1][0], red_erased[j][0], red_erased[j+1][0]) || collision(red_erased[i][0], red_erased[i+1][0], blue_erased[j][0], blue_erased[j+1][0])) */
                    if (collision(blue_erased[i][0], blue_erased[i+1][0], red_erased[j][0], red_erased[j+1][0]))
                            return true;
                }
            }
        }
    }
    return false;
}

void possible_positions(int pos[21][2], int red_erased[21][2], int blue_erased[21][2], int possible_pos[21][2]) {
    for (int i=0; i<blue_nodes; i+=2) {
        for (int j=0; j<21; j++) {
            int cx = pos[j][0];
            
            int xmi = blue_erased[i][0];
            int xmf = blue_erased[i+1][0];

            if (blue_erased[i][1] == pos[j][1]) {
                if ((cx >= xmi && cx <= xmf) || (cx >= xmf && cx <= xmi)) {
                    possible_pos[j][0] = 0;
                    possible_pos[j][1] = 0;
                }
            } 
        }
    }

    for (int i=0; i<red_nodes; i+=2) {
        for (int j=0; j<21; j++) {
            int cx = pos[j][0];

            int xhi = red_erased[i][0];
            int xhf = red_erased[i+1][0];

            if (red_erased[i][1] == pos[j][1]) {
                if ((cx >= xhi && cx <= xhf) || (cx >= xhf && cx <= xhi)) {
                    possible_pos[j][0] = 0;
                    possible_pos[j][1] = 0;
                }
            }
        }
    }
}

int random_int(int range) {
    return (rand() % range);
}

int get_y(int possible_pos[21][2]) {
    for (int i=0; i<50; i++) {
        int ans = possible_pos[random_int(21)][1];
        if (ans != 0)
            return ans;
    }
    return 0;
}

int get_x(int y, int possible_pos[21][2]) {
    int ans = 0;
    for (int i=0; i<100; i++) {
        int index = random_int(21);

        if (possible_pos[index][1] == y) {
            return possible_pos[index][0];
        }
    }
    return 0;
}
/* int get_x(int y, int possible_pos[21][2]) { */
/*     int c = 0; */
/*     int ans[6]; */
/*     for (int i=0; i<21; i++) { */
/*         if (possible_pos[i][1] == y) { */
/*             ans[c] = possible_pos[i][0]; */
/*             c++; */
/*         } */
/*     } */
/*     return ans[random_int(c)]; */
/* } */

void update_turn(void) {
    temp++;
    if (temp % 2 == 0) turn++;
}

void red_erase(int x, int y, int pos[21][2], int red_erased[21][2], int blue_erased[21][2], int possible_pos[21][2]) {
    if (turn % 2 == 0) {
        possible_positions(pos, red_erased, blue_erased, possible_pos);

        red_erased[red_nodes][0] = x;
        red_erased[red_nodes][1] = y;
        red_nodes++;

        update_turn();
    }
}

void blue_erase(int pos[21][2], int red_erased[21][2], int blue_erased[21][2], int possible_pos[21][2]) {
    if (turn % 2 == 1) {
        possible_positions(pos, red_erased, blue_erased, possible_pos);

        int rand_y = get_y(possible_pos);

        if (blue_nodes % 2 == 0)
            blue_erased[blue_nodes][1] = rand_y;
        else
            blue_erased[blue_nodes][1] = blue_erased[blue_nodes - 1][1];

        int rand_x = get_x(rand_y, possible_pos);
        blue_erased[blue_nodes][0] = rand_x;

        blue_nodes++;

        update_turn();
    }
}

void erase(Vector2 mouse_pos, Rectangle button, int pos[21][2], int red_erased[21][2], int blue_erased[21][2], int possible_pos[21][2]) {
    for (int i=0; i<21; i++) {
        int x = pos[i][0];
        int y = pos[i][1];

        if (clicked(mouse_pos, x, y)) {
            red_erase(x, y, pos, red_erased, blue_erased, possible_pos);
        }
    }

    if (button_pressed(mouse_pos, button)) {
        blue_erase(pos, red_erased, blue_erased, possible_pos);
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

void print_lines(int pos[21][2], int possible_pos[21][2], int blue_erased[21][2]) {
    printf("---------------------\n");
    for (int i = 0; i<21; i++) {
        printf("%d: poss %d %d --- pos %d %d", i, possible_pos[i][0], possible_pos[i][1], pos[i][0], pos[i][1]);
        printf(" --- erased: %d %d\n", blue_erased[i][0], blue_erased[i][1]);
    }

    /* for (int i = 0; i<blue_nodes; i++) { */
    /*     printf("%d: %d %d\n", i, blue_erased[i][0], blue_erased[i][1]); */
    /* } */
}

int main() {
    srand(time(NULL));

    InitWindow(X,Y, "dot2dot");
    SetTargetFPS(60);       

    int pos[21][2];
    int possible_pos[21][2];

    int blue_erased[21][2];
    int red_erased[21][2];

    init_points(pos, possible_pos);
    alloc_erased(red_erased, blue_erased);

	Rectangle button = {945, Y - 50, 15, 15};

    bool end_game = false;

    while (!WindowShouldClose() && !end_game) {
        Vector2 mouse_pos = GetMousePosition();
        print_lines(pos, possible_pos, blue_erased);

        if (who_won())
            end_game = true;

        if (already_erased(red_erased, blue_erased)) {
            printf("COLLISION\n");
            end_game = true;
        }

        n_erased = 0;
        erase(mouse_pos, button, pos, red_erased, blue_erased, possible_pos);


        BeginDrawing();
        ClearBackground(GRAY);
        DrawText("dot2dot game - erase dots in rows!", 50, 10, 50, BLACK);

        draw_points(pos);

        red_lines(red_erased);
        blue_lines(blue_erased);
        
		DrawRectangleRounded(button, 0.8, 40, RED);

        EndDrawing();
    }
    CloseWindow();

    return 0;
}
