#include <stdio.h>
#include <string.h>

double add(double a, double b);
double sub(double a, double b);
double mul(double a, double b);
double divide(double a, double b);

int main() {
    char line[256];

    printf("Simple Calculator CLI\n");
    printf("Enter expressions like: 10 + 5\n");
    printf("Type 'quit' to exit.\n");

    for (;;) {
        double a, b;
        char op;
        char extra;

        printf("> ");

        if (fgets(line, sizeof(line), stdin) == NULL) {
            printf("\n");
            break;
        }

        if (strcmp(line, "quit\n") == 0 || strcmp(line, "quit") == 0) {
            break;
        }

        if (sscanf(line, " %lf %c %lf %c", &a, &op, &b, &extra) != 3) {
            printf("Invalid input. Use format: number operator number\n");
            continue;
        }

        switch (op) {
        case '+':
            printf("%.2f\n", add(a, b));
            break;
        case '-':
            printf("%.2f\n", sub(a, b));
            break;
        case '*':
            printf("%.2f\n", mul(a, b));
            break;
        case '/':
            if (b == 0) {
                printf("Cannot divide by 0.\n");
            } else {
                printf("%.2f\n", divide(a, b));
            }
            break;
        default:
            printf("Unsupported operator. Use +, -, *, /\n");
            break;
        }
    }

    return 0;
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
