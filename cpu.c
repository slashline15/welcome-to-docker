#include <stdio.h>
#include <math.h>

#define MEMORY_SIZE 2001  // For range -10 to 10 with 0.01 step
#define PI 3.14159265358979323846

double memory[MEMORY_SIZE];
double wave1_memory[MEMORY_SIZE];
double wave2_memory[MEMORY_SIZE];

void generate_cos_wave(double *wave, int sign) {
    for (int i = 0; i < MEMORY_SIZE; i++) {
        double x = -10.0 + (20.0 * i) / (MEMORY_SIZE - 1);
        wave[i] = sign * cos(x);
    }
}

void generate_quadratic(double *wave, int sign) {
    for (int i = 0; i < MEMORY_SIZE; i++) {
        double x = -10.0 + (20.0 * i) / (MEMORY_SIZE - 1);
        wave[i] = sign * (-x * x + 1);
    }
}

void generate_cubic(double *wave, int sign) {
    for (int i = 0; i < MEMORY_SIZE; i++) {
        double x = -10.0 + (20.0 * i) / (MEMORY_SIZE - 1);
        wave[i] = sign * (x * x * x - x);
    }
}

void generate_quartic(double *wave, int sign) {
    for (int i = 0; i < MEMORY_SIZE; i++) {
        double x = -10.0 + (20.0 * i) / (MEMORY_SIZE - 1);
        wave[i] = sign * (x * x * x * x - x * x * x + x * x - x + 1);
    }
}

void generate_quintic(double *wave, int sign) {
    for (int i = 0; i < MEMORY_SIZE; i++) {
        double x = -10.0 + (20.0 * i) / (MEMORY_SIZE - 1);
        wave[i] = sign * (2 * x * x * x * x * x - x * x * x + x * x - x);
    }
}

void add_waves(int choice1, int sign1, int choice2, int sign2) {
    for (int i = 0; i < MEMORY_SIZE; i++) {
        memory[i] = wave1_memory[i] + wave2_memory[i];
    }
}

void find_global_extrema() {
    double max_y = memory[0];
    double min_y = memory[0];
    int max_index = 0;
    int min_index = 0;
    for (int i = 1; i < MEMORY_SIZE; i++) {
        if (memory[i] > max_y) {
            max_y = memory[i];
            max_index = i;
        }
        if (memory[i] < min_y) {
            min_y = memory[i];
            min_index = i;
        }
    }
    double max_x = -10.0 + (20.0 * max_index) / (MEMORY_SIZE - 1);
    double min_x = -10.0 + (20.0 * min_index) / (MEMORY_SIZE - 1);
    printf("Global maximum detected at: x = %.2f, y = %.2f\n", max_x, max_y);
    printf("Global minimum detected at: x = %.2f, y = %.2f\n", min_x, min_y);
}

void print_wave(const char* name, double *wave) {
    printf("%s:\n", name);
    for (int i = 0; i < MEMORY_SIZE; i += MEMORY_SIZE / 20) {
        double x = -10.0 + (20.0 * i) / (MEMORY_SIZE - 1);
        printf("x = %.2f, y = %.2f\n", x, wave[i]);
    }
    printf("\n");
}

int main() {
    int choice1, sign1, choice2, sign2;
    printf("Select first function:\n");
    printf("1. Cosine Wave\n");
    printf("2. Quadratic Function\n");
    printf("3. Cubic Function\n");
    printf("4. Quartic Function\n");
    printf("5. Quintic Function\n");
    scanf("%d", &choice1);
    printf("Select sign for the first function (1 for positive, -1 for negative):\n");
    scanf("%d", &sign1);

    printf("Select second function (or 0 for none):\n");
    printf("0. None\n");
    printf("1. Cosine Wave\n");
    printf("2. Quadratic Function\n");
    printf("3. Cubic Function\n");
    printf("4. Quartic Function\n");
    printf("5. Quintic Function\n");
    scanf("%d", &choice2);
    if (choice2 != 0) {
        printf("Select sign for the second function (1 for positive, -1 for negative):\n");
        scanf("%d", &sign2);
    }

    if (choice1 < 1 || choice1 > 5 || choice2 < 0 || choice2 > 5 || (sign1 != 1 && sign1 != -1) || (choice2 != 0 && sign2 != 1 && sign2 != -1)) {
        printf("Invalid choice\n");
        return 1;
    }

    switch(choice1) {
        case 1: generate_cos_wave(wave1_memory, sign1); break;
        case 2: generate_quadratic(wave1_memory, sign1); break;
        case 3: generate_cubic(wave1_memory, sign1); break;
        case 4: generate_quartic(wave1_memory, sign1); break;
        case 5: generate_quintic(wave1_memory, sign1); break;
    }
    print_wave("First Function", wave1_memory);

    if (choice2 != 0) {
        switch(choice2) {
            case 1: generate_cos_wave(wave2_memory, sign2); break;
            case 2: generate_quadratic(wave2_memory, sign2); break;
            case 3: generate_cubic(wave2_memory, sign2); break;
            case 4: generate_quartic(wave2_memory, sign2); break;
            case 5: generate_quintic(wave2_memory, sign2); break;
        }
        print_wave("Second Function", wave2_memory);
        add_waves(choice1, sign1, choice2, sign2);
        print_wave("Combined Wave", memory);
    } else {
        print_wave("Selected Function", wave1_memory);
        for (int i = 0; i < MEMORY_SIZE; i++) {
            memory[i] = wave1_memory[i];
        }
    }

    find_global_extrema();
    return 0;
}
//now we just need to decompose to get the zeros of the quantic functions...