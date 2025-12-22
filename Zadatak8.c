#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>

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
Queue front = NULL;
Queue rear = NULL;


Node createNode(int value); //create a new BST node

Node insert(Node root, int value); //insert value into BST

Node search(Node root, int value); //search value in BST

Node findMin(Node root); //find minimum value node(used in deletenode)

Node deleteNode(Node root, int value); //delete node from BST

void inorder(Node root);

void preorder(Node root);

void postorder(Node root);

int enqueue(Node node); //add element to the end of the queue

Node dequeue(); //remove element from the begininng of the queue

int levelOrder(Node root); //level order traversal

int main() {

	Node root = NULL;
	int choice, value, result;

	do {
		printf("\nBST MENU:\n");
		printf("1. Insert element\n");
		printf("2. Search element\n");
		printf("3. Delete element\n");
		printf("4. Inorder traversal\n");
		printf("5. Preorder traversal\n");
		printf("6. Postorder traversal\n");
		printf("7. Level order traversal\n");
		printf("0. Exit\n");
		printf("Choice: ");
		scanf("%d", &choice);

		switch (choice) {

        case 1:
            printf("Enter value: ");
            scanf("%d", &value);
            root = insert(root, value);
            break;

        case 2:
            printf("Enter value to search: ");
            scanf("%d", &value);
            if (search(root, value))
                printf("Element FOUND\n");
            else
                printf("Element NOT FOUND\n");
            break;

        case 3:
            printf("Enter value to delete: ");
            scanf("%d", &value);
            root = deleteNode(root, value);
            break;

        case 4:
            printf("Inorder: ");
            inorder(root);
            printf("\n");
            break;

        case 5:
            printf("Preorder: ");
            preorder(root);
            printf("\n");
            break;

        case 6:
            printf("Postorder: ");
            postorder(root);
            printf("\n");
            break;

        case 7:
            printf("Level order: ");
            result = levelOrder(root);
            printf("\nNodes printed: %d\n", result);
            break;

        case 0:
            printf("Exiting program...\n");
            break;

        default:
            printf("Invalid choice!\n");
        }
	
     
	} while(choice != 0);



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

    if (value < root->data)
        root->left = insert(root->left, value);
    else if (value > root->data)
        root->right = insert(root->right, value);

    return root;
}


Node search(Node root, int value) {
    if (root == NULL || root->data == value)
        return root;

    if (value < root->data)
        return search(root->left, value);
    else
        return search(root->right, value);
}


Node findMin(Node root) {
    while (root && root->left != NULL)
        root = root->left;
    return root;
}


Node deleteNode(Node root, int value) {
    if (root == NULL)
        return NULL;

    if (value < root->data)
        root->left = deleteNode(root->left, value);
    else if (value > root->data)
        root->right = deleteNode(root->right, value);
    else {
        /* Node with no child or one child */
        if (root->left == NULL) {
            Node temp = root->right;
            free(root);
            return temp;
        }
        if (root->right == NULL) {
            Node temp = root->left;
            free(root);
            return temp;
        }

        /* Node with two children */
        Node temp = findMin(root->right);
        root->data = temp->data;
        root->right = deleteNode(root->right, temp->data);
    }
    return root;
}

void inorder(Node root) {
    if (root == NULL)
        return;

    inorder(root->left);
    printf("%d ", root->data);
    inorder(root->right);
}

void preorder(Node root) {
    if (root == NULL)
        return;

    printf("%d ", root->data);
    preorder(root->left);
    preorder(root->right);
}

void postorder(Node root) {
    if (root == NULL)
        return;

    postorder(root->left);
    postorder(root->right);
    printf("%d ", root->data);
}

int enqueue(Node node) {
    Queue temp = (Queue)malloc(sizeof( struct queueNode));
    if (temp == NULL)
        return 0;

    temp->treeNode = node;
    temp->next = NULL;

    if (rear == NULL) {
        front = rear = temp;
        return 1;
    }

    rear->next = temp;
    rear = temp;
    return 1;
}

Node dequeue() {
    if (front == NULL)
        return NULL;

    Queue temp = front;
    Node result = temp->treeNode;

    front = front->next;
    if (front == NULL)
        rear = NULL;

    free(temp);
    return result;
}

int levelOrder(Node root) {
    if (root == NULL)
        return 0;

    int count = 0;
    enqueue(root);

    while (front != NULL) {
        Node current = dequeue();
        printf("%d ", current->data);
        count++;

        if (current->left)
            enqueue(current->left);
        if (current->right)
            enqueue(current->right);
    }

    return count;
}