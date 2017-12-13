#include <cpu.h>
#include <inttypes.h>
#include "ecran.h"
#include "temps.h"
#include "processus.h"
#include "malloc.c.h"
void traitant_IT_32(void);
void ctx_sw(uint32_t *, uint32_t *);

struct processus* table_proc[2];
uint32_t pid_courant = 0;

void idle(void){
  for (int i = 0; i < 3; i++) {
    printf("[idle] : je tente de passer la main a proc1 ...\n");
    ctx_sw(table_proc[0] -> registres, table_proc[1] -> registres);
    printf("[idle] : proc1 m'a redonne la main\n");
  }
  printf("[idle] : j'arrete le systeme\n");
  hlt();
}

void proc1(void){
for(;;){
    printf("[proc1] : je tente de lui redonner la main\n");
    ctx_sw(table_proc[1] -> registres, table_proc[0] -> registres);
    printf("[proc1] : idle m'a passe la main\n");
  }

}

int creer_processus(void (*code)(void), char * nom){
  struct processus * proc = malloc(sizeof(struct processus));
  if (pid_courant == 0) {
    proc ->pid = pid_courant;
    strcpy(proc->nom, nom);
    proc -> p_etat = ELU;
    table_proc[0] = proc;
  }
  else {
    proc ->pid = pid_courant;
    strcpy(proc->nom, nom);
    proc -> p_etat = ACTIVABLE;
    table_proc[1] = proc;
    proc -> pile_exec[511] = (uint32_t) code;
    proc -> registres[1]= (uint32_t) &(proc->pile_exec[511]);
    table_proc[1] = proc;
  }
  pid_courant ++;
  return pid_courant - 1;
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
    creer_processus(&idle, "idle");
    creer_processus(&proc1, "proc1");
    idle();
    // sti();
    // while (1) {
    //     // cette fonction arrete le processeur
    //     hlt();
    // }
}
