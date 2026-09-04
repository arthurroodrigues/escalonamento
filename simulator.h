#ifndef SIMULATOR_H
#define SIMULATOR_H

#define MAX_NOME 32

typedef struct {
    char nome[MAX_NOME];
    int periodo;
    int prazo;
    int rajada;

    int restante;
    int proximachegada;
    int prazoabsoluto;
    int ordem;

    int totalconcluidas;
    int totalperdidas;
} Task;


#endif