Skica rješenja i neki komentari
---------------------------------
Obzirom da je (prošlih godina) bilo svakakvih rješenja za lab3 u nastavku se nalazi skica jednog jednostavnog rješenja te nekoliko dodatnih komentara.

dretva kontrola_broja_dretvi {
	ponavljaj {
		x = dohvati_novi_broj_dretvi()
		ako je x unutar granica 1-100 tada {
			uđi_u_monitor(m)
			postavljeni_broj_dretvi = x
			dok je broj_dretvi < postavljeni_broj_dretvi radi {
				stvori_novu_dretvu(obrada)
				broj_dretvi++
			}
			izađi_iz_monitora(m)
		}
	}
}

dretva obrada() {
	uđi_u_monitor(m)
	dok nije označen kraj rada (ili zauvijek) radi
	{
		ako je broj_dretvi > postavljeni_broj_dretvi tada
			broj_dretvi--
			izađi_iz_monitora(m)
			završi_ovu_dretvu (pthread_exit ili return NULL)
		}

		moj_broj = broj
		broj++
		izađi_iz_monitora(m)

		x = obrada(moj_broj)
		simuliraj trajanje obrade (npr. sleep(5))

		uđi_u_monitor(m)
		dodaj x u obrada.txt
	}
	izađi_iz_monitora(m)
}

Što se tiče opisnika dretvi:
- nije potrebno pamtiti opisnike za radne dretve
- ako se baš želi, onda:
  * unaprijed rezervirati memoriju za maksimalan broj opisnika (pthread_t op[MAX])
  * pamtiti koji su opisnici aktivni s dodatnim poljem (int id[MAX])
  * inicijalizirati ga s nulama
  * svakoj radnoj dretvi dati jedan broj, npr. na početku:
	id[i] = i;
	pthread_create(&op[id[i]], NULL, obrada, &id[i])
  * kad dretva (void *obrada(void *p)) treba završiti s radom (jer ih ima previše)
	*((int*) p) = 0; //stavimo nulu na to mjesto u polju id
  * pri stvaranju dodatnih dretvi prolazi se polje i traži id[x] == 0,
    pronađeni element se iskoristi za novu dretvu

- dodatno (informativno): dretve na koje se ne čeka da završe bi trebalo posebno označiti
  (oznaka PTHREAD_CREATE_DETACHED pri stvaranju nove dretve)
  pogledati primjer na:
  http://www.zemris.fer.hr/~leonardo/os/fer/dodatni-materijali/primjeri-koda/03-semafori-i-monitori/monitori/stari_most.c


Većina studenata je limitirala paralelnost u radu, tako da samo jedna dretva radi.
To nije traženo niti poželjno!
Problem koji se može pojaviti (ali koji "ne smeta" u ovoj vježbi) jest da brojevi koji se dodaju u obrada.txt nisu po redu - manji broj može doći nakon većeg.
To se može rješiti na dva načina. Prvi je "varanjem" i zapisom broja u datoteku odmah nakon uzimanja a prije simulacije obrade:
		x = obrada(broj)
		broj++
		dodaj x u obrada.txt
		izađi_iz_monitora(m)
		simuliraj trajanje obrade (npr. sleep(5))
		uđi_u_monitor(m)

Drugo, bolje rješenje je paralelni rad uz osiguranje redoslijeda dodavanja brojeva. Međutim, to nije jednostavno i zahtjeva dodatno polje u koje bi svaka dretva stavila svoj broj koji obrađuje. Prije zapisa, svaka bi dretva trebala biti sigurna da je njen broj najmanji u tom polju (slično Lamportovu algoritmu).
Npr.
		moj_broj[moj_id] = broj //moj_id iz argumenta funkcije
		broj++
		izađi_iz_monitora(m)

		x = obrada(moj_broj[moj_id])
		simuliraj trajanje obrade (npr. sleep(5))

		uđi_u_monitor(m)
		za i = 0 do MAX_ID radi
			ako je moj_broj[i] > 0 I moj_broj[i] < moj_broj[moj_id] onda
				Čekaj_u_redu_uvjeta(red_za_upis, m)
		dodaj x u obrada.txt
		moj_broj[moj_id] = 0
		Oslobodi_sve_iz_reda_uvjeta(red_za_upis)

