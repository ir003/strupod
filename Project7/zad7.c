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
int dodajPoddir(DirPoz roditelj, DirPoz pod);
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
                printf("u rod smo direktroiju\n");
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
    if (!novi) return MALLOC_ERROR; // provjera 

    novi->dirPoz = dir;  // pokazivac na dir koji dodajemo
    novi->sljedeci = stog->sljedeci; // novi elem stoga pokazuje vrh stoga
    stog->sljedeci = novi; // Glava stoga sada pokazuje na novi element kao vrh stoga

    return 0;                   
}
// Funkcija uklanja vrh stoga (vraća se u roditeljski direktorij)
// Parametri:
// - stog: pokazivač na glavu stoga
// Povratna vrijednost: 0 ako je uspješno, EMPTY_STACK ako je stog već prazan
int pop(StogPozicija stog) {
    if (!stog->sljedeci) return EMPTY_STACK; // Ako stog nema elemenata (prazan je), vraća grešku

    StogPozicija temp = stog->sljedeci; // Privremeno sprema vrh stoga u temp
    stog->sljedeci = temp->sljedeci;   // Glava stoga sada pokazuje na sljedeći element ispod vrha
    free(temp);                        // Oslobađa memoriju za prethodni vrh stoga

    return 0;                          // Vraća uspješan završetak funkcije
}
// Funkcija mijenja trenutni direktorij u poddirektorij s danim imenom
// Parametri:
// - stog: pokazivač na glavu stoga
// - ime: ime poddirektorija u koji se želi promijeniti
int promijeniDirektorij(StogPozicija stog, char* ime) {
    DirPozicija trenutni = stog->dirPoz->poddirektorij; // Inicijalizira trenutni na prvi poddirektorij aktivnog direktorija

    // Petlja traži poddirektorij s traženim imenom
    while (trenutni && strcmp(trenutni->ime, ime) != 0) 
        trenutni = trenutni->sljedeci; // Prelazi na sljedeći poddirektorij dok ne pronađe odgovarajući

    if (trenutni) { // Ako je direktorij pronađen
        push(stog, trenutni); // Dodaje direktorij na stog (mijenja trenutni direktorij)
        printf("Promijenili ste direktorij u '%s'.\n", ime); // Ispisuje poruku o uspješnoj promjeni
    }
    else { // Ako nije pronađen odgovarajući direktorij
        printf("Direktorij '%s' nije pronaden.\n", ime); // Ispisuje poruku o grešci
    }

    return 0; // Vraća uspješan završetak funkcije
}



void brisiDirektorije(DirPozicija dir) {
    while (dir->poddirektorij) { // dok postoje poddirektoriji
        DirPozicija temp = dir->poddirektorij; // Privremeno sprema prvi poddirektorij
        dir->poddirektorij = temp->sljedeci;   // Ažurira pokazivač na sljedeći poddirektorij
        brisiDirektorije(temp); // Rekurzivno briše poddirektorije
        free(temp); // Oslobađa memoriju za trenutni direktorij
    }
}
// Funkcija briše sve elemente stoga i oslobađa memoriju
// Parametar:
// - stog: pokazivač na glavu stoga
void brisiStog(StogPozicija stog) {
    while (stog->sljedeci) { // Dok stog ima elemenata
        StogPozicija temp = stog->sljedeci; // Privremeno sprema vrh stoga
        stog->sljedeci = temp->sljedeci;    // Ažurira pokazivač na sljedeći element u stogu
        free(temp); // Oslobađa memoriju za trenutni element stoga
    }
}
