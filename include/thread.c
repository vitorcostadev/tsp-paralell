#include "../headers/thread.h"
#include "../headers/result.h"

#include <float.h>
#include <stdlib.h>
#include <stdio.h>
#include <time.h>

ThreadData *td_create(void *arg){
    return (ThreadData *)arg;
}

void *thread_tsp(void *arg)
{
    ThreadData *data = td_create(arg);
    int route[N];
    bool visited[N] = {false};

    route[0] = 0;
    visited[0] = true;

    route[1] = data->initial_city;
    visited[data->initial_city] = true;

    dgenerate_permutations(
        data->distance,
        2,
        route,
        visited,
        &data->best_cost,
        data->best_route
    );

    return NULL;
}

void run_sequential_tsp(Distance *distance, TSPSequentialResult *sr)
{
    clock_gettime(CLOCK_MONOTONIC, &sr->start);

    dgenerate_permutations(
        distance,
        1,
        sr->sroute,
        sr->svisited,
        &sr->sbest_cost,
        sr->sbest_route
    );

    clock_gettime(CLOCK_MONOTONIC, &sr->end);
}

void run_paralell_tsp(Distance *distance, TSPParallelResult *pr)
{
    for (int i = 0; i < NUM_THREADS; i++) {
        pr->threads_data[i].distance = distance;
    }

    clock_gettime(CLOCK_MONOTONIC, &pr->start);

    int created_threads = 0;

    for (int i = 0; i < NUM_THREADS; i++) {
        int result = pthread_create(
            &pr->threads[i],
            NULL,
            thread_tsp,
            &pr->threads_data[i]
        );

        if (result != 0) {
            fprintf(stderr,
                    "Erro ao criar thread: %d\n",
                    result);

            for (int j = 0; j < created_threads; j++) {
                pthread_join(pr->threads[j], NULL);
            }

            exit(1);
        }

        created_threads++;
    }

    double parallel_best_cost = DBL_MAX;
    int parallel_best_route[N];

    for (int i = 0; i < NUM_THREADS; i++) {
        int result = pthread_join(pr->threads[i], NULL);

        if (result != 0) {
            fprintf(stderr,
                    "Erro ao aguardar thread: %d\n",
                    result);
            exit(1);
        }

        if (pr->threads_data[i].best_cost < parallel_best_cost) {
            parallel_best_cost = pr->threads_data[i].best_cost;

            for (int j = 0; j < N; j++) {
                parallel_best_route[j] =
                    pr->threads_data[i].best_route[j];
            }
        }
    }

    pr->pbest_cost = parallel_best_cost;
    for (int j = 0; j < N; j++) {
        pr->pbest_route[j] = parallel_best_route[j];
    }

    clock_gettime(CLOCK_MONOTONIC, &pr->end);
}

void run(Distance *distance, TSPSequentialResult *sr, TSPParallelResult *pr){
    run_sequential_tsp(distance, sr);
    run_paralell_tsp(distance, pr);
}