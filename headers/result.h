#ifndef RESULT_H
#define RESULT_H

#include <stdbool.h>
#include <pthread.h>
#include <time.h>
#include "thread.h"

struct __tsp_sequential {
    struct timespec start, end;
    double sbest_cost;
    int sbest_route[N];
    int sroute[N];
    bool svisited[N];
};

struct __tsp_paralell {
    pthread_t threads[NUM_THREADS];
    ThreadData threads_data[NUM_THREADS];
    struct timespec start, end;
    double pbest_cost;
    int pbest_route[N];
};

TSPSequentialResult* tsps_init();
TSPParallelResult *tspp_init();

#endif