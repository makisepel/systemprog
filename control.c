/*
    control.c는 프로세스를 관리할 수 있는 코드이며
    nice값의 조정을 통해 우선순위를 바꾸거나 kill 함수를 통해 프로세스를 강제종료 할 수 있습니다.
*/

#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <signal.h>
#include <sys/resource.h>
#include <unistd.h>

/**
 * @brief Nice 값을 상승시켜 우선순위를 낮추는 함수
 * 
 * @param pid: Process ID.
 * @return: 0 on success, -1 on failure.
 */
int increase_nice(int pid) {
    int current_nice = getpriority(PRIO_PROCESS, pid);
    if (current_nice == -1 && errno != 0) return -1;
    if (setpriority(PRIO_PROCESS, pid, current_nice + 1) == -1) return -1;
    return 0;
}

/**
 * @brief Nice 값을 하락시켜 우선순위를 높이는 함수
 * 
 * @param pid: Process ID.
 * @return: 0 on success, -1 on failure.
 */
int decrease_nice(int pid) {
    int current_nice = getpriority(PRIO_PROCESS, pid);
    if (current_nice == -1 && errno != 0) return -1;
    if (setpriority(PRIO_PROCESS, pid, current_nice - 1) == -1) return -1;
    return 0;
}

/**
 * @brief 해당 PID의 프로세스에 대해 kill 명령을 보내는 함수
 * 
 * @param pid: Process ID.
 * @return: 0 on success, -1 on failure.
 */
int kill_process(int pid) {
    if (kill(pid, SIGKILL) == -1) return -1;
    return 0;
}