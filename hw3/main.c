#include <stdio.h>
#include <string.h>

#define TOKEN_NUM 256

FILE *input_file;
int current_token;
char lexeme[100];
int has_error = 0;


int is_digit(int c) {
    return (c >= '0' && c <= '9');
}

int get_next_token() {
    int ch;
    while ((ch = fgetc(input_file)) != EOF) {
        if (ch == ' ' || ch == '\n' ||
            ch == '\t' || ch == '\r') {
            continue;
        }

        if (is_digit(ch)) {
            int i = 0;
            lexeme[i++] = (char)ch;
            
            while (is_digit(ch = fgetc(input_file))) {
                lexeme[i++] = (char)ch;
            }
            lexeme[i] = '\0';
            
            if (ch != EOF) {
                ungetc(ch, input_file);
            }
            return TOKEN_NUM;
        }

        lexeme[0] = (char)ch;
        lexeme[1] = '\0';
        return ch;
    }
    return EOF;
}

void parse_S();
void parse_S_prime();
void parse_E();

void parse_error() {
    if (!has_error) {
        printf("ParseError\n");
        has_error = 1;
    }
}

void parse_S() {
    if (has_error) return;

    if (current_token == TOKEN_NUM ||
        current_token == '(') {
        
        printf("S -> E S'\n");
        parse_E();
        parse_S_prime();
        
    } else {
        parse_error();
    }
}

void parse_S_prime() {
    if (has_error) return;

    switch (current_token) {
        case '+':
            printf("S' -> + S\n");
            current_token = get_next_token();
            parse_S();
            return;
            
        case ')':
        case EOF:
            return;
            
        default:
            parse_error();
            return;
    }
}

void parse_E() {
    if (has_error) return;

    switch (current_token) {
        case TOKEN_NUM:
            printf("%s\n", lexeme);
            current_token = get_next_token();
            return;
            
        case '(':
            printf("E -> ( S )\n");
            current_token = get_next_token();
            parse_S();
            
            if (current_token != ')') {
                parse_error();
                return;
            }
            
            current_token = get_next_token();
            return;
            
        default:
            parse_error();
            return;
    }
}

int main() {
    input_file = stdin;
    current_token = get_next_token();
    
    parse_S();

    if (!has_error && current_token != EOF) {
        parse_error();
    }

    return 0;
}
