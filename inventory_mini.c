 
#include <stdio.h>
#include <string.h>
#include <strings.h> 
#include <stdlib.h>

#define MAX_PRODUCTS 100
#define LOW_STOCK_THRESHOLD 5
#define FILENAME "inventory.txt"

typedef struct {
    int id;
    char name[50];
    int qty;
    float price;
    char category[30];
} Product;

Product inventory[MAX_PRODUCTS];
int productCount = 0;

void loadFromFile();
void saveToFile();
void addProduct();
void viewProducts();
void searchProduct();
void updateStock();
void deleteProduct();
void showLowStock();
int findProductIndexById(int id);
void clearInputBuffer();
void pause_console();
void readValidatedField(char *buf, size_t size, const char *fieldName);

int main() {
    int choice = 0;   /* was uninitialized: if the first scanf failed, the
                          while(choice != 7) check below read garbage stack
                          memory (undefined behavior, confirmed with valgrind) */
    loadFromFile();
    do {
        printf("\n===== INVENTORY MANAGEMENT SYSTEM =====\n");
        printf("1. Add Product\n2. View All Products\n3. Search Product\n4. Update Stock\n5. Delete Product\n6. Show Low Stock Items\n7. Save & Exit\nEnter your choice: ");
        if (scanf("%d", &choice) != 1) {
            clearInputBuffer();
            printf("Invalid input. Please enter a number.\n");
            continue;
        }
        clearInputBuffer();
        switch (choice) {
            case 1: addProduct(); break;
            case 2: viewProducts(); break;
            case 3: searchProduct(); break;
            case 4: updateStock(); break;
            case 5: deleteProduct(); break;
            case 6: showLowStock(); break;
            case 7:
                saveToFile();
                printf("Data saved. Exiting program. Goodbye!\n");
                break;
            default:
                printf("Invalid choice. Try again.\n");
        }
    } while (choice != 7);
    return 0;
}

void clearInputBuffer() {
    int c;
    while ((c = getchar()) != '\n' && c != EOF);
}

void pause_console() {
    printf("Press Enter to continue...");
    getchar();
}

void readValidatedField(char *buf, size_t size, const char *fieldName) {
    for (;;) {
        fgets(buf, (int)size, stdin);
        buf[strcspn(buf, "\n")] = '\0';

        if (buf[0] == '\0') {
            printf("%s cannot be empty. Please re-enter %s: ", fieldName, fieldName);
            continue;
        }
        if (strchr(buf, '|') != NULL) {
            printf("%s cannot contain the '|' character. Please re-enter %s: ", fieldName, fieldName);
            continue;
        }
        return;
    }
}

int findProductIndexById(int id) {
    for (int i = 0; i < productCount; i++) {
        if (inventory[i].id == id) return i;
    }
    return -1;
}

void addProduct() {
    if (productCount >= MAX_PRODUCTS) {
        printf("Inventory full! Cannot add more products (max %d).\n", MAX_PRODUCTS);
        return;
    }
    Product p;
    printf("\n-- Add New Product --\n");
    printf("Enter Product ID: ");
    if (scanf("%d", &p.id) != 1 || p.id < 0) {
        clearInputBuffer();
        printf("Invalid ID. Must be a non-negative number.\n");
        return;
    }
    clearInputBuffer();
    if (findProductIndexById(p.id) != -1) {
        printf("Error: Product ID %d already exists.\n", p.id);
        return;
    }
    printf("Enter Product Name: ");
    readValidatedField(p.name, sizeof(p.name), "Product Name");
    printf("Enter Category: ");
    readValidatedField(p.category, sizeof(p.category), "Category");
    printf("Enter Quantity: ");
    if (scanf("%d", &p.qty) != 1 || p.qty < 0) {
        clearInputBuffer();
        printf("Invalid quantity. Must be a non-negative number.\n");
        return;
    }
    clearInputBuffer();
    printf("Enter Price: ");
    if (scanf("%f", &p.price) != 1 || p.price < 0) {
        clearInputBuffer();
        printf("Invalid price. Must be a non-negative number.\n");
        return;
    }
    clearInputBuffer();
    inventory[productCount++] = p;
    printf("Product added successfully.\n");
    saveToFile();
}

void viewProducts() {
    if (productCount == 0) {
        printf("\nNo products in inventory.\n");
        return;
    }
    printf("\n%-6s %-20s %-10s %-10s %-15s\n", "ID", "Name", "Qty", "Price", "Category");
    printf("---------------------------------------------------------------\n");
    for (int i = 0; i < productCount; i++) {
        printf("%-6d %-20s %-10d %-10.2f %-15s\n",
               inventory[i].id, inventory[i].name,
               inventory[i].qty, inventory[i].price, inventory[i].category);
    }
}

void searchProduct() {
    int choice;
    printf("\nSearch by:\n1. ID\n2. Name\nEnter choice: ");
    if (scanf("%d", &choice) != 1) { clearInputBuffer(); printf("Invalid choice.\n"); return; }
    clearInputBuffer();
    int found = 0;
    if (choice == 1) {
        int id;
        printf("Enter Product ID: ");
        if (scanf("%d", &id) != 1) { clearInputBuffer(); printf("Invalid ID.\n"); return; }
        clearInputBuffer();
        int idx = findProductIndexById(id);
        if (idx != -1) {
            found = 1;
            printf("\nFound:\nID: %d\nName: %s\nQty: %d\nPrice: %.2f\nCategory: %s\n",
                   inventory[idx].id, inventory[idx].name, inventory[idx].qty,
                   inventory[idx].price, inventory[idx].category);
        }
    } else if (choice == 2) {
        char name[50];
        printf("Enter Product Name: ");
        fgets(name, sizeof(name), stdin);
        name[strcspn(name, "\n")] = '\0';
        if (name[0] == '\0') { printf("Product not found.\n"); return; }
        for (int i = 0; i < productCount; i++) {
            if (strcasecmp(inventory[i].name, name) == 0) {
                found = 1;
                printf("\nFound:\nID: %d\nName: %s\nQty: %d\nPrice: %.2f\nCategory: %s\n",
                       inventory[i].id, inventory[i].name, inventory[i].qty,
                       inventory[i].price, inventory[i].category);
            }
        }
    } else {
        printf("Invalid choice.\n");
        return;
    }
    if (!found) printf("Product not found.\n");
}

void updateStock() {
    int id, choice, amount;
    printf("Enter Product ID to update: ");
    if (scanf("%d", &id) != 1) { clearInputBuffer(); printf("Invalid ID.\n"); return; }
    clearInputBuffer();
    int idx = findProductIndexById(id);
    if (idx == -1) {
        printf("Product not found.\n");
        return;
    }
    printf("Current Quantity: %d\n", inventory[idx].qty);
    printf("1. Increase Stock (Purchase)\n2. Decrease Stock (Sale)\nChoice: ");
    if (scanf("%d", &choice) != 1) { clearInputBuffer(); printf("Invalid choice.\n"); return; }
    clearInputBuffer();
    printf("Enter amount: ");
    if (scanf("%d", &amount) != 1 || amount < 0) {
        clearInputBuffer();
        printf("Invalid amount.\n");
        return;
    }
    clearInputBuffer();
    if (choice == 1) {
        inventory[idx].qty += amount;
        printf("Stock increased. New quantity: %d\n", inventory[idx].qty);
    } else if (choice == 2) {
        if (amount > inventory[idx].qty) {
            printf("Error: Not enough stock to sell (available: %d).\n", inventory[idx].qty);
            return;
        }
        inventory[idx].qty -= amount;
        printf("Stock decreased. New quantity: %d\n", inventory[idx].qty);
    } else {
        printf("Invalid choice.\n");
        return;
    }
    saveToFile();
}

void deleteProduct() {
    int id;
    printf("Enter Product ID to delete: ");
    if (scanf("%d", &id) != 1) { clearInputBuffer(); printf("Invalid ID.\n"); return; }
    clearInputBuffer();
    int idx = findProductIndexById(id);
    if (idx == -1) {
        printf("Product not found.\n");
        return;
    }
    for (int i = idx; i < productCount - 1; i++) {
        inventory[i] = inventory[i + 1];
    }
    productCount--;
    printf("Product deleted successfully.\n");
    saveToFile();
}

void showLowStock() {
    int found = 0;
    printf("\n-- Low Stock Items (qty < %d) --\n", LOW_STOCK_THRESHOLD);
    for (int i = 0; i < productCount; i++) {
        if (inventory[i].qty < LOW_STOCK_THRESHOLD) {
            found = 1;
            printf("ID: %d | Name: %s | Qty: %d\n",
                   inventory[i].id, inventory[i].name, inventory[i].qty);
        }
    }
    if (!found) printf("No low-stock items.\n");
}

void saveToFile() {
    FILE *fp = fopen(FILENAME, "w");
    if (fp == NULL) {
        printf("Error: Could not open file for saving.\n");
        return;
    }
    for (int i = 0; i < productCount; i++) {
        fprintf(fp, "%d|%s|%d|%.2f|%s\n",
                inventory[i].id, inventory[i].name,
                inventory[i].qty, inventory[i].price, inventory[i].category);
    }
    fclose(fp);
}

void loadFromFile() {
    FILE *fp = fopen(FILENAME, "r");
    if (fp == NULL) {
        return;
    }
    productCount = 0;
    while (productCount < MAX_PRODUCTS &&
           fscanf(fp, "%d|%49[^|]|%d|%f|%29[^\n]\n",
                  &inventory[productCount].id,
                  inventory[productCount].name,
                  &inventory[productCount].qty,
                  &inventory[productCount].price,
                  inventory[productCount].category) == 5) {
        productCount++;
    }
    fclose(fp);
    printf("Loaded %d product(s) from %s.\n", productCount, FILENAME);
}
