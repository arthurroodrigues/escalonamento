#include <stdio.h>
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

    return 0;
}