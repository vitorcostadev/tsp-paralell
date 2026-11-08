# Travelling Salesperson Problem (TSP)

Implementação em C do Problema do Caixeiro Viajante (PCV), com comparação
entre uma busca sequencial e uma busca paralela usando POSIX Threads
(Pthreads).

Trabalho para a disciplina de Arquitetura de Computadores.

## Visão geral

O programa gera aleatoriamente as coordenadas de `N` cidades, calcula as
distâncias euclidianas entre elas e procura a rota de menor custo que:

1. começa na cidade 1;
2. visita cada cidade exatamente uma vez;
3. retorna à cidade 1.

As versões sequencial e paralela usam a mesma matriz de distâncias. Ao final,
o programa exibe as melhores rotas, os tempos de execução, o *speedup*, a
eficiência e uma validação dos custos encontrados.

## Funcionalidades

- geração aleatória de coordenadas;
- matriz de distâncias euclidianas;
- busca exaustiva com *backtracking*;
- execução sequencial;
- execução paralela com `N - 1` threads;
- comparação de desempenho;
- validação de que as duas versões encontraram o mesmo custo ótimo;

## Requisitos

### Para gerar os docs

- Instale o [Doxygen](https://www.doxygen.nl/)

Após instalar, execute:

```bash
doxygen
```

E o Doxygen irá gerar a documentação em `docs/html`.

### Execução no Linux

- GCC;
- Bash;
- suporte a Pthreads;
- biblioteca matemática padrão (`libm`).

Em distribuições baseadas em Debian, o compilador pode ser instalado com:

```bash
sudo apt install build-essential
```

### Execução no Windows

- MinGW-w64 ou outro ambiente GCC compatível;
- suporte disponível a Pthreads;
- `gcc` acessível pelo `PATH`.

## Execução rápida

### Linux

```bash
chmod +x run.sh
./run.sh
```

O script compila com `-Wall -Wextra -Wpedantic`, habilita Pthreads com
`-pthread`, vincula a biblioteca matemática com `-lm` e executa o binário.

### Windows

No Prompt de Comando ou PowerShell:

```bat
run.bat
```

## Compilação manual

```bash
gcc -Wall -Wextra -Wpedantic -pthread \
    main.c include/utils.c include/distance.c include/result.c include/thread.c \
    -o main -lm
./main
```

O programa não recebe argumentos de linha de comando atualmente. Para alterar
o tamanho da instância, edite `N` em
[`headers/distance.h`](headers/distance.h) e compile novamente.

## Exemplo de saída

Os valores variam porque as coordenadas são geradas aleatoriamente:

```text
========== Sequential Execution ==========
Optimal cost: 241.16
Best route: 1 -> 3 -> 2 -> 4 -> 5 -> 1
Execution time: 0.000004928 seconds

========== Parallel Execution ==========
Optimal cost: 241.16
Best route: 1 -> 3 -> 2 -> 4 -> 5 -> 1
Execution time: 0.000255019 seconds

========== VALIDATION ==========
The sequential and parallel executions found the same optimal cost.
```

Com apenas cinco cidades, a versão paralela pode ser mais lenta por causa do
custo de criar e sincronizar threads. O paralelismo tende a ser mais útil
quando o espaço de busca é maior.

## Como funciona

### Distâncias

Cada cidade recebe coordenadas `(x, y)`. A distância entre duas cidades é:

$$
d(i,j)=\sqrt{(x_i-x_j)^2+(y_i-y_j)^2}
$$

Esses valores são armazenados em uma matriz compartilhada pelas execuções.
A matriz não é modificada durante a busca, portanto pode ser lida pelas
threads com segurança.

### Busca sequencial

A cidade 1 permanece fixa como origem. O algoritmo gera as permutações das
outras cidades usando *backtracking*, calcula o custo de cada ciclo e mantém a
melhor rota encontrada.

### Busca paralela

O primeiro destino depois da origem divide o trabalho. Para `N` cidades, são
criadas `N - 1` threads; cada uma explora as permutações que começam em uma
cidade diferente. Cada thread mantém seu melhor resultado local. A thread
principal aguarda todas com `pthread_join()` e escolhe o melhor resultado
global.

## Complexidade

Com a origem fixa, o algoritmo avalia `(N - 1)!` rotas. Considerando o custo
de calcular cada rota, a complexidade é:

$$
Tempo = O(n!)\\
Espaço = O(n)
$$

A versão paralela distribui partes da busca, mas mantém a mesma complexidade
assintótica. A aceleração real depende do tamanho da instância, do número de
núcleos e do custo de criação e sincronização das threads.

## Estrutura do projeto

```text
tsp/
├── headers/
│   ├── distance.h       # Tipos, constantes e API das distâncias
│   ├── result.h         # Tipos e inicialização dos resultados
│   ├── thread.h         # Execução paralela e sequencial
│   └── utils.h          # Impressão e medição de tempo
├── include/
│   ├── distance.c       # Coordenadas, distâncias e backtracking
│   ├── result.c         # Alocação e inicialização dos resultados
│   ├── thread.c         # Threads, execução e agregação dos resultados
│   └── utils.c          # Saída formatada e medição de tempo
├── main.c               # Fluxo principal e comparação
├── run.sh               # Compilação e execução no Linux
├── run.bat              # Compilação e execução no Windows
└── README.md
```
