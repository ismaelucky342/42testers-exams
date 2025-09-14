#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <signal.h>
#include <errno.h>
#include <sys/wait.h>
#include <sys/types.h>
#include <string.h>

// Variable global para el PID del proceso hijo
static pid_t child_pid;

// Manejador de señal para SIGALRM (timeout)
void alarm_handler(int sig)
{
    /*
     * MANEJADOR DE TIMEOUT:
     * - Se ejecuta cuando el alarm() expira
     * - No necesita hacer nada especial
     * - Su existencia hace que waitpid() retorne con EINTR
     */
    (void)sig;  // Suprimir warning de parámetro no usado
}

int sandbox(void (*f)(void), unsigned int timeout, bool verbose)
{
    /*
     * PARÁMETROS:
     * - f: Función a probar
     * - timeout: Tiempo límite en segundos
     * - verbose: Si imprimir mensajes de diagnóstico
     * 
     * RETORNO:
     * - 1: función "buena" (exit code 0, sin signals, sin timeout)
     * - 0: función "mala" (exit code != 0, signal, o timeout)
     * - -1: error en sandbox (fork falló, etc.)
     */
    
    struct sigaction sa;
    pid_t pid;
    int status;
    
    /*
     * CONFIGURAR MANEJADOR DE SIGALRM:
     * - Configurar manejador personalizado para timeout
     * - Limpiar máscara de señales
     * - No reiniciar syscalls automáticamente
     */
    sa.sa_handler = alarm_handler;
    sa.sa_flags = 0;  // No SA_RESTART: queremos que waitpid sea interrumpido
    sigemptyset(&sa.sa_mask);
    sigaction(SIGALRM, &sa, NULL);
    
    /*
     * FORK PROCESO HIJO:
     */
    pid = fork();
    if (pid == -1)
        return -1;  // Error en fork
    
    if (pid == 0)  // PROCESO HIJO
    {
        /*
         * EJECUTAR FUNCIÓN EN HIJO:
         * - Llamar a la función proporcionada
         * - Si retorna normalmente, salir con código 0
         * - Si hace segfault/abort, el kernel enviará señal
         */
        f();
        exit(0);  // Función terminó normalmente
    }
    
    // PROCESO PADRE
    child_pid = pid;
    
    /*
     * ESTABLECER TIMEOUT:
     * - alarm() envía SIGALRM después de timeout segundos
     * - Esto interrumpirá waitpid() si la función toma demasiado tiempo
     */
    alarm(timeout);
    
    /*
     * ESPERAR AL PROCESO HIJO:
     * - waitpid() puede retornar por varias razones:
     *   1. El hijo terminó normalmente (exit)
     *   2. El hijo fue terminado por una señal
     *   3. waitpid fue interrumpido por SIGALRM (timeout)
     */
    if (waitpid(pid, &status, 0) == -1)
    {
        if (errno == EINTR)  // Interrumpido por SIGALRM
        {
            /*
             * TIMEOUT DETECTADO:
             * - waitpid fue interrumpido por alarm
             * - El hijo probablemente sigue ejecutándose
             * - Matarlo con SIGKILL y recoger su estado
             */
            kill(pid, SIGKILL);
            waitpid(pid, NULL, 0);  // Recoger proceso zombie
            
            if (verbose)
                printf("Bad function: timed out after %d seconds\n", timeout);
            return 0;
        }
        return -1;  // Otro tipo de error
    }
    
    /*
     * ANALIZAR CÓMO TERMINÓ EL PROCESO:
     */
    
    if (WIFEXITED(status))
    {
        /*
         * TERMINACIÓN NORMAL (exit):
         * - El proceso llamó exit() o retornó de main
         * - Verificar el código de salida
         */
        if (WEXITSTATUS(status) == 0)
        {
            if (verbose)
                printf("Nice function!\n");
            return 1;  // Función buena
        }
        else
        {
            if (verbose)
                printf("Bad function: exited with code %d\n", WEXITSTATUS(status));
            return 0;  // Función mala
        }
    }
    
    if (WIFSIGNALED(status))
    {
        /*
         * TERMINACIÓN POR SEÑAL:
         * - El proceso fue terminado por una señal (segfault, abort, etc.)
         * - Obtener el número de señal para diagnóstico
         */
        int sig = WTERMSIG(status);
        if (verbose)
            printf("Bad function: %s\n", strsignal(sig));
        return 0;  // Función mala
    }
    
    return -1;  // Estado no reconocido
}
