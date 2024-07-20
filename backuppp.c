#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <raylib.h>

/* TODO:
 * + make single func that draws lines
 * + machine makes moves randomly
*/

#define X 1000
#define Y 800

# define R 40

int pos[21][2];

int human_erased[21][2];
int machine_erased[21][2];
int n_erased;

int possible_pos[21][2];

int human_nodes = 0;
int machine_nodes = 0;

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

void draw_points(void) {
    int n = 6;
    int count = 0;

    for (int j=0; j<6; j++) {
        for (int i=0; i<n; i++) {
            int x = 300 + (2*R+20)*i;
            int y = 170 + (2*R+10)*j;

            DrawCircle(x, y, R, (Color){255,255,0,90});
            DrawText(TextFormat("%d", count), x - 6, y - 10, 20,WHITE);

            pos[count][0] = x;
            pos[count][1] = y;

            possible_pos[count][0] = x;
            possible_pos[count][1] = y;

            count++;
        }
        n--;
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

bool collision(int x1, int x2, int x3, int x4) {
    if (x1 < x2)
        if ((x3 >= x1 && x3 <= x2) || (x4 >= x1 && x4 <= x2))
            return true;
    else 
        if ((x3 >= x2 && x3 <= x1) || (x4 >= x2 && x4 <= x1))
            return true;

    return false;
}

bool already_erased() {
    for (int i=0; i<21; i+=2) {
        for (int j=0; j<21; j+=2) {
            if (human_erased[i][1] != 0 && machine_erased[i][1] != 0 && machine_erased[j+1][1] != 0)
                if (machine_erased[i][1] == human_erased[j][1])
                    if (collision(machine_erased[i][0], machine_erased[i+1][0], human_erased[j][0], human_erased[j+1][0]))
                        return true;
        }
    }
    return false;
}

int random_int(int range) {
    return (rand() % range);
}

void possible_pos_macine() {
    for (int i=0; i<machine_nodes; i+=2) {
        for (int j=0; j<machine_nodes; j++) {
            int cx = pos[j][0];
            int xi = machine_erased[i][0];
            int xf = machine_erased[i+1][0];

            if (machine_erased[i][1] == pos[j][1]) {
                if ((cx >= xi && cx <= xf) || (cx >= xf && cx <= xi)) {
                    possible_pos[j][0] = 0;
                    possible_pos[j][1] = 0;
                }
            }
        }
    }
}

int get_y(void) {
    return possible_pos[random_int(21)][1];
}

int get_x(int y) {
    for (int i=0; i<100; i++) {
        int index = random_int(21);
        if (possible_pos[index][1] == y)
            return possible_pos[index][0];
    }
    return 0;
}

void update_turn(void) {
    temp++;
    if (temp % 2 == 0) turn++;
}

void erase_human(int x, int y) {
    if (turn % 2 == 0) {
        human_erased[human_nodes][0] = x;
        human_erased[human_nodes][1] = y;
        human_nodes++;

        update_turn();
    }
}

void machine_erase(void) {
    if (turn % 2 == 1) {
        int rand_y = get_y();

        if (machine_nodes % 2 == 0)
            machine_erased[machine_nodes][1] = rand_y;
        else
            machine_erased[machine_nodes][1] = machine_erased[machine_nodes - 1][1];

        int rand_x = get_x(rand_y);
        machine_erased[machine_nodes][0] = rand_x;

        machine_nodes++;

        update_turn();
    }
}

void erase(Vector2 mouse_pos, Rectangle button) {
    for (int i=0; i<21; i++) {
        int x = pos[i][0];
        int y = pos[i][1];

        if (clicked(x,y)) {
            erase_human(x, y);
        }
    }

    if (button_pressed(mouse_pos, button)) {
        machine_erase();
    }
}

void human_lines(void) {
    for (int i=1; i<human_nodes; i++)   {
        Vector2 p1 = {human_erased[i-1][0], human_erased[i-1][1]};
        Vector2 p2 = {human_erased[i][0], human_erased[i][1]};

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

void machine_lines(void) {
    for (int i=1; i<machine_nodes; i++) {
        Vector2 p1 = {machine_erased[i-1][0], machine_erased[i-1][1]};
        Vector2 p2 = {machine_erased[i][0], machine_erased[i][1]};

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

void print_lines(void) {
    printf("h %d  m %d\n", human_nodes, machine_nodes);
    printf("-----------------------------\n");
    for (int i = 0; i<21; i++)
        printf("%d: h %d %d  m %d %d\n", i, human_erased[i][0], human_erased[i][1], machine_erased[i][0], machine_erased[i][1]);
}

int main() {
    srand(time(NULL));

    InitWindow(X,Y, "dot2dot");
    SetTargetFPS(60);       

	Rectangle button = {945, Y - 50, 15, 15};

    bool end_game = false;

    while (!WindowShouldClose() && !end_game) {
        Vector2 mouse_pos = GetMousePosition();

         who_won()
        if (already_erased())
            printf("COLLISION\n");

        n_erased = 0;
        erase(mouse_pos, button);

        BeginDrawing();
        ClearBackground(GRAY);
        DrawText("dot2dot game - erase dots in rows!", 50, 10, 50, BLACK);

        draw_points();
        human_lines();
        machine_lines();
        
		DrawRectangleRounded(button, 0.8, 40, RED);

        EndDrawing();
    }
    CloseWindow();
}