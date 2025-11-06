/*5. Napisati program koji iz datoteke èita postfiks izraz i zatim korištenjem stoga raèuna
rezultat. Stog je potrebno realizirati preko vezane liste.
*/
 
//1.) postfix head = { number = 0;next = NULL };


/*2.) otvroi dat;
bufferi
zatvori datoteku*/

/*3.) a) broj? --> Position newEl=NULL;
--> alociraj prostor + inicijalizacija
	newEl->next=head->next;
	head->next=newEl;

b) matematicki simbol? --> first= head->next;
							second=first->next;
							first->number= first->number,second->number, "&c"; (7=3,4,+)
							pop(&first,&second);
							first->next=second->next;
							second->next=NULL;
							free(second);

c)trash podatak?-->
push(ubacuje element na vrh, treba koristiti &head i dodani broj)
pop(izbacuje zadnji element koji je usao, koristi &first, &second)*/

#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#define FILE_NOT_OPENED -1

typedef struct _postfix* Position;
typedef struct _postfix {
    double number;     
    Position next;     
} postfix;

int push(Position, double);
int pop(double*, Position);

int push(Position head, double broj) { //gura novi element na vrh stoga
    Position newEl = NULL;                  
    newEl = (Position)malloc(sizeof(postfix));   // alocira se memorija za novi èvor
    if (newEl == NULL) {                         // provjera alokacije
        printf("Neuspjela alokacija memorije!\n");
        return FILE_NOT_OPENED;
    }

    newEl->number = broj;        // upisujemo vrijednost broja u novi èvor
    newEl->next = head->next;    // novi èvor pokazuje na dosadašnji prvi element
    head->next = newEl;          // glava sada pokazuje na novi èvor (koji je postao vrh stoga)
    return 0;
}


int pop(double* rezultat, Position head) { //skida element s vrha stoga
    Position first = NULL;

    if (head->next == NULL) {   
        printf("Stog je prazan!\n");
        return FILE_NOT_OPENED; 
    }

    first = head->next;            // uzmi prvi element (vrh stoga)
    *rezultat = first->number;     // spremi njegovu vrijednost u rezultat
    head->next = first->next;      // prespoji glavu na sljedeæi element (skidanje s vrha)
    free(first);                   

    return 0;
}

int main() {

    postfix head = { .number = 0, .next = NULL }; //inicijaliziram pocetni element

    FILE* dat = fopen("postfix.txt", "r");
    if (!dat) {
        printf("Greska pri otvaranju datoteke!\n");
        return FILE_NOT_OPENED; 
    }

    char buffer[128] = { 0 };   // privremeni spremnik za svaku rijec/simbol iz datoteke

    // 3. Èitanje sadržaja datoteke, token po token (rijeè po rijeè)
    while (fscanf(dat, " %s", buffer) == 1) {

        // a) ako je proèitan broj
        double broj = 0;
        if (sscanf(buffer, "%lf", &broj) == 1) {
            // dodaj broj na stog
            push(&head, broj);
        }

        // b) ako je proèitan matematièki operator (+, -, *, /)
        else if (buffer[0] == '+' || buffer[0] == '-' || buffer[0] == '*' || buffer[0] == '/') {

            Position first = head.next;    // prvi operand (zadnji ubaèeni broj)
            Position second = NULL;        // drugi operand

           
            if (first == NULL || first->next == NULL) {
                printf("Nedovoljno operanada na stogu!\n");
                fclose(dat);
                return FILE_NOT_OPENED; 
            }

            second = first->next;          // drugi operand je odmah ispod prvog

            // Izvrši odgovarajuæu matematièku operaciju
            switch (buffer[0]) {
            case '+':
                first->number = second->number + first->number;
                break;
            case '-':
                first->number = second->number - first->number;
                break;
            case '*':
                first->number = second->number * first->number;
                break;
            case '/':
                if (first->number == 0) {  
                    printf("Dijeljenje s nulom!\n");
                    fclose(dat);
                    return FILE_NOT_OPENED; 
                }
                first->number = second->number / first->number;
                break;
            default:
                break;
            }

            // seecond je iskoristen pa ga uklanjam
            first->next = second->next;    // prespajanje pokazivaèa
            second->next = NULL;
            free(second);                  // oslobaðanje memorije
        }

        // c) ako je proèitan nepoznat podatak
        else {
            printf("Nepoznat podatak: %s\n", buffer);
        }
    }

    fclose(dat); 

    if (head.next != NULL)
        printf("Rezultat: %.2lf\n", head.next->number);
    else
        printf("Stog je prazan!\n");

    return 0;
}

