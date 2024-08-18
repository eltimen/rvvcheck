#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <unistd.h>

#include <sys/types.h>
#include <sys/wait.h>


void check_scalar() {
    puts("scalar");
}

void check_rvv071() {
    const int32_t vec[] = {1,2};
    __asm__(
        "add a0, x0, %0 \n" // a0 = vec
        "addi a1, x0, %1 \n" // a1 = length of vec
        ".byte 0xd7, 0xf2, 0x85, 0x00 \n" // vsetvli t0, a1, e32 // init vector
        ".byte 0x07, 0x60, 0x05, 0x12 \n" // vlw.v v0, (a0) // v0 = {vec[0], vec[1]}
        :
        :"r"(vec), "n"(sizeof(vec) / sizeof(vec[0]))
    );
    puts("rvv0.7.1");
}

void check_rvv10() {
    const int32_t vec[] = {1,2};
    __asm__(
        "add a0, x0, %0 \n" // a0 = vec
        "addi a1, x0, %1 \n" // a1 = length of vec
        ".byte 0xd7, 0xf2, 0x05, 0x01 \n" // vsetvli t0, a1, e32 // init vector
        ".byte 0x07, 0x60, 0x05, 0x02 \n" // vle32.v v0, (a0) // v0 = {vec[0], vec[1]}
        :
        :"r"(vec), "n"(sizeof(vec) / sizeof(vec[0]))
    );
    puts("rvv1.0");
}

int main() {
    void (*check_funcs[])() = {check_scalar, check_rvv071, check_rvv10};
    size_t n_funcs = sizeof(check_funcs) / sizeof(check_funcs[0]);
    pid_t pids[n_funcs];

    for (size_t i = 0; i < n_funcs; ++i) {
        pids[i] = fork();
        if (pids[i] < 0) {
            perror("fork() failed");
            abort();
        } else if (pids[i] == 0) {
            check_funcs[i]();
            exit(0);
        }
    }

    for (size_t i = n_funcs; i > 0; --i) {
        int status;
        pid_t pid = wait(&status);
        if (WIFSIGNALED(status)) {
            printf("Child #%d PID %d exited with signal %s\n", i, pid, strsignal(WTERMSIG(status)));
        } else {
            printf("Child #%d PID %d exited with status %d\n", i, pid, status);
        }
    }

    return 0;
}