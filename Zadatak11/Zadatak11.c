#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_NAME 50
#define HASH_SIZE 11


   /* City binary search tree */
typedef struct City {
    char name[MAX_NAME];
    int population;
    struct City* left;
    struct City* right;
} City;

/* Country linked list node (hash collision list) */
typedef struct Country {
    char name[MAX_NAME];
    City* cities;
    struct Country* next;
} Country;

/* Hash table */
typedef struct HashTable {
    Country* table[HASH_SIZE];
} HashTable;



int hashFunction(char*);
City* insertCity(City*, char*, int);
void printCityTree(City*);
void readCities(char*, City**);

Country* insertCountry(Country*, char*);
Country* findCountry(Country*, char*);
void printCountries(Country*);

void initHashTable(HashTable*);
void insertIntoHash(HashTable*, char*, char*);
void printHashTable(HashTable*);


int main() {

    HashTable hash;
    initHashTable(&hash);

    FILE* file = fopen("countries.txt", "r");
    if (!file) {
        printf("Cannot open countries.txt\n");
        return -1;
    }

    char country[MAX_NAME];
    char cityFile[MAX_NAME];

    while (fscanf(file, "%s %s", country, cityFile) == 2) {
        insertIntoHash(&hash, country, cityFile);
    }

    fclose(file);

    printf("\n===== HASH TABLE CONTENT =====\n");
    printHashTable(&hash);

    return 0;
}



   /* Hash function: sum of ASCII values of first 5 letters */
int hashFunction(char* name) {
    int sum = 0;
    for (int i = 0; i < 5 && name[i] != '\0'; i++)
        sum += (int)name[i];

    return sum % HASH_SIZE;
}

/* Initialize hash table */
void initHashTable(HashTable* hash) {
    for (int i = 0; i < HASH_SIZE; i++)
        hash->table[i] = NULL;
}

/* Insert country into hash table */
void insertIntoHash(HashTable* hash, char* countryName, char* cityFile) {

    int index = hashFunction(countryName);

    hash->table[index] = insertCountry(hash->table[index], countryName);

    Country* c = findCountry(hash->table[index], countryName);
    if (c != NULL)
        readCities(cityFile, &c->cities);
}

/* Print entire hash table */
void printHashTable(HashTable* hash) {
    for (int i = 0; i < HASH_SIZE; i++) {
        printf("\n[%d]\n", i);
        printCountries(hash->table[i]);
    }
}



   /* Insert country into sorted linked list */
Country* insertCountry(Country* head, char* name) {

    Country* newNode = (Country*)malloc(sizeof(Country));
    strcpy(newNode->name, name);
    newNode->cities = NULL;
    newNode->next = NULL;

    if (!head || strcmp(name, head->name) < 0) {
        newNode->next = head;
        return newNode;
    }

    Country* current = head;
    while (current->next && strcmp(current->next->name, name) < 0)
        current = current->next;

    newNode->next = current->next;
    current->next = newNode;

    return head;
}

/* Find country in list */
Country* findCountry(Country* head, char* name) {
    while (head) {
        if (strcmp(head->name, name) == 0)
            return head;
        head = head->next;
    }
    return NULL;
}

/* Print country list */
void printCountries(Country* head) {
    while (head) {
        printf("%s\n", head->name);
        printCityTree(head->cities);
        head = head->next;
    }
}



   /* Insert city into BST */
City* insertCity(City* root, char* name, int population) {

    if (!root) {
        City* node = (City*)malloc(sizeof(City));
        strcpy(node->name, name);
        node->population = population;
        node->left = node->right = NULL;
        return node;
    }

    if (population < root->population ||
        (population == root->population && strcmp(name, root->name) < 0))
        root->left = insertCity(root->left, name, population);
    else
        root->right = insertCity(root->right, name, population);

    return root;
}

/* Read cities from file */
void readCities(char* filename, City** root) {

    FILE* file = fopen(filename, "r");
    if (!file) {
        printf("Cannot open %s\n", filename);
        return;
    }

    char city[MAX_NAME];
    int population;

    while (fscanf(file, " %[^,],%d", city, &population) == 2)
        *root = insertCity(*root, city, population);

    fclose(file);
}

/* Print city BST */
void printCityTree(City* root) {
    if (!root) return;
    printCityTree(root->left);
    printf("    %s (%d)\n", root->name, root->population);
    printCityTree(root->right);
}
