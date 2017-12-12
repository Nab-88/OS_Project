#include <cpu.h>
#include <inttypes.h>
#include "ecran.h"
#include "temps.h"
void traitant_IT_32(void);

// on peut s'entrainer a utiliser GDB avec ce code de base
// par exemple afficher les valeurs de x, n et res avec la commande display

// une fonction bien connue
uint32_t fact(uint32_t n)
{
    uint32_t res;
    if (n <= 1) {
        res = 1;
    } else {
        res = fact(n - 1) * n;
    }
    return res;
}

void kernel_start(void)
{
    efface_ecran();
    // affichage_droite("coucou");
    init_horloge();
    // printf("A\n");
    init_traitant_IT(32, traitant_IT_32);
    // printf("B\n");
    masquage_IRQ(0, false);
    sti();
    while (1) {
        // cette fonction arrete le processeur
        hlt();
    }
}
