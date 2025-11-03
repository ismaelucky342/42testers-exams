#include "vbc.h"

char *s;  // Puntero global que recorre la expresión

// Función para imprimir mensajes de error de token inesperado
void unexpected(char c) {
    c ? printf("Unexpected token '%c'\n", c) : printf("Unexpected end of file\n");
}

int ft_sum(void);  // Declaración adelantada para uso en ft_factor

// Parsear un factor: dígito o expresión entre paréntesis
int ft_factor() {
    if (isdigit(*s)) return *s++ - '0';  // Si es dígito, convertir a int y avanzar
    if (*s == '(') {                      // Si es paréntesis de apertura
        s++;                              // Saltar '('
        int val = ft_sum();               // Evaluar expresión interna recursivamente
        (*s == ')') ? s++ : 0;            // Si hay ')', saltarlo
        return val;                       // Retornar valor de la expresión
    }
    return 0;  // Si no es ni dígito ni '(', retornar 0
}

// Parsear producto: factor o secuencia de factores multiplicados
int ft_product() {
    int a = ft_factor();                  // Obtener primer factor
    while (*s == '*') {                   // Mientras haya operador '*'
        s++;                              // Saltar '*'
        a *= ft_factor();                 // Multiplicar por siguiente factor
    }
    return a;                             // Retornar resultado del producto
}

// Parsear suma: producto o secuencia de productos sumados
int ft_sum() {
    int a = ft_product();                 // Obtener primer producto
    while (*s == '+') {                   // Mientras haya operador '+'
        s++;                              // Saltar '+'
        a += ft_product();                // Sumar siguiente producto
    }
    return a;                             // Retornar resultado de la suma
}

// Validar la sintaxis de la expresión
int check_input(char *str) {
    int par = 0, i = 0; char last = 0;    // par: contador de paréntesis, last: último carácter
    while (str[i]) {
        par += (str[i] == '(') - (str[i] == ')');  // Incrementar por '(', decrementar por ')'
        // Verificar que el carácter es válido (dígito, +, *, ( o ))
        if (!isdigit(str[i]) && str[i] != '+' && str[i] != '*' && str[i] != '(' && str[i] != ')')
            return unexpected(str[i]), 1;  // Carácter inválido
        // Verificar que no hay dos dígitos consecutivos (multi-dígito no permitido)
        if (isdigit(str[i]) && isdigit(str[i+1]))
            return unexpected(str[i+1]), 1;  // Error: números de más de un dígito
        last = str[i++];                   // Guardar último carácter y avanzar
    }
    if (par) return unexpected(par > 0 ? '(' : ')'), 1;  // Paréntesis desbalanceados
    if (last == '+' || last == '*') return unexpected(0), 1;  // No puede terminar en operador
    return 0;  // Entrada válida
}

// Función principal
int main(int argc, char **argv) {
    // Verificar exactamente 1 argumento y validar entrada
    // Si es válido: asignar s, evaluar ft_sum(), imprimir resultado y retornar 0
    // Si no: retornar 1
    return (argc != 2 || check_input(argv[1])) ? 1 : (s = argv[1], printf("%d\n", ft_sum()), 0);
}