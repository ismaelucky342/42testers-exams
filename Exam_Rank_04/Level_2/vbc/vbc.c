#include "vbc.h"

char *s;

void unexpected(char c) { printf("Unexpected %s\n", c ? (char[]){'\'', c, '\0'} : "end of file"); }

int ft_factor() { return isdigit(*s) ? *s++ - '0' : (*s == '(' ? (s++, ft_sum(), s++, 0) : 0); }

int ft_product() { int a = ft_factor(), b; while(*s == '*') s++, a *= (b = ft_factor()); return a; }

int ft_sum() { int a = ft_product(), b; while(*s == '+') s++, a += (b = ft_product()); return a; }

int check_input(char *str) {
    int par = 0, i = 0; char last = 0;
    while(str[i]) {
        par += (str[i] == '(') - (str[i] == ')');
        if(!isdigit(str[i]) && str[i] != '+' && str[i] != '*' && str[i] != '(' && str[i] != ')')
            return unexpected(str[i]), 1;
        if(isdigit(str[i]) && isdigit(str[i+1]))
            return unexpected(str[i+1]), 1;
        last = str[i++];
    }
    return par ? unexpected(par>0?'(':')'), 1 : last=='+'||last=='*'?unexpected(0),1:0;
}

int main(int argc, char **argv) {
    if(argc!=2 || check_input(argv[1])) return 1;
    s = argv[1];
    printf("%d\n", ft_sum());
}
