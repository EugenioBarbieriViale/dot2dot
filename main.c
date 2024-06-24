#include <stdio.h>
#include <stdlib.h>
#include <raylib.h>

#define X 1000
#define Y 800

# define R 40

int pos[21][3];

void draw_points() {
	int n = 6;
	int count = 0;

	for (int j=0; j<6; j++) {
		for (int i=0; i<n; i++) {
			int x = 300 + (2*R+20)*i;
			int y = 170 + (2*R+10)*j;

			DrawCircle(x, y, R, (Color){255,255,0,90});
			DrawText(TextFormat("%d", count+1), x - 6, y - 10, 20,WHITE);

			pos[count][0] = count;
			pos[count][1] = x;
			pos[count][2] = y;

			count++;
		}
		n--;
	}
}

void print_arr() {
	for (int i=0; i<21; i++) {
		printf("%d. x: %d  y: %d\n", pos[i][0], pos[i][1], pos[i][2]);
	}
}

int main() {
	InitWindow(X,Y, "Erma");
	SetTargetFPS(60);		

	while (!WindowShouldClose()) {
		BeginDrawing();
		ClearBackground(GRAY);

		DrawText("dot2dot game - erase dots in rows!", 50, 10, 50, BLACK);

		draw_points();

		EndDrawing();
	}
	print_arr();
}
