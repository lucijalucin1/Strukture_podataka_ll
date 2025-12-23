#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

typedef struct node* Node;
struct node {
    int data;
    Node left;
    Node right;
};


typedef struct queueNode* Queue;
struct queueNode {
    Node treeNode;
    Queue next;
};


Node createNode(int value);

Node insert(Node root, int value); //level order insert

int replace(Node root); //Replace values with sum of descendants

void inorderToFile(Node root, FILE* fp); //Inorder traversal to file 

int enqueue(Queue* front, Queue* rear, Node node);

Node dequeue(Queue* front, Queue* rear);



int main() {
    Node root = NULL;
    int n = 10;
    int value;

    srand((unsigned)time(NULL));

    for (int i = 0; i < n; i++) {
        value = rand() % 81 + 10;   // <10, 90>
        root = insert(root, value);
    }

    FILE* fp = fopen("order.txt", "w");
    if (fp == NULL) {
        printf("File error!\n");
        return 1;
    }

    fprintf(fp, "Inorder after insert:\n");
    inorderToFile(root, fp);

    replace(root);

    fprintf(fp, "\n\nInorder after replace:\n");
    inorderToFile(root, fp);

    fclose(fp);

    printf("Done. Check order.txt\n");
    return 0;
}



Node createNode(int value) {
    Node newNode = (Node)malloc(sizeof(struct node));
    if (newNode == NULL)
        return NULL;

    newNode->data = value;
    newNode->left = NULL;
    newNode->right = NULL;
    return newNode;
}


Node insert(Node root, int value) {
    if (root == NULL)
        return createNode(value);

    Queue front = NULL;
    Queue rear = NULL;
    enqueue(&front, &rear, root);

    while (front != NULL) {
        Node current = dequeue(&front, &rear);

        if (current->left == NULL) {
            current->left = createNode(value);
            break;
        }
        else {
            enqueue(&front, &rear, current->left);
        }

        if (current->right == NULL) {
            current->right = createNode(value);
            break;
        }
        else {
            enqueue(&front, &rear, current->right);
        }
    }

    return root;
}


int replace(Node root) {
    if (root == NULL)
        return 0;

    int leftSum = replace(root->left);
    int rightSum = replace(root->right);

    int oldValue = root->data;
    root->data = leftSum + rightSum;

    return root->data + oldValue;
}


void inorderToFile(Node root, FILE* fp) {
    if (root == NULL)
        return;

    inorderToFile(root->left, fp);
    fprintf(fp, "%d ", root->data);
    inorderToFile(root->right, fp);
}


int enqueue(Queue* front, Queue* rear, Node node) {
    Queue temp = (Queue)malloc(sizeof(struct queueNode));
    if (temp == NULL)
        return 0;

    temp->treeNode = node;
    temp->next = NULL;

    if (*rear == NULL) {
        *front = *rear = temp;
        return 1;
    }

    (*rear)->next = temp;
    *rear = temp;
    return 1;
}


Node dequeue(Queue* front, Queue* rear) {
    if (*front == NULL)
        return NULL;

    Queue temp = *front;
    Node result = temp->treeNode;

    *front = (*front)->next;
    if (*front == NULL)
        *rear = NULL;

    free(temp);
    return result;
}