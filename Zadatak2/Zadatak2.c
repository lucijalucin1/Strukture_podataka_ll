#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <string.h>



typedef struct person {
	char name[50];
	char lname[50];
	int year;
	struct person* next;
}_per;

void addBeginning(_per** head);

void addEnd(_per** head);

void printlist(_per* head);

_per* findByLname(_per* head, char* lname);

void deleteByLname(_per** head, char* lname);



int main() {

	_per* head = NULL;

	addBeginning(&head); //Dodavanje nove osobe na po�etak liste
	addBeginning(&head);

	addEnd(&head);

	printf("Lista osoba:\n");
	printlist(head);

	char searchLname[50];
	printf("Pretre�ite osobu po prezimenu:\n");
	scanf("%s", searchLname);

	_per* found = findByLname(head, searchLname);

	if (found != NULL)
		printf("Prona�ena osoba: %s %s, %d.\n", found->name, found->lname, found->year);
	else
		printf("Osoba sa prezimenom '%s' nije prona�ena.\n", searchLname);

	char deleteLname[50];
	printf("Obri�ite osobu po prezimenu:\n");
	scanf("%s", deleteLname);

	deleteByLname(&head, deleteLname);

	printf("Lista trenuta�no:\n");
	printlist(head);

	
	free(head); // Osloba�anje memorije nakon kori�tenja


	return 0;
}

void addBeginning(_per** head) {
	_per* newPer = (_per*)malloc(sizeof(_per));

	if (!newPer) {
		printf("Gre�ka pri alokaciji memorije!\n");
		return head; // Vra�amo originalnu listu bez promjena
	}

	printf("Dodajte ime i prezime(na po�etak liste):\n");
	scanf("%s %s", newPer->name, newPer->lname);


	printf("Unesi godinu ro�enja:\n");
	scanf("%d", &newPer->year);

	newPer->next = *head; // novi �vor pokazuje na stari prvi

	
	*head= newPer;  // "head" sada pokazuje na novi �vorpe

}

void addEnd(_per** head) {
	_per* newPer = (_per*)malloc(sizeof(_per));

	if (!newPer) {
		printf("Gre�ka pri alokaciji memorije!\n");
		return head; // Vra�amo originalnu listu bez promjena
	}

	printf("Dodaj ime i prezime(na kraj liste):\n");
	scanf("%s %s", newPer->name, newPer->lname);


	printf("Unesi godinu ro�enja:\n");
	scanf("%d", &newPer->year);

	newPer->next = NULL; //novi �vor je zadnji, pa pokazuje na NULL

	// Ako je lista prazna (nema elemenata)
	if (*head == NULL) {
		*head = newPer; // novi �vor postaje prvi i jedini
		return;
	}

	// Ina�e prona�i zadnji element
	_per* current = *head;
	while (current->next != NULL) {
		current = current->next; // ide do kraja
	}

	
	current->next = newPer; // Postavi zadnji element da pokazuje na novi �vor

}

void printlist(_per* head) {
	if (head == NULL) {
		printf("Lista je prazna.\n");
		return;
	}

	while (head != NULL) {
		printf("%s %s, %d.\n", head->name, head->lname, head->year);
		head = head->next;
	}
}

_per* findByLname(_per* head, char* lname) {
	while (head != NULL) {
		// usporedba prezimena (bez obzira na velika/mala slova)
		if (strcmp(head->lname, lname) == 0) {
			return head; // prona�ena osoba
		}
		head = head->next; // idi na sljede�u
	}
	return NULL; // nije prona�ena
}

void deleteByLname(_per** head,char* lname) {

	if (*head == NULL) return; //lista je prazna

	_per* current = *head;
	_per* previous = NULL;

	// Ako je osoba prva u listi
	if (strcmp(current->lname, lname) == 0) {
		*head = current->next;
		free(current);
		return;
	}

	// Ina�e prolazi kroz listu dok ne prona�e odgovaraju�e prezime
	while (current != NULL && strcmp(current->lname, lname) != 0) {
		previous = current;
		current = current->next;
	}

	if (current == NULL) {
		printf("Osoba nije prona�ena.\n");
		return;
	}

	// Bri�emo �vor iz sredine
	previous->next = current->next;
	free(current);
}