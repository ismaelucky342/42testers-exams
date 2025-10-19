#include "vbc.h"

char *s;

void unexpected(char c) {
    c ? printf("Unexpected token '%c'\n", c) : printf("Unexpected end of file\n");
}

int ft_sum(void);

int ft_factor() {
    if (isdigit(*s)) return *s++ - '0';
    if (*s == '(') { s++; int val = ft_sum(); (*s == ')') ? s++ : 0; return val; }
    return 0;
}

int ft_product() {
    int a = ft_factor();
    while (*s == '*') { s++; a *= ft_factor(); }
    return a;
}

int ft_sum() {
    int a = ft_product();
    while (*s == '+') { s++; a += ft_product(); }
    return a;
}

int check_input(char *str) {
    int par = 0, i = 0; char last = 0;
    while (str[i]) {
        par += (str[i] == '(') - (str[i] == ')');
        if (!isdigit(str[i]) && str[i] != '+' && str[i] != '*' && str[i] != '(' && str[i] != ')')
            return unexpected(str[i]), 1;
        if (isdigit(str[i]) && isdigit(str[i+1]))
            return unexpected(str[i+1]), 1;
        last = str[i++];
    }
    if (par) return unexpected(par > 0 ? '(' : ')'), 1;
    if (last == '+' || last == '*') return unexpected(0), 1;
    return 0;
}

int main(int argc, char **argv) {
    return (argc != 2 || check_input(argv[1])) ? 1 : (s = argv[1], printf("%d\n", ft_sum()), 0);
}