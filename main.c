#include <stdio.h>
#include "headers/distance.h"
#include <stdlib.h>
#include <time.h>
#include <pthread.h>

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

int main(void)
{
    srand(time(NULL));
    Distance distance;
    Create(&distance);

    int NumOfThreads = N - 1;
    pthread_t Threads[NumOfThreads];
    ThreadData ThreadsData[NumOfThreads];

    struct timespec start, end;
    clock_gettime(CLOCK_MONOTONIC, &start);

    for(int i = 0; i < NumOfThreads; i++){
        ThreadsData[i].InitialCity = i + 1;
        ThreadsData[i].Distance = &distance;
        ThreadsData[i].BestCost = DBL_MAX;

        if(pthread_create(&Threads[i], NULL, ThreadTSP, &ThreadsData[i]) != 0){
            perror("Erro na criação da thread");
            return 1;
        }
    }

    double BestCost = DBL_MAX;
    int BestRoute[N];

    for(int i = 0; i < NumOfThreads; i++){
        pthread_join(Threads[i], NULL);

        if(ThreadsData[i].BestCost < BestCost){
            BestCost = ThreadsData[i].BestCost;
            for(int j = 0; j<N; j++){
                BestRoute[j] = ThreadsData[i].BestRoute[j];
            }
        }
    }

    clock_gettime(CLOCK_MONOTONIC, &end);
    double execTime =   (end.tv_sec - start.tv_sec) + (end.tv_nsec - start.tv_nsec) / 1e9;

    printf("Custo em execução paralela: %.2f\n", BestCost);
    printf("Melhor rota: ");
    for (int i = 0; i < N; i++) {
        printf("%d -> ", BestRoute[i] + 1);
    }
    printf("%d\n", BestRoute[0] + 1);
    printf("Tempo de execução (Tp): %.6f segundos\n", execTime);

    return 0;
}