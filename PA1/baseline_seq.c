#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <errno.h>

static unsigned long long sum_upto(unsigned long long N) {
    unsigned long long s = 0;
    for (unsigned long long i = 0; i < N; ++i) s += i;
    return s;
}

int main(int argc, char **argv) {
    if (argc < 2) { fprintf(stderr, "usage: %s N\n", argv[0]); return 1; }
    char *end = NULL; errno = 0;
    unsigned long long N = strtoull(argv[1], &end, 10);
    if (errno || end == argv[1] || *end) { fprintf(stderr, "invalid N\n"); return 1; }

    struct timespec t0, t1;
    clock_gettime(CLOCK_MONOTONIC, &t0);
    unsigned long long s = sum_upto(N);
    clock_gettime(CLOCK_MONOTONIC, &t1);

    double dt = (t1.tv_sec - t0.tv_sec) + (t1.tv_nsec - t0.tv_nsec) / 1e9;
    printf("N=%llu, sum=%llu, time=%.6f s\n", N, s, dt);
    return 0;
}

