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
- exportação das cidades, distâncias e rotas em arquivos CSV;
- geração de um gráfico PNG da rota sequencial;

## Requisitos

- GCC com suporte a Pthreads e `libm`;
- Bash no Linux ou MinGW-w64 no Windows;
- Python 3 com `venv` e `pip`;
- `matplotlib` e `pandas`;
- CMake e Doxygen, opcionalmente.

## Execução rápida

### Linux

```bash
chmod +x run.sh
./run.sh
```

O script compila com `-Wall -Wextra -Wpedantic`, habilita Pthreads com
`-pthread`, vincula a biblioteca matemática com `-lm` e executa o binário.
Depois, verifica se o Python 3 está disponível, cria e ativa o ambiente
`.venv`, instala as dependências de [`requirements.txt`](requirements.txt) e
gera `output/sequential_route.png` com o script de plotagem.

### Windows

No Prompt de Comando ou PowerShell:

```bat
run.bat
```

O script executa as mesmas etapas no Windows. Ele procura `python` ou `py -3`,
cria `.venv\Scripts`, instala as dependências e executa a plotagem da rota
sequencial.

Os scripts encerram com erro quando o compilador ou o Python não está
disponível. O ambiente virtual é reutilizado em execuções posteriores.

## Compilação manual

```bash
gcc -Wall -Wextra -Wpedantic -pthread \
    main.c include/utils.c include/distance.c include/result.c include/thread.c \
    -o main -lm
./main
```

### CMake e Make

Com CMake, configure e compile o projeto e execute todos os testes com:

```bash
cmake -S . -B build
cmake --build build
ctest --test-dir build --output-on-failure
```

O projeto também possui um Makefile:

```bash
make
make test
```

O alvo `test` compila o executável de teste e executa a validação das versões
sequencial e paralela.

Os testes também validam:

- a matriz completa de distâncias euclidianas;
- as fórmulas de *speedup* (`Ts / Tp`) e eficiência (`speedup / P`);
- a enumeração de `(N - 1)!` rotas com a cidade inicial fixa;
- a identificação da rota ótima pela busca exaustiva;
- a equivalência dos custos ótimos sequencial e paralelo.

## Saídas e visualização

Ao executar o programa, os arquivos abaixo são gerados no diretório `output/`:

```text
output/
├── cities.csv             # Coordenadas das cidades
├── distances.csv          # Distâncias entre os pares de cidades
├── sequential_route.csv   # Rota ótima sequencial
└── sequential_route.png   # Gráfico gerado por plot.py
```

O gráfico é gerado somente para a rota sequencial:

```bash
.venv/bin/python scripts/plot.py       # Linux/macOS
.venv\Scripts\python.exe scripts\plot.py  # Windows
```

O script [`scripts/plot.py`](scripts/plot.py) usa `pandas` para ler os CSVs e
`matplotlib` para desenhar as cidades, a matriz de distâncias e a rota ótima.

As dependências Python estão listadas em
[`requirements.txt`](requirements.txt):

```text
matplotlib
pandas
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
├── scripts/
│   └── plot.py           # Geração do gráfico da rota sequencial
├── tests/
│   └── test_tsp.c        # Testes do algoritmo e das métricas
├── output/               # CSVs e PNG gerados durante a execução
├── CMakeLists.txt        # Configuração de build e CTest
├── Makefile              # Build e alvo make test
├── requirements.txt      # Dependências Python do plot
├── run.sh                # Compilação, execução e plot no Linux
├── run.bat               # Compilação, execução e plot no Windows
└── README.md
```
