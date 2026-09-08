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

    Evento *eventos;
    int numeventos;

    if (simular(tarefas, numtarefas, tempototal, algoritmo, &eventos, &numeventos) != 0) {
        return 1;
    }

    char nomearquivo[64];
    snprintf(nomearquivo, sizeof(nomearquivo), "%s_ara.out", algoritmo);

    if (gravar_saida(nomearquivo, tarefas, numtarefas, eventos, numeventos, algoritmo) != 0) {
        free(eventos);
        return 1;
    }

    free(eventos);
    return 0;
}