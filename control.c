#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <signal.h>
#include <sys/resource.h>
#include <unistd.h>

/**
 * Increase the NICE value of a process (lower priority).
 * @param pid: Process ID.
 * @return: 0 on success, -1 on failure.
 */
int increase_nice(int pid) {
    int current_nice = getpriority(PRIO_PROCESS, pid);
    if (current_nice == -1 && errno != 0) {
        perror("Failed to get NICE value");
        return -1;
    }

    // Attempt to increase the NICE value
    if (setpriority(PRIO_PROCESS, pid, current_nice + 1) == -1) {
        perror("Failed to increase NICE value");
        return -1;
    }

    printf("Successfully increased NICE value of PID %d to %d\n", pid, current_nice + 1);
    return 0;
}

/**
 * Decrease the NICE value of a process (higher priority).
 * @param pid: Process ID.
 * @return: 0 on success, -1 on failure.
 */
int decrease_nice(int pid) {
    int current_nice = getpriority(PRIO_PROCESS, pid);
    if (current_nice == -1 && errno != 0) {
        perror("Failed to get NICE value");
        return -1;
    }

    // Attempt to decrease the NICE value
    if (setpriority(PRIO_PROCESS, pid, current_nice - 1) == -1) {
        perror("Failed to decrease NICE value");
        return -1;
    }

    printf("Successfully decreased NICE value of PID %d to %d\n", pid, current_nice - 1);
    return 0;
}

/**
 * Kill the specified process.
 * @param pid: Process ID.
 * @return: 0 on success, -1 on failure.
 */
int kill_process(int pid) {
    if (kill(pid, SIGKILL) == -1) {
        perror("Failed to kill process");
        return -1;
    }

    printf("Successfully killed process with PID %d\n", pid);
    return 0;
}
