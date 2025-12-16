#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define MAX 50

typedef struct dir* Dir;

struct dir {
	char name[MAX];
	Dir parent;
	Dir child;
	Dir sibling;
};

//Creates a new directory node and initializes all pointers
//Returns pointer to the newly created directory
Dir createDir(const char* name, Dir parent);

int makeDir(Dir current);

//cd dir command, Returns pointer to the new current directory
Dir changeDir(Dir current);

//Moves to the parent directory
Dir goBack(Dir current);

//Lists all subdirectories of the current directory, Returns number of listed directories
int listDir(Dir current);

//free memory
int freeTree(Dir current);

int main() {

	int choice;

	// Create root directory
	Dir root = createDir("C:", NULL);
	Dir current = root;

	do {
		printf("\nCurrent directory: %s\n", current->name);
		printf("1 - md\n");
		printf("2 - cd dir\n");
		printf("3 - cd..\n");
		printf("4 - dir\n");
		printf("5 - exit\n");
		printf("Choice: ");
		scanf("%d", &choice);

		switch (choice) {
		case 1:
			makeDir(current);
			break;
		case 2:
			current = changeDir(current);
			break;
		case 3:
			current = goBack(current);
			break;
		case 4:
			listDir(current);
			break;
		case 5:
			printf("Exiting program.\n");
			break;
		default:
			printf("Invalid option!\n");
		}
	} while (choice != 5);

	// Free all dynamically allocated memory
	freeTree(root);


	return 0;
}

Dir createDir(const char* name, Dir parent) {

	Dir newDir = (Dir)malloc(sizeof(struct dir));

	if (newDir == NULL) {
		printf("Memory allocation failed!\n");
		return NULL;
	}

	strcpy(newDir->name, name); //copy directory name

	//set relations
	newDir->parent = parent;
	newDir->child = NULL;
	newDir->sibling = NULL;

	return newDir;
}

int makeDir(Dir current) {
	char name[MAX];

	printf("Enter directory name: ");
	scanf("%s", name);

	Dir newDir = createDir(name, current);
	if (newDir == NULL)
		return 0;

	//If the current directory has no children, new directory becomes the first child
	if (current->child == NULL) {
		current->child = newDir;
	}

	//Otherwise, add the new directory to the end, of the sibling list
	else {
		Dir temp = current->child;
		while (temp->sibling != NULL)
			temp = temp->sibling;

		temp->sibling = newDir;
	}

	return 1;
}

Dir changeDir(Dir current) {
	char name[MAX];

	printf("Enter directory name: ");
	scanf("%s", name);

	// Start searching from the first child
	Dir temp = current->child;

	while (temp != NULL) {
		// If directory name matches, change directory
		if (strcmp(temp->name, name) == 0)
			return temp;

		temp = temp->sibling;
	}

	printf("Directory does not exist!\n");  // Directory not found
	return current;
}

Dir goBack(Dir current) {
	if (current->parent != NULL)
		return current->parent;

	printf("Already in root directory!\n");
	return current;
}


int listDir(Dir current) {
	Dir temp = current->child;
	int count = 0;

	// No subdirectories
	if (temp == NULL) {
		printf("Directory is empty.\n");
		return 0;
	}

	// Traverse sibling list and print names
	while (temp != NULL) {
		printf("%s\n", temp->name);
		temp = temp->sibling;
		count++;
	}

	return count;
}


int freeTree(Dir current) {
	if (current == NULL)
		return 0;

	// Free all subdirectories first
	freeTree(current->child);

	// Free all sibling directories
	freeTree(current->sibling);

	// Free current directory
	free(current);

	return 1;
}
