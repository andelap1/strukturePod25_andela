/*9. Zadan je niz brojeva 2, 5, 7, 8, 11, 1, 4, 2, 3, 7 koji su spremljeni u èvorove binarnog stabla.
a) Napisati funkciju insert koja dodaje element u stablo tako da se pozivima te funkcije za
sve element zadanog niza brojeva stvori stablo kao na slici Slika 1. Funkcije vraæa
pokazivaè na korijen stabla.
b) Napisati funkciju replace koja æe svaki element stabla zamijeniti sumom elemenata u
njegovom lijevom i desnom podstablu (tj. sumom svih potomaka prije zamjene
vrijednosti u tim potomcima). Npr. stablo sa slike Slika 1 transformirat æe se u stablo na
slici Slika 2.
c) Prepraviti program na naèin da umjesto predefiniranog cjelobrojnog polja korištenjem
funkcije rand() generira sluèajne brojeve u rasponu <10, 90>. Takoðer, potrebno je
upisati u datoteku sve brojeve u inorder prolasku nakon korištenja funkcije iz a), zatim b)
dijela zadatka.*/

#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

typedef struct nodeTree* treePosition;
struct nodeTree {
	int value;
	treePosition left;
	treePosition right;
};

//a)
treePosition insert(treePosition root, int value) {
    if (root == NULL) {
        treePosition newNode = (treePosition)malloc(sizeof(struct nodeTree));
        if (newNode == NULL) 
            return NULL; 

        newNode->value = value;
        newNode->left = NULL;
        newNode->right = NULL;
        return newNode;
    }

    if (value > root->value) {
        root->right = insert(root->right, value);
    }
    else {
        root->left = insert(root->left, value);
    }

    return root; 
}

//b)
int replace(treePosition root) {
	if (root == NULL)
		return 0;
	int prevValue = root->value;

	root->value = replace(root->left) + replace(root->right);

	return prevValue + root->value;
}

//c)
int inorderToFile(treePosition root, FILE* fp) {
    if (root == NULL) return;

    inorderToFile(root->left, fp);
    fprintf(fp, "%d ", root->value);
    inorderToFile(root->right, fp);

    return 0;
}

int main() {
    treePosition root = NULL; 
    FILE* fp = NULL; 
    int i = 0;
    int randomNum = 0;

    srand((unsigned)time(NULL));

    // Formula: rand() % (max - min - 1) + (min + 1)
    for (i = 0; i < 10; i++) {
        randomNum = (rand() % 79) + 11; 
        root = insert(root, randomNum);
    }

    fp = fopen("stablo.txt", "w");
    if (fp == NULL) {
        printf("Pogreska pri otvaranju datoteke!\n");
        return 1;
    }

    // Upis nakon koraka a)
    fprintf(fp, "Inorder nakon umetanja (a):\n");
    inorderToFile(root, fp);
    fprintf(fp, "\n\n");

    replace(root);

    // Upis nakon koraka b)
    fprintf(fp, "Inorder nakon zamjene suma (b):\n");
    inorderToFile(root, fp);

    fclose(fp);
    printf("Rezultati su uspjesno upisani u datoteku 'stablo.txt'.\n");

    return 0;
}