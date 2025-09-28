#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <pthread.h>
#include <time.h>
#include <errno.h>

typedef struct { unsigned long long begin, count; } Chunk;

static void* worker(void *arg) {
    Chunk *c = (Chunk*)arg;
    unsigned long long *ret = malloc(sizeof(*ret));
    if (!ret) pthread_exit(NULL);
    unsigned long long s = 0, end = c->begin + c->count;
    for (unsigned long long i = c->begin; i < end; ++i) s += i;
    *ret = s;
    return ret;
}

static inline double elapsed(struct timespec a, struct timespec b) {
    return (b.tv_sec - a.tv_sec) + (b.tv_nsec - a.tv_nsec) / 1e9;
}

int main(int argc, char **argv) {
    if (argc < 3) { fprintf(stderr, "usage: %s N NUM_THREADS\n", argv[0]); return 1; }

    char *endp = NULL; errno = 0;
    unsigned long long N = strtoull(argv[1], &endp, 10);
    if (errno || endp == argv[1] || *endp) { fprintf(stderr, "invalid N\n"); return 1; }

    errno = 0; endp = NULL;
    long req = strtol(argv[2], &endp, 10);
    if (errno || endp == argv[2] || *endp || req <= 0) { fprintf(stderr, "invalid NUM_THREADS\n"); return 1; }
    int T = (int)req;

    if (N % (unsigned long long)T != 0ULL) { fprintf(stderr, "N %% NUM_THREADS must be 0\n"); return 1; }
    unsigned long long chunk = N / (unsigned long long)T;

    pthread_t *th = malloc(T * sizeof(*th));
    Chunk *ch = malloc(T * sizeof(*ch));
    if (!th || !ch) { fprintf(stderr, "alloc failed\n"); return 1; }

    for (int k = 0; k < T; ++k) { ch[k].begin = (unsigned long long)k * chunk; ch[k].count = chunk; }

    struct timespec t0, t1;
    clock_gettime(CLOCK_MONOTONIC, &t0);
    for (int k = 0; k < T; ++k)
        if (pthread_create(&th[k], NULL, worker, &ch[k]) != 0) { fprintf(stderr, "pthread_create %d\n", k); return 1; }

    unsigned long long total = 0;
    for (int k = 0; k < T; ++k) {
        void *ret = NULL;
        if (pthread_join(th[k], &ret) != 0) { fprintf(stderr, "pthread_join %d\n", k); return 1; }
        if (ret) { total += *(unsigned long long*)ret; free(ret); }
    }
    clock_gettime(CLOCK_MONOTONIC, &t1);

    printf("N=%llu, threads=%d, sum=%llu, time=%.6f s\n", N, T, total, elapsed(t0, t1));
    free(th); free(ch);
    return 0;
}
