# Travelling Salesperson Problem (TSP)

## Introdução

O Problema do Caixeiro Viajante (PCV), conhecido internacionalmente como *Travelling Salesperson Problem* (TSP), consiste em encontrar a rota de menor custo que visita um conjunto de cidades exatamente uma vez e retorna à cidade de origem.

O problema pode ser representado por um grafo completo ponderado, no qual as cidades correspondem aos vértices e as arestas representam os deslocamentos entre elas. Cada aresta possui um peso associado ao custo de deslocamento entre duas cidades.

## Objetivo

Este projeto tem como objetivo aplicar conceitos de programação paralela à resolução do Problema do Caixeiro Viajante, utilizando a linguagem C e a biblioteca Pthreads.

Para encontrar a solução ótima, foi escolhida a estratégia de Busca Exaustiva (*Brute Force Search*). Essa abordagem consiste em enumerar e avaliar todas as soluções candidatas, selecionando aquela que apresenta o menor custo total.

No PCV, cada solução candidata corresponde a uma rota que visita todas as cidades exatamente uma vez e retorna à origem. Assim, o algoritmo gera as permutações possíveis das cidades e calcula o custo de cada rota.

O projeto implementa duas versões do algoritmo:

- **Sequencial:** percorre todas as possibilidades em uma única thread de execução.
- **Paralela:** distribui a busca entre múltiplas threads utilizando Pthreads.

As duas implementações utilizam a mesma matriz de distâncias, permitindo comparar seus resultados e tempos de execução.

## Modelagem do problema

As cidades são representadas por coordenadas bidimensionais $(x,y)$, geradas aleatoriamente pelo programa.

A distância entre duas cidades é calculada utilizando a distância euclidiana:

$$
d(i,j)=\sqrt{(x_j-x_i)^2+(y_j-y_i)^2}
$$

Os valores calculados são armazenados em uma matriz de distâncias. Como a distância entre duas cidades é simétrica, a matriz também é simétrica, e sua diagonal principal contém valores iguais a zero.

A cidade 1 é fixada como origem em todas as rotas, evitando a avaliação de representações equivalentes que diferem apenas pelo ponto inicial.

## Busca exaustiva e backtracking

A geração das rotas utiliza backtracking, uma técnica que constrói soluções progressivamente, verificando quais cidades ainda não foram visitadas.

O algoritmo utiliza um vetor de controle para impedir que uma cidade seja adicionada mais de uma vez à mesma rota. Quando todas as cidades foram incluídas, o custo da rota é calculado, considerando também o retorno à cidade de origem.

A quantidade de rotas avaliadas é determinada por:

$$
P(N)=(N-1)!
$$

Como a cidade inicial permanece fixa, apenas as demais cidades precisam ser permutadas.

A complexidade da busca é:

$$
O(N\cdot(N-1)!)=O(N!)
$$

Essa abordagem garante a identificação da solução ótima, desde que todas as rotas válidas sejam enumeradas e avaliadas.

## Implementação paralela

A versão paralela utiliza a biblioteca Pthreads para distribuir a busca entre múltiplas threads.

A divisão do trabalho é realizada considerando a primeira cidade visitada após a origem. Cada thread recebe uma cidade inicial diferente e fica responsável por gerar todas as permutações possíveis das cidades restantes.

Para uma instância com $N$ cidades, são criadas $N-1$ threads. Dessa maneira, cada thread recebe um conjunto independente de rotas, sem sobreposição entre as buscas.

Cada thread mantém seus próprios dados de execução, incluindo o vetor de rota, o controle de cidades visitadas e os valores referentes à melhor solução local encontrada.

A matriz de distâncias é compartilhada entre as threads, pois seus valores não são modificados durante a busca.

Ao finalizar a execução, a thread principal aguarda o término das demais por meio de `pthread_join()` e compara os melhores resultados locais, identificando a solução ótima global.

Essa estratégia evita que múltiplas threads modifiquem simultaneamente uma variável global de melhor custo durante a busca.

## Análise de desempenho

A comparação entre as implementações considera três indicadores:

**Tempo de execução:** representa o tempo necessário para concluir a busca em cada implementação.

- $T_s$: tempo de execução sequencial.
- $T_p$: tempo de execução paralela.

**Speedup:** indica quantas vezes a execução paralela foi mais rápida em relação à sequencial.

$$
S(p)=\frac{T_s}{T_p}
$$

**Eficiência:** representa a relação entre o speedup obtido e a quantidade de threads utilizadas.

$$
E(p)=\frac{S(p)}{p}
$$

Onde $p$ representa o número de threads utilizadas na execução paralela.

Os resultados permitem observar o impacto da paralelização conforme o tamanho da instância aumenta.

## Experimentos

Foram realizados experimentos com diferentes quantidades de cidades, utilizando coordenadas geradas aleatoriamente. Em cada experimento, as versões sequencial e paralela utilizaram a mesma matriz de distâncias, permitindo validar a equivalência dos resultados.

### Experimento 1 — 5 cidades

- Quantidade de rotas: 24.
- Quantidade de threads: 4.
- Tempo sequencial: 0,000009222 segundos.
- Tempo paralelo: 0,000458647 segundos.
- Speedup: 0,0201.
- Eficiência: aproximadamente 0,50%.

Nesse experimento, a execução sequencial apresentou melhor desempenho. Como o número de rotas era pequeno, o trabalho computacional não foi suficiente para compensar os custos de criação e sincronização das threads.

### Experimento 2 — 12 cidades

- Quantidade de rotas: 39.916.800.
- Quantidade de threads: 11.
- Tempo sequencial: 5,632950716 segundos.
- Tempo paralelo: 2,278635078 segundos.
- Speedup: 2,4721.
- Eficiência: aproximadamente 22,47%.

Nesse experimento, a execução paralela apresentou melhor desempenho, reduzindo o tempo de processamento em aproximadamente 59,54% em relação à versão sequencial.

O aumento do número de cidades elevou significativamente o espaço de busca, permitindo que a distribuição do processamento entre as threads proporcionasse um ganho de desempenho.

## Conclusão

Os experimentos demonstraram que a programação paralela pode proporcionar ganhos de desempenho na resolução do Problema do Caixeiro Viajante por busca exaustiva, especialmente quando o volume de processamento é suficientemente elevado.

Na instância com cinco cidades, a execução paralela foi mais lenta devido à pequena quantidade de rotas disponíveis. Já na instância com doze cidades, a distribuição do trabalho entre as threads reduziu significativamente o tempo de execução.

Em ambos os experimentos, as implementações sequencial e paralela encontraram a mesma solução ótima, demonstrando a consistência dos resultados.

Conclui-se que a paralelização é uma alternativa para reduzir o tempo de processamento de algoritmos de busca exaustiva, embora seu benefício dependa do tamanho do problema e da quantidade de trabalho disponível para distribuição entre as threads.

## Execução

### Linux

É necessário possuir GCC e suporte a Pthreads instalados.

Para executar:

```bash
chmod +x run.sh
./run.sh
```

A compilação deve utilizar a opção `-pthread` para habilitar o suporte a Pthreads e `-lm` para vincular a biblioteca matemática.

## Windows

É necessário possuir um ambiente GCC com suporte à biblioteca Pthreads, como uma distribuição MinGW-w64 configurada adequadamente.

Execute:

```bash
run.bat
```

## Estrutura do Projeto

```plaintext
tsp-paralell/
├── headers/
│   └── distance.h
├── include/
│   └── distance.c
├── main.c
├── run.sh
├── run.bat
└── README.md
```

- `main.c`: contém a inicialização, as execuções sequencial e paralela, a medição dos tempos e a comparação dos resultados.
- `headers/distance.h`: define as estruturas, constantes e assinaturas das funções.
- `include/distance.c`: implementa a geração das cidades, o cálculo das distâncias e a busca por permutações.
- `run.sh`: script de compilação e execução no Linux.
- `run.bat`: script de compilação e execução no Windows.
