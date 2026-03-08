#include <stdio.h>
#include <stdlib.h>

struct Node {
    char x;
    int count;
    struct Node* next;
};

int main() {
    FILE *file = fopen("main.c", "r");
    if (file == NULL) {
        printf("無法打開檔案！\n");
        return 1;
    }

    struct Node* head = NULL;
    struct Node* shortcut[256] = {NULL};     
    int ch;

    while ((ch = fgetc(file)) != EOF) {
        if (ch == '\n' || ch == ' ' || ch == '\r') continue;
        if (ch >= 0 && ch < 256) {
            if (shortcut[ch] != NULL) {
                shortcut[ch]->count++;
            } else {
                struct Node* newNode = (struct Node*)malloc(sizeof(struct Node));                
                if (newNode == NULL) {
                    printf("Memory allocation failed\n");
                    return 1;
                }  
                newNode->x = (char)ch;
                newNode->count = 1;

                newNode->next = head;
                head = newNode;
                shortcut[ch] = newNode;
            }
        }
    }
    fclose(file);

    struct Node* current = head;
    while (current != NULL) {
        printf("%c:%d\n", current->x, current->count);
        current = current->next;
    }

    current = head;
    while (current != NULL) {
        struct Node* temp = current;
        current = current->next;
        free(temp);
    }

    return 0;
}
