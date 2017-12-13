#include <cpu.h>
#include <inttypes.h>
#include "ecran.h"
#include "temps.h"
#include "processus.h"
#include "malloc.c.h"
void traitant_IT_32(void);
void ctx_sw(uint32_t *, uint32_t *);
struct processus* actif;
struct processus* table_proc[2];
uint32_t pid_courant = 0;

char* mon_nom(void){
  return actif -> nom;
}

int32_t mon_pid(void){
  return actif -> pid;
}

void ordonnance(void){
  int32_t pid_actif = mon_pid();
  struct processus* ancien_actif = actif;
  ancien_actif -> p_etat = ACTIVABLE;
  pid_actif = (pid_actif +1) % 2;
  actif = table_proc[pid_actif];
  actif -> p_etat = ELU;
  ctx_sw(ancien_actif -> registres, actif -> registres);
}

void idle(void){
  for (;;) {
    printf("[%s] pid = %i\n", mon_nom(), mon_pid());
    ordonnance();
  }
}

void proc1(void){
for(;;){
    printf("[%s] pid = %i\n", mon_nom(), mon_pid());
    ordonnance();
  }
}

int creer_processus(void (*code)(void), char * nom){
  struct processus * proc = malloc(sizeof(struct processus));
  if (pid_courant == 0) {
    proc ->pid = pid_courant;
    strcpy(proc->nom, nom);
    proc -> p_etat = ELU;
    table_proc[0] = proc;
    actif = proc;
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
