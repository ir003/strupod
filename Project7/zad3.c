#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#define MAX_SIZE 50

/*void sort(person* headptr) {
	position current = headptr;
	bool swapped = true;

	if (!current->next || !current->next->next)
	{
		return;
	}

	while (swapped) {
		current = headptr;
		swapped = false;
		while (current->next->next)
		{
			if (strcmp(current->next->lname, current->next->next->lname) > 0) {
				swapped = true;
				position first = current->next;
				position second = first->next;
				first->next = second->next;
				second->next = first;
				current->next = second;
			}
			current = current->next;
		}
	}
}
*/
typedef struct _person* position;
typedef struct _person {
	//data
	char fname[32], lname[32];
	char birth_year;
	position next;
} person;
person* upis(char* fname, char* lname, int birth_year)
{
	for (int i = 0; i < 2; i++)
	{
		fscanf(fp, "%s %s %d", person[i].fname, person[i].lname, person[i].birth_year);


	}
}

position create_person(position head, char* fname, char* lname, int birth_year)
{
	position new_person = NULL;
	//stvaranje prve osobe
	new_person = (position*)malloc(sizeof(person));
	if (!new_person)
	{
		printf("error Malloc failed");
		return NULL;
	}

	strcpy(new_person->fname, fname);
	strcpy(new_person->lname, lname);
	new_person->birth_year = birth_year;
	new_person->next = NULL;

}

int insert_after_person()
{

}

int prepend_list(position head, char* fname, char* lname, int birth_year)
{
	position new_person = NULL;
	new_person = create_person(head, fname, lname, birth_year);

	new_person = head->next;
	head->next = new_person;

	return 0;

}
int ispis_osobe(position person)
{
	printf("\t%s %s rođen %d. godine\n", person->fname, person->lname, person->birth_year);

}
int ispis_liste(position current)
{
	while (current != NULL)
	{
		ispis_osobe(current);
		current = current->next;
	}

}
int append_list(position head)
{
	position newPerson = NULL;
	newPerson = create_person;
	if (!newPerson)
	{
		printf("Malloc error");
	}
	while (head->next != NULL)
	{
		head = head->next;
	}
	newPerson->next = head->next;
	head->next = newPerson;
}
int find_person(position current, char* sur)
{
	while (current != NULL && strcmp(current->lname, sur))
	{
		current = current->next;
	}
	return current;
}

int delete_person(position head, char* sur, position prev)
{
	position temp = NULL;
	prev = find_person(head, sur);
	if (prev != NULL)
	{
		temp = prev->next;
		prev->next = temp->next;
		printf("Osoba izbrisana");

	}
	else printf("Nije pronađena");


}
int main()
{
	char broj = 0;
	FILE* fp = NULL;
	char lname[32], fname[32];
	int birth_year = 0;
	person head = { .fname = {0}, .lname = {0}, .birth_year = 0 };
	upis(fname, lname, birth_year);
	



	printf("1 - dodaj novi element na početak liste\n2- ispiši listu\n3 - dodaj element na kraj liste\n4 - pronađi element\n5- briši element");

	do {
		printf("\n odaberi broj");
		scanf(" %c", &broj);
		switch (broj)
		{
		case '1':
		{
			prepend_list(&head, fname, lname, birth_year);
			break;
		}
		case '2':
		{
			ispis_liste(head.next);
			break;
		}
		case '3':
		{
			append_list(head.next);
			break;
		}
		case '4':
		{
			scanf("%s", lname);
			find_person(head.next, lname);
			break;
		}
		case '5':
		{
			scanf("%c", lname);
			//delete_person(&head, lname);
			break;
		}
		}
		}while (broj != '0');
	
}
