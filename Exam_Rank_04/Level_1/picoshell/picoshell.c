#include <unistd.h>
#include <stdlib.h>
#include <sys/wait.h>

int picoshell(char **cmds[]) {
    pid_t pid;                          // ID del proceso hijo
    int fd[2], last_fd = -1, i = 0;    // fd[2]: pipe actual, last_fd: lectura del pipe anterior, i: índice

    while (cmds[i]) {                   // Iterar por cada comando
        int has_next = cmds[i + 1] != NULL;  // Verificar si hay más comandos después
        if (has_next && pipe(fd) == -1) // Crear pipe solo si hay siguiente comando
            return 1;                   // Error al crear pipe

        if ((pid = fork()) == -1) {     // Crear proceso hijo
            if (has_next) close(fd[0]), close(fd[1]);  // Cerrar pipe si se creó
            return 1;                   // Error al hacer fork
        }
        if (!pid) {                     // Código del proceso hijo
            // Redirigir stdin desde el pipe anterior (si existe)
            if (last_fd != -1 && dup2(last_fd, STDIN_FILENO) == -1) exit(1);
            // Redirigir stdout al pipe actual (si hay siguiente comando)
            if (has_next && dup2(fd[1], STDOUT_FILENO) == -1) exit(1);

            if (last_fd != -1) close(last_fd);          // Cerrar fd anterior ya duplicado
            if (has_next) close(fd[0]), close(fd[1]);  // Cerrar ambos extremos del pipe actual

            execvp(cmds[i][0], cmds[i]);  // Ejecutar el comando
            exit(1);                      // Si execvp falla, salir con error
        }

        // Código del proceso padre
        if (last_fd != -1) close(last_fd);           // Cerrar el fd de lectura del pipe anterior
        last_fd = has_next ? fd[0] : -1;            // Guardar fd de lectura del pipe actual para el siguiente
        if (has_next) close(fd[1]);                 // Cerrar extremo de escritura (lo usa el hijo)
        i++;                                         // Avanzar al siguiente comando
    }

    while (wait(NULL) > 0);             // Esperar a que todos los hijos terminen
    return 0;                           // Éxito
}