#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_RED 1024
#define MAX_BODOVI 100

typedef struct student {
    char ime[50];
    char prezime[50];
    double bodovi;
} Stud;


int prebrojiRedke(const char* dat);
Stud* ucitajStudente(const char* dat, int n);
int ispisiStudente(Stud* studenti, int n);


int main() {
    char dat[50];
    printf("Unesite ime datoteke: ");
    scanf("%s", dat);

    int broj = prebrojiRedke(dat);
    if (broj <= 0) {
        printf("Greska: datoteka ne postoji ili je prazna!\n");
        return 1;
    }

    Stud* studenti = ucitajStudente(dat, broj);
    if (studenti == NULL) {
        printf("Greska pri ucitavanju studenata!\n");
        return 1;
    }

    ispisiStudente(studenti, broj);

    free(studenti);
    return 0;
}


int prebrojiRedke(const char* dat) {
    FILE* fp = fopen(dat, "r");
    if (fp == NULL) {
        return 0;
    }

    int brojac = 0;
    char buffer[MAX_RED];
    while (fgets(buffer, MAX_RED, fp) != NULL) {
        brojac++;
    }

    fclose(fp);
    return brojac;
}


Stud* ucitajStudente(const char* dat, int n) {
    Stud* studenti = (Stud*)malloc(n * sizeof(Stud));
    if (studenti == NULL) {
        return NULL;
    }

    FILE* fp = fopen(dat, "r");
    if (fp == NULL) {
        free(studenti);
        return NULL;
    }

    char buffer[MAX_RED];
    int i = 0;
    while (fgets(buffer, MAX_RED, fp) != NULL && i < n) {
        sscanf_s(buffer, "%s %s %lf",
            studenti[i].ime, (unsigned)_countof(studenti[i].ime),
            studenti[i].prezime, (unsigned)_countof(studenti[i].prezime),
            &studenti[i].bodovi);

        i++;
    }

    fclose(fp);
    return studenti;
}

int ispisiStudente(Stud* studenti, int n) {
    printf("\n%-15s %-15s %-15s %-15s\n", "IME", "PREZIME", "APS. BODOVI", "REL. BODOVI (%)");
    printf("---------------------------------------------------------------\n");

    for (int i = 0; i < n; i++) {
        double rel = (studenti[i].bodovi / MAX_BODOVI) * 100.0;
        printf("%-15s %-15s %-15.2lf %-15.2lf\n",
            studenti[i].ime,
            studenti[i].prezime,
            studenti[i].bodovi,
            rel);
    }

    return;
}
