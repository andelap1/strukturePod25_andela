#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_RED 1024
#define MAX_BODOVI 100

//definiramo strukturu student
typedef struct student {
    char ime[50];
    char prezime[50];
    double bodovi;
} Stud;


int prebrojiStudente(const char* dat);  //broji koliko studenata ima u datoteci
Stud* ucitajStudente(const char* dat, int n); //ucitava podatke o studentima iz datoteke u niz struktura
int ispisiStudente(Stud* studenti, int n); //ispis


int main() {
    char dat[50];
    printf("Unesite ime datoteke: ");
    scanf("%s", dat);

    int broj = prebrojiStudente(dat); //poziv funkcije koja broji studente, tj. redke
    if (broj <= 0) {
        printf("Greska: datoteka ne postoji ili je prazna!\n");
        return 1;
    }

    Stud* studenti = ucitajStudente(dat, broj); //ucitavanje studenata u dinamicki alociran niz jer ne znam unaprijed koliko ima studenata
    if (studenti == NULL) {
        printf("Greska pri ucitavanju studenata!\n");
        return 1;
    }

    ispisiStudente(studenti, broj); 

    free(studenti); //oslobadjam memoriju koju sam zauzela mallocom
    return 0;
}


int prebrojiStudente(const char* dat) {
    FILE* fp = fopen(dat, "r");
    if (fp == NULL) {
        return 0;
    }

    //brojimo studente, tj. redove dok ih ima
    int brojac = 0;
    char buffer[MAX_RED];
    while (fgets(buffer, MAX_RED, fp) != NULL) {
        brojac++;
    }

    fclose(fp);
    return brojac;
}


Stud* ucitajStudente(const char* dat, int n) {
    Stud* studenti = (Stud*)malloc(n * sizeof(Stud)); //dinamicki alociramo memoriju za n broj studenata
    if (studenti == NULL) {
        return NULL; //ovo ako nema dovoljno memorije
    }

    FILE* fp = fopen(dat, "r");
    if (fp == NULL) {
        free(studenti);
        return NULL;
    }

    //ucitavam svakog studenta iz datoteke red po red
    char buffer[MAX_RED];
    int i = 0;
    while (fgets(buffer, MAX_RED, fp) != NULL && i < n) {
        sscanf(buffer, "%s %s %lf",
            studenti[i].ime,
            studenti[i].prezime,
            &studenti[i].bodovi);

        i++;
    }

    fclose(fp);
    return studenti; //pokazivac vracam na niz studenata
}

int ispisiStudente(Stud* studenti, int n) {
    printf("\n%-15s %-15s %-15s %-15s\n", "IME", "PREZIME", "APS. BODOVI", "REL. BODOVI (%)");
    printf("---------------------------------------------------------------\n"); //zaglavlje tablice

    for (int i = 0; i < n; i++) {
        double rel_bod = (studenti[i].bodovi / MAX_BODOVI) * 100.0;
        printf("%-15s %-15s %-15.2lf %-15.2lf\n",
            studenti[i].ime,
            studenti[i].prezime,
            studenti[i].bodovi,
            rel_bod);
    }

    return;
}


