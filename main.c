#include <stdio.h>
#include <stdlib.h>
#include <raylib.h>

#define X 1000
#define Y 800

void draw_points() {
	int n = 6;
	for (int j=0; j<6; j++) {
		for (int i=0; i<n; i++) {
			DrawCircleLines(300 + 80*i, 200 + 70*j, 25, BLACK);
		}
		n--;
	}
}

int main() {
	InitWindow(X,Y, "Erma");
	SetTargetFPS(60);		

	int px1 = 0;
	int py1 = 0;

	while (!WindowShouldClose()) {
		int MX = GetMouseX();
		int MY = GetMouseY();

		BeginDrawing();
		ClearBackground(GRAY);

		if (IsMouseButtonPressed(0)) {
			px1 = MX;
			py1 = MY;
		}

		draw_points();
		DrawLine(MX, MY, px1, py1, BLACK);

		EndDrawing();
	}
}
