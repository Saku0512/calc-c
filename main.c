#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

double add(double a, double b);
double sub(double a, double b);
double mul(double a, double b);
double divide(double a, double b);
int parse_infix_expression(const char *input, double *a, char *op, double *b);
int build_prefix_expression(double a, char op, double b, char *prefix, size_t size);
double calculate_prefix(char op, double a, double b);
void trim_newline(char *text);

int main() {
    char line[256];
    char prefix[256];

    printf("Simple Calculator CLI\n");
    printf("Enter expressions like: 10+5\n");
    printf("Type 'quit' to exit.\n");

    for (;;) {
        double a, b;
        char op;

        printf("> ");

        if (fgets(line, sizeof(line), stdin) == NULL) {
            printf("\n");
            break;
        }

        trim_newline(line);

        if (strcmp(line, "quit") == 0) {
            break;
        }

        if (!parse_infix_expression(line, &a, &op, &b)) {
            printf("Invalid input. Use format: number operator number\n");
            continue;
        }

        if (!build_prefix_expression(a, op, b, prefix, sizeof(prefix))) {
            printf("Expression is too long.\n");
            continue;
        }

        printf("prefix: %s\n", prefix);

        if (op == '/' && b == 0) {
            printf("Cannot divide by 0.\n");
            continue;
        }

        printf("%.2f\n", calculate_prefix(op, a, b));
    }

    return 0;
}

int parse_infix_expression(const char *input, double *a, char *op, double *b) {
    char *end;
    char *next;

    while (isspace((unsigned char)*input)) {
        input++;
    }

    *a = strtod(input, &end);
    if (end == input) {
        return 0;
    }

    while (isspace((unsigned char)*end)) {
        end++;
    }

    if (*end != '+' && *end != '-' && *end != '*' && *end != '/') {
        return 0;
    }
    *op = *end;
    end++;

    while (isspace((unsigned char)*end)) {
        end++;
    }

    *b = strtod(end, &next);
    if (next == end) {
        return 0;
    }

    input = next;

    while (isspace((unsigned char)*input)) {
        input++;
    }

    return *input == '\0';
}

int build_prefix_expression(double a, char op, double b, char *prefix, size_t size) {
    return snprintf(prefix, size, "%c %.15g %.15g", op, a, b) < (int)size;
}

double calculate_prefix(char op, double a, double b) {
    switch (op) {
    case '+':
        return add(a, b);
    case '-':
        return sub(a, b);
    case '*':
        return mul(a, b);
    case '/':
        return divide(a, b);
    default:
        return 0;
    }
}

void trim_newline(char *text) {
    size_t len = strlen(text);

    if (len > 0 && text[len - 1] == '\n') {
        text[len - 1] = '\0';
    }
}

double add(double a, double b) {
    return a + b;
}

double sub(double a, double b) {
    return a - b;
}

double mul(double a, double b) {
    return a * b;
}

double divide(double a, double b) {
    return a / b;
}
