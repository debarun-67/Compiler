#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define MAX_LEN 1024

int tempVarCount = 1;
char* newTemp() {
    static char buf[10];
    snprintf(buf, sizeof(buf), "t%d", tempVarCount++);
    return strdup(buf);
}

typedef struct {
    char code[MAX_LEN];
    char result[20];
} Expr;

void skipSpaces(char **p) {
    while (isspace(**p)) (*p)++;
}

Expr parseExpr(char **), parseTerm(char **), parseFactor(char **);

Expr parseFactor(char **p) {
    skipSpaces(p);
    Expr e = { "", "" };
    if (isalnum(**p) || **p == '_') {
        int i = 0;
        while (isalnum(**p) || **p == '_') e.result[i++] = *(*p)++;
        e.result[i] = '\0';
    } else if (**p == '(') {
        (*p)++;
        e = parseExpr(p);
        if (**p == ')') (*p)++;
    }
    return e;
}

Expr parseTerm(char **p) {
    Expr left = parseFactor(p);
    skipSpaces(p);
    while (**p == '*' || **p == '/') {
        char op = *(*p)++;
        Expr right = parseFactor(p);
        char *tmp = newTemp();
        snprintf(left.code + strlen(left.code), MAX_LEN - strlen(left.code),
                 "%s%s = %s %c %s\n", right.code, tmp, left.result, op, right.result);
        strcpy(left.result, tmp);
    }
    return left;
}

Expr parseExpr(char **p) {
    Expr left = parseTerm(p);
    skipSpaces(p);
    while (**p == '+' || **p == '-') {
        char op = *(*p)++;
        Expr right = parseTerm(p);
        char *tmp = newTemp();
        snprintf(left.code + strlen(left.code), MAX_LEN - strlen(left.code),
                 "%s%s = %s %c %s\n", right.code, tmp, left.result, op, right.result);
        strcpy(left.result, tmp);
    }
    return left;
}

void generateTAC(char *line) {
    if (strstr(line, "return")) {
        char *p = strstr(line, "return") + 6;
        skipSpaces(&p);
        char *semi = strchr(p, ';');
        if (semi) *semi = '\0';
        printf("return %s\n", p);
        return;
    }

    if (strstr(line, "if")) {
        char *start = strchr(line, '('), *end = strchr(line, ')');
        if (start && end) {
            char cond[MAX_LEN];
            strncpy(cond, start + 1, end - start - 1);
            cond[end - start - 1] = '\0';
            printf("if (%s)\n", cond);
        }
        return;
    }

    char *type = strtok(line, " ");
    char *rest = strtok(NULL, ";");

    if (!type || !rest) return;

    if (strcmp(type, "int") == 0 || strcmp(type, "float") == 0 || strcmp(type, "char") == 0) {
        if (strchr(rest, '=')) {
            char *var = strtok(rest, "=");
            char *expr = strtok(NULL, "");
            skipSpaces(&expr);
            Expr e = parseExpr(&expr);
            printf("%s%s %s = %s\n", e.code, type, var, e.result);
        } else {
            printf("%s %s\n", type, rest);
        }
    } else if (strchr(line, '=')) {
        char *lhs = type, *rhs = rest;
        skipSpaces(&rhs);
        Expr e = parseExpr(&rhs);
        printf("%s%s = %s\n", e.code, lhs, e.result);
    }
}

int main() {
    char line[MAX_LEN];
    while (fgets(line, sizeof(line), stdin)) {
        if (strchr(line, ';') || strstr(line, "if") || strstr(line, "return")) {
            generateTAC(line);
        }
    }
    return 0;
}

