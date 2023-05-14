/*
Obrađuje se tako da dretva prilikom čitanja datoteke obrada.txt napiše u konzolu
"Pronasao sam broj x".
*/

make pokreni				// nareda

// output

cc -Wall -MMD   -c -o lab2.o lab2.c
cc -Wall -MMD   -c -o signali.o signali.c
cc -Wall -MMD   -c -o datoteke.o datoteke.c
cc -O lab2.o signali.o datoteke.o -lm -lpthread -o lab2
krecem s radom, PID=151, zadnji broj=1
Pronasao sam broj 1
Pronasao sam broj 4
Pronasao sam broj 9
Pronasao sam broj 16
4					// uneseno preko tipkovnice
Pronasao sam broj 4
Pronasao sam broj 9
Pronasao sam broj 16
Pronasao sam broj 25
Pronasao sam broj 36
Pronasao sam broj 49
Pronasao sam broj 64
Pronasao sam broj 81
Pronasao sam broj 100
Pronasao sam broj 121
Pronasao sam broj 144
Pronasao sam broj 169
Dodao broj 25 u datoreku obrada.txt	// u drugom terminalu je pokrenuta naredba "echo 25 > cijev"
Pronasao sam broj 25
Pronasao sam broj 36
^CIzlazim zbog prekida sa sigint.
Makefile:26: recipe for target 'pokreni' failed
make: *** [pokreni] Error 2