/*3. Prethodnom zadatku dodati funkcije:
A. dinamički dodaje novi element iza određenog elementa,
B. dinamički dodaje novi element ispred određenog elementa,
C. sortira listu po prezimenima osoba,
D. upisuje listu u datoteku,
E. čita listu iz datoteke.*/

#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <string.h> 
#define FILE_NOT_OPENED (-1)
#define MAX_NAME 50
#define MAX_SURNAME 50


typedef struct Person {
	char name[MAX_NAME];
	char surname[MAX_SURNAME];
	int birthYear;
	struct Person* next; 
} Person;

typedef struct Person* Position;

int addAtBeginning(Position*);
int printList(Position);
int addAtEnd(Position*);
int findBySurname(Position, char*);
int deleteBySurname(Position*, char*);
int addAfterElement(Position, char*);
int addBefore(Position*, char*);
int sortBySurname(Position);
int writeToFile(Position, char*);
int readFromFile(Position*, char*);

int main() {
	Position head = NULL; // početak liste
	char surname[MAX_SURNAME];
	int choice;

	do {
		printf("\n----- IZBORNIK -----\n");
		printf("1. Dodaj na pocetak\n");
		printf("2. Dodaj na kraj\n");
		printf("3. Ispis liste\n");
		printf("4. Pronadi po prezimenu\n");
		printf("5. Obrisi po prezimenu\n");
		printf("6. Dodaj iza osobe\n");
		printf("7. Dodaj ispred osobe\n");
		printf("8. Sortiraj po prezimenima\n");
		printf("9. Spremi u datoteku\n");
		printf("10. Učitaj iz datoteke\n");
		printf("0. Izlaz\n");
		printf("Odabir: ");

		if (scanf("%d", &choice) != 1) { 
			printf("Neispravan unos. Pokusajte ponovo.\n");
			while (getchar() != '\n'); // čišćenje buffera
			continue;
		}

		int result;
		switch (choice) {
		case 1:
			result = addAtBeginning(&head);
			if (result == 0) printf("Osoba dodana na pocetak.\n");
			break;
		case 2:
			result = addAtEnd(&head);
			if (result == 0) printf("Osoba dodana na kraj.\n");
			break;
		case 3:
			printList(head);
			break;
		case 4:
			printf("Unesite prezime za pretragu: ");
			scanf("%s", surname);
			findBySurname(head, surname);
			break;
		case 5:
			printf("Unesite prezime za brisanje: ");
			scanf("%s", surname);
			deleteBySurname(&head, surname);
			break;
		case 6:
			printf("Unesite prezime osobe iza koje želite dodati: ");
			scanf("%s", surname);
			addAfterElement(head, surname);
			break;
		case 7:
			printf("Unesite prezime osobe ispred koje želite dodati: ");
			scanf("%s", surname);
			addBefore(&head, surname);
			break;
		case 8:
			sortBySurname(head);
			break;
		case 9: {
			char fileName[100];
			printf("Unesite naziv datoteke: ");
			scanf(" %s", fileName);
			writeToFile(head, fileName);
			break;
		}
		case 10: {
			char fileName[100];
			printf("Unesite naziv datoteke: ");
			scanf(" %s", fileName);
			readFromFile(&head, fileName);
			break;
		}
		case 0:
			printf("Kraj programa.\n");
			break;
		default:
			printf("Nepoznata opcija.\n");
		}
	} while (choice != 0);

	// oslobađanje memorije cijele liste prije izlaza
	Position temp;
	while (head != NULL) {
		temp = head;
		head = head->next;
		free(temp);
	}

	return 0;
}

// Kreira novi čvor i traži unos podataka od korisnika
Position createNewPerson() {
	Position newPerson = (Position)malloc(sizeof(Person));
	if (newPerson == NULL) {
		printf("Greska pri alokaciji memorije!\n");
		return NULL;
	}

	newPerson->next = NULL; // inicijalno novi element nema sljedećeg

	printf("Unesite ime:");
	scanf(" %s", newPerson->name);
	printf("Unesite prezime:");
	scanf(" %s", newPerson->surname);
	printf("Unesite godinu rodjenja:");
	scanf("%d", &newPerson->birthYear);

	return newPerson;
}

// Dodaje osobu na početak liste
int addAtBeginning(Position* head) {
	Position newPerson = createNewPerson();
	if (newPerson == NULL)
		return FILE_NOT_OPENED;

	newPerson->next = head->next; // novi element pokazuje na stari početak
	head->next = newPerson;       // head sada pokazuje na novi element

	return 0;
}

// Ispis svih elemenata u listi
int printList(Position head) {
	if (head == NULL) {
		printf("Prazna lista.\n");
		return 0;
	}

	printf("\n------ Sadrzaj liste ------\n");
	while (head != NULL) {
		printf("%s %s (%d)\n", head->name, head->surname, head->birthYear);
		head = head->next;
	}

	return 0;
}

// Dodaje novi element na kraj liste
int addAtEnd(Position* head) {
	Position newPerson = createNewPerson();
	if (newPerson == NULL)
		return FILE_NOT_OPENED;

	if (*head == NULL) { // ako je lista prazna
		*head = newPerson;
		return 0;
	}

	Position temp = *head;
	while (temp->next != NULL) // pronalazak zadnjeg elementa
		temp = temp->next;

	temp->next = newPerson; // povezivanje zadnjeg elementa s novim
	return 0;
}

// Traži osobu prema prezimenu
int findBySurname(Position head, char* surname) {
	while (head != NULL) {
		if (strcmp(head->surname, surname) == 0) { // ako su prezimena ista
			printf("Pronađena osoba: %s %s (%d)\n",
				head->name, head->surname, head->birthYear);
			return 0;
		}
		head = head->next;
	}

	printf("Osoba s prezimenom %s nije pronađena.\n", surname);
	return FILE_NOT_OPENED;
}

// Briše osobu prema prezimenu
int deleteBySurname(Position* head, char* surname) {
	if (*head == NULL) {
		printf("Lista je prazna.\n");
		return FILE_NOT_OPENED;
	}

	Position current = *head;
	Position prev = NULL;

	while (current != NULL) {
		if (strcmp(current->surname, surname) == 0) {
			// ako je prvi element
			if (prev == NULL)
				*head = current->next;
			else
				prev->next = current->next;

			printf("Osoba s prezimenom '%s' je obrisana.\n", surname);
			free(current);
			return 0;
		}
		prev = current;
		current = current->next;
	}

	printf("Osoba s prezimenom '%s' nije pronađena.\n", surname);
	return FILE_NOT_OPENED;
}

// Dodaje osobu iza određene osobe prema prezimenu
int addAfterElement(Position head, char* surname) {
	Position newPerson = createNewPerson();
	if (newPerson == NULL)
		return FILE_NOT_OPENED;

	while (head != NULL) {
		if (strcmp(head->surname, surname) == 0) {
			newPerson->next = head->next; // novi element se umeće nakon trenutnog
			head->next = newPerson;
			printf("Osoba dodana iza prezimena '%s'.\n", surname);
			return 0;
		}
		head = head->next;
	}

	printf("Osoba s prezimenom '%s' nije pronađena.\n", surname);
	free(newPerson);
	return FILE_NOT_OPENED;
}

// Dodaje osobu ispred određene osobe prema prezimenu
int addBefore(Position* head, char* surname) {
	Position newPerson = createNewPerson();
	if (newPerson == NULL)
		return FILE_NOT_OPENED;

	Position current = *head;
	Position prev = NULL;

	while (current != NULL) {
		if (strcmp(current->surname, surname) == 0) {
			if (prev == NULL) { // dodavanje na početak
				newPerson->next = *head;
				*head = newPerson;
			}
			else {
				newPerson->next = current;
				prev->next = newPerson;
			}
			printf("Osoba dodana ispred prezimena '%s'.\n", surname);
			return 0;
		}
		prev = current;
		current = current->next;
	}

	printf("Osoba s prezimenom '%s' nije pronađena.\n", surname);
	free(newPerson);
	return FILE_NOT_OPENED;
}

// Sortira listu po prezimenu (bubble sort)
int sortBySurname(Position head) {
	if (head == NULL || head->next == NULL)
		return 0;

	int swapped;
	Position ptr1;
	Position lptr = NULL; // pokazuje na zadnji već sortiran element

	do {
		swapped = 0;
		ptr1 = head;

		while (ptr1->next != lptr) {
			if (strcmp(ptr1->surname, ptr1->next->surname) > 0) {
				// zamjena podataka između dva čvora
				Person temp = *ptr1;
				strcpy(ptr1->name, ptr1->next->name);
				strcpy(ptr1->surname, ptr1->next->surname);
				ptr1->birthYear = ptr1->next->birthYear;

				strcpy(ptr1->next->name, temp.name);
				strcpy(ptr1->next->surname, temp.surname);
				ptr1->next->birthYear = temp.birthYear;

				swapped = 1;
			}
			ptr1 = ptr1->next;
		}
		lptr = ptr1; // kraj liste se postupno pomiče
	} while (swapped);

	printf("Lista je sortirana po prezimenima.\n");
	return 0;
}

// Sprema sadržaj liste u tekstualnu datoteku
int writeToFile(Position head, char* fileName) {
	FILE* fp = fopen(fileName, "w");
	if (fp == NULL) {
		printf("Greška pri otvaranju datoteke '%s'!\n", fileName);
		return FILE_NOT_OPENED;
	}

	while (head != NULL) {
		fprintf(fp, "%s %s %d\n", head->name, head->surname, head->birthYear);
		head = head->next;
	}

	fclose(fp);
	printf("Lista je uspješno zapisana u datoteku '%s'.\n", fileName);
	return 0;
}

// Učitava listu iz datoteke (briše staru listu)
int readFromFile(Position* head, char* fileName) {
	FILE* fp = fopen(fileName, "r");
	if (fp == NULL) {
		printf("Greška pri otvaranju datoteke '%s'!\n", fileName);
		return FILE_NOT_OPENED;
	}

	// briše postojeću listu prije učitavanja nove
	Position temp;
	while (*head != NULL) {
		temp = *head;
		*head = (*head)->next;
		free(temp);
	}

	Position last = NULL;
	while (!feof(fp)) {
		Position newPerson = (Position)malloc(sizeof(Person));
		if (newPerson == NULL) {
			printf("Greška pri alokaciji memorije!\n");
			fclose(fp);
			return FILE_NOT_OPENED;
		}

		if (fscanf(fp, " %49s %49s %d",
			newPerson->name, newPerson->surname, &newPerson->birthYear) != 3) {
			free(newPerson);
			break;
		}

		newPerson->next = NULL;

		// ako je prvi element u listi
		if (*head == NULL)
			*head = newPerson;
		else
			last->next = newPerson; // povezuje novi element s prethodnim

		last = newPerson; // ažurira pokazivač na kraj liste
	}

	fclose(fp);
	printf("Lista je uspješno učitana iz datoteke '%s'.\n", fileName);
	return 0;
}

