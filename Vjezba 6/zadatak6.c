/*6. Napisati program koji èita datoteku racuni.txt u kojoj su zapisani nazivi svih datoteka koji
predstavljaju pojedini raèun. Na poèetku svake datoteke je zapisan datum u kojem vremenu je
raèun izdat u formatu YYYY-MM-DD. Svaki sljedeæi red u datoteci predstavlja artikl u formatu
naziv, kolièina, cijena. Potrebno je formirati vezanu listu raèuna sortiranu po datumu. Svaki èvor
vezane liste sadržava vezanu listu artikala sortiranu po nazivu artikla. Nakon toga potrebno je
omoguæiti upit kojim æe korisnik saznati koliko je novaca sveukupno potrošeno na specifièni
artikl u odreðenom vremenskom razdoblju i u kojoj je kolièini isti kupljen.*/
#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Definiramo maksimalnu duljinu naziva artikla za sigurnost
#define MAX_NAZIV 100
#define MAX_LINIJA 256
#define DATUM_LEN 11 // YYYY-MM-DD\0

typedef struct Artikal {
    char naziv[MAX_NAZIV];
    int kolicina;
    double cijena; // Koristimo double za bolju preciznost
    struct Artikal* next;
} Artikal;

typedef struct Racun {
    char datum[DATUM_LEN]; // YYYY-MM-DD\0
    Artikal* artikli;
    struct Racun* next;
} Racun;


/* ------------------ FUNKCIJE ZA SORTIRANO UBACIVANJE ------------------ */

void ubaciArtikalSortirano(Artikal** head, Artikal* novi) {
    if (*head == NULL || strcmp(novi->naziv, (*head)->naziv) < 0) {
        novi->next = *head;
        *head = novi;
        return;
    }
    Artikal* curr = *head;
    while (curr->next != NULL && strcmp(curr->next->naziv, novi->naziv) < 0) {
        curr = curr->next;
    }
    novi->next = curr->next;
    curr->next = novi;
}

void ubaciRacunSortirano(Racun** head, Racun* novi) {
    if (*head == NULL || strcmp(novi->datum, (*head)->datum) < 0) {
        novi->next = *head;
        *head = novi;
        return;
    }
    Racun* curr = *head;
    while (curr->next != NULL && strcmp(curr->next->datum, novi->datum) < 0) {
        curr = curr->next;
    }
    novi->next = curr->next;
    curr->next = novi;
}


/* ---------------------- UÈITAVANJE RAÈUNA --------------------- */

void ukloniNewline(char* s) {
    s[strcspn(s, "\n")] = 0;
}

Racun* ucitajRacun(const char* imeDatoteke) {
    FILE* f = fopen(imeDatoteke, "r");
    if (!f) {
        fprintf(stderr, "Greska: ne mogu otvoriti %s\n", imeDatoteke);
        return NULL;
    }

    Racun* r = (Racun*)malloc(sizeof(Racun));
    if (!r) {
        perror("Greska pri alokaciji racuna");
        fclose(f);
        return NULL;
    }
    r->artikli = NULL;
    r->next = NULL;

    // Èitanje datuma
    if (fgets(r->datum, DATUM_LEN, f) == NULL) {
        fprintf(stderr, "Greska: ne mogu procitati datum iz %s\n", imeDatoteke);
        free(r);
        fclose(f);
        return NULL;
    }
    ukloniNewline(r->datum);

    // Èitanje ostatka retka s datumom ako postoji (stari kod je imao fgetc za \n)
    int c;
    while ((c = fgetc(f)) != '\n' && c != EOF);

    char linija[MAX_LINIJA];

    while (fgets(linija, sizeof(linija), f)) {
        Artikal* a = (Artikal*)malloc(sizeof(Artikal));
        if (!a) {
            perror("Greska pri alokaciji artikla");
            // Ovdje bi trebalo osloboditi veæ uèitane artikle ako doðe do greške
            continue;
        }
        a->next = NULL;

        char temp_naziv[MAX_NAZIV];

        // Koristimo sscanf za sigurnije parsiranje, pazeæi na duljinu ulaza
        // Provjeravamo je li sscanf uspio proèitati sva 3 elementa (naziv, kolicina, cijena)
        if (sscanf(linija, "%99[^,],%d,%lf", temp_naziv, &a->kolicina, &a->cijena) == 3) {

            // Koristimo strncpy umjesto strcpy (rješava C4996)
            strncpy(a->naziv, temp_naziv, sizeof(a->naziv) - 1);
            a->naziv[sizeof(a->naziv) - 1] = '\0';

            ubaciArtikalSortirano(&(r->artikli), a);
        }
        else {
            fprintf(stderr, "Upozorenje: Preskocena neispravna linija u %s: %s", imeDatoteke, linija);
            free(a); // Oslobaðamo alocirani artikl ako nije uspješno proèitan
        }
    }

    fclose(f);
    return r;
}


/* --------------------------- ISPIS --------------------------- */

void ispisi(Racun* head) {
    while (head) {
        printf("Racun (%s):\n", head->datum);

        Artikal* a = head->artikli;
        while (a) {
            // Koristimo %lf za ispis double
            printf("    %-20s  %3d kom    %.2lf EUR\n", a->naziv, a->kolicina, a->cijena);
            a = a->next;
        }
        printf("\n");
        head = head->next;
    }
}


/* ------------------ UPIT: ARTIKL U PERIODU -------------------- */

int datumUIntervalu(const char* d, const char* od, const char* do_) {
    return strcmp(d, od) >= 0 && strcmp(d, do_) <= 0;
}

void upit(Racun* head) {
    char trazeni[MAX_NAZIV];
    char od[DATUM_LEN], do_[DATUM_LEN];

    // Ciscenje buffera je problematicno s kombinacijom scanf/fgets,
    // bolje je u potpunosti prebaciti na fgets ako je moguce, ali za sada:
    int c;
    while ((c = getchar()) != '\n' && c != EOF); // Puno sigurnije ciscenje buffera

    printf("Unesite naziv artikla: ");
    if (fgets(trazeni, sizeof(trazeni), stdin) == NULL) {
        printf("Greska pri citanju naziva artikla.\n");
        return;
    }
    ukloniNewline(trazeni);

    printf("Unesite pocetni datum (YYYY-MM-DD): ");
    // Ogranicavamo scanf na DATUM_LEN-1 za sigurnost (rješava C6054 i C4996)
    if (scanf("%10s", od) != 1) {
        printf("Greska pri citanju pocetnog datuma.\n");
        return;
    }

    printf("Unesite krajnji datum (YYYY-MM-DD): ");
    if (scanf("%10s", do_) != 1) { // Rješava C6054 i C4996
        printf("Greska pri citanju krajnjeg datuma.\n");
        return;
    }

    int ukupnaKolicina = 0;
    double ukupnoPotroseno = 0.0; // double

    while (head) {
        if (datumUIntervalu(head->datum, od, do_)) {
            Artikal* a = head->artikli;
            while (a) {
                if (strcmp(a->naziv, trazeni) == 0) {
                    ukupnaKolicina += a->kolicina;
                    ukupnoPotroseno += a->kolicina * a->cijena;
                }
                a = a->next;
            }
        }
        head = head->next;
    }

    printf("\nArtikal: %s\n", trazeni);
    printf("Ukupno kupljeno: %d kom\n", ukupnaKolicina);
    // Koristimo %lf za ispis double
    printf("Ukupno potroseno: %.2lf EUR\n\n", ukupnoPotroseno);
}


/* ------------------------ DODATNE FUNKCIJE ------------------------ */

// Ostale funkcije (ispisiArtikleGlobalno, ispisiPotrosnjuPoDanu, ispisiRacuneNaDatum)
// su samo prilagoðene da koriste %lf za ispis gdje je potrebno.

void ispisiArtikleGlobalno(Racun* head) {
    printf("\n--- Svi artikli koji se pojavljuju u svim raèunima ---\n");
    while (head) {
        Artikal* a = head->artikli;
        while (a) {
            printf("%s\n", a->naziv);
            a = a->next;
        }
        head = head->next;
    }
    printf("\n");
}

void ispisiPotrosnjuPoDanu(Racun* head) {
    printf("\n--- Potrošnja po danu ---\n");
    while (head) {
        double suma = 0.0;
        Artikal* a = head->artikli;
        while (a) {
            suma += a->kolicina * a->cijena;
            a = a->next;
        }
        printf("%s : %.2lf EUR\n", head->datum, suma);
        head = head->next;
    }
    printf("\n");
}

void ispisiRacuneNaDatum(Racun* head) {
    char datum[DATUM_LEN];

    int c;
    while ((c = getchar()) != '\n' && c != EOF); // Puno sigurnije ciscenje buffera

    printf("Unesite datum (YYYY-MM-DD): ");
    // Ogranicavamo scanf na DATUM_LEN-1 za sigurnost (rješava C6054 i C4996)
    if (scanf("%10s", datum) != 1) {
        printf("Greska pri citanju datuma.\n");
        return;
    }

    printf("\n--- Raèuni dana %s ---\n", datum);

    int pronadjen = 0;
    while (head) {
        if (strcmp(head->datum, datum) == 0) {
            pronadjen = 1;
            Artikal* a = head->artikli;
            while (a) {
                printf("%s %d kom %.2lf EUR\n", a->naziv, a->kolicina, a->cijena);
                a = a->next;
            }
            printf("\n");
        }
        head = head->next;
    }
    if (!pronadjen) {
        printf("Nema raèuna na taj datum.\n");
    }
}


/* ---------------------- OSLOBAÐANJE MEMORIJE ---------------------- */

void oslobodiArtikle(Artikal* a) {
    while (a) {
        Artikal* tmp = a;
        a = a->next;
        free(tmp);
    }
}

void oslobodiRacune(Racun* r) {
    while (r) {
        oslobodiArtikle(r->artikli);
        Racun* tmp = r;
        r = r->next;
        free(tmp);
    }
}


/* ---------------------------- MENI ---------------------------- */

void meni(Racun* listaRacuna) {
    int izbor;

    do {
        printf("\n--- MENI ---\n");
        printf("1. Pretraga artikla u vremenskom periodu\n");
        printf("2. Ispis svih racuna\n");
        printf("3. Ispis svih artikala (globalni pregled)\n");
        printf("4. Ispis ukupne potrošnje po danu\n");
        printf("5. Ispis svih racuna odreðenog datuma\n");
        printf("6. Oslobodi memoriju (reset)\n");
        printf("0. Izlaz iz programa\n> ");

        // Provjera povratne vrijednosti za scanf (rješava C6031)
        if (scanf("%d", &izbor) != 1) {
            fprintf(stderr, "Neispravan unos. Molimo unesite broj.\n");
            // Oèistiti buffer i ponovno zatražiti unos
            int c;
            while ((c = getchar()) != '\n' && c != EOF);
            izbor = -1; // Postavlja na neispravnu opciju
            continue;
        }

        switch (izbor) {
        case 1: upit(listaRacuna); break;
        case 2: ispisi(listaRacuna); break;
        case 3: ispisiArtikleGlobalno(listaRacuna); break;
        case 4: ispisiPotrosnjuPoDanu(listaRacuna); break;
        case 5: ispisiRacuneNaDatum(listaRacuna); break;
        case 6:
            oslobodiRacune(listaRacuna);
            listaRacuna = NULL;
            printf("Svi podaci obrisani iz memorije.\n");
            break;
        case 0:
            oslobodiRacune(listaRacuna);
            printf("Zatvaranje programa...\n");
            break;
        default: printf("Nepostojeca opcija.\n");
        }
    } while (izbor != 0);
}


/* ----------------------------- MAIN ----------------------------- */

int main() {
    FILE* f = fopen("racuni.txt", "r");
    if (!f) {
        fprintf(stderr, "Greska: ne mogu otvoriti racuni.txt. Provjerite da li datoteka postoji.\n");
        return 1;
    }

    Racun* listaRacuna = NULL;
    char imeDatoteke[128];

    while (fgets(imeDatoteke, sizeof(imeDatoteke), f)) {
        ukloniNewline(imeDatoteke);
        Racun* r = ucitajRacun(imeDatoteke);
        if (r)
            ubaciRacunSortirano(&listaRacuna, r);
    }

    fclose(f);

    if (listaRacuna == NULL) {
        printf("Nema uèitanih raèuna. Provjerite sadržaj datoteke racuni.txt i raèuna.\n");
        return 0;
    }

    meni(listaRacuna);

    // Osiguravamo da se memorija oslobodi pri izlasku
    if (listaRacuna) {
        oslobodiRacune(listaRacuna);
    }

    return 0;
}