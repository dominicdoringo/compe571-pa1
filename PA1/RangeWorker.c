#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <errno.h>

int main(int argc, char **argv){
    if (argc < 3) { fprintf(stderr,"usage: %s lo hi\n", argv[0]); return 1; }
    char *e=NULL; errno=0;
    unsigned long long lo = strtoull(argv[1], &e, 10);
    if (errno || e==argv[1] || *e) return 1;
    errno=0; e=NULL;
    unsigned long long hi = strtoull(argv[2], &e, 10);
    if (errno || e==argv[2] || *e) return 1;
    unsigned long long s=0;
    for (unsigned long long x=lo; x<hi; ++x) s+=x;
    printf("%llu\n", s);
    return 0;
}
