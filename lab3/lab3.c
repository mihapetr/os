/* Glavna datoteka za lab2 s funkcijom main */

#include <stdio.h>
#include <pthread.h>
#include <signal.h>
#include <stdlib.h>
#include <unistd.h>
#include <math.h>
#include <time.h>
#include "signali.h"
#include "datoteke.h"

// varijable za obradu
char *dat_status, *dat_obrada, *dat_mreza;
int broj = 0;
int nije_kraj = 1;

// vrijeme
time_t pocetak;

// varijable za broj dretvi
int broj_dretvi = 2;	// inicijalno
int postavljeni_broj_dretvi = 0;

// monitori i redovi
pthread_mutex_t m;  // monitor
pthread_t nebitno;	// zanemaruju se opisnici radnih dretvi


/* funkcije koje rade dretve */
void *obrada(void *);
void *mreza(void *);

int main(int argc, char *argv[])
{
	// inicijalizacija monitora
	pthread_mutex_init (&m, NULL);

	// vrijeme pocetka
	pocetak = time(NULL);

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

	/* stvori dretve: mreznu, pocetni broj radnih */
	pthread_t mrezna_o;
	int greska = 0;

	pthread_mutex_lock(&m);

	for(; postavljeni_broj_dretvi < broj_dretvi; postavljeni_broj_dretvi++) {
		// (opisnik, opcije, funkcija, argument)
		// ne pamti se opisnik (ide u varijablu nebitno)
		greska += pthread_create(&nebitno, NULL, obrada, NULL);
	}
	// mrezna dretva
	greska += pthread_create(&mrezna_o, NULL, mreza, NULL);

	pthread_mutex_unlock(&m);

	if(greska) {
		printf("Ne mogu stvoriti neku dretvu.\n");
        exit(1);
	}
	
	// glavna dretva je jedna od kontrolnih;
	// sluša ulaz s konzole i definira broj_dretvi tijekom izvođenja
	int konzola_br;
	while(nije_kraj) {
		// procitaj broj iz konzole, npr. sa scanf
		scanf("%d", &konzola_br);
		if(konzola_br >= 1 && konzola_br <= 100) {
			pthread_mutex_lock(&m);
			postavljeni_broj_dretvi = konzola_br;
			for(; broj_dretvi < postavljeni_broj_dretvi; broj_dretvi++) {
				// (opisnik, opcije, funkcija, argument)
				// ne pamti se opisnik
				pthread_create(&nebitno, NULL, obrada, NULL);
			}
			pthread_mutex_unlock(&m);
		}
	}

	return 0;
}


void *obrada(void *p)
{	
	pthread_mutex_lock(&m);

	// maksimalna trajnost radne dretve je 100 obrada
	for (int i = 0; i < 100; i++) {
		// ako je previše dretvi, završi ovu
		if(broj_dretvi > postavljeni_broj_dretvi) {
			broj_dretvi --;
			pthread_mutex_unlock(&m);
			pthread_exit(NULL);
		}

		int moj_broj = broj;
		broj++;

		pthread_mutex_unlock(&m);

		moj_broj = moj_broj * moj_broj;
		// simulacija obrade
		sleep(5);

		pthread_mutex_lock(&m);
		dodaj_broj(moj_broj);
		printf("unesen broj %d, vrijeme = %d\n", moj_broj, (int)(time(NULL) - pocetak));		
	}

	pthread_mutex_unlock(&m);

	return NULL;
}

// sluša kroz cijev unos korisnika i utječe na obradu
void *mreza(void *p)
{
	// u petlji cekaj da se nesto pojavi u cijevi
	// ako je procitano > 0 onda ga postavi u broj
	int cijev_br;
	for(int i=0; i<10; i++) {

		cijev_br = dohvati_iz_cijevi();
		if(cijev_br != -1) {
			// ovako dretva utječe na zajednički broj
			pthread_mutex_lock(&m);
			broj = cijev_br;
			pthread_mutex_unlock(&m);

			printf("mreža : broj = %d\n", cijev_br);
		}
	}

	return NULL;
}
