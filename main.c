#include <stdio.h>
#include <stdlib.h>
#include <raylib.h>

#define X 1000
#define Y 800

# define R 40

int pos[21][3];
int dots[21][2];

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
			pos[count][2] = count;

			count++;
		}
		n--;
	}
}

void draw_lines(int nodes, int erased[21][3]) {
	int c = 0;
	for (int i=1; i<nodes; i++)	{
		Vector2 p1 = {erased[i-1][0], erased[i-1][1]};
		Vector2 p2 = {erased[i][0], erased[i][1]};

		if (p1.x > p2.x) {
			p1.x += 3*R/4;
			p2.x -= 3*R/4;
		} else {
			p1.x -= 3*R/4;
			p2.x += 3*R/4;
		}

		/* printf("%d, %d\n", dots[i-1][0], dots[i-1][1]); */

		if (p1.y == p2.y && i%2==1) {
			dots[i-1][0] = erased[i-1][2];
			dots[i-1][1] = erased[i][2];

			DrawLineEx(p1, p2, 5, RED);
		}
	}
}

bool clicked(int x, int y) {
	Vector2 mouse_pos = GetMousePosition();
	Vector2 current_pos = {x,y};
	if (CheckCollisionPointCircle(mouse_pos, current_pos, R) && IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) return true;
	else return false;
}

int count_erased_dots() {
	printf("--------------\n");
	for (int i=0; i<21; i++) {
		printf("%d: %d, %d\n", i, dots[i][0], dots[i][1]);
	}
	return 0;
}

int main() {
	InitWindow(X,Y, "dot2dot");
	SetTargetFPS(60);		

	int nodes = 0;
	int erased[21][3];

	while (!WindowShouldClose()) {
		for (int i=0; i<21; i++)	{
			int x = pos[i][0];
			int y = pos[i][1];
			int c = pos[i][2];
			if (clicked(x,y)) {
				erased[nodes][0] = x;
				erased[nodes][1] = y;
				erased[nodes][2] = c;

				nodes++;
			}
		}

		BeginDrawing();
		ClearBackground(GRAY);
		DrawText("dot2dot game - erase dots in rows!", 50, 10, 50, BLACK);

		draw_points();
		draw_lines(nodes, erased);
		count_erased_dots();

		EndDrawing();
	}
}
