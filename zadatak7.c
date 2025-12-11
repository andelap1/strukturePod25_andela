/*Napisati program koji pomoæu vezanih listi (stabala) predstavlja strukturu direktorija.
Omoguæiti unos novih direktorija i pod-direktorija, ispis sadržaja direktorija i
povratak u prethodni direktorij. Toènije program treba preko menija simulirati
korištenje DOS naredbi: 1- "md", 2 - "cd dir", 3 - "cd..", 4 - "dir" i 5 – izlaz.*/
#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#define MAX_NAME 50

typedef struct cvor* Directory;
struct cvor {
    char name[MAX_NAME];
    Directory parent;
    Directory firstChild;
    Directory nextSibling;
};

//za pracenje putanje
typedef struct stack* PathStack;
struct stack {
    Directory dir;
    PathStack next;
};

// globalne varijable
Directory root = NULL;
Directory currentDir = NULL;
PathStack global_path = NULL; 

// direktorij na stog putanje
void push(Directory dir) {
    PathStack newNode = (PathStack)malloc(sizeof(struct stack));

    if (newNode == NULL) {
        perror("Greska alokacije stoga");
        return;
    }

    newNode->dir = dir;
    newNode->next = global_path;
    global_path = newNode;
}

// direktorij sa stoga putanje 
Directory pop() {
    if (global_path == NULL) {
        return NULL;
    }

    PathStack temp = global_path; 
    Directory dir = temp->dir;

    global_path = global_path->next; 
    free(temp);

    return dir;
}

// korijenski direktorij
void initializeRoot() {
    root = (Directory)malloc(sizeof(struct cvor));
    if (root == NULL) {
        perror("Greska alokacije korijenskog direktorija");
        exit(EXIT_FAILURE);
    }
    strcpy(root->name, "C:\\");
    root->parent = NULL;
    root->firstChild = NULL;
    root->nextSibling = NULL;
    currentDir = root;
    push(root);
}

// trenutna putanja koristeæi stog
void printCurrentPath() {
    // Ova funkcija privremeno koristi stog da prikaze putanju u ispravnom redoslijedu
    PathStack tempStack = NULL;
    PathStack current = global_path; 
    PathStack reversedCurrent = NULL;

    // Kopiramo elemente u obrnuti redoslijed
    while (current != NULL) {
        PathStack newNode = (PathStack)malloc(sizeof(struct stack));
        if (newNode == NULL) {
            break; }
        newNode->dir = current->dir;
        newNode->next = tempStack;
        tempStack = newNode;
        current = current->next;
    }

    // Ispisujemo i oslobaðamo privremeni stog
    printf("\nPUTANJA: ");
    reversedCurrent = tempStack;
    while (reversedCurrent != NULL) {
        printf("%s", reversedCurrent->dir->name);
        if (reversedCurrent->dir != currentDir) {
            printf("\\");
        }
        PathStack toFree = reversedCurrent;
        reversedCurrent = reversedCurrent->next;
        free(toFree);
    }
    printf(">");
}

// Oslobaðanje memorije cijelog stabla rekurzivno
void freeDirectories(Directory dir) {
    if (dir == NULL) return;

    // Rekurzivno oslobodi poddirektorije (firstChild)
    freeDirectories(dir->firstChild);

    // Oslobodi sljedeæe bratske direktorije (nextSibling)
    freeDirectories(dir->nextSibling);

    free(dir);
}

// 1. md [ime] 
void makeDirectory(const char* name) {
    if (strlen(name) >= MAX_NAME) {
        printf("Greska: Ime direktorija je predugo.\n");
        return;
    }

    // Provjera duplikata
    Directory sibling = currentDir->firstChild;
    while (sibling != NULL) {
        if (strcmp(sibling->name, name) == 0) {
            printf("Direktorij '%s' vec postoji.\n", name);
            return;
        }
        sibling = sibling->nextSibling;
    }

    // Kreiranje novog èvora
    Directory newDir = (Directory)malloc(sizeof(struct cvor));
    if (newDir == NULL) {
        perror("Greska alokacije novog direktorija");
        return;
    }
    strcpy(newDir->name, name);
    newDir->parent = currentDir;
    newDir->firstChild = NULL;
    newDir->nextSibling = NULL;

    // Dodavanje u vezanu listu poddirektorija
    if (currentDir->firstChild == NULL) {
        currentDir->firstChild = newDir;
    }
    else {
        Directory lastSibling = currentDir->firstChild;
        while (lastSibling->nextSibling != NULL) {
            lastSibling = lastSibling->nextSibling;
        }
        lastSibling->nextSibling = newDir;
    }

    printf("Direktorij '%s' uspjesno kreiran.\n", name);
}

// 2. cd [ime]
void changeDirectory(const char* name) {
    Directory child = currentDir->firstChild;

    while (child != NULL) {
        if (strcmp(child->name, name) == 0) {
            currentDir = child;
            push(currentDir);
            printf("Promijenjen direktorij u '%s'.\n", name);
            return;
        }
        child = child->nextSibling;
    }
    printf("Greska: Direktorij '%s' nije pronaden.\n", name);
}

// 3. cd.. 
void changeDirectoryUp() {
    if (currentDir->parent != NULL) {
        pop();
        currentDir = currentDir->parent;
        printf("Povratak u direktorij '%s'.\n", currentDir->name);
    }
    else {
        printf("Vec ste u korijenskom direktoriju (C:\\).\n");
    }
}

// 4. dir 
void listDirectory() {
    printf("\nSadrzaj direktorija:\n");
    if (currentDir->parent != NULL) {
        printf("  <DIR> ..\n"); // Roditeljski direktorij
    }

    Directory child = currentDir->firstChild;
    if (child == NULL) {
        printf("  <Prazno>\n");
        return;
    }

    // Prolazim kroz vezanu listu poddirektorija
    while (child != NULL) {
        printf("  <DIR> %s\n", child->name);
        child = child->nextSibling;
    }
}

int main() {
    initializeRoot();
    int choice;
    char name[MAX_NAME];

    do {
        printCurrentPath();
        printf("\n\n--- MENU ---\n");
        printf("1. md [ime] (Kreiraj novi direktorij)\n");
        printf("2. cd [ime] (Promijeni u poddirektorij)\n");
        printf("3. cd.. (Povratak u roditeljski direktorij)\n");
        printf("4. dir (Ispis sadrzaja)\n");
        printf("5. Izlaz\n");
        printf("Odaberite opciju: ");

        if (scanf("%d", &choice) != 1) {
            // Ciscenje ulaznog buffera u slucaju neispravnog unosa
            while (getchar() != '\n');
            printf("Neispravan unos. Molimo unesite broj.\n");
            continue;
        }

        switch (choice) {
        case 1:
            printf("Unesite ime novog direktorija: ");
            scanf("%s", name);
            makeDirectory(name);
            break;
        case 2:
            printf("Unesite ime poddirektorija za ulazak: ");
            scanf("%s", name);
            changeDirectory(name);
            break;
        case 3:
            changeDirectoryUp();
            break;
        case 4:
            listDirectory();
            break;
        case 5:
            printf("Izlaz iz programa. Memorija oslobodena.\n");
            break;
        default:
            printf("Neispravna opcija. Pokusajte ponovo.\n");
        }
    } while (choice != 5);

    // Oslobaðanje cjelokupne memorije stabla prije izlaza
    freeDirectories(root);

    return 0;
}
