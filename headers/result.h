#ifndef RESULT_H
#define RESULT_H

#include <stdbool.h>
#include <pthread.h>
#include <time.h>
#include "thread.h"

struct __tsp_sequential {
    struct timespec start, end;
    double sbest_cost;
    int *sbest_route;
    int *sroute;
    bool *svisited;
    unsigned city_count;
};

struct __tsp_paralell {
    pthread_t *threads;
    ThreadData *threads_data;
    struct timespec start, end;
    double pbest_cost;
    int *pbest_route;
    unsigned city_count;
    unsigned thread_count;
};

/**
 * @brief Struct to store the result of the sequential TSP algorithm.
 */
TSPSequentialResult* tsps_init(unsigned _N);

/**
 * @brief Struct to store the result of the parallel TSP algorithm.
 */
TSPParallelResult *tspp_init(unsigned _N, unsigned thread_count);

void tsps_destroy(TSPSequentialResult *result);
void tspp_destroy(TSPParallelResult *result);

void destroy(TSPSequentialResult *seq, TSPParallelResult *par);

#endif