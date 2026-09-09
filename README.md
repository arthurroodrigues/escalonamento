# Implementação 3 - Escalonamento

Simulador de escalonamento de tarefas periódicas utilizando os algoritmos
Rate Monotonic (RATE) e Earliest Deadline First (EDF).

## Arquivos

- main.c — programa principal e recebimento dos argumentos.
- simulator.c — leitura das tarefas, simulação e geração dos resultados.
- simulator.h — definição das estruturas e funções utilizadas.
- Makefile — compilação e limpeza do projeto.

## Compilação

Para compilar:

    make

O comando gera o executável scheduler.

Para limpar os arquivos compilados:

    make clean

## Execução

Rate Monotonic:

    ./scheduler rate voo.txt

Earliest Deadline First:

    ./scheduler edf voo.txt

Os resultados são salvos em:

    rate_ara.out
    edf_ara.out

## Entrada

O arquivo de entrada deve seguir o formato:

    [TEMPO TOTAL]
    [NOME] [PERÍODO] [DEADLINE] [BURST]

Exemplo:

    100
    ATT 20 12 8
    NAV 50 30 15

## Testes

Foram realizados testes para verificar a execução das tarefas, preempções,
deadlines perdidos, diferentes prioridades e situações de CPU ociosa.

## Sistema Operacional

Ubuntu Linux (WSL).