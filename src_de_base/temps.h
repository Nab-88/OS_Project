#include "ecran.h"
#include "string.h"
#include "segment.h"
#include "stdbool.h"
#include <inttypes.h>
#include <stdio.h>

#define QUARTZ 0x1234DD
#define CLOCKFREQ 50

void affichage_droite(char * chaine);

void init_horloge(void);

void init_traitant_IT(int32_t num_IT, void (*traitant)(void));

void masquage_IRQ(uint32_t num_IRQ, bool masque);

void tic_PIT(void);
