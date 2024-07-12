#include <stdio.h>
#include <stdlib.h>
#include <time.h>

int ans[2];

int random_int(int range) {
    return (rand() % range);
}

int count_non_zero(int arr[]) {
    int c = 0;
    for (int i=0; i<6; i++) {
        if (arr[i] != 0) {
            printf("%d\n", arr[i]);
            c++;
        }
    }
    return c;
}

void choose_random(int pos[][2]) {
    int chosen_y = pos[random_int(21)][1];
    int possible_xs[6] = {0,0,0,0,0,0};

    int c = 0;
    for (int i = 0; i<21; i++) {
        if (pos[i][1] == chosen_y) {
            possible_xs[c] = pos[i][0];
            c++;
        }
    }
    /* for (int i=0; i<6; i++) printf("%d\n", possible_xs[i]); */

    int j = random_int(count_non_zero(possible_xs));
    printf("J %d\n", j);
    int chosen_x = possible_xs[j];

    ans[0] = chosen_x;
    ans[1] = chosen_y;
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

    choose_random(pos);
    /* printf("%d %d", ans[0], ans[1]); */

    return 0;
}
