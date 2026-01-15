/*11. Prepraviti zadatak 10 na naèin da se formira hash tablica država. Tablica ima 11 mjesta, a
funkcija za preslikavanje kljuè raèuna da se zbraja ASCII vrijednost prvih pet slova države zatim
raèuna ostatak cjelobrojnog dijeljenja te vrijednosti s velièinom tablice. Države s istim kljuèem se
pohranjuju u vezanu listu sortiranu po nazivu države. Svaki èvor vezane liste sadrži stablo
gradova sortirano po broju stanovnika, zatim po nazivu grada.*/

#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_NAME 50
#define TABLE_SIZE 11

typedef struct countryNode* countryPosition;
typedef struct cityNode* cityPosition;

struct countryNode {
    char name[MAX_NAME];
    countryPosition next;
    cityPosition root; 
};

struct cityNode {
    char name[MAX_NAME];
    cityPosition left;
    cityPosition right;
    int population;
};


cityPosition createNewCity(const char* name, int population);
cityPosition insertNewCity(const char* name, int population, cityPosition root);
cityPosition readCitiesFromFile(const char* name);
int hash1(const char* Country, int tabSize);
countryPosition insertCountryInTable(countryPosition table[], const char* name, const char* fileName);
int printCountriesAndCities(countryPosition table[]);
int printCities(cityPosition root);
int printFilteredCities(cityPosition root, int limit);
int FoundCountryAndItsCities(countryPosition table[], const char* countryName, int limit);
int freeTree(cityPosition root);
int freeAll(countryPosition table[]);


cityPosition createNewCity(const char* name, int population) {
    cityPosition newCity = (cityPosition)malloc(sizeof(struct cityNode));
    if (newCity == NULL)
        return NULL;

    strncpy(newCity->name, name, MAX_NAME - 1);
    newCity->name[MAX_NAME - 1] = '\0';
    newCity->population = population;
    newCity->left = NULL;
    newCity->right = NULL;

    return newCity;
}

cityPosition insertNewCity(const char* name, int population, cityPosition root) {
    if (root == NULL)
        return createNewCity(name, population);

    if (population < root->population) {
        root->left = insertNewCity(name, population, root->left);
    }
    else if (population > root->population) {
        root->right = insertNewCity(name, population, root->right);
    }
    else { /* population equal -> tie-break by name */
        if (strcmp(name, root->name) < 0)
            root->left = insertNewCity(name, population, root->left);
        else
            root->right = insertNewCity(name, population, root->right);
    }

    return root;
}

cityPosition readCitiesFromFile(const char* name) {
    FILE* fp = fopen(name, "r");
    char cityName[MAX_NAME];
    int pop;
    cityPosition root = NULL;

    if (fp == NULL)
        return NULL;
    
    while (fscanf(fp, " %[^,], %d", cityName, &pop) == 2) {
        root = insertNewCity(cityName, pop, root);
    }

    fclose(fp);
    return root;
}


int hash1(const char* Country, int tabSize) {
    int sum = 0;
    int count = 0;
    for (int i = 0; Country[i] != '\0' && count < 5; ++i, ++count) {
        sum += (unsigned char)Country[i];
    }
    return sum % tabSize;
}


countryPosition insertCountryInTable(countryPosition table[], const char* name, const char* fileName) {
    int index = hash1(name, TABLE_SIZE); //odredivanje indeksa u koji drzava pripada pomocu hash funkcije

    countryPosition newCountry = (countryPosition)malloc(sizeof(struct countryNode)); //memorija za novi cvor u hash tablici
    if (!newCountry) return NULL;

    strncpy(newCountry->name, name, MAX_NAME - 1); 
    newCountry->name[MAX_NAME - 1] = '\0';
    newCountry->root = readCitiesFromFile(fileName); //gradovi idu u stablo
    newCountry->next = NULL;

    countryPosition head = table[index]; //dohvacanje glavne vezane liste na izracunatom indeksu

    //provjer treba li novi element umetnuti na pocetak liste
    if (head == NULL || strcmp(name, head->name) < 0) { 
        newCountry->next = head;
        table[index] = newCountry; //azuriranje heada u tablici
        return newCountry;
    }

    //trazenje mjesta unutar vezane liste na tom indeksu kako bi drzave bile sortirane
    countryPosition current = head;
    while (current->next != NULL && strcmp(current->next->name, name) < 0) {
        current = current->next;
    }

    //umetanje elemenata u listu
    newCountry->next = current->next;
    current->next = newCountry;

    return newCountry;
}

int printCities(cityPosition root) {
    if (root != NULL) {
        printCities(root->left);
        printf("%s: %d stanovnika\n", root->name, root->population);
        printCities(root->right);
    }
    return 0;
}

int printCountriesAndCities(countryPosition table[]) {
    int empty = 1;
    for (int i = 0; i < TABLE_SIZE; ++i) { //prolazak kroz hash tablicu
        countryPosition current = table[i]; //postavljanje na pocetak liste u i-tom bucketu
        if (current == NULL) continue; //preskoci ako je bucket prazan
        empty = 0;
        printf("\n-- Bucket %d --\n", i); //ispis rednog broja pretinca u tablici
        while (current != NULL) {
            printf("Drzava: %s\n", current->name);
            printf("Gradovi:\n");
            if (current->root == NULL) {
                printf("Nema upisanih gradova.\n");
            }
            else {
                printCities(current->root); //ispis stabla gradova za tu drzavu
            }
            current = current->next; //idi na iducu drzavu u tom bucketu
            printf("\n");
        }
    }
    if (empty) {
        printf("Tablica je prazna.\n");
    }
    return 0;
}

int printFilteredCities(cityPosition root, int limit) {
    if (root != NULL) {
        printFilteredCities(root->left, limit);
        if (root->population > limit) {
            printf("%s: %d stanovnika\n", root->name, root->population);
        }
        printFilteredCities(root->right, limit);
    }

    return 0;
}

int FoundCountryAndItsCities(countryPosition table[], const char* countryName, int limit) {
    //hash funkcija kako bi odmah znali u kojem bucketu trazimo 
    int index = hash1(countryName, TABLE_SIZE);
    countryPosition current = table[index];

    while (current != NULL && strcmp(current->name, countryName) != 0) {
        current = current->next;
    }

    //ako na kraju liste nismo nasli ime drzave
    if (current == NULL) {
        printf("Drzava nije pronadjena u tablici.\n");
        return 0;
    }
    else {
        printf("\n%s --> gradovi s vise od %d stanovnika:\n", countryName, limit);
        printFilteredCities(current->root, limit);
    }

    return 0;
}

int freeTree(cityPosition root) {
    if (root != NULL) {
        freeTree(root->left);
        freeTree(root->right);
        free(root);
    }
    return 0;
}

int freeAll(countryPosition table[]) {
    for (int i = 0; i < TABLE_SIZE; ++i) { //prolazak kroz svaki indeks tablice
        countryPosition head = table[i];
        countryPosition temp;
        while (head != NULL) { //brisanje vezane liste drzava na tom indeksu
            temp = head;
            head = head->next;
            freeTree(temp->root);
            free(temp); //brisanje drzave iz tablice
        }
        table[i] = NULL;
    }
    return 0;
}

int main() {
    countryPosition table[TABLE_SIZE] = { 0 }; // Inicijalizacija hash tablice, niz pokazivaca postavlja se na NULL
    char countryName[MAX_NAME];
    char fileName[MAX_NAME];

    FILE* fp = fopen("drzave.txt", "r");
    if (!fp) {
        printf("Pogreska: Ne mogu otvoriti drzave.txt\n");
        return 1;
    }

    while (fscanf(fp, "%49s %49s", countryName, fileName) == 2) {
        if (insertCountryInTable(table, countryName, fileName) == NULL) {
            fprintf(stderr, "Greska: alokacija neuspjela za drzavu %s\n", countryName);
        }
    }

    fclose(fp);

    printf("Sve drzave i gradovi su uspjesno ucitani!\n");
    printf("\nIspis drzava i gradova:\n");
    printCountriesAndCities(table);

    char findName[MAX_NAME];
    int limit = 0;

    printf("\nUnesite ime drzave koju trazite: \n");
    if (scanf("%49s", findName) != 1) {
        printf("Neispravan unos.\n");
        freeAll(table);
        return 1;
    }
    printf("Unesite minimalan broj stanovnika koji vas zanima kod njenih gradova: \n");
    if (scanf("%d", &limit) != 1) {
        printf("Neispravan unos.\n");
        freeAll(table);
        return 1;
    }

    FoundCountryAndItsCities(table, findName, limit);

    freeAll(table);

    return 0;


}
