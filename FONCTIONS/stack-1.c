#include <stdio.h>
#include <stdbool.h>

#define MAX 100

typedef struct {
    int items[MAX];
    int top;
} Stack;

void initStack(Stack* s) {
    s->top = -1;
}

bool isFull(Stack* s) {
    return s->top == MAX - 1;
}

bool isEmpty(Stack* s) {
    return s->top == -1;
}

void push(Stack* s, int value) {
    if (!isFull(s)) {
        s->items[++(s->top)] = value;
    }
}

int pop(Stack* s) {
    if (!isEmpty(s)) {
        return s->items[(s->top)--];
    }
    return -1;
}

int main() {
    Stack s;
    initStack(&s);

    push(&s, 10);
    push(&s, 20);

    printf("Popped : %d\n", pop(&s)); // 20
    printf("Popped : %d\n", pop(&s)); // 10
    return 0;
}