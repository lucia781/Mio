#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "gamelib.h"

int main() {
  static int scelta;
  time_t t;
  srand((unsigned) time(&t));
  printf("\n\n                                      Si consiglia di visualizzare il gioco a schermo intero,premere invio per continuare\n");      //convertire spazio con cifre
  while(getchar() != '\n');
  printf("%38s\n", "___");
  printf("%100s\n", ",--.'|_                                                         ,--,");
  printf("%100s\n", "|  | :,'   __  ,-.                 ,---,      ,---,    ,---.  ,--.'|");
  printf("%99s\n", ":  : ' : ,' ,'/ /|             ,-+-. /  | ,-+-. /  |  '   ,'\\ |  |,");
  printf("%99s\n", ".;__,'  /  '  | |' | ,--.--.    ,--.'|'   |,--.'|'   | /   /   |`--'_");
  printf("%100s\n", "|  |   |   |  |   ,'/       \\  |   |  ,\"' |   |  ,\"' |.   ; ,. :,' ,'|");
  printf("%100s\n", ":__,'| :   '  :  / .--.  .-. | |   | /  | |   | /  | |'   | |: :'  | |");
  printf("%100s\n", "'  : |__ |  | '   \\__\\/: . . |   | |  | |   | |  | |'   | .; :|  | :");
  printf("%102s\n", "|  | '.'|;  : |   ,\" .--.; | |   | |  |/|   | |  |/ |   :    |'  : |__");
  printf("%103s\n", ";  :    ;|  , ;  /  /  ,.  | |   | |--' |   | |--'   \\   \\  / |  | '.'|");
  printf("%103s\n", "|  ,   /  ---'  ;  :   .'   \\|   |/     |   |/        `----'  ;  :    ;");
  printf("%102s\n", "---`-'         |  ,     .-./'---'      '---'                 |  ,   /");
  printf("%102s\n", "`--`---'                                      ---`-'\n");
  while ((scelta = getchar()) != 1 || (scelta = getchar()) != 2 || (scelta = getchar()) != 3) {
    printf("\n\n\n\n%282s\n", "▀▄▀▄▀▄▀▄▀▄▀▄▀▄▀▄▀▄▀▄▀▄▀▄▀▄▀▄▀▄ 𝓜 𝓔 𝓝 𝓤' 𝓟 𝓡 𝓘 𝓝 𝓒 𝓘 𝓟 𝓐 𝓛 𝓔 ▄▀▄▀▄▀▄▀▄▀▄▀▄▀▄▀▄▀▄▀▄▀▄▀▄▀▄▀▄▀");
    printf("\n\n                                                        Inserisci un comando");             //convertire gli spazi in cifre
    printf("\n\n                                                  ___________________________\n");
    printf("\n\n                                              /\\                         \\.\n");
    printf("\n\n                                             |  |                        |.\n");
    printf("\n\n                                             \\_|                         |.\n");
    printf("\n\n                                               |                         |.\n");
    printf("\n\n                                               |  1  imposta il gioco    |.\n");    //freccia
    printf("\n\n                                               |                         |.\n");
    printf("\n\n                                               |         2  gioca        |.\n");
    printf("\n\n                                               |                         |.\n");
    printf("\n\n                                               |    3 termina gioco      |.\n");
    printf("\n\n                                               |                         |.\n");
    printf("\n\n                                               |   ______________________|_\n");
    printf("\n\n                                               |  /                       /\n");
    printf("\n\n                                               |_/_______________________/.\n");
    printf("\n");
    scanf("%d", %scelta);
    while(getchar() != '\n');
  switch (scelta) {
    case 1:
      imposta_gioco();
      break;
    case 2:
      gioca();
      break;
    case 3:
      termina_gioco();
      break;
    default:
    printf("%s\n", "Comando non corretto, inserire 1, 2 o 3.");
    break;
    }
  }
  return 0;
}
