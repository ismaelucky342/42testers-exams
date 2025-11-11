#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/wait.h>

int ft_popen(const char *file, char *const argv[], char type) 
{
    int fd[2];      // Array para los file descriptors del pipe: fd[0] lectura, fd[1] escritura
    pid_t pid;      // ID del proceso hijo

    // Validar parámetros: file y argv no nulos, type 'r' o 'w', y crear pipe
    if (!file || !argv || (type != 'r' && type != 'w') || pipe(fd) == -1)
        return -1;  // Error en validación o al crear pipe

    if ((pid = fork()) == -1) {     // Crear proceso hijo
        close(fd[0]);               // Si fork falla, cerrar ambos extremos del pipe
        close(fd[1]);
        return -1;                  // Retornar error
    }
    if (!pid) {                     // Código del proceso hijo
        // Si type='r': duplicar fd[1] (escritura) a STDOUT - el padre leerá la salida del comando
        // Si type='w': duplicar fd[0] (lectura) a STDIN - el padre escribirá la entrada al comando
        if (dup2(type == 'r' ? fd[1] : fd[0], type == 'r' ? STDOUT_FILENO : STDIN_FILENO) == -1)
            exit(1);                // Error al duplicar file descriptor
        close(fd[0]);               // Cerrar ambos extremos originales del pipe
        close(fd[1]);
        execvp(file, argv);         // Ejecutar el comando
        exit(1);                    // Si execvp falla, salir con error
    }
    // Código del proceso padre
    // Si type='r': cerrar fd[1] (escritura) porque el padre solo leerá
    // Si type='w': cerrar fd[0] (lectura) porque el padre solo escribirá
    close(type == 'r' ? fd[1] : fd[0]);
    // Retornar el fd que el padre usará: fd[0] para leer, fd[1] para escribir
    return type == 'r' ? fd[0] : fd[1];
}

int ft_popen(const char *file, char *const argv[], char type) 
{
    int fd[2]; 
    pid_t pid; 

    if(!file || !argv || (type != 'r' && type != 'w') || pipe(fd) == -1)
        return -1; 
    if((pid = fork()) == -1){
        close(fd[0]);
        close(fd[1]); 
        return -1; 
    }
    if(!pid)
    {
        if(dup2(type == 'r' ? fd[1] : fd[0], type == 'r' ? STDOUT_FILENO : STDIN_FILENO) == -1)
        close(fd[0]);
        close(fd[1]); 
        execvp(file, argv); 
        exit (1); 
    }
    close(type == 'r' ? fd[1] : fd[0]); 
    return type == 'r' ? fd[0] : fd[1];
}