#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>

#define MAX 100

typedef struct polynom* poly;

struct polynom {
	int coef;
	int exp;
	poly term[MAX];
	int n;
};

void insertSorted(poly P, int coef, int exp);

void readPolynomialFromLine(poly P, char* line);

int readPolynomials(const char* filename, poly polys[], int maxPolys);

void printPolynomial(poly P);

void addPolynomials(poly A, poly B, poly R);

void multiplyPolynomials(poly A, poly B, poly R);

int main() {
    poly polys[MAX];
    int nPolys = readPolynomials("Polynom.txt", polys, MAX);

    printf("Loaded %d polynomials:\n", nPolys);

    for (int i = 0; i < nPolys; i++) {
        printf("P%d(x) = ", i + 1);
        printPolynomial(polys[i]);
    }

    //add and multiply the FIRST TWO polynomials
    if (nPolys >= 2) {
        struct polynom Sum, Product;

        addPolynomials(polys[0], polys[1], &Sum);
        multiplyPolynomials(polys[0], polys[1], &Product);

        printf("\nSum: ");
        printPolynomial(&Sum);

        printf("Product: ");
        printPolynomial(&Product);
    }

    return 0;
}

// Insert one term in sorted order (descending exponent)
void insertSorted(poly P, int coef, int exp) {
    poly t = (poly)malloc(sizeof(struct polynom));
    t->coef = coef;
    t->exp = exp;
    t->n = 0;

    int i = 0;

    // Find correct position
    while (i < P->n && P->term[i]->exp > exp)
        i++;

    // Merge if exponent already exists
    if (i < P->n&& P->term[i]->exp == exp) {
        P->term[i]->coef += coef;
        free(t);
        return;
    }

    // Shift elements to make room
    for (int j = P->n; j > i; j--)
        P->term[j] = P->term[j - 1];

    P->term[i] = t;
    P->n++;
}

// Read polynomial from one line of text
void readPolynomialFromLine(poly P, char* line) {
    P->n = 0;
    int coef, exp;
    char* ptr = line;

    // Read coef-exp pairs
    while (sscanf(ptr, "%d %d", &coef, &exp) == 2) {
        insertSorted(P, coef, exp);

        // Skip parsed numbers
        while (*ptr && *ptr != ' ') ptr++;
        while (*ptr == ' ') ptr++;
        while (*ptr && *ptr != ' ') ptr++;
        while (*ptr == ' ') ptr++;
    }
}

// Read ALL polynomials from a file
int readPolynomials(const char* filename, poly polys[], int maxPolys) {
    FILE* fp = fopen(filename, "r");
    if (!fp) {
        printf("Error opening file!\n");
        exit(1);
    }

    char line[256];
    int count = 0;

    // Read until EOF
    while (fgets(line, sizeof(line), fp) && count < maxPolys) {
        polys[count] = (poly)malloc(sizeof(struct polynom));
        readPolynomialFromLine(polys[count], line);
        count++;
    }

    fclose(fp);
    return count; // number of polynomials read
}

// Print polynomial
void printPolynomial(poly P) {
    for (int i = 0; i < P->n; i++) {
        int c = P->term[i]->coef;
        int e = P->term[i]->exp;

        if (i > 0 && c > 0)
            printf(" + ");

        if (e == 0)
            printf("%d", c);
        else if (e == 1)
            printf("%dx", c);
        else
            printf("%dx^%d", c, e);
    }
    printf("\n");
}

// Add two polynomials
void addPolynomials(poly A, poly B, poly R) {
    R->n = 0;

    for (int i = 0; i < A->n; i++)
        insertSorted(R, A->term[i]->coef, A->term[i]->exp);

    for (int i = 0; i < B->n; i++)
        insertSorted(R, B->term[i]->coef, B->term[i]->exp);
}

// Multiply two polynomials
void multiplyPolynomials(poly A, poly B, poly R) {
    R->n = 0;

    for (int i = 0; i < A->n; i++) {
        for (int j = 0; j < B->n; j++) {
            int c = A->term[i]->coef * B->term[j]->coef;
            int e = A->term[i]->exp + B->term[j]->exp;
            insertSorted(R, c, e);
        }
    }
}

