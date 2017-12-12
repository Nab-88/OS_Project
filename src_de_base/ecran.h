#include "inttypes.h"
#include "cpu.h"

#define NB_COL 80
#define NB_LINE 25
uint16_t *ptr_mem(uint32_t lig, uint32_t col);

void ecrit_car(uint32_t lig, uint32_t col, char c, uint8_t couleur_char,
  uint8_t couleur_fond);

void efface_ecran();

void place_curseur(uint32_t lig, uint32_t col);

void traite_car(char c);

void defilement(void);

void console_putbytes(char* chaine, int32_t taille);
