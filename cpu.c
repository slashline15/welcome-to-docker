#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#define MEMORY_SIZE 2001  // For range -10 to 10 with 0.01 step
#define PI 3.14159265358979323846

double memory[MEMORY_SIZE];

void generate_cos_wave() {
    for (int i = 0; i < MEMORY_SIZE; i++) {
        double x = -10.0 + (20.0 * i) / (MEMORY_SIZE - 1);
        memory[i] = cos(x);
    }
}

void generate_quadratic() {
    for (int i = 0; i < MEMORY_SIZE; i++) {
        double x = -10.0 + (20.0 * i) / (MEMORY_SIZE - 1);
        memory[i] = -x * x + 1;  // Changed to -x*x + 1
    }
}

void add_waves() {
    for (int i = 0; i < MEMORY_SIZE; i++) {
        double cosine = memory[i];
        double x = -10.0 + (20.0 * i) / (MEMORY_SIZE - 1);
        double quadratic = -x * x + 1;  // -x*x + 1
        memory[i] = cosine + quadratic;  // Add cosine and quadratic
    }
}

void find_global_peak() {
    double max_y = memory[0];
    int max_index = 0;
    for (int i = 1; i < MEMORY_SIZE; i++) {
        if (memory[i] > max_y) {
            max_y = memory[i];
            max_index = i;
        }
    }
    double x = -10.0 + (20.0 * max_index) / (MEMORY_SIZE - 1);
    printf("Global peak detected at: x = %.2f, y = %.2f\n", x, max_y);
}

double calculate_area() {
    double area = 0.0;
    double dx = 20.0 / (MEMORY_SIZE - 1);
    
    for (int i = 0; i < MEMORY_SIZE - 1; i++) {
        double y1 = memory[i];
        double y2 = memory[i + 1];
        area += 0.5 * (y1 + y2) * dx;
    }
    
    return area;
}

void print_wave(const char* name) {
    printf("%s:\n", name);
    for (int i = 0; i < MEMORY_SIZE; i += MEMORY_SIZE / 20) {
        double x = -10.0 + (20.0 * i) / (MEMORY_SIZE - 1);
        printf("x = %.2f, y = %.2f\n", x, memory[i]);
    }
    printf("\n");
}

int main() {
    generate_cos_wave();
    print_wave("Cosine Wave");
    
    generate_quadratic();
    print_wave("Quadratic Function");
    
    add_waves();
    print_wave("Combined Wave");
    
    find_global_peak();
    
    double area = calculate_area();
    printf("Area under the curve: %.4f\n", area);
    
    return 0;
}