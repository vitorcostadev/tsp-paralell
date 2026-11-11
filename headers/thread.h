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
};

/**
 * @brief Creates a new thread data.
 * @param arg The argument to be passed to the thread.
 * @return A pointer to the newly created ThreadData.
 * @note The param arg is void* to allow for flexibility in the type of data passed to the thread. It is expected that the caller will cast the argument to the appropriate type before passing it to this function.
 */
ThreadData *td_create(void *arg, unsigned _N);

/**
 * @brief The function executed by each thread to solve the TSP problem.
 * @param arg A pointer to the ThreadData structure containing the initial city, distance matrix, best cost, and best route for the thread.
 * @return NULL
 * @note This function is designed to be run in a separate thread. It generates permutations of the TSP route starting from the initial city and updates the best cost and best route found by the thread.
 */
void *thread_tsp(void *arg);

/**
 * @brief Runs the sequential TSP algorithm.
 * @param distance A pointer to the Distance structure containing the coordinates and Euclidean distances.
 * @param sr A pointer to the TSPSequentialResult structure where the results of the sequential TSP algorithm will be stored.
 * @note This function measures the time taken to solve the TSP problem sequentially and updates the TSPSequentialResult structure with the best cost and route found.
 */
size_t run_sequential_tsp(Distance *distance, TSPSequentialResult *sr);

/**
 * @brief Runs the parallel TSP algorithm.
 * @param distance A pointer to the Distance structure containing the coordinates and Euclidean distances.
 * @param pr A pointer to the TSPParallelResult structure where the results of the parallel TSP algorithm will be stored.
 * @note This function creates multiple threads to solve the TSP problem in parallel. It measures the time taken to solve the TSP problem and updates the TSP
 */
size_t run_paralell_tsp(Distance *distance, TSPParallelResult *pr, unsigned _N);

void td_destroy(ThreadData *data);
#endif