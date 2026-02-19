#include <stdio.h>
#include <stdlib.h>
#include <string.h>
const char* ch;
#define SUCCESS 1
#define FAILED 0
/*
Grammar after removing Left Recursion:
E –> T E’
E’ –> + T E’ | ε
T –> F T’
T’ –> * F T’ | ε
F –> ( E ) | id
*/
int E(), E_dash(), T(), T_dash(), F();
int E() {
    printf("%-16s E -> T E'\n", ch);
    if (T()) {
        if (E_dash())
            return SUCCESS;
    }
    return FAILED;
}
int E_dash() {
    if (*ch == '+') {
        printf("%-16s E' -> + T E'\n", ch);
        ch++;
        if (T()) {
            if (E_dash())
                return SUCCESS;
        }
        return FAILED;
    } else {
        printf("%-16s E' -> ε\n", ch);
        return SUCCESS;
    }
}
int T() {
    printf("%-16s T -> F T'\n", ch);
    if (F()) {
        if (T_dash())
            return SUCCESS;
    }
    return FAILED;
}
int T_dash() {
    if (*ch == '*') {
        printf("%-16s T' -> * F T'\n", ch);
        ch++;
        if (F()) {
            if (T_dash())
                return SUCCESS;
        }
        return FAILED;
    } else {
        printf("%-16s T' -> ε\n", ch);
        return SUCCESS;
    }
}
int F() {
    if (*ch == '(') {
        printf("%-16s F -> ( E )\n", ch);
        ch++;
        if (E()) {
            if (*ch == ')') {
                ch++;
                return SUCCESS;
            }
        }
        return FAILED;
    } else if (*ch == 'i' && *(ch + 1) == 'd') {
        printf("%-16s F -> id\n", ch);
        ch += 2;
        return SUCCESS;
    } else {
        return FAILED;
    }
}
int main() {
    char str[100];
    printf("Enter string (e.g., id+id*id): ");
    scanf("%s", str);
    ch = str;
    if (E() && *ch == '\0') {
        puts("--------------------------------");
        puts("String is successfully parsed");
    } else {
        puts("--------------------------------");
        puts("Error in parsing String");
    }
    return 0;
}