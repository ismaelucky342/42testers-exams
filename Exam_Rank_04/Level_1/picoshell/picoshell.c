#include <unistd.h>
#include <stdlib.h>
#include <sys/wait.h>

int picoshell(char **cmds[]) {
    pid_t pid;
    int fd[2], last_fd = -1, i = 0;

    while (cmds[i]) {
        int has_next = cmds[i + 1] != NULL;
        if (has_next && pipe(fd) == -1)
            return 1;

        if ((pid = fork()) == -1) {
            if (has_next) close(fd[0]), close(fd[1]);
            return 1;
        }
        if (!pid) { // hijo
            if (last_fd != -1 && dup2(last_fd, STDIN_FILENO) == -1) exit(1);
            if (has_next && dup2(fd[1], STDOUT_FILENO) == -1) exit(1);

            if (last_fd != -1) close(last_fd);
            if (has_next) close(fd[0]), close(fd[1]);

            execvp(cmds[i][0], cmds[i]);
            exit(1);
        }

        if (last_fd != -1) close(last_fd);
        last_fd = has_next ? fd[0] : -1;
        if (has_next) close(fd[1]);
        i++;
    }

    while (wait(NULL) > 0);
    return 0;
}