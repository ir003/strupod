#define _CRT_SECURE_NO_WARNINGS

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_NAME 50
#define EMPTY_STACK 1
#define MALLOC_ERROR -1

// Struktura za direktorij
typedef struct dir {
    char ime[MAX_NAME];        // Ime direktorija
    struct dir* poddirektorij; // Pokazivač na prvi poddirektorij
    struct dir* sljedeci;      // Pokazivač na sljedeći direktorij u istoj razini
} Dir, *DirPozicija;

// Struktura za stog (povijest direktorija)
typedef struct stog {
    DirPozicija dirPoz;        // Pokazivač na trenutno aktivni direktorij
    struct stog* sljedeci;     // Pokazivač na sljedeći element stoga
} Stog, *StogPozicija;

// Funkcije
DirPozicija noviDirektorij(char* ime);
int dodajPoddirektorij(DirPozicija roditelj, DirPozicija pod);
int ispisiSadrzaj(DirPozicija dir, int dubina);
int push(StogPozicija stog, DirPozicija dir);
int pop(StogPozicija stog);
int promijeniDirektorij(StogPozicija stog, char* ime);
void brisiDirektorije(DirPozicija dir);
void brisiStog(StogPozicija stog);

int main() {
    char naredba;
    char ime[MAX_NAME] = { 0 };         // Privremeno ime direktorija
    Dir root = { "C:", NULL, NULL };    // Root direktorij
    Stog stog = { .dirPoz = &root, .sljedeci = NULL }; // Stog za povijest direktorija

    printf("1 - Dodaj direktorij\n2 - Promijeni direktorij\n3 - Vrati se na roditeljski direktorij\n");
    printf("4 - Ispisi sadrzaj direktorija\n5 - Kraj programa\n");

    do {
        printf("\nUnesi naredbu: ");
        scanf(" %c", &naredba);

        switch (naredba) {
        case '1': // Dodaj poddirektorij
            printf("Unesi ime novog direktorija: ");
            scanf(" %s", ime);
            DirPozicija novi = noviDirektorij(ime);
            if (novi) {
                dodajPoddirektorij(stog.dirPoz, novi);
                printf("Direktorij '%s' je dodan.\n", ime);
            }
            else {
                printf("Greska pri stvaranju direktorija.\n");
            }
            break;

        case '2': // Promijeni u poddirektorij
            printf("Unesi ime direktorija: ");
            scanf(" %s", ime);
            promijeniDirektorij(&stog, ime);
            break;

        case '3': // Vrati se na roditeljski direktorij
            if (pop(&stog)) {
                printf("Vec ste u root direktoriju.\n");
            }
            else {
                printf("Vracao se u roditeljski direktorij.\n");
            }
            break;

        case '4': // Ispisi sadrzaj direktorija
            printf("Sadrzaj direktorija '%s':\n", stog.dirPoz->ime);
            ispisiSadrzaj(stog.dirPoz, 0);
            break;

        case '5': // Kraj programa
            printf("Zavrsetak programa.\n");
            break;

        default:
            printf("Nepoznata naredba.\n");
        }
    } while (naredba != '5');

    brisiDirektorije(&root);  // Brisanje svih direktorija
    brisiStog(&stog);         // Brisanje stoga
    return 0;
}

// Funkcija za stvaranje novog direktorija
DirPozicija noviDirektorij(char* ime) {
    DirPozicija novi = (DirPozicija)malloc(sizeof(Dir));
    if (!novi) return NULL;

    strcpy(novi->ime, ime);
    novi->poddirektorij = NULL;
    novi->sljedeci = NULL;
    return novi;
}

// Dodaje poddirektorij unutar roditelja
int dodajPoddirektorij(DirPozicija roditelj, DirPozicija pod) {
    pod->sljedeci = roditelj->poddirektorij; // Postavljamo novi poddirektorij na početak liste
    roditelj->poddirektorij = pod;          // Ažuriramo pokazivač roditelja
    return 0;
}

// Ispisuje sadržaj direktorija rekurzivno
int ispisiSadrzaj(DirPozicija dir, int dubina) {
    DirPozicija trenutni = dir->poddirektorij;
    while (trenutni) {
        for (int i = 0; i < dubina; i++) printf("  "); // Ispisujemo razmake ovisno o dubini
        printf("%s\n", trenutni->ime);
        ispisiSadrzaj(trenutni, dubina + 1); // Rekurzivno ispisivanje poddirektorija
        trenutni = trenutni->sljedeci;
    }
    return 0;
}
// Funkcija dodaje novi element na stog (trenutni direktorij)
// Parametri: 
// - stog: pokazivač na glavu stoga
// - dir: pokazivač na direktorij koji se dodaje na stog
int push(StogPozicija stog, DirPozicija dir) {
    StogPozicija novi = (StogPozicija)malloc(sizeof(Stog)); // Dinamička alokacija memorije za novi element stoga
    if (!novi) return MALLOC_ERROR; // Provjera je li alokacija uspjela; ako nije, vraća grešku

    novi->dirPoz = dir;            // Postavlja pokazivač na direktorij koji dodajemo
    novi->sljedeci = stog->sljedeci; // Novi element stoga pokazuje na trenutni vrh stoga
    stog->sljedeci = novi;         // Glava stoga sada pokazuje na novi element kao vrh stoga

    return 0;                      // Vraća uspješan završetak funkcije
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
// Funkcija ispisuje sadržaj trenutnog direktorija rekurzivno
// Parametri:
// - dir: pokazivač na trenutni direktorij
// - dubina: trenutna dubina u hijerarhiji direktorija (koristi se za formatiranje)
int ispisiSadrzaj(DirPozicija dir, int dubina) {
    DirPozicija trenutni = dir->poddirektorij; // Početak poddirektorija aktivnog direktorija

    while (trenutni) { // Prolazi kroz sve poddirektorije
        for (int i = 0; i < dubina; i++) printf("  "); // Ispisuje razmake ovisno o dubini direktorija
        printf("%s\n", trenutni->ime); // Ispisuje ime trenutnog direktorija
        ispisiSadrzaj(trenutni, dubina + 1); // Rekurzivno poziva funkciju za poddirektorije
        trenutni = trenutni->sljedeci; // Prelazi na sljedeći direktorij u istoj razini
    }

    return 0; // Vraća uspješan završetak funkcije
}
// Funkcija rekurzivno briše sve poddirektorije i oslobađa memoriju
// Parametar:
// - dir: pokazivač na trenutni direktorij
void brisiDirektorije(DirPozicija dir) {
    while (dir->poddirektorij) { // Dok postoje poddirektoriji
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
