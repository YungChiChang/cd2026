#include <stdio.h>
#include <stdlib.h>

struct Node {
    char x;
    struct Node* next;
};

int main() {

    FILE *file = fopen("main.c", "r");
    if (file == NULL) {
        printf("無法打開檔案！\n");
        return 1;
    }

    struct Node* head = NULL;
    struct Node* tail = NULL;

    int ch;

    while ((ch = fgetc(file)) != EOF) {

        struct Node* newNode = (struct Node*)malloc(sizeof(struct Node));
        if (newNode == NULL) {
            printf("Memory allocation failed\n");
            return 1;
        }

        newNode->x = (char)ch;
        newNode->next = NULL;

        if (head == NULL) {
            head = newNode;
            tail = newNode;
        } else {
            tail->next = newNode;
            tail = newNode;
        }
    }

    fclose(file);

    struct Node* current = head;

    while (current != NULL) {

        if (current->x == '\n')
            printf("'\\n' ");
        else if (current->x == '\t')
            printf("'\\t' ");
        else
            printf("'%c' ", current->x);

        current = current->next;
    }

    printf("\n");

    current = head;
    while (current != NULL) {
        struct Node* temp = current;
        current = current->next;
        free(temp);
    }

    return 0;
}
