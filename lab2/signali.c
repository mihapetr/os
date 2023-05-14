// done
/* Operacije nad signalima */

#include <stdio.h>
#include <signal.h>
#include <stdlib.h>
#include <unistd.h>
#include "signali.h"
#include "datoteke.h"

/* implementacija funkcija, vec opisanih u lab1 + postavi_signale() */

void postavi_signale() {

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
