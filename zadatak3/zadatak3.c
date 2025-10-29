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
#define MAX_PREZIME 50

// Definiranje strukture Person
typedef struct Person {
	char name[MAX_PREZIME];
	char surname[MAX_PREZIME];
	int yearOfBirth;
	struct Person* next; // pokazivac na sljedeci element, veza u listi
}Person;

typedef struct Person* Position;

int addAtBeginning(Position*);
int printList(Position);
int addAtEnd(Position* );
int findBySurname(Position , char*);
int deleteBySurname(Position* , char*);
int addAfterElement(Position , char*);



int main() {
	Position head = NULL; //pokazivac na pocetak liste, inicijaliziran na NULL
	char surname[MAX_PREZIME];
	int choice;
	int result; // Za provjeru povratne vrijednosti funkcija


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


		if (scanf("%d", &choice) != 1) { //ako je odabir izbornika razlicit od 1
			printf("Neispravan unos. Pokusajte ponovo.\n");
			while (getchar() != '\n'); //cisti ulazni buffer od neispravnih znakova
			continue; //preskace ostatak petlje i pocinje ispocetka
		}

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
			scanf(" %49s", surname);
			findBySurname(head, surname);
			break;
		case 5:
			printf("Unesite prezime za brisanje: ");
			scanf(" %49s", surname);
			deleteBySurname(&head, surname); //salje se adresa heada
			break;
		case 6:
			printf("Unesite prezime osobe iza koje želite dodati: ");
			scanf(" %49s", surname);
			addAfterElement(head, surname); 
			break;
		case 7:
			printf("Unesite prezime osobe ispred koje želite dodati: ");
			scanf(" %49s", surname);
			addBefore(&head, surname);
			break;
		case 8:
			sortBySurname(head);
			break;
		case 9: {
			char fileName[100];
			printf("Unesite naziv datoteke: ");
			scanf(" %99s", fileName);
			writeToFile(head, fileName);
			break;
		}
		case 10: {
			char fileName[100];
			printf("Unesite naziv datoteke: ");
			scanf(" %99s", fileName);
			readFromFile(&head, fileName);
			break;
		}

		case 0:
			printf("Kraj programa.\n");
			break;
		default:
			printf("Nepoznata opcija.\n");
		}
	} while (choice != 0); //petlja se izvrsava dok korisnik ne odabere 'Izlaz'

	// Oslobadanje memorije prije izlaza
	Position temp;
	while (head != NULL) { //dok lista nije prazna
		temp = head; //sprema trenutni element koji treba obrisati
		head = head->next; //postavlja head na sljedeci element
		free(temp);
	}

	return 0;
}

// Pomocna funkcija za kreiranje i unos podataka
Position createNewPerson() {
	Position newPerson = (Position)malloc(sizeof(Person));
	if (newPerson == NULL) {
		printf("Greska pri alokaciji memorije!\n");
		return NULL;
	}
	newPerson->next = NULL; //postavlja se pokazivac next na NULL jer ce to biti novi kraj liste 

	printf("Unesite ime:");
	scanf(" %49s", newPerson->name);
	printf("Unesite prezime:");
	scanf(" %49s", newPerson->surname);
	printf("Unesite godinu rodjenja:");
	scanf("%d", &newPerson->yearOfBirth);

	return newPerson; //vraca pokazivac na novi cvor
}


// A. Dinamicki dodaje novi element na pocetak liste
int addAtBeginning(Position* head) {
	Position newPerson = createNewPerson();
	if (newPerson == NULL) {
		return FILE_NOT_OPENED;
	}

	newPerson->next = *head; //pokazivac novog elementa se postavlja na trenutni pocetak
	*head = newPerson; //sad head pokazuje na novi element koji je sad pocetak liste

	return 0;
}

// B. Ispisuje listu
int printList(Position head) {
	if (head == NULL) {
		printf("Prazna lista.\n");
		return 0;
	}

	printf("\n--------- Sadrzaj liste ---------\n");
	while (head != NULL) { // radi dok lista ne dode do kraja 
		printf("%s %s (%d)\n", head->name, head->surname, head->yearOfBirth);
		head = head->next; //prelazak na sljedeci element
	}

	return 0;
}


// C. Dinamicki dodati novi element na kraj liste
int addAtEnd(Position* head) {
	Position newPerson = createNewPerson();
	if (newPerson == NULL) {
		return FILE_NOT_OPENED;
	}

	// Ako je lista prazna
	if (*head == NULL) {
		*head = newPerson; //novi cvor postaje pocetak
		return 0;
	}

	// Tražimo zadnji element
	Position temp = *head;
	while (temp->next != NULL) {
		temp = temp->next;
	}

	// Povezujemo zadnji element s novim
	temp->next = newPerson;
	return 0;
}

// D. Pronalazi element u listi (po prezimenu)
int findBySurname(Position head, char* surname) {
	while (head != NULL) {
		if (strcmp(head->surname, surname) == 0) {   // usporedba stringova
			printf("Pronađena osoba: %s %s (%d)\n",
				head->name, head->surname, head->yearOfBirth);
			return 0;
		}
		head = head->next;
	}

	printf("Osoba s prezimenom %s nije pronađena.\n", surname);
	return FILE_NOT_OPENED;
}



// E. Briše određeni element iz liste (po prezimenu)
int deleteBySurname(Position* head, char* surname) {
	if (*head == NULL) {
		printf("Lista je prazna. Nema elemenata za brisanje.\n");
		return FILE_NOT_OPENED;
	}

	Position current = *head;
	Position prev = NULL;

	while (current != NULL) {
		if (strcmp(current->surname, surname) == 0) {   // usporedba stringova
			// Ako je prvi element u listi
			if (prev == NULL) {
				*head = current->next;
			}
			else {
				prev->next = current->next;
			}

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

int addAfterElement(Position head, char* surname) {

		Position newPerson = createNewPerson();
		if (newPerson == NULL)
			return FILE_NOT_OPENED;

		while (head != NULL) {
			if (strcmp(head->surname, surname) == 0) {
				newPerson->next = head->next;
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

int addBefore(Position* head, char* surname) {
	Position newPerson = createNewPerson();
	if (newPerson == NULL)
		return FILE_NOT_OPENED;

	Position current = *head;
	Position prev = NULL;

	while (current != NULL) {
		if (strcmp(current->surname, surname) == 0) {
			if (prev == NULL) {
				// Dodajemo na početak
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

int sortBySurname(Position head) {
	if (head == NULL || head->next == NULL)
		return 0;

	int swapped;
	Position ptr1;
	Position lptr = NULL;

	do {
		swapped = 0;
		ptr1 = head;

		while (ptr1->next != lptr) {
			if (strcmp(ptr1->surname, ptr1->next->surname) > 0) {
				// Zamjena podataka
				Person temp = *ptr1;
				strcpy(ptr1->name, ptr1->next->name);
				strcpy(ptr1->surname, ptr1->next->surname);
				ptr1->yearOfBirth = ptr1->next->yearOfBirth;

				strcpy(ptr1->next->name, temp.name);
				strcpy(ptr1->next->surname, temp.surname);
				ptr1->next->yearOfBirth = temp.yearOfBirth;

				swapped = 1;
			}
			ptr1 = ptr1->next;
		}
		lptr = ptr1;
	} while (swapped);

	printf("Lista je sortirana po prezimenima.\n");
	return 0;
}

int writeToFile(Position head, char* fileName) {
	FILE* fp = fopen(fileName, "w");
	if (fp == NULL) {
		printf("Greška pri otvaranju datoteke '%s'!\n", fileName);
		return FILE_NOT_OPENED;
	}

	while (head != NULL) {
		fprintf(fp, "%s %s %d\n", head->name, head->surname, head->yearOfBirth);
		head = head->next;
	}

	fclose(fp);
	printf("Lista je uspješno zapisana u datoteku '%s'.\n", fileName);
	return 0;
}

int readFromFile(Position* head, char* fileName) {
	FILE* fp = fopen(fileName, "r");
	if (fp == NULL) {
		printf("Greška pri otvaranju datoteke '%s'!\n", fileName);
		return FILE_NOT_OPENED;
	}

	// Oslobodi staru listu ako postoji
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

		if (fscanf(fp, " %49s %49s %d", newPerson->name, newPerson->surname, &newPerson->yearOfBirth) != 3) {
			free(newPerson);
			break;
		}

		newPerson->next = NULL;

		if (*head == NULL)
			*head = newPerson;
		else
			last->next = newPerson;

		last = newPerson;
	}

	fclose(fp);
	printf("Lista je uspješno učitana iz datoteke '%s'.\n", fileName);
	return 0;
}
