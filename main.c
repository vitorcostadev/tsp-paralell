#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <pthread.h>
#include <float.h>
#include "headers/distance.h"

#define NUM_THREADS (N - 1)

void *ThreadTSP(void *Arg)
{
    ThreadData *Data = (ThreadData *)Arg;

    int route[N];
    bool visited[N] = {false};

    route[0] = 0;
    visited[0] = true;

    route[1] = Data->InitialCity;
    visited[Data->InitialCity] = true;

    GeneratePermutations(
        Data->Distance,
        2,
        route,
        visited,
        &Data->BestCost,
        Data->BestRoute
    );

    return NULL;
}

double GetElapsedTime(struct timespec start, struct timespec end)
{
    return (end.tv_sec - start.tv_sec) +
           (end.tv_nsec - start.tv_nsec) / 1e9;
}

void PrintMatrices(Distance *distance)
{
    printf("\n========== COORDENADAS DAS CIDADES ==========\n");

    for (int i = 0; i < N; i++) {
        printf("Cidade %d: (%.2f, %.2f)\n",
               i + 1,
               distance->Coordinates[i][0],
               distance->Coordinates[i][1]);
    }

    printf("\n========== MATRIZ DE DISTANCIAS ==========\n");

    printf("      ");

    for (int j = 0; j < N; j++) {
        printf("%10d", j + 1);
    }

    printf("\n");

    for (int i = 0; i < N; i++) {
        printf("%5d ", i + 1);

        for (int j = 0; j < N; j++) {
            printf("%10.2f",
                   distance->EuclidianDistances[i][j]);
        }

        printf("\n");
    }
}

void PrintResult(
    const char *title,
    double bestCost,
    int bestRoute[N],
    double executionTime
)
{
    printf("\n========== %s ==========\n", title);

    printf("Custo otimo: %.2f\n", bestCost);

    printf("Melhor rota: ");

    for (int i = 0; i < N; i++) {
        printf("%d -> ", bestRoute[i] + 1);
    }

    printf("%d\n", bestRoute[0] + 1);

    printf("Tempo de execucao: %.9f segundos\n", executionTime);
}

int main(void)
{
    srand(time(NULL));

    Distance distance;
    Create(&distance);

    PrintMatrices(&distance);

    struct timespec start, end;

    double SequentialBestCost = DBL_MAX;
    int SequentialBestRoute[N];

    int SequentialRoute[N];
    bool SequentialVisited[N] = {false};

    SequentialRoute[0] = 0;
    SequentialVisited[0] = true;

    clock_gettime(CLOCK_MONOTONIC, &start);

    GeneratePermutations(
        &distance,
        1,
        SequentialRoute,
        SequentialVisited,
        &SequentialBestCost,
        SequentialBestRoute
    );

    clock_gettime(CLOCK_MONOTONIC, &end);

    double Ts = GetElapsedTime(start, end);

    PrintResult(
        "EXECUCAO SEQUENCIAL",
        SequentialBestCost,
        SequentialBestRoute,
        Ts
    );

    pthread_t Threads[NUM_THREADS];
    ThreadData ThreadsData[NUM_THREADS];

    for (int i = 0; i < NUM_THREADS; i++) {
        ThreadsData[i].InitialCity = i + 1;
        ThreadsData[i].Distance = &distance;
        ThreadsData[i].BestCost = DBL_MAX;
    }

    clock_gettime(CLOCK_MONOTONIC, &start);

    int CreatedThreads = 0;

    for (int i = 0; i < NUM_THREADS; i++) {
        int result = pthread_create(
            &Threads[i],
            NULL,
            ThreadTSP,
            &ThreadsData[i]
        );

        if (result != 0) {
            fprintf(stderr,
                    "Erro ao criar thread: %d\n",
                    result);

            for (int j = 0; j < CreatedThreads; j++) {
                pthread_join(Threads[j], NULL);
            }

            return 1;
        }

        CreatedThreads++;
    }

    double ParallelBestCost = DBL_MAX;
    int ParallelBestRoute[N];

    for (int i = 0; i < NUM_THREADS; i++) {
        int result = pthread_join(Threads[i], NULL);

        if (result != 0) {
            fprintf(stderr,
                    "Erro ao aguardar thread: %d\n",
                    result);
            return 1;
        }

        if (ThreadsData[i].BestCost < ParallelBestCost) {
            ParallelBestCost = ThreadsData[i].BestCost;

            for (int j = 0; j < N; j++) {
                ParallelBestRoute[j] =
                    ThreadsData[i].BestRoute[j];
            }
        }
    }

    clock_gettime(CLOCK_MONOTONIC, &end);

    double Tp = GetElapsedTime(start, end);

    PrintResult(
        "EXECUCAO PARALELA",
        ParallelBestCost,
        ParallelBestRoute,
        Tp
    );

    printf("\n========== COMPARACAO ==========\n");

    printf("Tempo sequencial (Ts): %.9f segundos\n", Ts);
    printf("Tempo paralelo (Tp):   %.9f segundos\n", Tp);

    if (Tp > 0.0) {
        double Speedup = Ts / Tp;
        double Efficiency = Speedup / NUM_THREADS;

        printf("Speedup: %.4f\n", Speedup);
        printf("Eficiencia: %.2f%%\n", Efficiency * 100.0);
    }

    printf("\n========== VALIDACAO ==========\n");

    if (SequentialBestCost == ParallelBestCost) {
        printf("As duas execucoes encontraram o mesmo custo otimo.\n");
    } else {
        printf("ATENCAO: os resultados sao diferentes!\n");
    }

    return 0;
}
