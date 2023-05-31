/* Glavna datoteka za lab2 s funkcijom main */

#include <stdio.h>
#include <pthread.h>
#include <signal.h>
#include <stdlib.h>
#include <unistd.h>
#include <math.h>
#include "signali.h"
#include "datoteke.h"

char *dat_status, *dat_obrada, *dat_mreza;
int broj = 0;
int nije_kraj = 1;

/* funkcije koje rade dretve */
void *obrada(void *);
void *mreza(void *);

int main(int argc, char *argv[])
{
	if (argc < 4) {
		fprintf(stderr, "Koristenje: %s <status-datoteka> "
		 "<datoteka-s-podacima> <cjevovod>\n", argv[0]);
		exit(1);
	}
	dat_status = argv[1];
	dat_obrada = argv[2];
	dat_mreza = argv[3]; // naziv za cijevnu datoteku

	postavi_signale();
	
	/* simulacija rada */
	/* pocetak : dohvati prvi broj i zapisi 0 u status */
	broj = procitaj_status();
	if (broj == 0) {
		printf("program bio prekinut, potraga za brojem...\n");
		broj = pronadji_zadnji_broj();
		broj = (int) sqrt(broj);
	}
	zapisi_status(0); //radim
	printf("krecem s radom, PID=%ld, zadnji broj=%d\n", (long) getpid(), broj);

	/* stvori dretve: radnu i mreznu */
	pthread_t radna_o, mrezna_o;
	if(
		pthread_create(&radna_o, NULL, obrada, NULL) ||
		pthread_create(&mrezna_o, NULL, mreza, NULL)
	) {
		printf("Ne mogu stvoriti neku od dretvi.\n");
        exit(1);
	}
	
	// glavna dretva sluša ulaz s konzole
	int konzola_br;
	while(nije_kraj) {
		//procitaj broj iz konzole, npr. sa scanf
		//ako je > 0 onda ga postavi kao broj
		//inace prekini s radom => postavi nije_kraj = 0
		scanf("%d", &konzola_br);
		if(konzola_br > 0) {
			dodaj_broj(konzola_br);
		}
		else {
			nije_kraj = 0;
		}
	}

	printf("Cekam kraj obrade.\n");
	//cekaj na kraj dretve obrada, ali ne i one druge mrezne
	pthread_join(radna_o, NULL);

	return 0;
}

void *obrada(void *p)
{
	int broj, x;

	for (int i = 0; i < 100; i++) {
		// moguća izvanredna promjena preko mreže ili konzole
		// pa je potrebno pročitati zadnji broj
		broj = pronadji_zadnji_broj();
		printf("Pronasao sam broj %d\n", broj);
		broj = (int) sqrt(broj);
		broj++;
		x = broj * broj;
		dodaj_broj(x);
		sleep(5);
	}

	return NULL;
}

void *mreza(void *p)
{
	//u petlji cekaj da se nesto pojavi u cijevi
	//ako je procitano > 0 onda ga postavi u broj
	int cijev_br;
	for(int i=0; i<10; i++) {
		cijev_br = dohvati_iz_cijevi();
		if(cijev_br != -1) {
			dodaj_broj(cijev_br);
			printf("Dodao broj %d u datoreku obrada.txt\n", cijev_br);
		}
	}

	return NULL;
}
