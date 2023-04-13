// prekidanje sa SIGINT : ctrl + c
make pokreni
cc program.c -Wall -O -lm -o simulacija
krecem s radom, PID=337, zadnji broj=1
^CIzlazim zbog prekida sa sigint.
Makefile:22: recipe for target 'pokreni' failed
make: *** [pokreni] Error 2

// pokretanje nakon prekida sa SIGINT, prekidanje sa SIGTERM
make pokreni
program bio prekinut, potraga za brojem...
krecem s radom, PID=341, zadnji broj=4
Zapisujem broj u status i izlazim.
Makefile:22: recipe for target 'pokreni' failed
make: *** [pokreni] Error 1

// pokretanje nakon prekida sa SIGTERM, slanje SIGUSR1 signala, prekid sa SIGINT
make pokreni
krecem s radom, PID=345, zadnji broj=7
Trenutni broj je 11.
^CIzlazim zbog prekida sa sigint.
Makefile:22: recipe for target 'pokreni' failed
make: *** [pokreni] Error