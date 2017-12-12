#include "ecran.h"
#include "string.h"
#include "temps.h"
#include "segment.h"
#include "stdbool.h"
#include <inttypes.h>
#include <stdio.h>

uint32_t compteur = 0;
uint32_t temps = 0;
void affichage_droite(char * chaine){
  size_t taille_chaine = strlen(chaine);
  place_curseur(0, NB_COL - taille_chaine);
  console_putbytes(chaine, taille_chaine);
}
void init_horloge(void){
  outb(0x34, 0x43);
  outb((uint8_t) ((QUARTZ / CLOCKFREQ) & 0x00ff), 0x40);
  outb((uint8_t)((QUARTZ / CLOCKFREQ) >> 8), 0X40);
}

void init_traitant_IT(int32_t num_IT, void (*traitant)(void)){
    uint32_t* adresse_table =(uint32_t*) 0x1000 + 2  * num_IT;
    uint32_t premier_mot;
    uint32_t deuxieme_mot;
    premier_mot = (KERNEL_CS << 16) + ((uint32_t) traitant & 0x0000FFFF);
    deuxieme_mot = ((uint32_t) traitant & 0xFFFF0000) + 0x8E00;
    *adresse_table = premier_mot;
    adresse_table ++;
    *adresse_table = deuxieme_mot;
}

void masquage_IRQ(uint32_t num_IRQ, bool masque){
  uint8_t valeur_masque = inb(0x21);
  uint8_t bit_a_forcer = 1 << num_IRQ;
  if (masque) {
    valeur_masque = valeur_masque | bit_a_forcer;
  }
  else{
    valeur_masque = valeur_masque & ~ bit_a_forcer;
  }
  outb(valeur_masque, 0x21);
}
void tic_PIT(void){
  //on acquitte l'interruption
  outb(0x20, 0x20);
  compteur ++;
  if (compteur == 50) {
    temps += 1;
    compteur = 0;
  }
  char chaine[50];
  sprintf(chaine, "nombre de secondes ecoulees : %d", temps);
  affichage_droite(chaine);

  //
}
