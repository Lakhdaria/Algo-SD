#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define SIZE 10

typedef struct HashNode {
    char key[50];
    int value;
    struct HashNode* next;
} HashNode;

typedef struct {
    HashNode* buckets[SIZE];
} HashTable;

// Fonction de hachage simple
unsigned int hash(const char* key) {
    unsigned int hash = 0;
    while (*key) {
        hash = (hash << 5) + *key++;
    }
    return hash % SIZE;
}

void insert(HashTable* ht, const char* key, int value) {
    unsigned int index = hash(key);
    HashNode* newNode = (HashNode*)malloc(sizeof(HashNode));
    strcpy(newNode->key, key);
    newNode->value = value;
    newNode->next = ht->buckets[index];
    ht->buckets[index] = newNode;
}

int search(HashTable* ht, const char* key) {
    unsigned int index = hash(key);
    HashNode* current = ht->buckets[index];
    while (current != NULL) {
        if (strcmp(current->key, key) == 0) {
            return current->value;
        }
        current = current->next;
    }
    return -1; // Non trouvé
}

int main() {
    HashTable ht = {0};

    insert(&ht, "Alice", 25);
    insert(&ht, "Bob", 30);

    printf("Âge d'Alice : %d\n", search(&ht, "Alice")); // 25
    printf("Âge de Bob : %d\n", search(&ht, "Bob"));     // 30

    return 0;
}