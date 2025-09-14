#include <unistd.h>
#include <stdlib.h>
#include <sys/wait.h>

int picoshell(char **cmds[])
{
    /*
     * PARÁMETROS:
     * - cmds: Array de arrays de strings (comandos)
     * - cmds[0] = {"ls", "-l", NULL}
     * - cmds[1] = {"grep", "txt", NULL}  
     * - cmds[2] = NULL (terminador)
     * 
     * RETORNO:
     * - 0: todos los comandos ejecutados exitosamente
     * - 1: error en algún comando o syscall
     */
    
    pid_t pid;
    int pipefd[2];      // Pipe actual
    int prev_fd = -1;   // Descriptor del pipe anterior
    int status;
    int exit_code = 0;
    int i = 0;
    
    /*
     * BUCLE PRINCIPAL:
     * Procesar cada comando en la pipeline
     */
    while (cmds[i])
    {
        /*
         * CREAR PIPE (excepto para el último comando):
         * - Solo crear pipe si hay un comando siguiente
         * - Este pipe conectará el comando actual con el siguiente
         */
        if (cmds[i + 1] && pipe(pipefd) == -1)
            return 1;
        
        /*
         * FORK PROCESO PARA EL COMANDO ACTUAL:
         */
        pid = fork();
        if (pid == -1)
        {
            // Error en fork, cerrar descriptores creados
            if (cmds[i + 1])
            {
                close(pipefd[0]);
                close(pipefd[1]);
            }
            return 1;
        }
        
        if (pid == 0)  // PROCESO HIJO
        {
            /*
             * CONFIGURACIÓN DEL STDIN DEL HIJO:
             * - Si hay prev_fd, el comando debe leer del pipe anterior
             * - Redirigir stdin al read end del pipe anterior
             */
            if (prev_fd != -1)
            {
                if (dup2(prev_fd, STDIN_FILENO) == -1)
                    exit(1);
                close(prev_fd);
            }
            
            /*
             * CONFIGURACIÓN DEL STDOUT DEL HIJO:
             * - Si hay comando siguiente, el comando debe escribir al pipe actual
             * - Redirigir stdout al write end del pipe actual
             */
            if (cmds[i + 1])
            {
                close(pipefd[0]);  // Cerrar read end (no lo necesitamos)
                if (dup2(pipefd[1], STDOUT_FILENO) == -1)
                    exit(1);
                close(pipefd[1]);
            }
            
            /*
             * EJECUTAR COMANDO:
             * - cmds[i][0] es el nombre del comando
             * - cmds[i] es el array completo de argumentos
             */
            execvp(cmds[i][0], cmds[i]);
            exit(1);  // Solo se ejecuta si execvp falla
        }
        
        // PROCESO PADRE
        /*
         * GESTIÓN DE DESCRIPTORES EN EL PADRE:
         * - Cerrar prev_fd si existe (ya no lo necesitamos)
         * - Para el pipe actual:
         *   - Cerrar write end (lo usa el hijo)
         *   - Guardar read end como prev_fd para el siguiente comando
         */
        
        if (prev_fd != -1)
            close(prev_fd);
        
        if (cmds[i + 1])
        {
            close(pipefd[1]);     // Cerrar write end
            prev_fd = pipefd[0];  // Guardar read end para siguiente iteración
        }
        
        i++;
    }
    
    /*
     * ESPERAR A TODOS LOS PROCESOS HIJOS:
     * - Usar wait() para recoger todos los procesos
     * - Verificar códigos de salida
     * - Si algún proceso falla, retornar error
     */
    while (wait(&status) != -1)
    {
        if (WIFEXITED(status) && WEXITSTATUS(status) != 0)
            exit_code = 1;
    }
    
    return exit_code;
}