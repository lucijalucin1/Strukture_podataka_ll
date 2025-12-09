#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct Artikal* Artikal;

struct Artikal {
    char naziv[100];
    int quantity;
    float price;
    Artikal next;
};

typedef struct Invoice* Invoice;

struct Invoice {
    char date[11];   // YYYY-MM-DD
    Artikal items;   // linked list of items
    Invoice next;    // next invoice
};


int compareDates(const char* d1, const char* d2) {
    return strcmp(d1, d2);
}


void insertItemSorted(Artikal* head, Artikal newItem) {
    if (*head == NULL || strcmp(newItem->naziv, (*head)->naziv) < 0) {
        newItem->next = *head;
        *head = newItem;
        return;
    }

    Artikal tmp = *head;
    while (tmp->next && strcmp(newItem->naziv, tmp->next->naziv) > 0)
        tmp = tmp->next;

    newItem->next = tmp->next;
    tmp->next = newItem;
}


void insertInvoiceSorted(Invoice* head, Invoice newInv) {
    if (*head == NULL || compareDates(newInv->date, (*head)->date) < 0) {
        newInv->next = *head;
        *head = newInv;
        return;
    }

    Invoice tmp = *head;
    while (tmp->next && compareDates(newInv->date, tmp->next->date) > 0)
        tmp = tmp->next;

    newInv->next = tmp->next;
    tmp->next = newInv;
}


Invoice loadSingleInvoice(const char* filename) {
    FILE* f = fopen(filename, "r");
    if (!f) {
        printf("Error: cannot open %s\n", filename);
        return NULL;
    }

    Invoice inv = (Invoice)malloc(sizeof(struct Invoice));
    inv->items = NULL;
    inv->next = NULL;

    fgets(inv->date, 11, f);
    fgetc(f); // consume newline

    char name[100];
    int q;
    float p;

    while (fscanf(f, " %[^,], %d, %f", name, &q, &p) == 3) {
        Artikal item = (Artikal)malloc(sizeof(struct Artikal));
        strcpy(item->naziv, name);
        item->quantity = q;
        item->price = p;
        item->next = NULL;

        insertItemSorted(&(inv->items), item);
    }

    fclose(f);
    return inv;
}


void loadAllInvoices(Invoice* head) {
    FILE* f = fopen("Racuni.txt", "r");
    if (!f) {
        printf("Error: cannot open racuni.txt\n");
        return;
    }

    char filename[100];
    while (fscanf(f, "%s", filename) == 1) {
        Invoice inv = loadSingleInvoice(filename);
        if (inv)
            insertInvoiceSorted(head, inv);
    }

    fclose(f);
}

void query(Invoice head) {
    char itemName[100];
    char from[11], to[11];

    printf("Enter item name: ");
    scanf(" %s", itemName);

    printf("Enter start date (YYYY-MM-DD): ");
    scanf(" %s", from);

    printf("Enter end date (YYYY-MM-DD): ");
    scanf(" %s", to);

    int totalQty = 0;
    float totalCost = 0.0f;

    Invoice inv = head;
    while (inv) {
        if (compareDates(inv->date, from) >= 0 &&
            compareDates(inv->date, to) <= 0)
        {
            Artikal a = inv->items;
            while (a) {
                if (strcmp(a->naziv, itemName) == 0) {
                    totalQty += a->quantity;
                    totalCost += a->quantity * a->price;
                }
                a = a->next;
            }
        }
        inv = inv->next;
    }

    printf("\nTotal quantity of '%s': %d\n", itemName, totalQty);
    printf("Total money spent: %.2f\n", totalCost);
}


void freeItems(Artikal head) {
    while (head) {
        Artikal tmp = head;
        head = head->next;
        free(tmp);
    }
}

void freeInvoices(Invoice head) {
    while (head) {
        Invoice tmp = head;
        head = head->next;

        freeItems(tmp->items);  // free linked list of items
        free(tmp);              // free invoice node
    }
}


int main() {
    Invoice invoices = NULL;

    loadAllInvoices(&invoices);
    query(invoices);

    freeInvoices(invoices);  // free everything before exit

    return 0;

}
