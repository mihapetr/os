> make pokreni				// inicijalni broj radnih dretvi je 2 (hardcoded)

cc -Wall -MMD   -c -o lab3.o lab3.c
cc -Wall -MMD   -c -o signali.o signali.c
cc -Wall -MMD   -c -o datoteke.o datoteke.c
cc -O lab3.o signali.o datoteke.o -lm -lpthread -o lab3
krecem s radom, PID=638, zadnji broj=1
unesen broj 1, vrijeme = 5
unesen broj 4, vrijeme = 5
unesen broj 9, vrijeme = 10
unesen broj 16, vrijeme = 10
5					// glavna dretva regulira broj radnih dretvi
unesen broj 25, vrijeme = 15
unesen broj 36, vrijeme = 15
unesen broj 81, vrijeme = 16
unesen broj 64, vrijeme = 16
unesen broj 49, vrijeme = 16
unesen broj 100, vrijeme = 20
unesen broj 121, vrijeme = 20
unesen broj 144, vrijeme = 21
unesen broj 169, vrijeme = 21
unesen broj 196, vrijeme = 21
1					// ovdje samo jedna dretva radi obradu
unesen broj 225, vrijeme = 25
unesen broj 256, vrijeme = 25
unesen broj 289, vrijeme = 26
unesen broj 324, vrijeme = 26
unesen broj 361, vrijeme = 26
unesen broj 400, vrijeme = 31
mreža : broj = 2			// utjecaj na broj iz druge konzole
unesen broj 441, vrijeme = 36
unesen broj 4, vrijeme = 41
unesen broj 9, vrijeme = 46
^CIzlazim zbog prekida sa sigint.
Makefile:26: recipe for target 'pokreni' failed
make: *** [pokreni] Error 2