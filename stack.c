#include "befunge.h"

stack* alloc() {
    stack* s = (stack*)malloc(sizeof(stack));

    s->top = 0;
    s->bottom = 0;

    return s;
}

void push(stack* stack, int val) {
    node* temp = (node*)malloc(sizeof(node));

    if (stack->top == 0) {
        temp->val = val;
        temp->next = 0;
        stack->top = temp;
        stack->bottom = temp;
    }

    temp->val = val;
    temp->next = stack->top;
    stack->top = temp;
}

int pop(stack* stack) {
    int val = stack->top->val;

    if (stack->top == stack->bottom && stack->top != 0) {
        stack->top->val = 0;
        stack->top->next = 0;
    }
    else if (stack->top != 0) {
        node* del = stack->top;
        stack->top = stack->top->next;
        free(del);
    }

    return val;
}

void destroy(stack* stack) {
    while (stack->top->next != 0)
        pop(stack);
}