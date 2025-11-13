/*napravi datoteke s polinomima (2), npr. 4x^(2)+3x-2 --> 4 2 + 3 1 - 2 0
											2-4x^(3)+2x+8x^(4) --> 2 0 - 4 3 + 2 1 + 8 4

1.) typedef struct _poly* Position;
    typedef struct _poly{
	   int coeff;
	   int pow;
	   Position next;
	   }poly;
	
	poly Head = {.coeff=0,.pow=0, .next=NULL};
	firstPolynom=head;
	secondPolynom=head;
	
2.) - otvori datoteku
    - pohrani podatke [4,2,3,1,-2,0]
	- zatvori datoteku
	
3.) - alokacija newEl
    prevEl=&firstPolynom;
	q=prevEl-->next;
	
	a) if(q == NULL) (ako je q null, trebamo dodat element na previous)
	   newEl-->next = prevEl-->next;
	   prevEl-->next = newEl;
	   q = prevEl-->next;
	   
	b) if(q-->pow > newEl-->pow)
	   prevEl = q;
	   q = prevEl-->next;

4.) - alokacija newEl
    prevEl=&secondPolynom;
	q=prevEl-->next;
	   
	c) if( q-->pow < newEl-->pow)
	   newEl-->next = q;
	   prevEl-->next = newEl;
	   
	d) za zbrajanje
	   q-->coeff += p-->coeff;
	 
	 e) za brisanje
	    prevEl-->next = q-->next;
		q-->next = NULL;
		free(q);

	 f) iniciraj newEl
	    first-->coeff * second-->coeff;
		first-->pow + second-->pow;
		*/
    
#define _CRT_SECURE_NO_WARNINGS   
#include <stdio.h>                
#include <stdlib.h>               
#define FILE_NOT_OPENED -1        


typedef struct _poly* Position;   
typedef struct _poly {
    int coeff;                    
    int pow;                      
    Position next;                
} poly;


int ReadFromFile(Position, char*);
int PrintPoly(Position);
int AddPoly(Position, Position, Position);
int MultiplyPoly(Position, Position, Position);
Position createNewEl(int, int);
int InsertSorted(Position, Position);

int main() {
    // kreiramo cetiri glave lista (prazne polinome)
    poly firstHead = { .coeff = 0, .pow = 0, .next = NULL };   
    poly secondHead = { .coeff = 0, .pow = 0, .next = NULL };   
    poly sumHead = { .coeff = 0, .pow = 0, .next = NULL };      
    poly productHead = { .coeff = 0, .pow = 0, .next = NULL };  

    ReadFromFile(&firstHead, "suma1.txt");
    ReadFromFile(&secondHead, "suma2.txt");
	
    printf("Prvi polinom: ");
    PrintPoly(&firstHead);
 
    printf("Drugi polinom: ");
    PrintPoly(&secondHead);

    AddPoly(&sumHead, &firstHead, &secondHead);
    printf("Suma polinoma je: ");
    PrintPoly(&sumHead);

    MultiplyPoly(&productHead, &firstHead, &secondHead);
    printf("Umnozak polinoma je: ");
    PrintPoly(&productHead);

    return 0;  
}


int ReadFromFile(Position head, char* fileName) {
    FILE* fp = fopen(fileName, "r");     
    if (!fp) {                            
        printf("Greska pri otvaranju datoteke %s!\n", fileName);
        return FILE_NOT_OPENED;           
    }

    int coeff, pow;                       
    while (fscanf(fp, "%d %d", &coeff, &pow) == 2) {  // citamo parove brojeva dok ih ima
        if (coeff != 0) {                 
            Position newEl = createNewEl(coeff, pow);  // kreiramo novi clan
            InsertSorted(head, newEl);                 // ubacujemo ga u sortiran redoslijed
        }
    }

    fclose(fp);                           
    return 0;                            
}


int PrintPoly(Position head) {
    Position p = head->next;              // pocinjem od prvog pravog clana (preskacemo glavu)

    if (p == NULL) {                      
        printf("0\n");                    
        return 0;
    }

    while (p != NULL) {                   
        if (p->coeff > 0 && p != head->next)
            printf("+");                  // dodajemo plus ispred pozitivnih clanova osim prvog

        if (p->pow == 0)
            printf("%d", p->coeff);       
        else if (p->pow == 1)
            printf("%dx", p->coeff);      
        else
            printf("%dx^%d", p->coeff, p->pow); 

        printf(" ");                      
        p = p->next;                      // prelazimo na sljedeci clan
    }
    printf("\n");                         
    return 0;
}


Position createNewEl(int coeff, int pow) {
    Position newEl = (Position)malloc(sizeof(poly));  
    if (!newEl) {                                   
        printf("Greska u alokaciji memorije!\n");
        return FILE_NOT_OPENED;                      
    }
    newEl->coeff = coeff;                            
    newEl->pow = pow;                                
    newEl->next = NULL;                              // novi clan jos nije povezan
    return newEl;                                    // vracamo pokazivac na novi clan
}

int InsertSorted(Position head, Position newEl) {
    Position prev = head;             // prethodni clan
    Position curr = head->next;       // trenutni clan (odmah iza glave)

    // prolazimo dok eksponent trenutnog clana nije manji od novog
    while (curr != NULL && curr->pow > newEl->pow) {
        prev = curr;                  // pomicem pokazivace
        curr = curr->next;
    }

    // ako postoji clan sa istim eksponentom
    if (curr != NULL && curr->pow == newEl->pow) {
        curr->coeff += newEl->coeff;  // zbrajam koeficijente
        free(newEl);                  

        if (curr->coeff == 0) {
            prev->next = curr->next;
            free(curr);
        }
        return 0;
    }

    // ako nema istog eksponenta, ubacujemo novi clan izmedju prev i curr
    newEl->next = curr;
    prev->next = newEl;

    return 0;
}


int AddPoly(Position sum, Position p1, Position p2) {
    Position a = p1->next;    
    Position b = p2->next;    
    Position temp = sum;      

    while (a != NULL && b != NULL) {        // dok oba polinoma imaju clanove
        if (a->pow > b->pow) {              
            temp->next = createNewEl(a->coeff, a->pow);
            a = a->next;
        }
        else if (a->pow < b->pow) {         
            temp->next = createNewEl(b->coeff, b->pow);
            b = b->next;
        }
        else {                             
            int c = a->coeff + b->coeff;    // zbroji koeficijente
            if (c != 0)
                temp->next = createNewEl(c, a->pow); // kreiraj novi clan ako nije 0
            a = a->next;
            b = b->next;
        }
        if (temp->next != NULL)
            temp = temp->next;        
    }

    // ako su ostali clanovi u prvom polinomu
    while (a != NULL) {
        temp->next = createNewEl(a->coeff, a->pow);
        temp = temp->next;
        a = a->next;
    }

    // ako su ostali clanovi u drugom polinomu
    while (b != NULL) {
        temp->next = createNewEl(b->coeff, b->pow);
        temp = temp->next;
        b = b->next;
    }

    return 0;
}


int MultiplyPoly(Position result, Position p1, Position p2) {
    Position a = p1->next;                    
    while (a != NULL) {                       // prolaz kroz clanove prvog polinoma
        Position b = p2->next;                
        while (b != NULL) {                   // prolaz kroz clanove drugog polinoma
            Position newEl = createNewEl(a->coeff * b->coeff, a->pow + b->pow); // mnozimo clanove
            InsertSorted(result, newEl);      // ubacujem rezultat u sortiran redoslijed
            b = b->next;                      // sljedeci clan drugog polinoma
        }
        a = a->next;                          // sljedeci clan prvog polinoma
    }
    return 0;
}


