#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define MAX_LINE 1024


typedef struct stackItem {
    float value;
    struct stackItem* next;
} StackItem;

typedef struct {
    StackItem* top;
} Stack;

// fun stacka
int push(Stack* stack, float x);
int pop(Stack* stack, float* x);

int main() {
    char postfix[MAX_LINE] = {0};  //postfix-string
    char* curr = NULL;  //pok-trenutni ele postfixa
    float firstOperand = 0, secondOperand = 0, result = 0;
    Stack stack = {.top = NULL}; //stack kao prazan
    FILE* file = fopen("postfix.txt", "r"); //otvara dat post izraza

    if (!file) {
        printf("Datoteka nije otvorena!\n");
        return 0;
    }

    fgets(postfix, MAX_LINE, file); //"xita" post
    fclose(file);     

    printf("Postfix izraz: %s\n", postfix);

    curr = strtok(postfix, " ");          // djelimo string na dijelove prema razmacima
    while (curr != NULL) {                // prolazi kroz svaki ele (broj ili operator)
        if (isdigit(curr[0]) || (curr[0] == '-' && isdigit(curr[1]))) {
            // ak je elem br (pozitivan ili negativan), dodajemo ga na stack
            if (push(&stack, atof(curr)) != 0) return -1;
        } else if (strchr("+-*/", curr[0])) {
            // ako je elem operator, skidamo 2br sa stacka
            if (pop(&stack, &secondOperand) != 0 || pop(&stack, &firstOperand) != 0) {
                printf("Stek je prazan!\n");
                return 1;
            }
            // izracunavamo rezultat i dodajemo ga na stack
            switch (curr[0]) {
                case '+': push(&stack, firstOperand + secondOperand); break;
                case '-': push(&stack, firstOperand - secondOperand); break;
                case '*': push(&stack, firstOperand * secondOperand); break;
                case '/': push(&stack, firstOperand / secondOperand); break;
            }
        } else {
            printf("Neispravan znak: %s\n", curr);
            return 1;
        }
        curr = strtok(NULL, " ");         //  sljedeci ele
    }

    if (pop(&stack, &result) != 0) return 1; // uzimamo zadnji 
    printf("Rezultat: %g\n", result);     // ispis 

    return 0;
}

// daje element na vrh 
int push(Stack* stack, float x) {
    StackItem* newItem = (StackItem*)malloc(sizeof(StackItem)); // aloc mem 
    if (!newItem) {
        printf("Greška pri alokaciji memorije!\n");
        return -1;
    }
    newItem->value = x;      // stavljamo vrijedn
    newItem->next = stack->top;    // novi ele pokazuje na vrh
    stack->top = newItem;        // i postaje vrh 
    return 0;
}

// mice elem s vrha 
int pop(Stack* stack, float* x) {
    if (!stack->top) {                   // prazan stack
        printf("Stek je prazan!\n");
        return 1;
    }
    StackItem* temp = stack->top; // privremen vrh
    *x = temp->value;      // kopir vrijednost vrha u x
    stack->top = temp->next;             // vrh-sljedeci ele u stavku
    free(temp);                          // free mem
    return 0;
}
