#ifndef THREAD_H
#define THREAD_H

#include "distance.h"

typedef struct __thread_data ThreadData;
typedef struct __tsp_sequential TSPSequentialResult;
typedef struct __tsp_paralell TSPParallelResult;

struct __thread_data {
    unsigned first_city;
    unsigned end_city;
    Distance *distance;
    double best_cost;
    int *best_route;
    size_t routes_evaluated;
    unsigned city_count;
    bool completed;
};

/**
 * @brief Allocates the per-thread best-route buffer.
 * @param arg ThreadData object to initialize.
 * @param _N Number of cities.
 * @return arg on success, or NULL if best_route cannot be allocated.
 */
ThreadData *td_create(void *arg, unsigned _N);

/**
 * @brief pthread entry point for processing one city's work range.
 * @param arg Pointer to the worker's ThreadData.
 * @return Always NULL; completion is reported through ThreadData.completed.
 * @note Allocation failures leave completed false and routes_evaluated zero.
 */
void *thread_tsp(void *arg);

/**
 * @brief Runs the sequential TSP algorithm.
 * @param distance Distance object containing the coordinates and matrix.
 * @param sr Result object that receives timing, cost, and route data.
 * @return Number of complete routes evaluated.
 */
size_t run_sequential_tsp(Distance *distance, TSPSequentialResult *sr);

/**
 * @brief Runs the parallel TSP algorithm.
 * @param distance Distance object containing the coordinates and matrix.
 * @param pr Result object containing workers and receiving the aggregate result.
 * @param _N Number of cities.
 * @return Number of complete routes evaluated, or SIZE_MAX if a worker fails.
 * @note Work is distributed in contiguous ranges of initial cities.
 */
size_t run_paralell_tsp(Distance *distance, TSPParallelResult *pr, unsigned _N);

/**
 * @brief Releases a ThreadData object and its best-route buffer.
 * @param data ThreadData object to release.
 */
void td_destroy(ThreadData *data);

#endif
