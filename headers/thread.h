#ifndef THREAD_H
#define THREAD_H

#include "distance.h"

typedef struct __thread_data ThreadData;
typedef struct __tsp_sequential TSPSequentialResult;
typedef struct __tsp_paralell TSPParallelResult;

struct __thread_data {
    int initial_city;
    Distance *distance;
    double best_cost;
    int best_route[N];
};

ThreadData *td_create(void *arg);
void *thread_tsp(void *arg);
void run_sequential_tsp(Distance *distance, TSPSequentialResult *sr);
void run_paralell_tsp(Distance *distance, TSPParallelResult *pr);
void run(Distance *distance, TSPSequentialResult *sr, TSPParallelResult *pr);
#endif