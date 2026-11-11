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

## Requisitos

- GCC com suporte a pthreads;
- Python para a geração dos gráficos (opcional);

## Execução rápida

### Linux

```bash
chmod +x scripts/run.sh
./scripts/run.sh
```

Para gerar os gráficos, execute o ['plot.sh'](scripts/plot.sh) no Linux:

```bash
chmod +x scripts/plot.sh
./scripts/plot.sh
```

### Windows

No Prompt de Comando ou PowerShell:

```bat
run.bat
```

Para gerar os gráficos, execute o ['plot.bat'](scripts/plot.bat) no Windows:

```bat
plot.bat
```

## Compilação manual

### No Linux

```bash
gcc -Wall -Wextra -Wpedantic -pthread \
    main.c include/utils.c include/distance.c include/result.c include/thread.c \
    -o main -lm
./main --N 5
```

O número de cidades é definido com `--N`. A quantidade de threads pode ser
definida com `--threads`, `-t` ou `-T`. Se não for informada, o programa usa
`N / 2` threads por padrão. Para definir a seed a ser usada na execução, use `--seed`, `-s` ou `-S`. Se não informar, o programa executa com seed aleatória:

```bash
./main --N 5 --threads 3 --seed 0101
./main --N 5 -t 3 -s 0202
```

### No Windows

```bat
gcc -Wall -Wextra -Wpedantic -pthread ^
    main.c include/utils.c include/distance.c include/result.c include/thread.c ^
    -o main.exe -lm
main.exe --N 5
```

### Testes

Com CMake, configure e compile o projeto e execute todos os testes com:

```bash
cmake -S . -B build
cmake --build build
ctest --test-dir build --output-on-failure
```

O alvo `test` compila o executável de teste e executa a validação das versões
sequencial e paralela.

Os testes também validam:

- A matriz completa de distâncias euclidianas;
- As fórmulas de *speedup* (`Ts / Tp`) e eficiência (`speedup / P`);
- A enumeração de `(N - 1)!` rotas com a cidade inicial fixa;
- A identificação da rota ótima pela busca exaustiva;
- A equivalência dos custos ótimos sequencial e paralelo.

## Gráficos

Ao compilar o programa, ele vai gerar os CSVs das coordenadas das cidades, das distâncias entre os pares, e as melhores rotas tanto da execução sequencial quanto da paralela.

A pasta que é gerada após a compilação é a `output/`, nela vai estar estes arquivos:

```text
output/
├── cities.csv             # Coordenadas das cidades
├── distances.csv          # Distâncias entre os pares de cidades
├── sequential_route.csv   # Rota ótima sequencial
└── parallel_route.csv     # Rota ótima paralela
```

Quando executar o script [`scripts/plot.py`](scripts/plot.py), ele vai ler todos esses CSVs e gerar os gráficos para ambas as rotas.

Para executar mais rapidamente, utilize o [`scripts/plot.sh`](scripts/plot.sh) (Linux) ou [`scripts/plot.bat`](scripts/plot.bat) (Windows) onde será criado a venv no projeto, instalado as dependências e executar o plot.py.

As dependências para executar o script em Python está em [`requirements.txt`](requirements.txt).

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

O primeiro destino depois da origem divide o trabalho entre a quantidade de
threads configurada. As cidades iniciais são distribuídas em blocos
contíguos e equilibrados, garantindo que cada rota seja avaliada uma única
vez. Cada thread mantém seu melhor resultado local. A thread principal
aguarda todas com `pthread_join()` e escolhe o melhor resultado global.

## Complexidade

Com a origem fixa, o algoritmo avalia `(N - 1)!` rotas. Considerando o custo
de calcular cada rota, a complexidade é:

$$
Tempo = O(n!) \\
Espaço = O(n)
$$

A versão paralela distribui partes da busca, mas mantém a mesma complexidade
assintótica. A aceleração real depende do tamanho da instância, do número de
núcleos e do custo de criação e sincronização das threads.
