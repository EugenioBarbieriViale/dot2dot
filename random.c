#include <stdio.h>
#include <stdlib.h>
#include <time.h>

int random_int(int range) {
    return (rand() % range);
}

int count_non_zero(int arr[]) {
    int c = 0;
    for (int i=0; i<6; i++) {
        if (arr[i] != 0) {
            c++;
        }
    }
    return c;
}

int random_y(int pos[][2]) {
    return pos[random_int(21)][1];
}

int random_x(int pos[][2]) {
    int possible_xs[6] = {0,0,0,0,0,0};

    int c = 0;
    for (int i = 0; i<21; i++) {
        if (pos[i][1] == random_y(pos)) {
            possible_xs[c] = pos[i][0];
            c++;
        }
    }

    int j = random_int(count_non_zero(possible_xs));
    return possible_xs[j];
}

int main() {
    srand(time(NULL));

    int pos[][2] = {
        {300, 170},
        {400, 170},
        {500, 170},
        {600, 170},
        {700, 170},
        {800, 170},
        {300, 260},
        {400, 260},
        {500, 260},
        {600, 260},
        {700, 260},
        {300, 350},
        {400, 350},
        {500, 350},
        {600, 350},
        {300, 440},
        {400, 440},
        {500, 440},
        {300, 530},
        {400, 530},
        {300, 620}
    };

    int x1 = random_x(pos);
    int y1 = random_y(pos);

    int x2 = random_x(pos);
    int y2 = random_y(pos);

    printf("%d %d\n", x1, y1);
    printf("%d %d\n", x2, y2);

    return 0;
}
