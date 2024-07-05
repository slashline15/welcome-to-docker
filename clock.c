//this is the newer one 
#include <stdio.h>
#include <math.h>
#include <sys/time.h>
#include <unistd.h>

#define MEMORY_SIZE 2001  // For range -10 to 10 with 0.01 step
#define PI 3.14159265358979323846

double memory[MEMORY_SIZE];
double wave1_memory[MEMORY_SIZE];
double wave2_memory[MEMORY_SIZE];

double tanh_scaled(double x) {
    return tanh(x / 10.0);
}

void generate_tanh_wave(double *wave, int sign) {
    for (int i = 0; i < MEMORY_SIZE; i++) {
        double x = tanh_scaled(-10.0 + (20.0 * i) / (MEMORY_SIZE - 1));
        wave[i] = sign * tanh(x);
    }
}

void generate_cos_wave(double *wave, int sign) {
    for (int i = 0; i < MEMORY_SIZE; i++) {
        double x = tanh_scaled(-10.0 + (20.0 * i) / (MEMORY_SIZE - 1));
        wave[i] = sign * cos(x);
    }
}

void generate_quadratic(double *wave, int sign) {
    for (int i = 0; i < MEMORY_SIZE; i++) {
        double x = tanh_scaled(-10.0 + (20.0 * i) / (MEMORY_SIZE - 1));
        wave[i] = sign * (-x * x + 1);
    }
}

void generate_cubic(double *wave, int sign) {
    for (int i = 0; i < MEMORY_SIZE; i++) {
        double x = tanh_scaled(-10.0 + (20.0 * i) / (MEMORY_SIZE - 1));
        wave[i] = sign * (x * x * x - x);
    }
}

void generate_quartic(double *wave, int sign) {
    for (int i = 0; i < MEMORY_SIZE; i++) {
        double x = tanh_scaled(-10.0 + (20.0 * i) / (MEMORY_SIZE - 1));
        wave[i] = sign * (x * x * x * x - x * x * x + x * x - x );
    }
}

void generate_quintic(double *wave, int sign) {
    for (int i = 0; i < MEMORY_SIZE; i++) {
        double x = tanh_scaled(-10.0 + (20.0 * i) / (MEMORY_SIZE - 1));
        wave[i] = sign * (2 * x * x * x * x * x - 2* x * x * x + x * x - 4* x + 1);
    }
}

void generate_linear(double *wave, int sign) {
    for (int i = 0; i < MEMORY_SIZE; i++) {
        double x = tanh_scaled(-10.0 + (20.0 * i) / (MEMORY_SIZE - 1));
        wave[i] = sign * x;
    }
}

void generate_zero(double *wave) {
    for (int i = 0; i < MEMORY_SIZE; i++) {
        wave[i] = 0;
    }
}

void add_waves(double *wave1, double *wave2) {
    for (int i = 0; i < MEMORY_SIZE; i++) {
        memory[i] = wave1[i] + wave2[i];
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
    double max_x = tanh_scaled(-10.0 + (20.0 * max_index) / (MEMORY_SIZE - 1));
    double min_x = tanh_scaled(-10.0 + (20.0 * min_index) / (MEMORY_SIZE - 1));
    printf("Global maximum detected at: x = %.2f, y = %.10f\n", max_x, max_y);
    printf("Global minimum detected at: x = %.2f, y = %.10f\n", min_x, min_y);
}

void find_global_zeros() {
    printf("Global zeros detected at:\n");
    for (int i = 1; i < MEMORY_SIZE; i++) {
        if ((memory[i - 1] > 0 && memory[i] < 0) || (memory[i - 1] < 0 && memory[i] > 0)) {
            double x1 = tanh_scaled(-10.0 + (20.0 * (i - 1)) / (MEMORY_SIZE - 1));
            double x2 = tanh_scaled(-10.0 + (20.0 * i) / (MEMORY_SIZE - 1));
            double zero_x = (x1 * x2); // Approximate zero crossing
            printf("x = %.2f\n", zero_x);
        }
    }
}

void print_wave(const char* name, double *wave) {
    printf("%s:\n", name);
    for (int i = 0; i < MEMORY_SIZE; i += MEMORY_SIZE / 20) {
        double x = tanh_scaled(-10.0 + (20.0 * i) / (MEMORY_SIZE - 1));
        printf("x = %.2f, y = %.10f\n", x, wave[i]);
    }
    printf("\n");
}

void display_time_using_tanh_wave() {
    struct timeval tv;
    struct tm *time_info;
    char buffer[20];

    while (1) {
        gettimeofday(&tv, NULL);
        time_info = localtime(&tv.tv_sec);
        strftime(buffer, sizeof(buffer), "%H:%M:%S", time_info);

        int milliseconds = (tv.tv_sec % 60) * 1000 + tv.tv_usec / 1000;
        double x = tanh_scaled(-10.0 + (20.0 * milliseconds) / 59999.0);  // Normalize milliseconds to range -10 to 10
        double y_tanh = tanh(x / 10.0);

        // Format time as HH:MM:SS.mmm
        printf("\rCurrent Time: %s.%03d, Tanh Value: %.10f", buffer, tv.tv_usec / 1000, y_tanh);
        fflush(stdout);
        usleep(1000);  // Sleep for 1 millisecond
    }
}

int main() {
    int choice1, sign1, choice2, sign2;
    printf("Select first function:\n");
    printf("1. Cosine Wave\n");
    printf("2. Quadratic Function\n");
    printf("3. Cubic Function\n");
    printf("4. Quartic Function\n");
    printf("5. Quintic Function\n");
    printf("6. Tanh Wave\n");
    printf("7. Linear Function\n");
    printf("8. Zero Function\n");
    scanf("%d", &choice1);
    if (choice1 != 8) { // No sign needed for zero function
        printf("Select sign for the first function (1 for positive, -1 for negative):\n");
        scanf("%d", &sign1);
    }

    printf("Select second function (or 0 for none):\n");
    printf("0. None\n");
    printf("1. Cosine Wave\n");
    printf("2. Quadratic Function\n");
    printf("3. Cubic Function\n");
    printf("4. Quartic Function\n");
    printf("5. Quintic Function\n");
    printf("6. Tanh Wave\n");
    printf("7. Linear Function\n");
    printf("8. Zero Function\n");
    scanf("%d", &choice2);
    if (choice2 != 0 && choice2 != 8) { // No sign needed for zero function
        printf("Select sign for the second function (1 for positive, -1 for negative):\n");
        scanf("%d", &sign2);
    }

    if (choice1 < 1 || choice1 > 8 || choice2 < 0 || choice2 > 8 || (choice1 != 8 && (sign1 != 1 && sign1 != -1)) || (choice2 != 0 && choice2 != 8 && (sign2 != 1 && sign2 != -1))) {
        printf("Invalid choice\n");
        return 1;
    }

    switch(choice1) {
        case 1: generate_cos_wave(wave1_memory, sign1); break;
        case 2: generate_quadratic(wave1_memory, sign1); break;
        case 3: generate_cubic(wave1_memory, sign1); break;
        case 4: generate_quartic(wave1_memory, sign1); break;
        case 5: generate_quintic(wave1_memory, sign1); break;
        case 6: generate_tanh_wave(wave1_memory, sign1); break;
        case 7: generate_linear(wave1_memory, sign1); break;
        case 8: generate_zero(wave1_memory); break;
    }
    print_wave("First Function", wave1_memory);

    if (choice2 != 0) {
        switch(choice2) {
            case 1: generate_cos_wave(wave2_memory, sign2); break;
            case 2: generate_quadratic(wave2_memory, sign2); break;
            case 3: generate_cubic(wave2_memory, sign2); break;
            case 4: generate_quartic(wave2_memory, sign2); break;
            case 5: generate_quintic(wave2_memory, sign2); break;
            case 6: generate_tanh_wave(wave2_memory, sign2); break;
            case 7: generate_linear(wave2_memory, sign2); break;
            case 8: generate_zero(wave2_memory); break;
        }
        print_wave("Second Function", wave2_memory);
        add_waves(wave1_memory, wave2_memory);
        print_wave("Combined Wave", memory);
    } else {
        print_wave("Selected Function", wave1_memory);
        for (int i = 0; i < MEMORY_SIZE; i++) {
            memory[i] = wave1_memory[i];
        }
    }

    find_global_extrema();
    find_global_zeros();

    // Uncomment this to display the hyperbolic tangent clock in parallel
    display_time_using_tanh_wave();

    return 0;
}
