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

int simular(Task tarefas[], int numtarefas, int tempototal, const char *algoritmo,
            Evento **eventos, int *numeventos) {
    int capacidade = 0;
    int count = 0;
    Evento *lista = NULL;

    int *pronta = malloc(numtarefas * sizeof(int));
    if (pronta == NULL) {
        fprintf(stderr, "erro: falha ao alocar memoria\n");
        return -1;
    }
    for (int i = 0; i < numtarefas; i++) pronta[i] = 0;

    int atual = -1;
    int inicio = 0;

    for (int t = 0; t < tempototal; t++) {

        for (int i = 0; i < numtarefas; i++) {
            if (tarefas[i].proximachegada == t) {
                tarefas[i].restante = tarefas[i].rajada;
                tarefas[i].prazoabsoluto = t + tarefas[i].prazo;
                pronta[i] = 1;
            }
        }

        int escolhida = -1;
        for (int i = 0; i < numtarefas; i++) {
            if (!pronta[i]) continue;
            if (escolhida == -1) { escolhida = i; continue; }

            int melhor, empate;
            if (strcmp(algoritmo, "rate") == 0) {
                melhor = tarefas[i].periodo < tarefas[escolhida].periodo;
                empate = tarefas[i].periodo == tarefas[escolhida].periodo;
            } else {
                melhor = tarefas[i].prazoabsoluto < tarefas[escolhida].prazoabsoluto;
                empate = tarefas[i].prazoabsoluto == tarefas[escolhida].prazoabsoluto;
            }
            if (melhor || (empate && tarefas[i].ordem < tarefas[escolhida].ordem)) {
                escolhida = i;
            }
        }

        if (escolhida != atual) {
            char motivo;
            if (atual == -1) {
                motivo = ' ';
            } else {
                motivo = 'H';
            }
            if (adicionar_evento(&lista, &count, &capacidade, atual, t - inicio, motivo) != 0) {
                fprintf(stderr, "erro: falha ao alocar memoria para eventos\n");
                free(pronta); free(lista);
                return -1;
            }
            inicio = t;
            atual = escolhida;
        }

        if (escolhida != -1) {
            tarefas[escolhida].restante--;

            if (tarefas[escolhida].restante == 0) {
                if (adicionar_evento(&lista, &count, &capacidade, escolhida, (t + 1) - inicio, 'F') != 0) {
                    fprintf(stderr, "erro: falha ao alocar memoria para eventos\n");
                    free(pronta); free(lista);
                    return -1;
                }
                tarefas[escolhida].totalconcluidas++;
                pronta[escolhida] = 0;
                tarefas[escolhida].proximachegada += tarefas[escolhida].periodo;
                atual = -1;
                inicio = t + 1;
            }
        }

        for (int i = 0; i < numtarefas; i++) {
            if (pronta[i] && tarefas[i].restante > 0 && tarefas[i].prazoabsoluto == t + 1) {
                if (i == atual) {
                    if (adicionar_evento(&lista, &count, &capacidade, i, (t + 1) - inicio, 'L') != 0) {
                        fprintf(stderr, "erro: falha ao alocar memoria para eventos\n");
                        free(pronta); free(lista);
                        return -1;
                    }
                    atual = -1;
                    inicio = t + 1;
                }
                tarefas[i].totalperdidas++;
                tarefas[i].restante = 0;
                pronta[i] = 0;
                tarefas[i].proximachegada += tarefas[i].periodo;
            }
        }
    }

    char motivofinal;
    if (atual == -1) {
        motivofinal = ' ';
    } else {
        motivofinal = 'H';
    }
    if (adicionar_evento(&lista, &count, &capacidade, atual, tempototal - inicio, motivofinal) != 0) {
        fprintf(stderr, "erro: falha ao alocar memoria para eventos\n");
        free(pronta); free(lista);
        return -1;
    }

    free(pronta);
    *eventos = lista;
    *numeventos = count;
    return 0;
}