/* Primjer rada s datotekama */

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <math.h>
#include <signal.h>

char *dat_stat, *dat_obrada;
int broj = 0;

/* funkcije za rad s datotekama */
int procitaj_status();
void zapisi_status(int broj);
void dodaj_broj(int broj);
int pronadji_zadnji_broj();

/*dodatak rješenju*/
void maskiraj_signale();
void obradi_dogadjaj(int sig);
void obradi_sigterm(int sig);
void obradi_sigint(int sig);

int main(int argc, char *argv[])
{
	int i, x;
	if (argc < 3) {
		printf("Koristenje: %s <status-datoteka> <datoteka-s-podacim>",
			argv[0]);
		return 1;
	}
	dat_stat = argv[1];
	dat_obrada = argv[2];

    maskiraj_signale();

	/* simulacija rada */
	/* pocetak */
	broj = procitaj_status();
	if (broj == 0) {
		printf("program bio prekinut, potraga za brojem...\n");
		broj = pronadji_zadnji_broj();
		broj = (int) sqrt(broj);
	}
	zapisi_status(0); //radim
	printf("krecem s radom, PID=%ld, zadnji broj=%d\n", (long) getpid(), broj);

	//simulacija obrade
	for (i = 0; i < 100; i++) {
		broj++;
		x = broj * broj;
		dodaj_broj(x);
		sleep(5);
	}

	//kraj
	zapisi_status(broj);
	printf("kraj rada, zadnji broj=%d\n", broj);

	return 0;
}

void maskiraj_signale() {

    struct sigaction act;

    // maskiranje korisnickog signala
    act.sa_handler = obradi_dogadjaj;
    sigemptyset(&act.sa_mask);
    sigaddset(&act.sa_mask, SIGTERM);
    sigaddset(&act.sa_mask, SIGINT);
    act.sa_flags = 0;
    sigaction(SIGUSR1, &act, NULL);

    // maskiranje terminate signala
    // buduci da je duzi zadatak ignorira ostale signale
    act.sa_handler = obradi_sigterm;
    sigemptyset(&act.sa_mask);
    sigaddset(&act.sa_mask, SIGINT);
    sigaddset(&act.sa_mask, SIGUSR1);
    act.sa_flags = 0;
    sigaction(SIGTERM, &act, NULL);

    // maskiranje interrupt signala
    act.sa_handler = obradi_sigint;
	sigaction(SIGINT, &act, NULL);
}

void obradi_dogadjaj(int sig) {
    printf("Trenutni broj je %d.\n", broj);
}

void obradi_sigterm(int sig) {
    printf("Zapisujem broj u status i izlazim.\n");
    zapisi_status(broj);
    exit(1);
}

void obradi_sigint(int sig) {
    printf("Izlazim zbog prekida sa sigint.\n");
    exit(2);
}

int procitaj_status()
{
	int broj;
	FILE *fp;

	fp = fopen(dat_stat, "r");
	if (!fp) {
		printf("Ne mogu otvoriti %s\n", dat_stat);
		exit(1);
	}
	if (fscanf(fp, "%d", &broj) != 1) {
		printf("Nije procitan broj iz %s!\n", dat_stat);
		exit(1);
	}
	fclose(fp);
	return broj;
}

void zapisi_status(int broj)
{
	FILE *fp;

	fp = fopen(dat_stat, "w");
	if (!fp) {
		printf("Ne mogu otvoriti %s\n", dat_stat);
		exit(1);
	}
	if (fprintf(fp, "%d\n", broj) < 1) {
		printf("Nije upisan broj u %s!\n", dat_stat);
		exit(1);
	}
	fclose(fp);
}

void dodaj_broj(int broj)
{
	FILE *fp;

	fp = fopen(dat_obrada, "a");
	if (!fp) {
		printf("Ne mogu otvoriti %s\n", dat_obrada);
		exit(1);
	}
	if (fprintf(fp, "%d\n", broj) < 1) {
		printf("Nije upisan broj u %s!\n", dat_obrada);
		exit(1);
	}
	fclose(fp);
}

int pronadji_zadnji_broj()
{
	FILE *fp;
	int broj = -1;

	fp = fopen(dat_obrada, "r");
	if (!fp) {
		printf("Ne mogu otvoriti %s\n", dat_obrada);
		exit(1);
	}
	while(!feof(fp))
		if (fscanf(fp, "%d", &broj) != 1)
			break;
	fclose(fp);
	return broj;
}
