#include<stdio.h>

double add(double a, double b);
double sub(double a, double b);
double mul(double a, double b);
double div(double a, double b);

int main() {
    double a, b;
    char ap;
    for(;;) {
        
    }
    printf("> ");
    scanf("%lf %c %lf", &a, &op, &b);

    switch (op) {
        case '+':
            printf("%.2f\n", add(a, b));
        case '-':
            printf("%.2f\n", sub(a, b));
        case '*':
            printf("%.2f\n", mul(a, b));
        case '/':
            if (b==0) {
                printf("Invalid.Can't divide by 0");
            } else {
                printf("%.2f\n", div(a, b));
            }
            break;
        default:
            printf("Invalid formula.");
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

double div(double a, double b) {
    return a / b;
}
