#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <unistd.h>
#include <sys/wait.h>
#include <time.h>
#include <errno.h>

static inline double elapsed(struct timespec a, struct timespec b){
    return (b.tv_sec - a.tv_sec) + (b.tv_nsec - a.tv_nsec)/1e9;
}

int main(int argc, char **argv){
    if (argc < 3) { fprintf(stderr,"usage: %s N NUM_TASKS\n", argv[0]); return 1; }

    char *e = NULL; errno = 0;
    unsigned long long N = strtoull(argv[1], &e, 10);
    if (errno || e == argv[1] || *e) { fprintf(stderr,"invalid N\n"); return 1; }

    errno = 0; e = NULL;
    long tmp = strtol(argv[2], &e, 10);
    if (errno || e == argv[2] || *e || tmp <= 0) { fprintf(stderr,"invalid NUM_TASKS\n"); return 1; }
    int P = (int)tmp;

    if (N % (unsigned long long)P != 0ULL) { fprintf(stderr,"N %% NUM_TASKS must be 0\n"); return 1; }
    unsigned long long part = N / (unsigned long long)P;

    int (*fd)[2] = malloc(P * sizeof *fd);
    FILE **rf = malloc(P * sizeof *rf);
    pid_t *kid = malloc(P * sizeof *kid);
    if (!fd || !rf || !kid) { fprintf(stderr,"alloc failed\n"); return 1; }

    struct timespec t0,t1;
    clock_gettime(CLOCK_MONOTONIC, &t0);

    for (int i = 0; i < P; ++i) {
        if (pipe(fd[i]) == -1) { perror("pipe"); return 1; }
        pid_t pid = fork();
        if (pid == -1) { perror("fork"); return 1; }
        if (pid == 0) {
            close(fd[i][0]);
            if (dup2(fd[i][1], STDOUT_FILENO) == -1) _exit(1);
            close(fd[i][1]);
            unsigned long long lo = (unsigned long long)i * part, hi = lo + part, s = 0;
            for (unsigned long long x = lo; x < hi; ++x) s += x;
            printf("%llu\n", s);
            fflush(stdout);
            _exit(0);
        } else {
            kid[i] = pid;
            close(fd[i][1]);
            rf[i] = fdopen(fd[i][0], "r");
            if (!rf[i]) { perror("fdopen"); return 1; }
        }
    }

    unsigned long long total = 0, piece = 0;
    for (int i = 0; i < P; ++i) {
        if (fscanf(rf[i], "%llu", &piece) != 1) { fprintf(stderr,"read fail %d\n", i); return 1; }
        total += piece;
        fclose(rf[i]);
    }
    for (int i = 0; i < P; ++i) waitpid(kid[i], NULL, 0);

    clock_gettime(CLOCK_MONOTONIC, &t1);
    printf("N=%llu, tasks=%d, sum=%llu, time=%.6f s\n", N, P, total, elapsed(t0,t1));

    free(fd); free(rf); free(kid);
    return 0;
}
