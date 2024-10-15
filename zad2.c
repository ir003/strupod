#define _CRT_SECURE_NO_WARNINGS 
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
typedef struct _person* position;
typedef struct _person {
	//data
	char fname[32], lname[32];
	char birth_year;
	//link
	 position next;
}person;

position find_by_lname(position first, char*lname);

position create_person(position head, char* fname, char* lname, int birth_year) {
	position new_person = NULL;
	//stvaranje prve osobe
	new_person = (position*)malloc(sizeof(person));
		if (!new_person) {
			printf("error Malloc failed");
			return NULL;
		}

	strcpy(new_person->fname, fname);
	strcpy(new_person->lname, lname);
	new_person->birth_year = birth_year;
	new_person->next = NULL;

}
int print_list(position first) {
	position temp = first;
	while(temp){
		printf("first name: %s\nlast name : %s\n birth year: %d", temp->fname, temp->lname, temp->birth_year);
		temp = temp->next;
	}
	return 0;
}
int prepend_list(position head, char* fname, char* lname, int birth_year) {
	position new_person = NULL;
	new_person = create_person(fname, lname, birth_year);
	new_person = (position*)malloc(sizeof(person));
	if (!new_person) {
		printf("error");
		return NULL;
	}
	new_person = head->next;
	head->next = new_person; 

	return 0;

}
int append_list(position head, char* fname, char* lname, int birth_year) { //dodavanje na kraj
	position new_person = NULL, last = NULL;
	new_person = create_person(fname, lname, birth_year);
	if (!new_person) {
		printf("error");
		return NULL;
	}
	
	last = find_last(head);
	insert_after(last, new_person);
	
}

position find_last(position head) {
	position temp = head;
	while (temp->next) {
		temp = temp->next;
	}
	return temp;
}
int insert_after(position previuos, position to_insert)
{

}

int main()
{
	person head = { .fname = "",.lname = "",.birth_year = "",.next = NULL };

	return 0;







}
position find_by_lname(position first, char* lname)
{
	position temp = NULL;
	while (temp)
	{
		if (strcmp(temp->lname, lname)==0) //ako su jednaki vraca 0
		{
			return temp;
		}
		temp = temp->next;

	}
	return NULL;
}



//dodatak
// funkcija za brisanje: find_previous u kojoj cemo pronac prwethodnika i postavit da pokazuje na sljedbenika od "toDelet"
