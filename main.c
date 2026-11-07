#include <stdio.h>
#include "headers/distance.h"
#include <stdlib.h>
#include <time.h>

int main(void)
{
    srand(time(NULL));
    Distance distance;
    Create(&distance);

    int route[N];
    bool visited[N] = {false};

    route[0] = 0;
    visited[0] = true;

    GeneratePermutations(&distance, 1, route, visited);

    printf("Custo ótimo sequencial: %.2lf\n", distance.BestCost);
    printf("Melhor rota global: ");
    for (int i = 0; i < N; i++) {
        printf("%d -> ", distance.BestRoute[i] + 1);
    }
    printf("%d\n", distance.BestRoute[0] + 1);

    return 0;
}