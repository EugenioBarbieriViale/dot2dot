#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <raylib.h>

#define X 1000
#define Y 800

#define N 40
#define border 100
#define R 5

float rand_float() {
	return (float)rand() / (float)RAND_MAX;
}

float pos[N][2];
void init_pos() {
	for (int i=0; i<N; i++)	{
		float x = border + rand_float() * (X - border*2);
		float y = border + rand_float() * (Y - border*2);

		pos[i][0] = x;
		pos[i][1] = y;
	}
}

void draw_points() {
	for (int i=0; i<N; i++)	{
		float x = pos[i][0];
		float y = pos[i][1];

		DrawCircle(x, y, R, BLACK);
		if (x <= 0 || x >= X || y <= 0 || y >= Y) printf("ERROR at %d\n", i);

		DrawRectangleLines(border, border, X - 2*border, Y - 2*border, RED);
		/* DrawText(TextFormat("%d", i), x+4, y+4, 30, RED); */
	}
}

bool clicked(float x, float y) {
	Vector2 mouse_pos = GetMousePosition();
	Vector2 current_pos;
	current_pos.x = x;
	current_pos.y = y;
	if (CheckCollisionPointCircle(mouse_pos, current_pos, R) && IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) return true;
	else return false;
}

int main() {
	InitWindow(X,Y, "points");
	SetTargetFPS(60);		

	srand(time(NULL));
	init_pos();

	int size = 20;
	Rectangle yellow = {border, Y - border - size, size, size};
	Rectangle blue = {X - border - size, border, size, size};

	float yellow_path[N][2] = {
		{yellow.x + size / 2, yellow.y + size / 2}
	};

	float blue_path[N][2] = {
		{blue.x - size / 2, yellow.y + size / 2}
	};

	int yellow_nodes = 1;
	int blue_nodes = 1;

	while (!WindowShouldClose()) {
		for (int i=0; i<N; i++)	{
			float x = pos[i][0];
			float y = pos[i][1];
			if (clicked(x,y)) {
				printf("x %f, y %f\n", x,y);
				yellow_path[yellow_nodes][0] = x;
				yellow_path[yellow_nodes][1] = y;

				yellow_nodes++;
			}
		}

		BeginDrawing();
		ClearBackground(GRAY);

		draw_points();
		DrawRectangleRec(yellow, YELLOW);
		DrawRectangleRec(blue, BLUE);

		for (int i=1; i<yellow_nodes; i++)	{
			DrawLine(yellow_path[i-1][0], yellow_path[i-1][1], yellow_path[i][0], yellow_path[i][1], YELLOW);
		}

		EndDrawing();
	}
}
