// done
/* Operacije nad datotekama */

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include "datoteke.h"

/* implementacija funkcija, vec opisanih u lab1 + dohvati_iz_cijevi */

int procitaj_status()
{
	int broj;
	FILE *fp;

	fp = fopen(dat_status, "r");
	if (!fp) {
		printf("Ne mogu otvoriti %s\n", dat_status);
		exit(1);
	}
	if (fscanf(fp, "%d", &broj) != 1) {
		printf("Nije procitan broj iz %s!\n", dat_status);
		exit(1);
	}
	fclose(fp);
	return broj;
}

void zapisi_status(int broj)
{
	FILE *fp;

	fp = fopen(dat_status, "w");
	if (!fp) {
		printf("Ne mogu otvoriti %s\n", dat_status);
		exit(1);
	}
	if (fprintf(fp, "%d\n", broj) < 1) {
		printf("Nije upisan broj u %s!\n", dat_status);
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

// nova funkcija za dohvaćanje iz cijevi
// vraća -1 ako nije zadovoljen uvijet da je unseno
	// nešto veće od nula
int dohvati_iz_cijevi()
{
	FILE *fp;
	int x, y = -1; 

	fp = fopen(dat_mreza, "r"); // dat_mreza je naziv varijable s "datotekom" cijevi
	if (!fp) {
		fprintf(stderr, "Ne mogu otvoriti cijev %s\n", dat_mreza);
		exit(1);
	}
	if (fscanf(fp, "%d", &x) > 0)
		y = x;
	fclose(fp);

	return y;
}
