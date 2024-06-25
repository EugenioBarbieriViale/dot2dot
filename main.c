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

#define max_letters 10

int *arr_eq_to(int arr[max_letters]) {
	int ans[max_letters];
	for (int i=0; i<max_letters; i++) ans[i] = arr[i];
	return ans;
}

int main() {
	InitWindow(X,Y, "Erma");
	SetTargetFPS(60);		

	char input[max_letters];
	int *numbs[max_letters];
	int letter_count = 0;

	int frames_count = 0;

	Rectangle box = {40, Y - 70, 330, 50};
	bool on_text = false;

	while (!WindowShouldClose()) {
		if (CheckCollisionPointRec(GetMousePosition(), box)) on_text = true;
        else on_text = false;

		if (on_text) {
			SetMouseCursor(MOUSE_CURSOR_IBEAM);
			int key = GetCharPressed();

			while (key > 0) {
				if ((key >= 32) && (key <= 125) && letter_count < max_letters) {
					input[letter_count] = (char)key;
					input[letter_count + 1] = '\0';
					letter_count++;
				}
				key = GetCharPressed();
			}
			if (IsKeyPressed(KEY_BACKSPACE)) {
				letter_count--;
				if (letter_count < 0) letter_count = 0;
				input[letter_count] = '\0';
			} else if (IsKeyPressed(KEY_ENTER)) numbs = arr_eq_to(input);
		} else SetMouseCursor(MOUSE_CURSOR_DEFAULT);

		if (on_text) frames_count++;
		else frames_count = 0;

		BeginDrawing();
		ClearBackground(GRAY);
		DrawText("dot2dot game - erase dots in rows!", 50, 10, 50, BLACK);

		DrawRectangleRec(box, LIGHTGRAY);

		if (on_text) DrawRectangleLines((int)box.x, (int)box.y, (int)box.width, (int)box.height, RED);
        else DrawRectangleLines((int)box.x, (int)box.y, (int)box.width, (int)box.height, DARKGRAY);

		DrawText(input, (int)box.x + 5, (int)box.y + 8, 40, MAROON);

		draw_points();
		get_coords(1, 2);

		EndDrawing();
	}
	/* print_arr(); */
	/* for (int i=0; i<2; i++) printf("%d, %d\n", coords[i][0], coords[i][1]); */
	for (int i=0; i<max_letters; i++) printf("%c\n", numbs[i]);
}
