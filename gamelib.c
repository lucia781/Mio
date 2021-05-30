#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "gamelib.h"

//elenco delle vaiabili globali
static struct Stanza *stanza_inizio;                        //prima stanza
static struct Stanza *lista_stanze;                         //lista delle stanze create
static struct Stanza *stanze_botola;                        //lista delle stanze con la botola
static struct Stanza *stanza_destra;                        //lista delle nuove tanze create a destra
static struct Stanza *stanza_sinistra;                      //lista delle nuove tanze create a sinistra
static struct Stanza *stanza_avanti;                        //lista delle nuove tanze create in avanti
static struct Giocatore *giocatori;                         //array di giocatori
static struct Giocatore *g_disponibili;                     //array di giocatori che potrebbero essere uccisi
static struct Giocatore *g_rischio;                         //array di giocatori che potrebbero essere defenestrati
static int n_giocatori = 0;                                 //numero di giocatori
static unsigned short int quest_da_finire = 0;              //tiene il conto delle quest che mancano per vincere il gioco
static unsigned short int n_quest;                          //numero di quest da effettuare per vincere il gioco
static unsigned short int n_botola = 0;                     //numero di stanze create con la botola
static unsigned short int stato_imposta_gioco = 0;          //tiene conto se è stato gia impostato il gioco
static unsigned short int n_astronauti = 0;                 //numero di astronauti
static unsigned short int n_impostori = 0;                  //numero di impostori
static unsigned short int ucciso = 11;                      //indice del giocatore che verrà assassinato
static unsigned short int n_uccisi = 0;                     //numero dei giocatori uccisi
static unsigned short int emergenza_chiamata = 0;           //tiene conto se è stato gia impostato il gioco
static unsigned short int n_stanze = 0;                     //tiene conto del numero di stanze create

//elenco delle funzioni
void mischia_array(int arr[],int n);                                                //mischia l'array
static const char *colore_giocatore(unsigned short int n);                          //definisce il colore a ogni giocatore
static const char *stato_giocatore(unsigned short int n);                           //definisce lo stato a ogni giocatore
static const char *tipo_stanza(unsigned short int n);                               //definisce il tipo alla stanza
static void stampa_giocatori(struct Giocatore *giocatori, int n);                   //stampa i giocatori
static int inizia_gioco();                                                          //termina la funzione imposta_gioco e apre la funzione gioca
static char *stato_turni(unsigned short int t);                                     //definisce i turni dei giocatori
static void esegui_quest(int n);                                                    //fa eseguire la quest al giocatore
static void chiamata_emergenza(int n);                                              //esegue la chiamata d'emergenza
static int uccidi_astronauta(int t);                                                //uccide il giocatore scelto dall'impostore
static void usa_botola(int m);                                                      //permette all'impostore di spostarsi tramite le botole
static void sabotaggio(int m);                                                      //permette di cambiare lo stato della stanza
static void crea_stanza(struct Stanza *nuova_stanza, struct Stanza *g_posizione);   //crea una nuova stanza
static void avanza(struct Giocatore *giocatore, struct Stanza *g_posizione);        //permette di far avanzare il giocatore
static void clrscr();                                                               //cancella le scelte fatte dagli altri giocatori

void imposta_gioco(){
  if (stato_imposta_gioco != 0) {     //controlla se è stato già impostato il gioco
    printf("Il gioco è stato già impostato in precedenza. Ora verranno resettatele scelte precenti.\n");
    termina_gioco();                 //se è stato gia impostato viene chiamata la funzione termina_gioco
  }
  do {
    printf("\n%s\n", "Inserisci il numero di giocatori.\nIl numero minimo di giocatori è 4 ed il massimo è 10.");
    scanf("%d",&n_giocatori);
    while(getchar() != '\n');
    if (!(n_giocatori >= 4 && n_giocatori <= 10)){          //controlla se il numero dei giocatori è compreso tra 4 e 10
      printf("%s\n", "Numero gicatori non corretto.");
    }
  }while(!(n_giocatori >= 4 && n_giocatori <= 10));
  giocatori = (struct Giocatore*) calloc(n_giocatori, sizeof(struct Giocatore));   //alloca l'array di giocatori
  int colori[n_giocatori];            //inizializza l'array di colori
  mischia_array(colori, n_giocatori);
  for (size_t i = 0; i < n_giocatori; i++) {
    giocatori[i].nome = colori[i];                  //assegna il colore a ogni giocatore
  }
  int token_1;
  token_1 = (rand() % 100) + 1;
  if (token_1 <= 60) {
    static int imp1;
    imp1 = rand() %n_giocatori;
    giocatori[imp1].stato = impostore;      //assegna lo stato di impostore
    n_impostori = 1;
    for (size_t i = 0; i < n_giocatori; i++) {
      if(giocatori[i].stato != giocatori[imp1].stato){
      giocatori[i].stato = astronauta;
      }
    }
  }else if (token_1 > 60 && token_1 <= 80){
    static int imp1, imp2;
    do {
      imp1 = rand() %n_giocatori;
      imp2 = rand() %n_giocatori;
    } while(imp1 == imp2);
    giocatori[imp1].stato = impostore;      //assegna lo stato di impostore
    giocatori[imp2].stato = impostore;
    n_impostori = 2;
    for (size_t i = 0; i < n_giocatori; i++) {
      if(giocatori[i].stato != giocatori[imp1].stato && giocatori[i].stato != giocatori[imp2].stato){
      giocatori[i].stato = astronauta;
      }
    }
  }else{
    static int imp1, imp2, imp3;
    do {
      imp1 = rand() %n_giocatori;
      imp2 = rand() %n_giocatori;
      imp3 = rand() %n_giocatori;
    } while(imp1 == imp2 || imp1 == imp3 || imp3 == imp2);
    giocatori[imp1].stato = impostore;      //assegna lo stato di impostore
    giocatori[imp2].stato = impostore;
    giocatori[imp3].stato = impostore;
    n_impostori = 3;
    for (size_t i = 0; i < n_giocatori; i++) {
      if(giocatori[i].stato != giocatori[imp1].stato && giocatori[i].stato != giocatori[imp2].stato && giocatori[i].stato != giocatori[imp3].stato){
      giocatori[i].stato = astronauta;
      }
    }
    n_impostori = 3;
  }
  n_astronauti = n_giocatori - n_impostori;
  do {
    printf("Inserisci un numero di quest da svolgere per vincere il gioco:\n");
    scanf("%hu", &n_quest);
    while(getchar() != '\n');
  } while(n_quest == 0);

  lista_stanze = malloc (sizeof(struct Stanza));        //alloca lista_stanze
  stanza_inizio = malloc (sizeof(struct Stanza));        //alloca stanza_inizio
  n_stanze += 1;
  stanza_inizio -> avanti = NULL;         //imposta il puntatore avanti
  stanza_inizio -> destra = NULL;         //imposta il puntatore detsra
  stanza_inizio -> sinistra = NULL;       //imposta il puntatore sinistra
  stanza_inizio -> stanza_precedente = NULL;  //imposta il puntatore stanza_precedente
  stanze_botola = malloc(sizeof(struct Stanza));
  int token = (rand() % 100) + 1;   //genera un numero per stabilire tipo di stanza
  if (token <= 30) {
    int token_2 = rand() % 2;
    if (token_2 == 0) {
      stanza_inizio -> stanza = quest_semplice;
    }if (token_2 == 1) {
      stanza_inizio -> stanza = vuota;
    }
  } else if (token <= 55) {
    stanza_inizio -> stanza = botola;
    n_botola += 1;
  }else{
    stanza_inizio -> stanza = quest_complicata;
  }
  printf("\ntipo stanza: %s\n", tipo_stanza(stanza_inizio -> stanza));
  lista_stanze -> prossimo = (struct Stanza *) malloc(sizeof(struct Stanza));       //crea il prossimo elemento
  lista_stanze -> prossimo = stanza_inizio;
  lista_stanze = lista_stanze -> prossimo;                    //collega il nuovo elemento
  lista_stanze -> prossimo = NULL;                            //il nuovo elelmento punta a NULL

  if (stanza_inizio -> stanza == botola) {
    stanza_botola -> prossimo = (struct Stanza *) malloc(sizeof(struct Stanza));
    stanza_botola -> prossimo = stanza_inizio;
    stanza_botola = stanza_botola -> prossimo;
    stanza_botola -> prossimo = NULL;
  }
  for (size_t i = 0; i < n_giocatori; i++) {
    giocatori[i].posizione = stanza_inizio;               //assegna come posizione la prima stanza
  }
  stampa_giocatori(giocatori, n_giocatori);
  inizia_gioco();
  printf("\nIl gioco è stato impostato con successo!\n");
  printf("                       !\n");
  printf("                       ~\n");
  printf("                      / \\\n");
  printf("                     /___\\\n");
  printf("                    |=   =|\n");
  printf("                    |     |\n");
  printf("                    |     |\n");
  printf("                    |     |\n");
  printf("                    |     |\n");
  printf("                    |     |\n");
  printf("                   /|##!##|\\\n");
  printf("                  / |##!##| \\\n");
  printf("                 /   |##!##|  \\\n");
  printf("                |  / ^ | ^  \\  |\n");
  printf("                | /  ( | )   \\ |\n");
  printf("                |/   ( | )    \\|\n");
  printf("                    ((   ))\n");
  printf("                     (( ))\n" );
  printf("                      ( )\n");
  printf("                       .\n", );
  return
}

void gioca(){
  if (stato_imposta_gioco == 0) {
    do {
      printf("%s\n", "E' necessario impostare prima il gioco.");
      return;
    } while(getchar() = '\n');
  }
  int j = 0;                            //indice del giocatore che sta giocando
  int turni[n_giocatori];               //array dei stato_turni
  mischia_array(turni, n_giocatori);
  int temp1 = -1;                       //variabile che memorizza l'indice dell'impostore
  int temp2 = -1;
  int temp3 = -1;
  for (size_t i = 0; i < n_giocatori; i++) {
    if (giocatori[i].stato == impostore) {
      if (temp1 == -1)
        temp1 = giocatori[i].nome;
      else if(temp2 == -1)
        temp2 = giocatori[i].nome;
      else if(temp3 == -1)
        temp3 = giocatori[i].nome;
    }
  }
  for (size_t i = 0; i < n_giocatori; i++) {
    giocator[i].nome = turni[i];                     //assegna il turno al giocatore
    if (turni[i] == temp1 || turni[i] == temp2 || turni[i] == temp3) {
      giocatori[i].nome = turni[i];
      giocatori[i].stato = impostore;         //riassegna lo stato al giocatore
    }else{
      giocatori[i].nome = turni[i];
      giocatori[i].stato = astronauta;        //riassegna lo stato al giocatore
    }
    printf("%s: %s\n", stato_turni(i), colore_giocatore(giocatori[i].nome));          //stampa il turno dei n_giocatori
  }

    while (n_impostori != 0 || n_astronauti == || n_quest != quest_da_finire || n_quest > quest_da_finire) {      //controlla che gli impostori non siano stati defenestrati o uccisi o che il numero di quest sia stato raggiunto
    if (n_quest <= quest_da_finire || n_quest == quest_da_finire || n_impostori == 0) {         //controllo per vedere che gli astronauti non abbaino già vinto
      printf("\n\n                        \033[1;97mGli astonauti hanno vinto il gioco!\033[0m\n\n");
      break;
    }
    if (n_astronauti == 0) {             //controllo per vedere che gli impostori non abbaino già vinto
      printf("\n\n                          \300[1;97mGli impostori hanno vinto il gioco!\300[0m\n\n");
      break;
    }
    if (giocatori[j].stato == impostore || giocatori[j].stato == astronauta) {        //controllo per far giocare solo i giocatori che non sono stati n_uccisi
      printf("\nE' il turno del giocatore %s\n", colore_giocatore(giocatori[j].nome));
      printf("Ti trovi in una stanza di tipo: %s\n", tipo_stanza((giocatori[j].posizione) -> stanza));
      if (ucciso != 11 && giocatori[ucciso].posizione == (giocatori[j].posizione && (giocatori[j].posizione) -> emergenza_chiamata == 0) {        //controllo per far effettuare la chiamata d'emergenzasolo ai giocatori vivi e dove non sia stata già effettuata
        printf("\nNella stanza è stato assassinato il giocatore %s\n\n",colore_giocatore(giocatori[ucciso].nome));
        int scelta = 0;
        if (giocatori[j].stato == astronauta) {           //controllo per far si che il giocatore sia un astronauta
          do {
            printf("Decidere l'azione da eseguire:\n");
            printf("1) avanza\n2) esegui quest\n3) chiamata d'emergenza\n");
            scanf("%d", &scelta);
            while (getchar() != '\n');
            switch (scelta) {
              case 1:
                avanza(&giocatori[j], giocatori[j].posizione);
                break;
              case 2:
                  if ((giocatori[j].posizione) -> stanza == quest_semplice || (giocatori[j].posizione) -> stanza == quest_complicata) {     //controllo per vedere se la stanza è una stanza di tipo quest semplice o quest_complicata
                    esegui_quest(j);
                  }else{
                    printf("Non è possibile effettuare nessun tipo di quest.\n");
                    continue;
                  }
                  break;
              case 3:
                if ((giocatori[j].posizione) -> emergenza_chiamata == 0 && giocatori[ucciso].posizione == (giocatori[j].posizione)) {     //controllare se è presente un corpo nella stanza_avanti
                  chiamata_emergenza(j);
                } else {
                    printf("Non è possibile effettuare la chiamata d'emergenza.\n");
                    continue
                }
                break;
              default:
                printf("%s\n", "Comando non corretto, inserire 1, 2 o 3.\nPremere invio per continuare.");
                break;
            }
          if (scelta == 1 || scelta == 2 || scelta == 3)
            break;
          else{
            continue;
           }
         } while((scelta = getchar()) != 1 || (scelta = getchar()) != 2 || (scelta = getchar()) != 3);
        }
        if(giocatori[j]. stato == impostore){         //controllo per vedere se il giocatore è un impostore
          do {
            printf("Decidere l'azione da eseguire:\n");
            int n_disponibili = 0;          //variabile che memorizza il numero dei giocatori che potrebbero essere n_uccisi
            for (size_t i = 0; i < n_giocatori; i++) {
              if (giocatori[j].posizione == giocatori[i].posizione && giocatori[i].nome != gioatori[j].nome){     //controllo per vedere se ci sono dei giocatori nella stanza_avanti
                if (giocatori[i].stato == impostore || giocatori[i].stato == astronauta) {      // controllo per far si che i giocatori siano vivi
                  n_disponibili += 1;
                }
              }
            }
            if (n_disponibili != 0)
              printf("\nNella stanza sono presenti %d giocatori\n\n", n_disponibili);
              else
                printf("\nNon è presente nessun giocatore\n");
            printf("1) avanza\n2) chiamata d'emergenza\n3) uccidi l'astronauta\n4) usa la botola\n5) sabota\n");    //chiede all'utente la scelta da eseguire
            scanf("%d", &scelta);
            switch (scelta) {
              case 1:
                avanza(&giocatori[j], giocatori[j].posizione);
                break;
              case 2:
                if ((giocatori[j].posizione) -> emergenza_chiamata == 0 && giocatori[ucciso].posizione == (giocatori[j].posizione)) {       //controllo per vedere se la stanza è di tipo quest semplice o quest_complicata
                  chiamata_emergenza(j);
                }else {
                  printf("Non è possibile effettuare la chiamata d'emergenza.\n");
                  continue;
                }
                break;
              case 3:
                if (n_disponibili != 0) {             //controllo per vedere se c'è almeno un giocatore nella stanza
                  ucciso = uccidi_astronauta(j);
                } else {
                    printf("Non è presente nessun giocatore.\n");
                    continue;
                }
                break;
              case 4:
                if ((giocatori[j].posizione) -> stanza == botola) {
                  usa_botola(j);
                } else {
                      printf("In questa stanza non è presente la botola.\n");
                      continue;
                    }
                break;
              case 5:
                if ((giocatori[j].posizione) -> stanza == quest_semplice || (giocatori[j].posizione) -> stanza == quest_complicata) {       //controllo per vedere se la stanza è di tipo quest semplice o complicata
                  sabotaggio(j);
                } else {
                    printf("Non è possibile cambiare lo stato della stanza.\n");
                    continue;
                }
                break;
              default:
                printf("%s\n", "Comando non corretto, inserire 1, 2, 3, 4 o 5.");
                break;
                }
            if (scelta == 1 || scelta == 2 || scelta == 3 || scelta == 4 || scelta == 5)         //controllo per vedere se la scelta è giusta
              break;
            else{
              continue;
            }
          } while((scelta == getchar()) != 1 || (scelta == getchar()) != 2 || (scelta == getchar()) != 3 || (scelta == getchar()) != 4 || (scelta == getchar()) != 5);
          }
      }
    if (j < n_giocatori) {
      j++;                        //se il giocatore ha terminato il suo turno j viene incrementata di 1
    } else {
        j = 0;                    //se il giocatore ha  terminato il suo turno e il giro è concluse il giro rinizia
    //  clrscr();
      }
    return;
      }
    }
  }

void termina_gioco(){
  if (stanza_inizio != NULL && giocatori != NULL) {       //conrollo per vedere se c'e almeno un giocatore e una stanza
    while (stanza_inizio != NULL) {           //il ciclo continua finchè verranno deallocate tutte le stanza
        free(stanza_inzio);               //dealloca la stanza_avanti
        stanza_inzio = stanza -> prossimo;    //spoasta il puntatore alla stanza successiva
        printf("Rimossa la stanza %s", tipo_stanza(stanza_inzio));
    }
    free(n_giocatori);                 //dealloca l'array di giocatori
    giocatori = NULL;
    free(g_disponibili);                 //dealloca l'array di giocatori disponibili
    g_disponibili = NULL;
    free(g_rischio);                 //dealloca l'array di giocatori a rischio
    g_rischio = NULL;
  }
  while (stanza_botola != NULL) {
    free(stanza_botola);          //dealloca la stanza con la botola
    stanza_botola = stanza_botola -> prossimo;    //sposta il puntatore
  }
  //tutti i puntatori verranno impostati a NULL
  lista_stanze = NULL;
  stanza_botola = NULL;
  stanza_inzio = NULL;
  stanza_destra = NULL;
  stanza_sinistra =NULL;
  stanza_avanti = NULL;
  //le variabili verranno impostati ai valori inizializza
  quest_da_finire = 0;
  n_quest = 0;
  stato_imposta_gioco = 0;
  n_uccisi = 0;
  n_botola = 0;
  n_astronauti = -1;          //inizialmente era = 0
  n_impostori = 0;
  ucciso = 11;
  emergenza_chiamata = 0;
  n_stanze = 0 ;
  return;
}

void mischia_array(int arr[], int n){                          //mischia_array
  int arr_casuale[n];
  for (size_t j = 0; j < n; j++) {
    arr_casuale[j] = j;       //popola l'array con numeri progressivi
  }
  int temp;
  for (size_t i = 0; i < n; i++) {        //ciclo che mischia l'array
    int j = rand() % n;
    temp = arr_casuale[i] = arr_casuale[j];
    arr_casuale[j] = temp;
    for (size_t i = 0; i < n; i++) {
      arr[i] = arr_casuale[i];        //associa l'array con numeri casuali all'array di partenza
    }
  }
}
static const char *colore_giocatore(unsigned short int n){                          //definisce il colore a ogni giocatore
  switch (n) {
    case 0:
      return "\033[1;31mrosso\033[0n";
    case 1:
      return "\033[1;31mblu\033[0n";
    case 2:
      return "\033[1;31mnero\033[0n";
    case 3:
      return "\033[1;31mbianco\033[0n";
    case 4:
      return "\033[1;31mverde\033[0n";
    case 5:
      return "\033[1;31mrosa\033[0n";
    case 6:
      return "\033[1;31mgiallo\033[0n";
    case 7:
      return "\033[1;31mviola\033[0n";
    case 8:
      return "\033[1;31mlime\033[0n";
    case 9:
      return "\033[1;31mcyan\033[0n";
  }
  return "NULL";
}
static const char *stato_giocatore(unsigned short int n){                           //definisce lo stato a ogni giocatore
  switch (n) {
    case 0:
      return "\033[1;31mastronauta\033[0n";
    case 1:
      return "\033[1;31mimpostore\033[0n";
    case 2:
      return "\033[1;31massassinato\033[0n";
    case 3:
      return "\033[1;31mdefenestrato\033[0n";
  }
  return "NULL";
}
static const char *tipo_stanza(unsigned short int n){                              //definisce il tipo alla stanza
  switch (n) {
    case 0:
      return "vuota";
    case 1:
      return "quest_semplice";
    case 2:
      return "quest_complicata";
    case 3:
      return "botola";
  }
  return "NULL";
}
static void stampa_giocatori(struct Giocatore *giocatori, int n){                  //stampa i giocatori
  for (size_t i = 0; i < n; i++) {
    printf("%s: %s\n", colore_giocatore(giocatori[i].nome), stato_giocatore(giocatori[i].stato));
  }
}
static int inizia_gioco(){                                                          //termina la funzione imposta_gioco e apre la funzione gioca
  stato_imposta_gioco = 1;
  return 1;
}
static char *stato_turni(unsigned short int t){                                     //definisce i turni dei giocatori
  switch (t) {
    case 0:
      return "primo";
    case 1:
      return "secondo";
    case 2:
      return "terzo";
    case 3:
      return "quarto";
    case 4:
      return "quinto";
    case 5:
      return "sesto";
    case 6:
      return "settimo";
    case 7:
      return "ottavo";
    case 8:
      return "nono";
    case 9:
      return "decimo";
  }
  return "NULL";
}
static void esegui_quest(int n){                                                    //fa eseguire la quest al giocatore
  if ((giocatori[n].posizione) -> stanza == quest_semplice) {             //controllo per vedere se la stanza è di tipo quest semplice
    printf("La quest semplice è stata effettuata.\n");
  }else{
    quest_da_finire += 2;
    printf("La quest complicata è stata effettuata.\n");
  }
  printf("Punto quest %d\n", quest_da_finire);
  (giocatori[m].posizione) -> stanza = vuota;       //dopo aver affettuato la quest la stanza diventerà vuota
}
static void chiamata_emergenza(int n){                                              //esegue la chiamata d'emergenza
  int probabilita = 30;
  g_rischio = (struct Giocatore*) calloc((n_giocatori - 1), sizeof(struct Giocatore));    //alloca l'array di giocatori che potrebbero essere uccusi
  int n_rischio = 0;      //numero di giocatori che potrebbero essere uccisi
  for (size_t i = 0; i < n_giocatori; i++) {
    if (giocatori[n].posizione == giocatori[i].posizione)   {       //controllo che stabilisce se ci sono dei giocatori nella stanza
      if (giocatori[i].stato == impostore || giocatori[i].stato == astronauta) {        //controllo per far si che solo i giocatori vivi giochino
        n_rischio += 1;
        g_rischio[i].nome = giocatori[i].nome;
        printf("%s\n", colore_giocatore(giocatori[i].nome));
      }
    }
  }
  int token_2 = rand() % n_rischio;
  if (g_rischio[token_2].stato == astronauta) {
    for (size_t i = 0; i < n_rischio; i++) {
      if (giocatori[i].posizione == giocatori[n].posizione && giocatori[i].stato == impostore) {      //controllo per vedere se ci sono degli impostori
        probabilita += 20;
      }
      if (giocatori[i].posizione == giocatori[n].posizione && giocatori[i].stato == astronauta) {      //controllo per vedere se ci sono degli astronauti
        probabilita -= 30;
      }
    }
  }else
    for (size_t i = 0; i < n_rischio; i++) {
      if (giocatori[i].posizione == giocatori[n].posizione && giocatori[i].stato == astronauta) {     //controllo per vedere se ci sono degli astronauti
        probabilita += 20;
      }
      if (giocatori[i].posizione == giocatori[n].posizione && giocatori[i].stato == impostore) {      //controllo per vedere se ci sono degli impostori
        probabilita -= 30;
      }
    }
  if (giocatori[token_2].stato == impostore)
    n_impostori -= 1;
   else
    n_astronauti -= 1;
  if (probabilita >= 100) {
    giocatori[token_2].stato = defenestrato;      //il giocatore viene defenestrato
    n_uccisi += 1;
    printf("%s: %s\n", colore_giocatore(giocatori[n].nome), stato_giocatore(giocatori[n].stato));
  } else {
    printf("Non è stato defenestrato nessun giocatore\n");
    (giocatori[m].posizione) -> emergenza_chiamata = 1;       //la chiamata d'emergenza viene impostata a 1
    }
}
static int uccidi_astronauta(int t){                                                //uccide il giocatore scelto dall'impostore
  int j, token, scelta, g_d[n_giocatori], probabilita = 0;
  printf("Nella stanza sono disponibili i seguenti giocatori, scegli quale uccidere.\nInserisci:\n");
  for (size_t i = 0; i < n_giocatori; i++) {
    if (i != t && giocatori[i].stato != assassinato && giocatori[i].stato != defenestrato && giocatori[i].posizione == giocatori[t].posizione && giocatori[i].nome != giocatori[t].nome) {    //controllo per far si che solo i giocatori ancora in vita e che si trovano nella stessa stanza dell'impostore
      printf("%d ⭢ %s\n", i, colore_giocatore(giocatori[i].nome));               
      g_d[i] = i;                     //salva i giiocatori che potrebbero essere uccisi nell'array
    }
  }
  scanf("%d", &scelta);
  while (getchar() != '\n');
  for (size_t j = 0; i < n_giocatori; j++) {
    if (g_d[j] == scelta) {
      if (giocatori[j].stato == astronauta)
        n_astronauti -= 1;
       else
        n_impostori -= 1;
        giocatori[j].stato = assassinato;     //cambia lo stato del Giocatore
        ucciso = j;
        n_uccisi += 1;
        printf("Il giocatore %s è stato %s.\n", colore_giocatore(giocatori[j].nome), stato_giocatore(giocatori[j].stato));
    }
  }
  for (size_t i = 0; i < n_giocatori; i++) {
    if (i != t && giocatori[i].posizione == giocatori[t].posizione && giocatori[i].stato == astronauta) {
      probabilita += 50;
    }
    if (i != t && ((giocatori[i].posizione) -> stanza_precedente) == giocatori[t].posizione && giocatori[i].stato == astronauta) {
      probabilita += 20;
    }
  }
  token = rand() % 100;
  if (token <= probabilita) {
    giocatori[t].stato = defenestrato;
    n_impostori -= 1;
    n_uccisi += 1;
    printf("Il giocatore %s è stato %s.\n", colore_giocatore(giocatori[t].nome), stato_giocatore(giocatori[t].stato));
  } else {
    printf("Non è stato defenestrato nessun giocatore.\n");
    return ucciso;        //ritorna l'indice del giocatore assassinato
  }
}
static void usa_botola(int m){                                                      //permette all'impostore di spostarsi tramite le botole
  if (n_botola < 1) {
    giocatori[m].posizione = stanza_botola;
    printf("Sei stato spostato nella stanza di tipo: %s\n", tipo_stanza(giocatori[m].posizione));
  } else{
    printf("Non sono presenti altre stanza con la botola quindi verrai spostato in una stanza qualunque.\n");
    if (giocatori[m].posizione != lista_stanze) {
      giocatori[m].posizione = lista_stanze;
      printf("Sei stato spostato nella stanza di tipo: %s\n", tipo_stanza(giocatori[m].posizione));
    } else {
      printf("Non sono presenti altre stanza quindi rimarrai in questa stanza\n");
    }
  }
}
static void sabotaggio(int m){                                                      //permette di cambiare lo stato della stanza
  (giocatori[m].posizione) -> stanza = vuota;       //cambia lo stato della stanza
  printf("La stanza è diventata %s\n", tipo_stanza((giocatori[m].posizione) -> stanza));
}
static void crea_stanza(struct Stanza *nuova_stanza, struct Stanza *g_posizione){   //crea una nuova stanza
  n_stanze += 1;
  nuova_stanza -> avanti = NULL;                    //imposta il puntatore avanti
  nuova_stanza -> destra = NULL;                    //imposta il puntatore detsra
  nuova_stanza -> sinistra = NULL;                  //imposta il puntatore sinistra
  nuova_stanza -> stanza_precedente = g_posizione;  //imposta il puntatore stanza_precedente

  int token = (rand() % 100) + 1;   //genera un numero per stabilire tipo di stanza
  if (token <= 30) {
    int token_2 = rand() % 2;
    if (token_2 == 0) {
      nuova_stanza -> stanza = quest_semplice;
    }if (token_2 == 1) {
      nuova_stanza -> stanza = vuota;
    }
  } else if (token <= 55) {
    nuova_stanza -> stanza = botola;
    n_botola += 1;
  }else{
    nuova_stanza -> stanza = quest_complicata;
  }
  printf("\ntipo stanza: %s\n", tipo_stanza(stanza_inizio -> stanza));
  lista_stanze -> prossimo = (struct Stanza *) malloc(sizeof(struct Stanza));       //crea il prossimo elemento
  lista_stanze -> prossimo = nuova_stanza;
  lista_stanze = lista_stanze -> prossimo;                    //collega il nuovo elemento
  lista_stanze -> prossimo = NULL;                            //il nuovo elelmento punta a NULL

  if (nuova_stanza -> stanza == botola) {
    stanza_botola -> prossimo = (struct Stanza *) malloc(sizeof(struct Stanza));
    stanza_botola -> prossimo = nuova_stanza;
    stanza_botola = stanza_botola -> prossimo;
    stanza_botola -> prossimo = NULL;
  }
}
static void avanza(struct Giocatore *giocatore, struct Stanza *g_posizione){        //permette di far avanzare il giocatore
  printf("Decidere l'azione da eseguire:\n");
  printf("1) avanza a destra\n2) avanza a sinistra\n3) avanza in avanti\n4) rimani in questa stanza\n");
  static int scelta;
  scanf("%d", &scelta);
  switch (scelta) {
    case 1:
      if (g_posizione -> destra == NULL) {                 //se è già presente una stanza a destra
        stanza_destra = malloc (sizeof(struct Stanza));    //alloca una nuova stanza
        crea_stanza(stanza_destra, g_posizione);
        giocatore -> posizione = stanza_destra;            //il gicoatore viene spostato nella nuova stanza
        g_posizione -> destra = stanza_destra
        stanza_destra -> stanza_precedente = g_posizione   //collega la nuova stanza alla precendente
      } else{                                              //se la stanza è già presente
          giocatori -> posizione = stanza_destra;          //sposta il giocatore nella stanza
          printf("La stanza era già presente.\n");
      }
      break;
    case 2:
      if (g_posizione -> sinistra == NULL) {                 //se è già presente una stanza a sinistra
        stanza_sinistra = malloc (sizeof(struct Stanza));    //alloca una nuova stanza
        crea_stanza(stanza_sinistra, g_posizione);
        giocatore -> posizione = stanza_sinistra;            //il gicoatore viene spostato nella nuova stanza
        g_posizione -> destra = stanza_sinistra
        stanza_sinistra -> stanza_precedente = g_posizione   //collega la nuova stanza alla precendente
      } else{                                              //se la stanza è già presente
          giocatori -> posizione = stanza_sinistra;          //sposta il giocatore nella stanza
          printf("La stanza era già presente.\n");
      }
      break;
    case 3:
      if (g_posizione -> avanti == NULL) {                 //se è già presente una stanza a avanti
        stanza_avanti = malloc (sizeof(struct Stanza));    //alloca una nuova stanza
        crea_stanza(stanza_avanti, g_posizione);
        giocatore -> posizione = stanza_avanti;            //il gicoatore viene spostato nella nuova stanza
        g_posizione -> destra = stanza_avanti
        stanza_avanti -> stanza_precedente = g_posizione   //collega la nuova stanza alla precendente
      } else{                                              //se la stanza è già presente
          giocatori -> posizione = stanza_avanti;          //sposta il giocatore nella stanza
          printf("La stanza era già presente.\n");
      }
      break;
    case 3:
      printf("Sei rimasto nella stessa stanza\n");
      break;
  }
}
static void clrscr(){                                                               //cancella le scelte fatte dagli altri giocatori
  system(""@cls||clear");
}
