#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#define ERROR_OPENING_FILE 1
#define BUFFER_SIZE 1024
#define MAX_BODOVA 100
#include <stdlib.h>
#include <string.h>


typedef struct {
    char ime[BUFFER_SIZE];
    char prezime[BUFFER_SIZE];
    int bodovi;
} Student;

int count_rows(char* filename);
void ispis(Student* studenti, int brojStudenata);
Student* upis(char* filename, int brojStudenata);



int main()
{
    char filename[] = "studenti.txt";
    int brojStudenata = count_rows(filename);

    Student* studenti = upis(filename, brojStudenata);
    ispis(studenti, brojStudenata);
    free(studenti);


}
int count_rows(char* filename)
{
    FILE* fp = NULL;
    char buffer[BUFFER_SIZE];
    fp = fopen(filename, "r");
    int row_count = 0;
    if (fp == NULL)
    {
        printf("Error opening file");
        return ERROR_OPENING_FILE;
    }

    while (!feof(fp))
    {
        fgets(buffer, BUFFER_SIZE, fp);
        ++row_count;
    }
    fclose(fp);
    return row_count;

}

Student* upis(char* filename, int brojStudenata)
{
    FILE* fp = NULL;
    Student* studenti = (Student*)malloc(brojStudenata * sizeof(Student));

    for (int i = 0; i < brojStudenata; i++)
    {
        fscanf(fp, "%s %s %d", studenti[i].ime, studenti[i].prezime, &studenti[i].bodovi);

    }
    fclose(fp);
    return studenti;

}

void ispis(Student* studenti, int brojStudenata)
{

    for (int i = 0; i < brojStudenata; i++) {

        double relativan_br_bodova = (studenti[i].bodovi / MAX_BODOVA) * 100.0;


        printf("%s %s ima %d bodova, ?to je %.2f%%\n",
            studenti[i].ime, studenti[i].prezime,
            studenti[i].bodovi, relativan_br_bodova);
    }
}
