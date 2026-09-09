# Implementação 3 - Escalonamento

Simulador de escalonamento de tarefas periódicas utilizando os algoritmos
Rate Monotonic (RATE) e Earliest Deadline First (EDF).

## Arquivos

- main.c — programa principal: valida argumentos da linha de comando, chama a leitura do arquivo de entrada, executa a simulação e grava o resultado.
- simulator.c — implementa a leitura e validação do arquivo de entrada, o motor de simulação (preempção, deadlines) e a geração do arquivo de saída.
- simulator.h — definição das estruturas (Task, Evento) e assinaturas das funções utilizadas.
- Makefile — compilação e limpeza do projeto.
- voo.txt — arquivo de teste com o exemplo do enunciado.

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

O arquivo voo.txt (exemplo do próprio enunciado) foi usado para validar o
funcionamento do simulador com os dois algoritmos:

    ./scheduler rate voo.txt
    cat rate_ara.out

    ./scheduler edf voo.txt
    cat edf_ara.out

Também foram testados os seguintes casos de erro, cada um alterando o
conteúdo de um arquivo de entrada e verificando a mensagem em stderr, o
código de saída diferente de zero e a ausência de arquivo .out gerado:

- Número incorreto de argumentos
- Algoritmo diferente de rate/edf
- Arquivo de entrada inexistente
- Linha malformada (campo faltando)
- Valor não numérico
- Valor não positivo
- Violação de C > D (rajada maior que o prazo)
- Violação de D > P (prazo maior que o período)

## Sistema Operacional

Ubuntu Linux (WSL2, rodando sobre Windows).