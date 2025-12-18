/*Napisati program koji omoguæava rad s binarnim stablom pretraživanja. Treba
omoguæiti unošenje novog elementa u stablo, ispis elemenata (inorder, preorder, postorder i
level order), brisanje i pronalaženje nekog elementa.*/
#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>

typedef struct Node* Tree;

struct Node { 
	int data;
	Tree left;
	Tree right;
};


Tree createNewNode(int data) {
	Tree newNode = (Tree)malloc(sizeof(struct Node));
	if (newNode == NULL)
		return NULL;

	newNode->data = data;
	newNode->left = NULL; 
	newNode->right = NULL;

	return newNode;
}

Tree insertNewEl(int data, Tree root) {
	if (root == NULL) {
		return createNewNode(data);
	}

	if (data < root->data) {
		root->left = insertNewEl(root->left, data); 
	}

	else if (data > root->data) {
		root->right = insertNewEl(root->right, data); 
	}

	return root;
}

int inorder(Tree root) {
	if (root == NULL) {
		return 0;
	}

	inorder(root->left);

	if (printf("%d ", root->data) < 0) {
		return -1; 
	}

	inorder(root->right);

	return 0;
}

int preorder(Tree root) {
	if (root == NULL) {
		return 0;
	}
	
	if (printf("%d ", root->data) < 0) {
		return -1;
	}	
	preorder(root->left);
	preorder(root->right);

	return 0;
}

int postorder(Tree root) {
	if (root == NULL) {
		return 0;
	}
	
	postorder(root->left);
	postorder(root->right);
	if (printf("%d ", root->data) < 0) {
		return -1; 
	}

	return 0;
}

struct QueueElement {
	Tree node;
	struct QueueElement* next;
};

struct Queue {
	struct QueueElement* head;
	struct QueueElement* tail;
};

int InitializeQueue(struct Queue* q) {
	if (q == NULL) {
		printf("Error: Queue pointer is NULL.\n");
		return -1;
	}
	q->head = NULL;
	q->tail = NULL;
	return 0;
}

// Dodavanje elementa u Red - dodaje se na kraj (tail)
int Enqueue(struct Queue* q, Tree t) {
	if (q == NULL) return -1;

	struct QueueElement* newNode = (struct QueueElement*)malloc(sizeof(struct QueueElement));
	if (newNode == NULL) {
		printf("Memory allocation error for QueueElement!\n");
		return -1;
	}

	newNode->node = t;
	newNode->next = NULL;

	if (q->tail == NULL) { // Ako je Red prazan, novi element je i head i tail
		q->head = newNode;
		q->tail = newNode;
	}
	else {
		q->tail->next = newNode; // Stari 'tail' sada pokazuje na novi element
		q->tail = newNode;       // Novi element postaje novi 'tail'
	}

	return 0;
}

// Vaðenje elementa iz Reda (Dequeue) - vadi se s poèetka (head)
// Vraæa pokazivaè na èvor stabla (Tree)
Tree Dequeue(struct Queue* q) {
	if (q == NULL || q->head == NULL) {
		return NULL; // Red je prazan ili nevalidan
	}

	// 1. Priprema za brisanje i vraæanje vrijednosti
	struct QueueElement* temp = q->head;
	Tree treeNodeToReturn = temp->node;

	// 2. Pomicanje head-a
	q->head = q->head->next;

	// 3. Oslobodi memoriju temp
	free(temp);

	// 4. Ažuriraj tail ako je Red postao prazan
	if (q->head == NULL) {
		q->tail = NULL;
	}

	return treeNodeToReturn;
}

// Provjera je li Red prazan
int IsQueueEmpty(struct Queue* q) {
	if (q == NULL || q->head == NULL) {
		return 1; // Prazan je
	}
	return 0; // Nije prazan
}

// Level Order Ispis (Ispis po razinama - koristi Red)
int LevelOrder(Tree Root) {
	if (Root == NULL) {
		printf("Tree is empty.\n");
		return 0;
	}

	struct Queue q;

	// 1. Inicijalizacija Reda
	if (InitializeQueue(&q) != 0) {
		return -1; // Greška pri inicijalizaciji
	}

	// 2. Stavi korijen u Red
	if (Enqueue(&q, Root) != 0) {
		// Ako Enqueue ne uspije, Red je možda neispravan
		return -1;
	}

	printf("Level Order: ");

	// 3. Glavna petlja: dok Red nije prazan
	while (!IsQueueEmpty(&q)) {

		// Izvadi element iz Reda
		Tree currentNode = Dequeue(&q);

		// Trebali bismo provjeriti je li Dequeue vratio NULL, ali 
		// buduæi da je petlja kontrolirana s IsQueueEmpty, to se ne bi smjelo dogoditi.

		// Ispiši vrijednost
		if (printf("%d ", currentNode->data) < 0) {
			return -1; // Greška pri ispisu
		}

		// 4. Stavi lijevo dijete u Red (ako postoji)
		if (currentNode->left != NULL) {
			if (Enqueue(&q, currentNode->left) != 0) {
				return -1;
			}
		}

		// 5. Stavi desno dijete u Red (ako postoji)
		if (currentNode->right != NULL) {
			if (Enqueue(&q, currentNode->right) != 0) {
				return -1;
			}
		}
	}

	printf("\n");
	return 0;
}

Tree FindMin(Tree Root) {
	if (Root == NULL) {
		return NULL;
	}

	// Iterativno idemo lijevo dok ne doðemo do kraja
	while (Root->left != NULL) {
		Root = Root->left;
	}

	return Root;
}

int DeleteNode(Tree* RootPtr, int data) {

	// Kljuèno: radimo s pokazivaèem na pokazivaè (*RootPtr) kako bismo
	// mogli trajno promijeniti vezu roditelju (ili korijenu).
	Tree Root = *RootPtr;

	// BAZNI SLUÈAJ: Stablo je prazno ili element nije pronaðen
	if (Root == NULL) {
		printf("Error: Element %d not found in the tree.\n", data);
		return -1;
	}

	// 1. REKURZIVNA POTRAGA: Pronaði èvor za brisanje
	if (data < Root->data) {
		// Traženi element je u lijevom podstablu
		return DeleteNode(&(Root->left), data);
	}
	else if (data > Root->data) {
		// Traženi element je u desnom podstablu
		return DeleteNode(&(Root->right), data);
	}
	else {
		// 2. PRONAÐEN JE ÈVOR ZA BRISANJE (Root je èvor koji treba obrisati)

		// SLUÈAJ 1: Èvor s nula ili jednim djetetom
		if (Root->left == NULL) {
			Tree temp = Root->right; // Zapamti desno dijete (može biti NULL)
			free(Root);
			*RootPtr = temp; // Ažuriraj pokazivaè roditelja (ili korijena)
			return 0; // Uspjeh
		}
		else if (Root->right == NULL) {
			Tree temp = Root->left;  // Zapamti lijevo dijete
			free(Root);
			*RootPtr = temp; // Ažuriraj pokazivaè roditelja (ili korijena)
			return 0; // Uspjeh
		}

		// SLUÈAJ 2: Èvor s dva djeteta
		// Pronaði in-order nasljednika (minimum iz desnog podstabla)
		Tree temp = FindMin(Root->right);

		// Kopiraj sadržaj in-order nasljednika u trenutni èvor
		Root->data = temp->data;

		return DeleteNode(&(Root->right), temp->data);
	}

	return 0;
}


Tree Find(Tree Root, int data) {
	// 1. Bazni sluèaj: Element nije pronaðen (stiglo se do NULL) ILI je element pronaðen
	if (Root == NULL || Root->data == data) {
		return Root;
	}

	// 2. Idi lijevo (data < Root->data)
	if (data < Root->data) {
		// Vraæamo rezultat rekurzivnog poziva za lijevo podstablo
		return Find(Root->left, data);
	}

	// 3. Idi desno (data > Root->data)
	else {
		// Vraæamo rezultat rekurzivnog poziva za desno podstablo
		return Find(Root->right, data);
	}
}


int printMenu() {
	printf("\n--- BINARNO STABLO PRETRAZIVANJA (BST) ---\n");
	printf("1. Unos novog elementa\n");
	printf("2. Ispis elemenata (Inorder)\n");
	printf("3. Ispis elemenata (Preorder)\n");
	printf("4. Ispis elemenata (Postorder)\n");
	printf("5. Ispis elemenata (Level Order)\n");
	printf("6. Pronalazenje elementa\n");
	printf("7. Brisanje elementa\n");
	printf("8. Ispis svih metoda\n");
	printf("0. Izlaz\n");
	printf("Odaberite opciju: ");

	return 0;
}

int main() {
	Tree root = NULL;
	int choice;
	int data;
	Tree foundNode = NULL;

	do {
		printMenu();
		// Uzimanje unosa od korisnika
		if (scanf("%d", &choice) != 1) {
			printf("Greska pri unosu! Pokusajte ponovo.\n");
			// Ciscenje ulaznog buffera u slucaju neispravnog unosa (npr. slova)
			while (getchar() != '\n');
			continue;
		}

		switch (choice) {
		case 1: // Unos novog elementa
			printf("Unesite element za unosenje: ");
			if (scanf("%d", &data) == 1) {
				// Kljuèno: rezultat rekurzivnog umetanja se MORA dodijeliti root varijabli!
				root = insertNewEl(data, root);
				printf("Element %d je unesen.\n", data);
			}
			else {
				printf("Neispravan unos.\n");
				while (getchar() != '\n');
			}
			break;

		case 2: // Inorder
			printf("Inorder ispis (Sortirano): ");
			inorder(root);
			printf("\n");
			break;

		case 3: // Preorder
			printf("Preorder ispis: ");
			preorder(root);
			printf("\n");
			break;

		case 4: // Postorder
			printf("Postorder ispis: ");
			postorder(root);
			printf("\n");
			break;

		case 5: // Level Order
			LevelOrder(root);
			break;

		case 6: // Pronalazenje elementa
			printf("Unesite element koji trazite: ");
			if (scanf("%d", &data) == 1) {
				foundNode = Find(root, data);
				if (foundNode != NULL) {
					printf("Element %d je pronadjen.\n", data);
				}
				else {
					printf("Element %d nije pronadjen u stablu.\n", data);
				}
			}
			else {
				printf("Neispravan unos.\n");
				while (getchar() != '\n');
			}
			break;

		case 7: // Brisanje elementa
			printf("Unesite element za brisanje: ");
			if (scanf("%d", &data) == 1) {
				// Poziv DeleteNode, prosljeðujemo adresu pokazivaèa na korijen
				if (DeleteNode(&root, data) == 0) {
					printf("Element %d je uspjesno obrisan.\n", data);
				}
				// Greška pri brisanju se ispisuje unutar DeleteNode
			}
			else {
				printf("Neispravan unos.\n");
				while (getchar() != '\n');
			}
			break;

		case 8: // Ispis svih metoda
			printf("\n--- Ispis svih metoda ---\n");
			printf("Inorder (Sortirano): ");
			inorder(root);
			printf("\nPreorder: ");
			preorder(root);
			printf("\nPostorder: ");
			postorder(root);
			printf("\n");
			LevelOrder(root);
			printf("-------------------------\n");
			break;

		case 0:
			printf("Izlaz iz programa.\n");
			break;

		default:
			printf("Neispravna opcija. Pokusajte ponovo.\n");
		}

	} while (choice != 0);

	return 0;
}