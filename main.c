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
			DrawText(TextFormat("%d", count), x - 6, y - 10, 20,WHITE);

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

int coords[2][2];
int get_coords(int min, int max) {
	if (pos[min][2] == pos[max][2]) {
		coords[0][0] = pos[min][1];
		coords[0][1] = pos[min][2];

		coords[1][0] = pos[max][1];
		coords[1][1] = pos[max][2];
		return 1;
	} else return 0;
}

int main() {
	InitWindow(X,Y, "Erma");
	SetTargetFPS(60);		

	while (!WindowShouldClose()) {
		BeginDrawing();
		ClearBackground(GRAY);

		DrawText("dot2dot game - erase dots in rows!", 50, 10, 50, BLACK);

		draw_points();
		get_coords(1, 2);

		EndDrawing();
	}
	print_arr();
	for (int i=0; i<2; i++) printf("%d, %d\n", coords[i][0], coords[i][1]);
}
