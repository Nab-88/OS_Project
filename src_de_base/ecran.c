/*Fichier regroupant l'ensemble des fonctions permettant de gérer l'affichage
sur l'écran*/

#include "inttypes.h"
#include "cpu.h"
#include "string.h"
#include "ecran.h"
/* Cette fonction renvoie un pointeur sur la case mémoire
correspondant aux coordonnées fournies*/

uint16_t *ptr_mem(uint32_t lig, uint32_t col){
  uint16_t *cellule = (uint16_t*)0xB8000 + (lig * NB_COL + col);
  return cellule;

}


/* Cette fonction ecrit le caractere c aux coordonnées spécifiées*/

void ecrit_car(uint32_t lig, uint32_t col, char c, uint8_t couleur_char,
  uint8_t couleur_fond){
  uint8_t clignote = 0;
  uint8_t octet_fort = (clignote<< 7) + (couleur_fond << 4) + couleur_char;
  uint16_t* pointeur = ptr_mem(lig, col);
  *pointeur =  (octet_fort << 8) + c;
}

/* cette fonction parcourt les lignes et les colonnes de l'écran pour ecrire dans
chaque case une espace en blanc sur fond noir */
void efface_ecran(){
  for (uint32_t j = 0; j < NB_LINE; j++) {
    for (uint32_t i = 0; i < NB_COL; i++) {
      ecrit_car( j, i, ' ', 15, 0);
    }
  }
}

/*place le curseur à la position fournie*/
void place_curseur(uint32_t lig, uint32_t col){
  uint16_t octet_entier = col + NB_COL * lig;
  uint8_t octet_fort = octet_entier >> 8;
  uint8_t octet_faible = octet_entier ;
  outb(0x0F, 0x3D4);
  outb(octet_faible, 0x3D5);
  outb(0x0E, 0x3D4);
  outb(octet_fort, 0x3D5);
  }

void traite_car(char c){
  uint16_t pos_curseur;
  outb(0x0F, 0x3D4);
  pos_curseur = inb(0x3D5);
  outb(0x0E, 0x3D4);
  pos_curseur += (inb(0x3D5))<<8;

  uint32_t lig;
  uint32_t col;
  lig = pos_curseur / NB_COL;
  col = pos_curseur % NB_COL;

  switch (c) {
    case '\b':
      if (col != 0) {
        col --;
      }
      break;
    case '\t':
      col += 8;
      col = col - (col%NB_COL);
      if(col == 80){
        col --;
      }
      break;
    case '\n':
      lig ++;
      col = 0;
      if (lig == 25) {
        lig = lig % 25;
      }
      break;
    case '\f':
      efface_ecran();
      col = 0;
      lig = 0;
      break;
    case '\r':
      col = 0;
      break;
    default:
      if (col == 0) {
        for (uint32_t i = 0; i < NB_COL; i++) {
          ecrit_car( lig, i, ' ', 15, 0);
        }
      }
      ecrit_car(lig, col, c, 15, 0);
      if (col == 79) {
        col = col%80;
        lig += 1;
        lig = lig % NB_LINE;
      }
      else{
        col +=1;
      }
      break;
  }
  place_curseur(lig, col);
}


void defilement(void){
  size_t taille_du_bloc_en_bit = (NB_COL * 2) * (NB_LINE -1);
  void * dest = (void *) 0xB8000;
  void * src = (void*) 0xB8000;
  src += (size_t)(2 * NB_COL);
  memmove(dest, src, taille_du_bloc_en_bit);
  for(uint32_t i=0; i<NB_COL; i++){
    ecrit_car(NB_LINE-1, i, ' ', 15, 0);
  }
}

void console_putbytes(char* chaine, int32_t taille){
  for (uint16_t i = 0 ; i < taille; i++) {
    char c = chaine[i];
    traite_car(c);
  }
}
