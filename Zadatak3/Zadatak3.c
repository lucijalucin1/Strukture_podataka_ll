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

void addAfter(_per* head, char* lname);

void addBefore(_per** head, char* lname);

void sortByLname(_per* head);

void writeToFile(_per* head, char* filename);

_per* readFromFile(char* filename);



int main() {

	_per* head = NULL;

	addBeginning(&head); //Adding new person to the beginning of the list
	addBeginning(&head);

	addEnd(&head);

	printf("List of people:\n");
	printlist(head);

	char searchLname[50];
	printf("Search a person by last name:\n");
	scanf("%s", searchLname);

	_per* found = findByLname(head, searchLname);

	if (found != NULL)
		printf("Person found: %s %s, %d.\n", found->name, found->lname, found->year);
	else
		printf("Person with last name '%s' not found.\n", searchLname);

	char deleteLname[50];
	printf("Delete a person by last name:\n");
	scanf("%s", deleteLname);

	deleteByLname(&head, deleteLname);

	printf("Current list:\n");
	printlist(head);

	char lastn[50];
	printf("After which last name do you want to add a new person?\n");
	scanf("%s", lastn);

	addAfter(head, lastn);


	printf("Current list:\n");
	printlist(head);

	char lastBefore[50];
	printf("Before which last name do you want to add a new person?\n");
	scanf("%s", lastBefore);

	addBefore(&head, lastBefore);

	printf("Current list:\n");
	printlist(head);

	sortByLname(head);

	printf("List sorted by last name:\n");
	printlist(head);

	writeToFile(head, "people.txt");

	_per* temp;
	while (head != NULL) {
		temp = head;
		head = head->next;
		free(temp);
	}
	head = NULL;

	head = readFromFile("people.txt");
	printlist(head);



	// Freeing memory of the whole list
	while (head != NULL) {
		temp = head;        // remember the current element
		head = head->next;  // go to the next element
		free(temp);         // free the remembered element
	}


	return 0;
}

void addBeginning(_per** head) {
	_per* newPer = (_per*)malloc(sizeof(_per));

	if (!newPer) {
		printf("Memory allocation error!\n");
		return -1; // Return the original list without changes
	}

	printf("Enter name and last name (add to the beginning):\n");
	scanf("%s %s", newPer->name, newPer->lname);


	printf("Enter birth year:\n");
	scanf("%d", &newPer->year);

	newPer->next = *head; // the new node point to the current first node


	*head = newPer;  // head now points to the new node

}

void addEnd(_per** head) {
	_per* newPer = (_per*)malloc(sizeof(_per));

	if (!newPer) {
		printf("Memory allocation error!\n");
		return -1; // Return the original list without changes
	}

	printf("Enter name and last name (add to the end):\n");
	scanf("%s %s", newPer->name, newPer->lname);


	printf("Enter birth year:\n");
	scanf("%d", &newPer->year);

	newPer->next = NULL; //the new node is the last, it points to NULL

	// if the list is empty
	if (*head == NULL) {
		*head = newPer; // the new node becomes the first and only element
		return;
	}

	// Otherwise, find the last node
	_per* current = *head;
	while (current->next != NULL) {
		current = current->next; //move to the end
	}


	current->next = newPer; // connect the last node to the new one
}

void printlist(_per* head) {
	if (head == NULL) {
		printf("The list is empty.\n");
		return;
	}

	while (head != NULL) {
		printf("%s %s, %d.\n", head->name, head->lname, head->year);
		head = head->next;
	}
}

_per* findByLname(_per* head, char* lname) {
	while (head != NULL) {
		// last name comparison
		if (strcmp(head->lname, lname) == 0) {
			return head; // person found
		}
		head = head->next; // move to the next node
	}
	return NULL; // person not found
}

void deleteByLname(_per** head, char* lname) {

	if (*head == NULL) return; 

	_per* current = *head;
	_per* previous = NULL;

	// if the wanted person is the first node
	if (strcmp(current->lname, lname) == 0) {
		*head = current->next;
		free(current);
		return;
	}

	// Otherwise, go through the list until you find the right one
	while (current != NULL && strcmp(current->lname, lname) != 0) {
		previous = current;
		current = current->next;
	}

	if (current == NULL) {
		printf("Person not found.\n");
		return;
	}

	// Delete a node from the middle
	previous->next = current->next;
	free(current);

}

void addAfter(_per* head, char* lname) {
	if (head == NULL) {
		printf("The list is empty — cannot add a new element.\n");
		return;
	}

	_per* newPer = (_per*)malloc(sizeof(_per));
	if (!newPer) {
		printf("Memory allocation error!\n");
		return -1;
	}

	printf("Enter the name and last name of the new person:\n ");
	scanf("%s %s", newPer->name, newPer->lname);
	printf("Enter birth year:\n ");
	scanf("%d", &newPer->year);

	_per* q;
	for (q = head; q != NULL; q = q->next) {
		if (strcmp(q->lname, lname) == 0) {
			// Insert after the found element
			newPer->next = q->next;
			q->next = newPer;

			printf("Person successfully added after '%s'.\n", lname);
			return; 
		}
	}

	printf("Person with last name '%s' not found — entry discarded.\n", lname);
	free(newPer); // stops memory leaks
}

void addBefore(_per** head, char* lname) {
	if (*head == NULL) {
		printf("The list is empty — cannot add a new element.\n");
		return;
	}

	_per* newPer = (_per*)malloc(sizeof(_per));
	if (!newPer) {
		printf("Memory allocation error!\n");
		return -1;
	}

	printf("Enter the name and last name of the new personn:\n ");
	scanf("%s %s", newPer->name, newPer->lname);
	printf("Enter birth year:\n ");
	scanf("%d", &newPer->year);

	_per* q = *head;
	_per* p = NULL;

	// If we need to insert before the first node
	if (strcmp(q->lname, lname) == 0) {
		newPer->next = *head;
		*head = newPer;
		printf("Person added before '%s'.\n", lname);
		return;
	}

	// Otherwise, go through the list
	for (q = *head; q != NULL; q = q->next) {
		if (strcmp(q->lname, lname) == 0) {
			// Last name found — insert the new element before it
			newPer->next = q;
			p->next = newPer;
			printf("Person added before '%s'.\n", lname);
			return;
		}
		p = q;  // store the previous node
	}

	//if the last name is not found
	printf("Person with last name '%s' not found — entry discarded.\n", lname);
	free(newPer);
}

void sortByLname(_per* head) {
	if (head == NULL || head->next == NULL) {
		printf("The list is empty or has only one element — nothing to sort.\n");
		return;
	}

	_per* i;  
	_per* j; 

	// go through the list multiple times
	for (i = head; i->next != NULL; i = i->next) {
		for (j = head; j->next != NULL; j = j->next) {
			if (strcmp(j->lname, j->next->lname) > 0) {
				// swap node contents(not pointers)
				char tempName[50];
				char tempLname[50];
				int tempYear;

				strcpy(tempName, j->name);
				strcpy(j->name, j->next->name);
				strcpy(j->next->name, tempName);

				strcpy(tempLname, j->lname);
				strcpy(j->lname, j->next->lname);
				strcpy(j->next->lname, tempLname);

				tempYear = j->year;
				j->year = j->next->year;
				j->next->year = tempYear;
			}
		}
	}

	printf("\nList successfully sorted by last name!\n");
}

void writeToFile(_per* head, char* filename) {
	FILE* file = fopen(filename, "w");

	if (!file) {
		printf("Error opening file!\n");
		return;
	}

	_per* q = head;

	while (q != NULL) {
		// Write each person in one line: name surname year
		fprintf(file, "%s %s %d\n", q->name, q->lname, q->year);
		q = q->next;
	}
	
	fclose(file);
	printf("Data sucessfully written to the file '%s'.\n", filename);
}

_per* readFromFile(char* filename) {
	FILE* file = fopen(filename, "r");
	if (!file) {
		printf("Error opening file!\n");
		return NULL;
	}

	_per* head = NULL;
	_per* last = NULL;

	while (1) {
		_per* newPer = (_per*)malloc(sizeof(_per));
		if (!newPer) {
			printf("Memory allocation error!\n");
			fclose(file);
			return head;
		}

		// Read one record: name surname yea
		if (fscanf(file, "%s %s %d", newPer->name, newPer->lname, &newPer->year) != 3) {
			free(newPer);
			break; // end of file
		}

		newPer->next = NULL;

		if (head == NULL)
			head = newPer;       // first element becomes head
		else
			last->next = newPer; // add to the end of the list

		last = newPer; // move the pointer to the last element
	}

	fclose(file);
	printf("Data successfully read from file '%s'.\n", filename);

	return head;
}


