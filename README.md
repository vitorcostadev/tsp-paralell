# Travelling Salesperson Problem (TSP)

## Introdução

O problema do caixeiro viajante (TSP) é uma generalização do ciclo hamiltoniano, onde: dado um conjunto de cidades, encontrar a rota de menor custo total que visita uma das cidades apenas uma vez e retorna à cidadde de origem.

As cidades são as vértices de um grafo completo cujo pesos associados às arestas são custos associados à realização de uma viagem entre um par de cidades.

## Trazendo para a programação paralela

O objetivo é otimizar o problema do caixeiro viajante utilizando programação paralela, e o algoritmo que será utilizado é a Busca Exaustiva (Brute Force Search). A busca exaustiva é uma abordagem algorítmica que consiste em enumerar e avaliar todas as soluções candidatas de um determinado problema, com o objetivo de identificar aquela que satisfaz o critério de otimização estabelecido.

No Problema do Caixeiro Viajante (PCV), cada solução candidata corresponde a uma rota que visita todas as cidades exatamente uma vez e retorna à cidade de origem. A busca exaustiva consiste, portanto, em gerar todas as permutações possíveis das cidades, respeitando essas condições, e calcular o custo total de cada rota.

Após a avaliação de todas as possibilidades, o algoritmo compara os custos obtidos e seleciona a rota de menor distância como solução ótima. Essa abordagem garante a identificação da solução ótima, desde que todas as rotas válidas sejam enumeradas e avaliadas corretamente.
Entretanto, essa garantia possui um custo computacional elevado. À medida que o número de cidades aumenta, a quantidade de rotas possíveis cresce fatorialmente, tornando a busca exaustiva progressivamente mais custosa.

## Implementação

Este projeto usa \(N = 5\) cidades. Para cada cidade, são geradas coordenadas
aleatórias \((x, y)\). A distância entre duas cidades é calculada pela distância
euclidiana:

$$
d = \sqrt{(x_2 - x_1)^2 + (y_2 - y_1)^2}
$$

A rota é construída por permutação. A cidade 1 é mantida como origem para
evitar avaliar rotações equivalentes. Ao final, o programa exibe o custo e a
melhor rota encontrada.

O programa atual executa a busca de forma sequencial. A complexidade da
enumeração é fatorial, aproximadamente \(O((N - 1)!)\).

## Execução

### Linux

```bash
./run.sh
```

Caso necessário, dê permissão de execução ao script:

```bash
chmod +x run.sh
```

### Windows

Execute o arquivo `run.bat`. Ele requer um compilador GCC disponível no
`PATH`.

Os scripts compilam `main.c` e `include/distance.c` e, em seguida, executam o
programa.

## Estrutura

- `main.c`: inicializa os dados e exibe a solução.
- `headers/distance.h`: define a estrutura e as funções do problema.
- `include/distance.c`: gera as cidades, calcula as distâncias e busca a rota.
- `run.sh`: compilação e execução no Linux.
- `run.bat`: compilação e execução no Windows.
