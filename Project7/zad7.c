#define _CRT_SECURE_NO_WARNINGS

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_NAME 50
#define EMPTY_STACK 1
#define MALLOC_ERROR -1


typedef struct dir {
    char ime[MAX_NAME];    //  dir
    struct dir* poddir; // pokaz na prvi poddir
    struct dir* sljedeci; // pok na sljed. dir u istoj razini
} Dir, *DirPoz;

// povijest direktorija
typedef struct stog {
    DirPoz dirPoz;   // pokaz na trenutni dir
    struct stog* sljedeci;   // pokaz na sledec elem stoga
} Stog, *StogPoz;

DirPoz noviDir(char* ime);
int dodajPoddir(DirPoz rod, DirPoz pod);
int ispisiSadr(DirPoz dir, int dubina);
int push(StogPoz stog, DirPoz dir);
int pop(StogPoz stog);
int promDir(StogPoz stog, char* ime);
void brisiDir(DirPoz dir);
void brisiStog(StogPoz stog);

int main() {
    char naredba;
    char ime[MAX_NAME] = { 0 };  // privr ime dir
    Dir root = { "C:", NULL, NULL };   // root dir
    Stog stog = { .dirPoz = &root, .sljedeci = NULL }; // stog za povijest dir

    printf("1 - dodaj direktorij\n2 - promijeni direktorij\n3 - vrati se na rod. direktorij\n - 4 - spisi sadrzaj direktorija\n5 - kraj\n");

    do {
        printf("\nunesi naredbu: ");
        scanf(" %c", &naredba);

        switch (naredba) {
        case '1': // dodavanje poddir
            printf("unesi ime novog dir: ");
            scanf(" %s", ime);
            DirPoz novi = noviDir(ime);
            if (novi) {
                dodajPoddir(stog.dirPoz, novi);
                printf("dir '%s' je dodan\n", ime);
            }
            else {
                printf("greska\n");
            }
            break;

        case '2': // promijena u poddir
            printf("unesi ime dir ");
            scanf(" %s", ime);
            promijeniDir(&stog, ime);
            break;

        case '3': // vracanje na rod dir
            if (pop(&stog)) {
                printf("u direktoriju smo\n");
            }
            else {
                printf("u rod. smo direktroiju\n");
            }
            break;

        case '4': // ispisi
            printf("sadrzaj direktorija '%s':\n", stog.dirPoz->ime);
            ispisiSadr(stog.dirPoz, 0);
            break;

        case '5': // kraj
            printf("zavrsetak \n");
            break;

        default:
            printf("nepoznata naredba\n");
        }
    } while (naredba != '5');

    brisiDir(&root);  
    brisiStog(&stog);        
    return 0;
}

// fun za stvaranje novog dir
DirPoz noviDir(char* ime) {
    DirPoz novi = (DirPoz)malloc(sizeof(Dir));
    if (!novi) return NULL;

    strcpy(novi->ime, ime);
    novi->poddir = NULL;
    novi->sljedeci = NULL;
    return novi;
}

// dodavanje poddir unutar rod
int dodajPoddir(DirPoz rod, DirPoz pod) {
    pod->sljedeci = rod->poddir; //stavljamo novi poddir na pocetak 
    rod->poddir = pod;   // ažuriramo pokaz rod.
    return 0;
}

// pise sadr dir rekurz
int ispisiSadr(DirPoz dir, int dubina) {
    DirPoz trenutni = dir->poddir;
    while (trenutni) {
        for (int i = 0; i < dubina; i++) printf("  "); // pisemo razmake ovisno o dubini
        printf("%s\n", trenutni->ime);
        ispisiSadr(trenutni, dubina + 1); // rekurzija
        trenutni = trenutni->sljedeci;
    }
    return 0;
}
// daje novi elem na stog odn trenutni dir
int push(StogPoz stog, DirPoz dir) {
    StogPoz novi = (StogPoz)malloc(sizeof(Stog)); // alok za novi elem 
    if (!novi) return MALLOC_ERROR;

    novi->dirPoz = dir;  // pokazivac na dir koji dodajemo
    novi->sljedeci = stog->sljedeci; // novi elem stoga --> vrh stoga
    stog->sljedeci = novi; // vrh stoga  pokazuje novi elem kao vrh stoga

    return 0;                   
}
//  uklanjanje vrha stoga 
int pop(StogPoz stog) {
    if (!stog->sljedeci) return EMPTY_STACK; // stog nema elem (prazan 

    StogPoz temp = stog->sljedeci; //  vrh stoga ---> temp
    stog->sljedeci = temp->sljedeci;   // vrh stoga pokazuje  sljedeci elem ispod vrha
    free(temp);                       

    return 0;                       
}
//promejna trenutng dir u poddir s datim imenom
int promijeniDir(StogPoz stog, char* ime) {
    DirPoz trenutni = stog->dirPoz->poddir; // trenutni na prvi poddir aktivnog dir

    //traži se poddir sa datim imenom
    while (trenutni && strcmp(trenutni->ime, ime) != 0) 
        trenutni = trenutni->sljedeci; // ide na sljedeći poddi dok ne nađe

    if (trenutni) { // dir naden
        push(stog, trenutni); // dodaje direktorij na stog 
        printf("promijenili smo direktorij u '%s'\n", ime); 
    }
    else { //  nije pronaden taj dir
        printf("direktorij '%s' nije pronaden\n", ime); 
    }

    return 0; 
}


void brisiDir(DirPoz dir) {
    while (dir->poddir) { // dok postoje p
        DirPoz temp = dir->poddir; //  sprema prvi poddir
        dir->poddir = temp->sljedeci;   // stavlja na sljedeci poddir
        brisiDir(temp);  //rekuz
        free(temp); 
    }
}
void brisiStog(StogPoz stog) {
    while (stog->sljedeci) { // stog ima elem
        StogPoz temp = stog->sljedeci;// sprema na vrh 
        stog->sljedeci = temp->sljedeci; // prebacuje pok na sljedeci elem
        free(temp); 
    }
}
