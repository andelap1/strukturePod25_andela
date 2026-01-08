/*10. Napisati program koji èita datoteku drzave.txt u kojoj su zapisani nazivi pojedinih država. Uz
ime države u datoteci se nalazi i ime dodatne datoteke u kojoj se nalaze gradovi pojedine
države. Svaka datoteka koja predstavlja državu sadrži popis gradova u formatu naziv_grada,
broj_stanovnika.

a) Potrebno je formirati sortiranu vezanu listu država po nazivu. Svaki èvor vezane liste
sadrži stablo gradova sortirano po broju stanovnika, zatim po nazivu grada.
*/

#define _CRT_SECURE_NO_WARNINGS 
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_NAME 50
#define FILE_NOT_OPENED -1
 
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


cityPosition createNewCity(char* name, int population) {
    cityPosition newCity = (cityPosition)malloc(sizeof(struct cityNode));
    if (newCity == NULL)
        return NULL; 

    strcpy(newCity->name, name);
    newCity->population = population;
    newCity->left = NULL;
    newCity->right = NULL;

    return newCity;
}


cityPosition insertNewCity(char* name, int population, cityPosition root) {
    if (root == NULL)
        return createNewCity(name, population); 

    if (population < root->population) {
        root->left = insertNewCity(name, population, root->left); 
    }
    else if (population > root->population) {
        root->right = insertNewCity(name, population, root->right); 
    }
  

    return root;
}


cityPosition readCitiesFromFile(char* name) {
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


countryPosition insertCountry(countryPosition head, char* name, char* fileName) {
    countryPosition newCountry = (countryPosition)malloc(sizeof(struct countryNode));
    if (!newCountry) return head;

    strcpy(newCountry->name, name);
    newCountry->root = readCitiesFromFile(fileName);

    if (head == NULL || strcmp(name, head->name) < 0) {
        newCountry->next = head;
        return newCountry;
    }

    countryPosition current = head;
    while (current->next != NULL && strcmp(name, current->next->name) > 0) {
        current = current->next;
    }

    newCountry->next = current->next;
    current->next = newCountry;

    return head;
}

int printCities(cityPosition root) {
    if (root != NULL) {
        printCities(root->left); 
        printf("%s: %d stanovnika\n", root->name, root->population);
        printCities(root->right); 
    }
    return 0;
}

int printCountriesAndCities(countryPosition head) {
    countryPosition current = head;   
    if (current == NULL) {
        printf("Lista je prazna.");
        return 0;
    }

    while (current != NULL) {
        printf("\nDrzava: %s\n", current->name);
        printf("Gradovi:\n");

        if (current->root == NULL) {
            printf("Nema upisanih gradova.\n");
        }
        else {
            printCities(current->root); 
        } 
        current = current->next; 
    }

    return 0;
}

/*Ove dvije iduce funkcije omogucuju korisniku da pretrazuje*/

int printFilteredCities(cityPosition root, int limit) {
    if (root != NULL) {
        printFilteredCities(root->left, limit);
        if (root->population > limit) {
            printf("\n%s: %d stanovnika\n", root->name, root->population);
        }
        printFilteredCities(root->right, limit);
    }

    return 0;
}

int FoundCountryAndItsCities(countryPosition head, char* countryName, int limit) {
    countryPosition current = head;

    while (current != NULL && strcmp(current->name, countryName) != 0) {
        current = current->next;
    }

    if (current == NULL) {
        printf("Drzava nije pronadjena u listi.\n");
        return NULL;
    }
    else {
        printf("\n%s --> gradovi s vise od %d stanovnika: ", countryName, limit);
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

int freeAll(countryPosition head) {
    countryPosition temp;

    while (head != NULL) {
        temp = head;
        head = head->next;

        freeTree(temp->root); 
        free(temp);           
    }
 return 0;
}

int main() {
    countryPosition head = NULL;
    char countryName[MAX_NAME];
    char fileName[MAX_NAME];

    FILE* fp = fopen("drzave.txt", "r");
    if (!fp) {
        printf("Pogreska: Ne mogu otvoriti drzave.txt\n");
        return 1;
    }

    while (fscanf(fp, "%s %s", countryName, fileName) == 2) {
        head = insertCountry(head, countryName, fileName);
    }

    fclose(fp);

    printf("Sve drzave i gradovi su uspjesno ucitani!\n");
    printf("\nIspis drzava i gradova:\n");
    printCountriesAndCities(head);

    char findName[MAX_NAME];
    int limit = 0;

    printf("\nUnesite ime drzave koju trazite: \n");
    scanf("%s", findName);
    printf("Unesite minimalan broj stanovnika koji vas zanima kod njenih gradova: \n");
    scanf("%d", &limit);
    FoundCountryAndItsCities(head, findName, limit);

    freeAll(head);
    
    return 0;

}
