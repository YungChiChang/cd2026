#include <stdio.h>
#include <stdlib.h>
#include <string.h>

struct Node {
    char lexeme[100]; 
    char tokenType[50]; 
    struct Node* next;
};

int is_alpha(int c) {
    return (c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z') || (c == '_');
}

int is_digit(int c) {
    return (c >= '0' && c <= '9');
}

int main() {
    FILE *file = fopen("main.c", "r");
    if (file == NULL) {
        printf("無法打開檔案！\n");
        return 1;
    }

    struct Node* head = NULL;
    struct Node* tail = NULL; 
    int ch;
    char buffer[100];
    int bufIndex = 0;

    while ((ch = fgetc(file)) != EOF) {
        if (ch == ' ' || ch == '\n' || ch == '\r' || ch == '\t') {
            continue;
        }

        bufIndex = 0;
        char currentType[50] = "";

        if (is_alpha(ch)) {
            while (is_alpha(ch) || is_digit(ch)) {
                buffer[bufIndex++] = (char)ch;
                ch = fgetc(file);
            }
            buffer[bufIndex] = '\0'; 
            ungetc(ch, file); 

            if (strcmp(buffer, "int") == 0) strcpy(currentType, "TYPE_TOKEN");
            else if (strcmp(buffer, "main") == 0) strcpy(currentType, "MAIN_TOKEN");
            else if (strcmp(buffer, "if") == 0) strcpy(currentType, "IF_TOKEN");
            else if (strcmp(buffer, "else") == 0) strcpy(currentType, "ELSE_TOKEN");
            else if (strcmp(buffer, "while") == 0) strcpy(currentType, "WHILE_TOKEN");
            else strcpy(currentType, "ID_TOKEN"); 
        }
        else if (is_digit(ch)) {
            while (is_digit(ch)) {
                buffer[bufIndex++] = (char)ch;
                ch = fgetc(file);
            }
            buffer[bufIndex] = '\0';
            ungetc(ch, file); 
            strcpy(currentType, "LITERAL_TOKEN");
        }
        else {
            buffer[0] = (char)ch;
            buffer[1] = '\0';

            switch (ch) {
                case '=':
                    ch = fgetc(file);
                    if (ch == '=') { 
                        buffer[1] = '='; buffer[2] = '\0';
                        strcpy(currentType, "EQUAL_TOKEN");
                    } else {         
                        ungetc(ch, file);
                        strcpy(currentType, "ASSIGN_TOKEN");
                    }
                    break;
                case '>':
                    ch = fgetc(file);
                    if (ch == '=') { 
                        buffer[1] = '='; buffer[2] = '\0';
                        strcpy(currentType, "GREATEREQUAL_TOKEN");
                    } else {         
                        ungetc(ch, file);
                        strcpy(currentType, "GREATER_TOKEN");
                    }
                    break;
                case '<':
                    ch = fgetc(file);
                    if (ch == '=') {
                        buffer[1] = '='; buffer[2] = '\0';
                        strcpy(currentType, "LESSEQUAL_TOKEN");
                    } else {         
                        ungetc(ch, file);
                        strcpy(currentType, "LESS_TOKEN");
                    }
                    break;
                case '+':
                    strcpy(currentType, "PLUS_TOKEN");
                    break;
                case '-':
                    strcpy(currentType, "MINUS_TOKEN");
                    break;
                case '(':
                    strcpy(currentType, "LEFTPAREN_TOKEN");
                    break;
                case ')':
                    strcpy(currentType, "REFTPAREN_TOKEN"); 
                    break;
                case '{':
                    strcpy(currentType, "LEFTBRACE_TOKEN");
                    break;
                case '}':
                    strcpy(currentType, "REFTBRACE_TOKEN"); 
                    break;
                case ';':
                    strcpy(currentType, "SEMICOLON_TOKEN");
                    break;
                default:
                    continue; 
            }
        }

        if (currentType[0] != '\0') {
            struct Node* newNode = (struct Node*)malloc(sizeof(struct Node));
            strcpy(newNode->lexeme, buffer);
            strcpy(newNode->tokenType, currentType);
            newNode->next = NULL;

            if (head == NULL) {
                head = newNode;
                tail = newNode;
            } else {
                tail->next = newNode;
                tail = newNode;
            }
        }
    }
    fclose(file);

    struct Node* current = head;
    while (current != NULL) {
        printf("%s: %s\n", current->lexeme, current->tokenType);
        struct Node* temp = current;
        current = current->next;
        free(temp);
    }

    return 0;
}
