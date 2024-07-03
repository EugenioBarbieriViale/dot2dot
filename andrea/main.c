#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <raylib.h>

#define X 1000
#define Y 800

#define N 40
#define border 100
#define R 5

const int size = 20;
const Rectangle yellow = {border, Y - border - size, size, size};
const Rectangle blue = {X - border - size, border, size, size};

float pos[N][2];

float yellow_path[N][2] = {
	{yellow.x + size / 2, yellow.y + size / 2}
};

float blue_path[N][2] = {
	{blue.x + size / 2, blue.y + size / 2}
};

int yellow_nodes = 1;
int blue_nodes = 1;

int turn = 0;

float rand_float() {
	return (float)rand() / (float)RAND_MAX;
}

int choose_turn(int turn);

void init_pos(void);
void draw_points(void);

bool clicked(float x, float y);
void create_path(void);
void draw_lines(void);

bool check_collision(int counter);

bool game_over(void);


int main() {
	InitWindow(X,Y, "points");
	SetTargetFPS(60);		

	srand(time(NULL));
	init_pos();

	bool quit = false;
	while (!WindowShouldClose() && !quit) {
		create_path();

		BeginDrawing();
		ClearBackground(GRAY);

		choose_turn(turn);
		draw_points();
		draw_lines();


		DrawRectangleRec(yellow, YELLOW);
		DrawRectangleRec(blue, BLUE);

		DrawText(TextFormat("N: %d", N), X - 90, 10, 30, RED);
		
		quit = game_over();
		EndDrawing();
	}
	CloseWindow();
	return 0;
}

void init_pos(void) {
	for (int i=0; i<N; i++)	{
		float x = border + rand_float() * (X - border*2);
		float y = border + rand_float() * (Y - border*2);

		pos[i][0] = x;
		pos[i][1] = y;
	}
}

void draw_points(void) {
	for (int i=0; i<N; i++)	{
		float x = pos[i][0];
		float y = pos[i][1];

		DrawCircle(x, y, R, BLACK);

		DrawRectangleLines(border, border, X - 2*border, Y - 2*border, RED);
		/* DrawText(TextFormat("%d", i), x+4, y+4, 30, RED); */
	}
}

bool clicked(float x, float y) {
	Vector2 mouse_pos = GetMousePosition();
	Vector2 current_pos = {x, y};
	if (CheckCollisionPointCircle(mouse_pos, current_pos, R) && IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) return true;
	else return false;
}

bool game_over(void) {
	Vector2 mouse_pos = GetMousePosition();

	if (CheckCollisionPointRec(mouse_pos, yellow) && IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) {
		DrawText("BLUE WON", 200,200, 100, BLUE);
		return true;
	}
	else if (CheckCollisionPointRec(mouse_pos, blue) && IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) {
		DrawText("YELLOW WON", 200,200, 100, YELLOW);
		return true;
	}
	else return false;
}

int choose_turn(int turn) {
	if (turn % 2 == 0) {
		DrawText("YELLOW'S TURN", 10, 10, 30, BLACK);
		return 0;
	} else {
		DrawText("BLUE'S TURN", 10, 10, 30, BLACK);
		return 1;
	}
}

void create_path(void) {
	for (int i=0; i<N; i++)	{
		float x = pos[i][0];
		float y = pos[i][1];
		if (clicked(x,y)) {
			if (choose_turn(turn) == 0) {
				yellow_path[yellow_nodes][0] = x;
				yellow_path[yellow_nodes][1] = y;

				yellow_nodes++;
			} else {
				blue_path[blue_nodes][0] = x;
				blue_path[blue_nodes][1] = y;

				blue_nodes++;
			}
			if (check_collision(blue_nodes)) printf("Y%d B%d COLLISION\n", yellow_nodes, blue_nodes);
			turn++;
		}
	}
}

void draw_lines(void) {
	for (int i=1; i<yellow_nodes; i++) {
		DrawLine(yellow_path[i-1][0], yellow_path[i-1][1], yellow_path[i][0], yellow_path[i][1], YELLOW);
		/* if (check_collision(yellow_nodes, blue_nodes)) printf("%d COLLISION\n", i); */
	}
	for (int i=1; i<blue_nodes; i++) {
		DrawLine(blue_path[i-1][0], blue_path[i-1][1], blue_path[i][0], blue_path[i][1], BLUE);
	}
}

bool is_intersecting(Vector2 start1, Vector2 end1, Vector2 start2, Vector2 end2) {
	float denominator = ((end1.x - start1.x) * (end2.y - start2.y)) - ((end1.y - start1.y) * (end2.x - start2.x));
    float numerator1 = ((start1.y - start2.y) * (end2.x - start2.x)) - ((start1.x - start2.x) * (end2.y - start2.y));
    float numerator2 = ((start1.y - start2.y) * (end1.x - start1.x)) - ((start1.x - start2.x) * (end1.y - start1.y));

    if (denominator == 0) return numerator1 == 0 && numerator2 == 0;
    
    float r = numerator1 / denominator;
    float s = numerator2 / denominator;

    return (r > 0 && r < 1) && (s > 0 && s < 1);
}

bool check_collision(int counter) {
	Vector2 start_yellow = {yellow_path[counter-1][0], yellow_path[counter-1][1]};
	Vector2 end_yellow = {yellow_path[counter][0], yellow_path[counter][1]};

	Vector2 start_blue = {blue_path[counter-1][0], blue_path[counter-1][1]};
	Vector2 end_blue = {blue_path[counter][0], blue_path[counter][1]};
	/* Vector2 start_blue = {blue_path[blue_nodes-1][0], blue_path[blue_nodes-1][1]}; */
	/* Vector2 end_blue = {blue_path[blue_nodes][0], blue_path[blue_nodes][1]}; */
	/* printf("%f, %f\n", start_yellow.x, start_yellow.y); */
	return (is_intersecting(start_yellow, end_yellow, start_blue, end_blue));
}
