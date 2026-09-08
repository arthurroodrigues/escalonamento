#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include "simulator.h"

#define MAX_LINHA 256

int ler_entrada(const char *caminho, Task tarefas[], int *numtarefas, int *tempototal) {
    FILE *arquivo = fopen(caminho, "r");
    if (arquivo == NULL) {
        fprintf(stderr, "erro: nao foi possivel abrir o arquivo '%s': %s\n",
                caminho, strerror(errno));
        return -1;
    }

    char linha[MAX_LINHA];

    if (fgets(linha, sizeof(linha), arquivo) == NULL) {
        fprintf(stderr, "erro: arquivo de entrada vazio ou ilegivel\n");
        fclose(arquivo);
        return -1;
    }

    int total;
    if (sscanf(linha, "%d", &total) != 1 || total <= 0) {
        fprintf(stderr, "erro: tempo total invalido na primeira linha\n");
        fclose(arquivo);
        return -1;
    }

    int count = 0;

    while (fgets(linha, sizeof(linha), arquivo) != NULL) {
        int vazia = 1;
        for (char *c = linha; *c != '\0'; c++) {
            if (*c != ' ' && *c != '\t' && *c != '\n' && *c != '\r') {
                vazia = 0;
                break;
            }
        }
        if (vazia) continue;

        if (count == MAX_TAREFAS) {
            fprintf(stderr, "erro: numero de tarefas excede o limite maximo (%d)\n", MAX_TAREFAS);
            fclose(arquivo);
            return -1;
        }

        char nome[MAX_NOME];
        int periodo, prazo, rajada;
        int campos = sscanf(linha, "%31s %d %d %d", nome, &periodo, &prazo, &rajada);

        if (campos != 4) {
            fprintf(stderr, "erro: linha malformada: %s", linha);
            fclose(arquivo);
            return -1;
        }

        if (periodo <= 0 || prazo <= 0 || rajada <= 0) {
            fprintf(stderr, "erro: tarefa '%s' tem valor nao positivo\n", nome);
            fclose(arquivo);
            return -1;
        }

        if (rajada > prazo || prazo > periodo) {
            fprintf(stderr, "erro: tarefa '%s' viola rajada <= prazo <= periodo (C=%d D=%d P=%d)\n",
                    nome, rajada, prazo, periodo);
            fclose(arquivo);
            return -1;
        }

        strncpy(tarefas[count].nome, nome, MAX_NOME);
        tarefas[count].periodo = periodo;
        tarefas[count].prazo = prazo;
        tarefas[count].rajada = rajada;
        tarefas[count].ordem = count;

        tarefas[count].restante = rajada;
        tarefas[count].proximachegada = 0;
        tarefas[count].prazoabsoluto = prazo;
        tarefas[count].totalconcluidas = 0;
        tarefas[count].totalperdidas = 0;

        count++;
    }

    fclose(arquivo);

    if (count == 0) {
        fprintf(stderr, "erro: nenhuma tarefa encontrada no arquivo\n");
        return -1;
    }

    *tempototal = total;
    *numtarefas = count;
    return 0;
}

static int adicionar_evento(Evento **eventos, int *count, int *capacidade, int tarefa, int duracao, char motivo) {
    if (duracao <= 0) return 0;

    if (*count == *capacidade) {
        int novacapacidade;
        if (*capacidade == 0) {
            novacapacidade = 16;
        } else {
            novacapacidade = *capacidade * 2;
        }

        Evento *cresceu = realloc(*eventos, novacapacidade * sizeof(Evento));
        if (cresceu == NULL) return -1;
        *eventos = cresceu;
        *capacidade = novacapacidade;
    }

    (*eventos)[*count].tarefa = tarefa;
    (*eventos)[*count].duracao = duracao;
    (*eventos)[*count].motivo = motivo;
    (*count)++;
    return 0;
}

