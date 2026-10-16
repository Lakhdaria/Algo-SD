#include <stdio.h>
#include <stdlib.h>

typedef struct Node {
    int data;
    struct Node* next;
} Node;

// Insertion au début
void pushCircular(Node** head, int val) {
    Node* new_node = (Node*)malloc(sizeof(Node));
    new_node->data = val;
    new_node->next = *head;

    if (*head != NULL) {
        // Trouver le dernier nœud pour mettre à jour son pointeur next
        Node* temp = *head;
        while (temp->next != *head) {
            temp = temp->next;
        }
        temp->next = new_node;
    } else {
        new_node->next = new_node; // Premier nœud pointe sur lui-même
    }

    *head = new_node;
}

// Affichage
void printCircularList(Node* head) {
    if (head == NULL) return;

    Node* temp = head;
    do {
        printf("%d -> ", temp->data);
        temp = temp->next;
    } while (temp != head);
    
    printf("(head)\n");
}

int main() {
    Node* head = NULL;
    pushCircular(&head, 30);
    pushCircular(&head, 20);
    pushCircular(&head, 10);

    printCircularList(head); // 10 -> 20 -> 30 -> (head)
    return 0;
}