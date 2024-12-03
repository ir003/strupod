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

// Funkcije za rad sa stekom
int push(Stack* stack, float x);
int pop(Stack* stack, float* x);

int main() {
    char postfix[MAX_LINE] = {0};         // Postfix izraz kao string
    char* curr = NULL;                    // Pokazivač na trenutni element postfix izraza
    float firstOperand = 0, secondOperand = 0, result = 0;
    Stack stack = {.top = NULL};          // Stek inicijaliziran na prazan
    FILE* file = fopen("postfix.txt", "r"); // Otvara datoteku s postfix izrazom

    if (!file) {
        printf("Datoteka nije otvorena!\n");
        return error;
    }

    fgets(postfix, MAX_LINE, file);       // Čita postfix izraz iz datoteke
    fclose(file);                         // Zatvara datoteku

    printf("Postfix izraz: %s\n", postfix);

    curr = strtok(postfix, " ");          // Djelimo string na dijelove prema razmacima
    while (curr != NULL) {                // Iteriramo kroz svaki token (broj ili operator)
        if (isdigit(curr[0]) || (curr[0] == '-' && isdigit(curr[1]))) {
            // Ako je token broj (pozitivan ili negativan), dodajemo ga na stek
            if (push(&stack, atof(curr)) != 0) return -1;
        } else if (strchr("+-*/", curr[0])) {
            // Ako je token operator, skidamo dva broja sa steka
            if (pop(&stack, &secondOperand) != 0 || pop(&stack, &firstOperand) != 0) {
                printf("Stek je prazan!\n");
                return 1;
            }
            // Izračunavamo rezultat i dodajemo ga na stek
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
        curr = strtok(NULL, " ");         // Prijelaz na sljedeći token
    }

    if (pop(&stack, &result) != 0) return 1; // Skidamo konačni rezultat sa steka
    printf("Rezultat: %g\n", result);     // Ispis rezultata

    return 0;
}

// Dodaje element na vrh steka
int push(Stack* stack, float x) {
    StackItem* newItem = (StackItem*)malloc(sizeof(StackItem)); // Alociramo memoriju za novi čvor
    if (!newItem) {
        printf("Greška pri alokaciji memorije!\n");
        return -1;
    }
    newItem->value = x;                  // Postavljamo vrijednost
    newItem->next = stack->top;          // Novi element pokazuje na trenutni vrh steka
    stack->top = newItem;                // Novi element postaje vrh steka
    return 0;
}

// Skida element s vrha steka
int pop(Stack* stack, float* x) {
    if (!stack->top) {                   // Ako je stek prazan
        printf("Stek je prazan!\n");
        return 1;
    }
    StackItem* temp = stack->top;        // Privremeno spremamo trenutni vrh
    *x = temp->value;                    // Kopiramo vrijednost vrha u x
    stack->top = temp->next;             // Vrh postaje sljedeći element u steku
    free(temp);                          // Oslobađamo memoriju za prethodni vrh
    return 0;
}
