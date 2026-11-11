#include "../headers/thread.h"
#include "../headers/result.h"

#include <float.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <time.h>

ThreadData *td_create(void *arg, unsigned _N){
    ThreadData *data = (ThreadData *)arg;
    data->best_route = NULL;
    data->best_route = malloc(_N * sizeof(int));

    return data->best_route != NULL ? data : NULL;
}

void *thread_tsp(void *arg)
{
    ThreadData *data = (ThreadData *)arg;
    data->routes_evaluated = 0;
    data = td_create(data, data->city_count);

    if(data != NULL){
        int route[data->city_count];
        bool *visited = calloc(data->city_count, sizeof(*visited));
        if (visited == NULL) {
            free(data->best_route);
            data->best_route = NULL;
            return NULL;
        }

        for (unsigned initial_city = data->first_city;
            initial_city < data->end_city;
            initial_city++) {
            memset(visited, 0, data->city_count * sizeof(*visited));
            route[0] = 0;
            visited[0] = true;

            route[1] = initial_city;
            visited[initial_city] = true;

            data->routes_evaluated += dgenerate_permutations(
                data->distance,
                2,
                route,
                visited,
                &data->best_cost,
                data->best_route,
                data->city_count
            );
        }

        free(visited);
    }
    return NULL;
}

size_t run_sequential_tsp(Distance *distance, TSPSequentialResult *sr)
{
    clock_gettime(CLOCK_MONOTONIC, &sr->start);

    size_t result = dgenerate_permutations(
        distance,
        1,
        sr->sroute,
        sr->svisited,
        &sr->sbest_cost,
        sr->sbest_route,
        sr->city_count
    );

    clock_gettime(CLOCK_MONOTONIC, &sr->end);

    return result;
}

size_t run_paralell_tsp(Distance *distance, TSPParallelResult *pr, unsigned _N)
{
    for (unsigned i = 0; i < pr->thread_count; i++) {
        pr->threads_data[i].distance = distance;
    }

    clock_gettime(CLOCK_MONOTONIC, &pr->start);

    int created_threads = 0;
    size_t total_routes = 0;

    for (unsigned i = 0; i < pr->thread_count; i++) {
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
    int parallel_best_route[_N];


    for (unsigned i = 0; i < pr->thread_count; i++) {
        int result = pthread_join(pr->threads[i], NULL);
        if (result != 0) {
            fprintf(stderr,
                    "Erro ao aguardar thread: %d\n",
                    result);
            exit(1);
        }

        total_routes += pr->threads_data[i].routes_evaluated;

        if (pr->threads_data[i].best_cost < parallel_best_cost) {
            parallel_best_cost = pr->threads_data[i].best_cost;

            for (unsigned j = 0; j < _N; j++) {
                parallel_best_route[j] =
                    pr->threads_data[i].best_route[j];
            }
        }
    }

    pr->pbest_cost = parallel_best_cost;
    for (unsigned j = 0; j < _N; j++) {
        pr->pbest_route[j] = parallel_best_route[j];
    }

    clock_gettime(CLOCK_MONOTONIC, &pr->end);

    return total_routes;
}

void td_destroy(ThreadData *data){
    if (data != NULL) {
        free(data->best_route);
        free(data);
    }
}