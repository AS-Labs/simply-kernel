
// LIFO data structure example in C
#include <stdio.h>
#include <stdlib.h>

// define a struct for the linked list node
typedef struct Node {
        int data;
        struct Node* next;
} Node;

// define a struct for the LIFO stack
typedef struct Stack {
    Node* top;
} Stack;

// initialize an empty stack
void init(Stack* s) {
    s->top = NULL;
}

// check if the stack is empty
int isEmpty(Stack* s) {
    return s->top == NULL;
}

// push an element onto the stack
void push(Stack* s, int data) {
    // allocate a new node
    Node* newNode = (Node*) malloc(sizeof(Node));
    if (newNode == NULL) {
            fprintf(stderr, "Error: memory allocation failed\n");
            exit(EXIT_FAILURE);
        }
        // set the node data and next pointer
        newNode->data = data;
        newNode->next = s->top;
        // set the new node as the top of the stack
        s->top = newNode;
}

// pop an element off the stack
int pop(Stack* s) {
    if (isEmpty(s)) {
            fprintf(stderr, "Error: stack is empty\n");
            exit(EXIT_FAILURE);
        }
        // save the top node and data
        Node* topNode = s->top;
        int data = topNode->data;
        // set the next node as the new top of the stack
        s->top = topNode->next;
        // free the top node
        free(topNode);
        // return the popped data
        return data;
}


// print the stack contents
void printStack(Stack* s) {
    printf("Stack: ");
    Node* node = s->top;
    while (node != NULL) {
            printf("%d ", node->data);
            node = node->next;
        }
        printf("\n");

}

// main function to test the stack implementation
int main() {
    Stack s;
    init(&s);
    push(&s, 1);
    push(&s, 2);
    push(&s, 3);
    printStack(&s);
    printf("Popped: %d\n", pop(&s));
    printStack(&s);
    printf("Popped: %d\n", pop(&s));
    printStack(&s);
    printf("Popped: %d\n", pop(&s));
    printStack(&s);
    // the following line will cause an error
    // printf("Popped: %d\n", pop(&s));
    return 0;
}
