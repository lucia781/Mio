#include <stdio.h>
#include <stdlib.h>
#include <time.h>


enum Stato_giocatore {astronauta, impostore, assassinato, defenestrato};
enum Nome_giocatore {rosso, blu, nero, bianco, verde, rosa, giallo, viola, lime, cyan};
enum Tipo_stanza {vuota, quest_semplice, quest_complicata, botola};

struct Stanza{
    struct Stanza *avanti;
    struct Stanza *destra;
    struct Stanza *sinistra;
    struct Stanza *stanza_precedente;
    struct Stanza *prossimo;                      //puntatore che punta alla stanza successiva
    enum Tipo_stanza stanza;
    unsigned short int emergenza_chiamata;
  };
struct Giocatore {
  struct Stanza *posizione;
  enum Stato_giocatore stato;
  enum Nome_giocatore nome;
};

void imposta_gioco();
void gioca();
void termina_gioco();
