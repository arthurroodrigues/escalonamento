#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "simulator.h"

int main(int argc, char *argv[]) {
    if (argc != 3) {
        fprintf(stderr, "erro: uso: %s [rate|edf] [arquivo_de_entrada]\n", argv[0]);
        return 1;
    }

    const char *algoritmo = argv[1];
    const char *caminho = argv[2];

    if (strcmp(algoritmo, "rate") != 0 && strcmp(algoritmo, "edf") != 0) {
        fprintf(stderr, "erro: algoritmo invalido '%s'. use 'rate' ou 'edf'\n", algoritmo);
        return 1;
    }

    Task tarefas[MAX_TAREFAS];
    int numtarefas;
    int tempototal;

    if (ler_entrada(caminho, tarefas, &numtarefas, &tempototal) != 0) {
        return 1;
    }

    printf("tempo total: %d\n", tempototal);
    printf("tarefas lidas: %d\n", numtarefas);
    for (int i = 0; i < numtarefas; i++) {
        printf("  [%d] %s periodo=%d prazo=%d rajada=%d\n",
               tarefas[i].ordem, tarefas[i].nome,
               tarefas[i].periodo, tarefas[i].prazo, tarefas[i].rajada);
    }

    Evento *eventos;
    int numeventos;

    if (simular(tarefas, numtarefas, tempototal, algoritmo, &eventos, &numeventos) != 0) {
        return 1;
    }

    const char *nomealgoritmo;
    if (strcmp(algoritmo, "rate") == 0) {
        nomealgoritmo = "RATE";
    } else {
        nomealgoritmo = "EDF";
    }
    printf("\nEXECUTION BY %s\n", nomealgoritmo);

    for (int i = 0; i < numeventos; i++) {
        if (eventos[i].tarefa == -1) {
            printf("idle for %d units\n", eventos[i].duracao);
        } else {
            printf("[%s] for %d units - %c\n",
                   tarefas[eventos[i].tarefa].nome, eventos[i].duracao, eventos[i].motivo);
        }
    }

    printf("\nLOST DEADLINES\n");
    for (int i = 0; i < numtarefas; i++) {
        printf("[%s] %d\n", tarefas[i].nome, tarefas[i].totalperdidas);
    }

    printf("\nCOMPLETE EXECUTION\n");
    for (int i = 0; i < numtarefas; i++) {
        printf("[%s] %d\n", tarefas[i].nome, tarefas[i].totalconcluidas);
    }

    free(eventos);
    return 0;
}