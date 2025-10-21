/* Definirati strukturu osoba (ime, prezime, godina roðenja) i napisati program koji:
A. dinamièki dodaje novi element na poèetak liste,
B. ispisuje listu,
C. dinamièki dodaje novi element na kraj liste,
D. pronalazi element u listi (po prezimenu),
E. briše odreðeni element iz liste,
U zadatku se ne smiju koristiti globalne varijable.*/

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
	struct Person* next; // pokazivaè na sljedeæi element, veza u listi
}Person;

typedef struct Person* Position;

int addAtBeginning(Position* head);
int printList(Position head);
int addAtEnd(Position* head);
int findBySurname(Position head, char* surname); 
int deleteBySurname(Position* head, char* surname);

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
		case 0:
			printf("Kraj programa.\n");
			break;
		default:
			printf("Nepoznata opcija.\n");
		}
	} while (choice != 0); //petlja se izvrsava dok korisnik ne odabere 'Izlaz'

	// Oslobaðanje memorije prije izlaza
	Position temp;
	while (head != NULL) { //dok lista nije prazna
		temp = head; //sprema trenutni element koji treba obrisati
		head = head->next; //postavlja head na sljedeci element
		free(temp);
	}

	return 0;
}

// Pomoæna funkcija za kreiranje i unos podataka
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


// A. Dinamièki dodaje novi element na poèetak liste
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


// C. Dinamièki dodati novi element na kraj liste
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
		int i = 0;
		// Usporedujemo dok su znakovi jednaki i dok nismo dosli do kraja
		while (head->surname[i] != '\0' && surname[i] != '\0' && head->surname[i] == surname[i]) {
			i++;
		}

		// Provjera: Jesu li oba stringa zavrsila istovremeno?
		if (head->surname[i] == '\0' && surname[i] == '\0') {
			printf("Pronaðena osoba: %s %s (%d)\n",
				head->name, head->surname, head->yearOfBirth);
			return 0; 
		}

		head = head->next; 
	}

	printf("Osoba s prezimenom %s nije pronaðena.\n", surname);
	return FILE_NOT_OPENED; 
}


// E. Briše odreðeni element iz liste (po prezimenu)
int deleteBySurname(Position* head, char* surname) {
	if (*head == NULL) {
		printf("Lista je prazna. Nema elemenata za brisanje.\n");
		return FILE_NOT_OPENED;
	}

	Position current = *head;
	Position prev = NULL;
	int istaPrezimena; // Pomocna varijabla

	// Tražimo element
	while (current != NULL) {
		int i = 0;
		istaPrezimena = 0;
		while (current->surname[i] != '\0' && surname[i] != '\0' && current->surname[i] == surname[i]) {
			i++;
		}
		if (current->surname[i] == '\0' && surname[i] == '\0') {
			istaPrezimena = 1; // Prezime pronadeno
			break;
		}

		// Ako prezime nije pronadeno, idem na sljedeci cvor
		prev = current;
		current = current->next; //trenutni ide na sljedeci
	}

	// ako nismo pronasli osobu
	if (current == NULL) {
		printf("Osoba s prezimenom '%s' nije pronaðena.\n", surname);
		return FILE_NOT_OPENED;
	}

	// BRISANJE ELEMENTA:

	// Ako je pronaden i to je prvi element (prev == NULL)
	if (prev == NULL) {
		*head = current->next; // Novi pocetak liste
	}
	else {
		// Preskaèemo element koji se brise
		prev->next = current->next;
	}

	printf("Osoba %s %s je obrisana.\n", current->name, current->surname);
	free(current); 
	return 0;
}

