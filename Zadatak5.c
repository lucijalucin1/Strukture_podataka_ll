#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>

typedef struct node {
	double value;
	struct node* next;
}Node;

void push(Node** head, double val); //push a value onto the stack

double pop(Node** head); //pop a value from the stack

double postfix(char* filename); // Evaluate a postfix expression read from a file

int main() {
	char filename[100];
	printf("Enter file name containing postfix expression:\n");
	scanf("%s", filename);

	double result = postfix(filename);

	printf("Result: %lf", result);


	return 0;
}

void push(Node** head, double val) {

	Node* newNumber = (Node*)malloc(sizeof(Node));

	if (!newNumber) {
		printf("Memmory allocation failed!\n");
		exit(1);
	}

	newNumber->value = val;
	newNumber->next = *head; //New node points to the previous top of the stack
	*head = newNumber; // New number now becomes the new head
}

double pop(Node** head) {

	//Check if the stack is empty
	if (*head == NULL) {
		printf("Error: stack underflow!");
		exit(1);
	}

	Node* temp = *head; // temporeraly store the head
	double val = temp->value; //Save the value of the head to return
	*head = temp->next; //move the head to the next element
	free(temp); //free memory of the removed node

	return val; //return the popped value
}


double postfix(char* filename) {

	FILE* file = fopen(filename, "r");

	if (!file) {
		printf("Error: could not open file.\n");
		exit(1);
	}

	Node* head = NULL;

	char buffer[50]; // Temporary storage for tokens read from file

	// Read token by token (numbers or operators)
	while (fscanf(file, "%s", buffer)==1) {

		char* endptr;
		double num = strtod(buffer, &endptr);

		//if the entire token is a number
		if (*endptr == '\0') {
			push(&head, num);
		}

		// Otherwise, it's an operator: pop two operands
		else {
			double second = pop(&head);
			double first = pop(&head);
			double result;

			switch (buffer[0]) {
			case '+': result = first + second; break;
			case '-': result = first - second; break;
			case '*': result = first * second; break;
			case '/': result = first / second; break;
			default:
				printf("Unknown operator!\n");
				fclose(file);
				return -1;
			}

			push(&head, result);  // Push the result of the operation back onto the stack
		}
	}

	fclose(file);
	double finalResult = pop(&head); // Final result is on top of the stack

	// Free remaining nodes (if any)
	while (head != NULL) {
		Node* temp = head;
		head = head->next;
		free(temp);
	}

	return finalResult;
}


