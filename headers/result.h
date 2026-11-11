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
 * @brief Creates a sequential TSP result object.
 * @param _N Number of cities.
 * @return Initialized result, or NULL on allocation failure.
 */
TSPSequentialResult *tsps_init(unsigned _N);

/**
 * @brief Creates a parallel TSP result object.
 * @param _N Number of cities.
 * @param thread_count Requested number of worker threads.
 * @return Initialized result, or NULL if thread_count is zero or allocation fails.
 * @note The effective thread count is capped at _N - 1.
 */
TSPParallelResult *tspp_init(unsigned _N, unsigned thread_count);

/**
 * @brief Releases a sequential TSP result and its buffers.
 * @param result Result object to release; NULL is accepted.
 */
void tsps_destroy(TSPSequentialResult *result);

/**
 * @brief Releases a parallel TSP result and its thread buffers.
 * @param result Result object to release.
 */
void tspp_destroy(TSPParallelResult *result);

/**
 * @brief Releases both sequential and parallel result objects.
 * @param seq Sequential result.
 * @param par Parallel result.
 */
void destroy(TSPSequentialResult *seq, TSPParallelResult *par);

#endif
