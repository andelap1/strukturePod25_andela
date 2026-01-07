/*10. Napisati program koji čita datoteku drzave.txt u kojoj su zapisani nazivi pojedinih država. Uz
ime države u datoteci se nalazi i ime dodatne datoteke u kojoj se nalaze gradovi pojedine
države. Svaka datoteka koja predstavlja državu sadrži popis gradova u formatu naziv_grada,
broj_stanovnika.

b) Potrebno je formirati stablo država sortirano po nazivu. Svaki čvor stabla sadrži vezanu
listu gradova sortiranu po broju stanovnika, zatim po nazivu grada.
Nakon formiranja podataka potrebno je ispisati države i gradove te omogućiti korisniku putem
tastature pretragu gradova određene države koji imaju broj stanovnika veći od unosa na
tastaturi.*/

#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_NAME 50
#define FILE_NOT_OPENED -1

typedef struct cityNode* cityPosition;
typedef struct countryNode* countryPosition;

struct cityNode {
	char name[MAX_NAME];
	int population;
	cityPosition next;
};

struct countryNode {
	char name[MAX_NAME];
	cityPosition cityHead;
	countryPosition left;
	countryPosition right;
};

cityPosition insertCityInList(cityPosition head, char* name, int population) {
	cityPosition newCity = (cityPosition)malloc(sizeof(struct cityNode));
	if (!newCity) 
		return head;

	strcpy(newCity->name, name);
	newCity->population = population;
	newCity->next = NULL;

	// Sortiranje po broju stanovnika, pa po imenu
	if (head == NULL || population > head->population || (population == head->population && strcmp(name, head->name) < 0)) {
		newCity->next = head;
		return newCity;
	}

	cityPosition current = head;
	while (current->next != NULL && (current->next->population > population || (current->next->population == population && strcmp(name, current->next->name) > 0))) {
		current = current->next;
	}

	newCity->next = current->next;
	current->next = newCity;

	return head;
}

cityPosition readCitiesFromFile(char* fileName) {
	FILE* fp = fopen(fileName, "r");
	if (!fp) return NULL;

	cityPosition head = NULL;
	char cityName[MAX_NAME];
	int pop;

	while (fscanf(fp, " %[^,], %d", cityName, &pop) == 2) {
		head = insertCityInList(head, cityName, pop);
	}

	fclose(fp);
	return head;
}

countryPosition insertCountryInTree(countryPosition root, char* name, char* fileName) {
	if (root == NULL) {
		countryPosition newNode = (countryPosition)malloc(sizeof(struct countryNode));
		if (!newNode) return NULL;

		strcpy(newNode->name, name);
		newNode->left = newNode->right = NULL;
		newNode->cityHead = readCitiesFromFile(fileName);
		return newNode;
	}

	if (strcmp(name, root->name) < 0)
		root->left = insertCountryInTree(root->left, name, fileName);
	else if (strcmp(name, root->name) > 0)
		root->right = insertCountryInTree(root->right, name, fileName);

	return root;
}

int printCitiesFromList(cityPosition head, int limit) {
	cityPosition current = head;
	while (current != NULL) {
		if (current->population > limit) {
			printf("\t%s: %d\n", current->name, current->population);
		}
		current = current->next;
	}

	return 0;
}

int printTree(countryPosition root) {
	if (root != NULL) {
		printTree(root->left);
		printf("\nDrzava: %s\n", root->name);
		printCitiesFromList(root->cityHead, 0);
		printTree(root->right);
	}

	return 0;
}

int findAndPrint(countryPosition root, char* target, int limit) {
	if (root == NULL) {
		printf("Drzava '%s' nije pronadjena u stablu.\n", target);
		return;
	}

	int res = strcmp(target, root->name);
	if (res == 0) {
		printf("\nGradovi u %s s vise od %d stanovnika:\n", target, limit);
		printCitiesFromList(root->cityHead, limit);
	}
	else if (res < 0)
		findAndPrint(root->left, target, limit);
	else
		findAndPrint(root->right, target, limit);

	return 0;
}

int freeAll(countryPosition root) {
	if (root != NULL) {
		freeAll(root->left);
		freeAll(root->right);

		cityPosition currentCity = root->cityHead;
		while (currentCity != NULL) {
			cityPosition temp = currentCity;
			currentCity = currentCity->next;
			free(temp);
		}
		free(root);
	}

	return 0;
}

int main() {
	countryPosition root = NULL;
	FILE* fp = fopen("drzave.txt", "r");
	if (!fp) {
		printf("Greska pri otvaranju drzave.txt\n");
		return FILE_NOT_OPENED;
	}

	char countryName[MAX_NAME], fileName[MAX_NAME];
	while (fscanf(fp, "%s %s", countryName, fileName) == 2) {
		root = insertCountryInTree(root, countryName, fileName);
	}
	fclose(fp);

	printf("--- ISPIS DRZAVA I GRADOVA ---\n");
	printTree(root);

	char searchName[MAX_NAME];
	int limit = 0;
	printf("\nUnesite ime drzave za pretragu: ");
	scanf("%s", searchName);
	printf("Minimalan broj stanovnika: ");
	scanf("%d", &limit); 

	findAndPrint(root, searchName, limit);

	freeAll(root);
	return 0;
}

