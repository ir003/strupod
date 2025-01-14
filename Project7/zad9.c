#define _CRT_SECURE_NO_WARNINGS

#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define FILE_ERROR -1
#define MALLOC_ERROR -2

// def fu
typedef struct node {
    int data;   // vrijee cvora
    struct node* left;  // ljevi i
    struct node* right;  // desni podcvor
} Node, *Pos;

Pos insert(Pos root, int value); // fun za umetanje vrije
int replace(Pos root);   // fun za zamjenu 
int writeToFile(Pos root, FILE* fp);  // fun za pisanje 
int deleteTree(Pos root);   // fun za oslobadanje memorije 

int main() {
    srand(time(NULL)); // gen slucajnih br

    FILE* fp = fopen("inorder.txt", "w"); //  dat za pisanje
    if (!fp) { 
        printf("greska \n");
        return FILE_ERROR;
    }

    Pos root = NULL; // inic korijena 
    for (int i = 0; i < 10; i++) { // Petlja koja generira 10 slucajnih br
        root = insert(root, rand() % 81 + 10); // umetanje slucajnog br u stablo (raspon 10-90)
        if (!root) { 
            printf("greska\n");
            return MALLOC_ERROR;
        }
    }

    fprintf(fp, "orig stablo :\n"); // ispis zaglavlja u dat
    writeToFile(root, fp); // Pisanje origstabla u dat

    replace(root); // zamjena 
    fprintf(fp, "\ntablo nakon zamjene :\n"); // ispis zaglavlja za zamijenjeno stablo
    writeToFile(root, fp); // pisanje zam stabla u dat

    fclose(fp); 
    deleteTree(root); 

    return 0; 
}

// fun za umetanje vrije
Pos insert(Pos root, int value) {
    if (!root) { // ako trenutni cvor NULL--> kreiraj novi cvor
        root = (Pos)malloc(sizeof(Node)); // aloc memorije za novi cvor
        if (!root) return NULL; // aloc nije uspjela

        root->data = value; // prbac vrijei cvora
        root->left = root->right = NULL; // stavljanje pokza na NULL
    } 
    else if (value < root->data) { // vrije manja--> idi lijevo
        root->left = insert(root->left, value); // rekurz
    } 
    else { // idi desno
        root->right = insert(root->right, value); // rekurz
    }

    return root; 
}

// fun za zamjenu vrije cvorova bin st
int replace(Pos root) {
    if (!root) return 0; //  cvor NULL--> vrati 0

    int originalValue = root->data; // sprema orig vrije cvora
    root->data = replace(root->left) + replace(root->right); // zam--> zbr lijevog i desnog podstabla

    return originalValue + root->data; // vracanje zbr orig  i nove vrije cvora
}

// fun za pisanje stabla u dat (Inorder
int writeToFile(Pos root, FILE* fp) {
    if (!root) return 0; // cvor NULL-->ne diraj

    writeToFile(root->left, fp); // rekur pisi lijevo podstablo
    fprintf(fp, "%d\n", root->data); // pisanje vrije trenutnog cvora
    writeToFile(root->right, fp); // rekur pisi desno podstablo

    return 0; 
}

// fun za oslobadanje memorije binarnog stabla
int deleteTree(Pos root) {
    if (!root) return 0; //cvor NULL-->ne diraj

    deleteTree(root->left); //rek
    deleteTree(root->right); // rek
    free(root); 

    return 0; 
}
