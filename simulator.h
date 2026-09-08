#ifndef SIMULATOR_H
#define SIMULATOR_H

#define MAX_NOME 32
#define MAX_TAREFAS 64

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
    int totalkilled;
} Task;

typedef struct {
    int tarefa;
    int duracao;
    char motivo;
} Evento;

int simular(Task tarefas[], int numtarefas, int tempototal, const char *algoritmo,
            Evento **eventos, int *numeventos);

int ler_entrada(const char *caminho, Task tarefas[], int *numtarefas, int *tempototal);

#endif