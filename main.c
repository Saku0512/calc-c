#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

typedef struct {
    const char *current;
    int has_error;
    int divide_by_zero;
    int expression_too_long;
} Parser;

enum {
    PARSE_OK = 1,
    PARSE_INVALID = 0,
    PARSE_ERROR = -1
};

double add(double a, double b);
double sub(double a, double b);
double mul(double a, double b);
double divide(double a, double b);
int parse_expression_input(const char *input, double *value, char *prefix, size_t size);
void parse_expression(Parser *parser, double *value, char *prefix, size_t size);
void parse_term(Parser *parser, double *value, char *prefix, size_t size);
void parse_factor(Parser *parser, double *value, char *prefix, size_t size);
void skip_spaces(Parser *parser);
int combine_prefix(char op, const char *left, const char *right, char *prefix, size_t size);
void trim_newline(char *text);

int main() {
    char line[256];
    char prefix[256];

    printf("Simple Calculator CLI\n");
    printf("Enter expressions like: 10+5 or 1+2*3\n");
    printf("Type 'quit' to exit.\n");

    for (;;) {
        double result;

        printf("> ");

        if (fgets(line, sizeof(line), stdin) == NULL) {
            printf("\n");
            break;
        }

        trim_newline(line);

        if (strcmp(line, "quit") == 0) {
            break;
        }

        switch (parse_expression_input(line, &result, prefix, sizeof(prefix))) {
        case PARSE_OK:
            printf("%.2f\n", result);
            break;
        case PARSE_INVALID:
            printf("Invalid input. Use expressions like: 1+2*3\n");
            break;
        case PARSE_ERROR:
            break;
        }
    }

    return 0;
}

int parse_expression_input(const char *input, double *value, char *prefix, size_t size) {
    Parser parser = {input, 0, 0, 0};

    parse_expression(&parser, value, prefix, size);
    skip_spaces(&parser);

    if (parser.divide_by_zero) {
        printf("Cannot divide by 0.\n");
        return PARSE_ERROR;
    }

    if (parser.expression_too_long) {
        printf("Expression is too long.\n");
        return PARSE_ERROR;
    }

    if (parser.has_error || *parser.current != '\0') {
        return PARSE_INVALID;
    }

    return PARSE_OK;
}

void parse_expression(Parser *parser, double *value, char *prefix, size_t size) {
    double right_value;
    char op;
    char left_prefix[256];
    char right_prefix[256];

    parse_term(parser, value, prefix, size);
    if (parser->has_error || parser->divide_by_zero || parser->expression_too_long) {
        return;
    }

    while (1) {
        skip_spaces(parser);
        op = *parser->current;
        if (op != '+' && op != '-') {
            return;
        }

        strncpy(left_prefix, prefix, sizeof(left_prefix) - 1);
        left_prefix[sizeof(left_prefix) - 1] = '\0';
        parser->current++;

        parse_term(parser, &right_value, right_prefix, sizeof(right_prefix));
        if (parser->has_error || parser->divide_by_zero || parser->expression_too_long) {
            return;
        }

        if (!combine_prefix(op, left_prefix, right_prefix, prefix, size)) {
            parser->expression_too_long = 1;
            return;
        }

        if (op == '+') {
            *value = add(*value, right_value);
        } else {
            *value = sub(*value, right_value);
        }
    }
}

void parse_term(Parser *parser, double *value, char *prefix, size_t size) {
    double right_value;
    char op;
    char left_prefix[256];
    char right_prefix[256];

    parse_factor(parser, value, prefix, size);
    if (parser->has_error || parser->divide_by_zero || parser->expression_too_long) {
        return;
    }

    while (1) {
        skip_spaces(parser);
        op = *parser->current;
        if (op != '*' && op != '/') {
            return;
        }

        strncpy(left_prefix, prefix, sizeof(left_prefix) - 1);
        left_prefix[sizeof(left_prefix) - 1] = '\0';
        parser->current++;

        parse_factor(parser, &right_value, right_prefix, sizeof(right_prefix));
        if (parser->has_error || parser->divide_by_zero || parser->expression_too_long) {
            return;
        }

        if (!combine_prefix(op, left_prefix, right_prefix, prefix, size)) {
            parser->expression_too_long = 1;
            return;
        }

        if (op == '*') {
            *value = mul(*value, right_value);
        } else {
            if (right_value == 0) {
                parser->divide_by_zero = 1;
                return;
            }
            *value = divide(*value, right_value);
        }
    }
}

void parse_factor(Parser *parser, double *value, char *prefix, size_t size) {
    char *end;

    skip_spaces(parser);

    if (*parser->current == '(') {
        parser->current++;
        parse_expression(parser, value, prefix, size);
        if (parser->has_error || parser->divide_by_zero || parser->expression_too_long) {
            return;
        }

        skip_spaces(parser);
        if (*parser->current != ')') {
            parser->has_error = 1;
            return;
        }

        parser->current++;
        return;
    }

    *value = strtod(parser->current, &end);
    if (end == parser->current) {
        parser->has_error = 1;
        return;
    }

    if (snprintf(prefix, size, "%.15g", *value) >= (int)size) {
        parser->expression_too_long = 1;
        return;
    }

    parser->current = end;
}

void skip_spaces(Parser *parser) {
    while (isspace((unsigned char)*parser->current)) {
        parser->current++;
    }
}

int combine_prefix(char op, const char *left, const char *right, char *prefix, size_t size) {
    return snprintf(prefix, size, "%c %s %s", op, left, right) < (int)size;
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
