#define _POSIX_C_SOURCE 200112L

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <time.h>

typedef struct {
    int x;
    int y;
    int z;
    int bias;
} Sample;

static long long now_ms(void) {
    struct timespec ts;
    clock_gettime(CLOCK_MONOTONIC, &ts);
    return (long long)ts.tv_sec * 1000LL + (long long)ts.tv_nsec / 1000000LL;
}

static inline int clamp_int(int v, int lo, int hi) {
    if (v < lo) return lo;
    if (v > hi) return hi;
    return v;
}

void init_samples(Sample *a, size_t n) {
    for (size_t i = 0; i < n; ++i) {
        int base = (int)(i * 3 + 7);
        a[i].x    =  (base % 50) - 25;
        a[i].y    = ((base * 2) % 60) - 30;
        a[i].z    = ((base * 5) % 40) - 20;
        a[i].bias =  (int)(i % 8) - 4;
    }
}

void process_samples(const Sample *a, size_t n, int *energy, int *score) {
    for (size_t i = 0; i < n; ++i) {
        int x = a[i].x;
        int y = a[i].y;
        int z = a[i].z;
        int b = a[i].bias;

        // Normes
        int e = x * x + y * y + z * z;

        // Calcul d'un score abitraire
        int s = (x + y - z) * 3 + b * 5;

        // Clamp foncièrement inutile mais intéressant pour l'exercice
        s = clamp_int(s, -1000, 1000);

        energy[i] = e;
        score[i]  = s;
    }
}

void process_samples_simd(/*TODO*/){
    // TODO : implémenter une version SIMD de process_samples
    return;
}

void compare_arrays(const int *a, const int *b, size_t n, const char *name) {
    for (size_t i = 0; i < n; ++i) {
        if (a[i] != b[i]) {
            printf("Mismatch in %s at index %zu: %d != %d\n", name, i, a[i], b[i]);
            return;
        }
    }
    printf("Arrays %s match.\n", name);
}

int main(int argc, char** argv) {
    const size_t n = 300000;
    Sample *samples  = (Sample*)malloc(n * sizeof(Sample));
    int *energy      = (int*)malloc(n * sizeof(int));
    int *score       = (int*)malloc(n * sizeof(int));

    if (!samples || !energy || !score) {
        perror("malloc");
        free(samples);
        free(energy);
        free(score);
        return 1;
    }

    init_samples(samples, n);

    long long startTime = now_ms();
    process_samples(samples, n, energy, score);
    long long endTime = now_ms();
    long long endProcessNormal = endTime - startTime;

    startTime = now_ms();
    process_samples_simd(/*TODO*/);
    endTime = now_ms();
    long long endProcessSimd = endTime - startTime;

    printf("Processing time (normal) : %lld ms\n", endProcessNormal);
    printf("Processing time (SIMD)   : %lld ms\n", endProcessSimd);

    // compare_arrays(energy, <Your array>, n, "energy");

    free(samples);
    free(energy);
    free(score);
    return 0;
}