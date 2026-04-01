#define _POSIX_C_SOURCE 200112L

#include <stdio.h>
#include <stdlib.h>
#include <time.h>

typedef struct {
    float position;
    float velocity;
} Point;

static long long now_ms(void) {
    struct timespec ts;
    clock_gettime(CLOCK_MONOTONIC, &ts);
    return (long long)ts.tv_sec * 1000LL + (long long)ts.tv_nsec / 1000000LL;
}

int main(int argc, char** argv)
{
    const float DELTA_TIME = 1.f / 60.f;
    const float POSITION_LIMIT = 1000.f;

    if (argc < 3) return 1;

    size_t numPoints = (size_t)atoi(argv[1]);
    size_t numIters  = (size_t)atoi(argv[2]);

    Point* points = (Point*)malloc(numPoints * sizeof(Point));
    if (!points) {
        perror("malloc");
        return 1;
    }

    /* init random */
    srand((unsigned)time(NULL));
    for (size_t i = 0; i < numPoints; ++i) {
        points[i].position = (float)(rand() % 100);
        points[i].velocity = (float)(rand() % 1000) / 100.0f;
    }

    printf("Starting %zu update loops of %zu points...", numIters, numPoints);
    fflush(stdout);

    long long startTime = now_ms();

    for (size_t i = 0; i < numIters; ++i) {
        for (size_t j = 0; j < numPoints; ++j) {
            Point* p = &points[j];

            p->position += p->velocity * DELTA_TIME;

            if ((p->position < 0.f && p->velocity < 0.f) ||
                (p->position > POSITION_LIMIT && p->velocity > 0.f))
            {
                p->velocity *= -1.f;
            }
        }
    }

    long long endTime = now_ms();
    long long milliseconds = endTime - startTime;

    printf(" ran for %lldms\n", milliseconds);

    free(points);
    return 0;
}
