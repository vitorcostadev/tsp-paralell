#include <float.h>
#include <string.h>
#include <stdlib.h>

#include "../headers/result.h"

TSPSequentialResult *tsps_init(unsigned _N)
{
    TSPSequentialResult *tsps = malloc(sizeof(TSPSequentialResult));
    if(tsps != NULL){
    
        tsps->sbest_cost = DBL_MAX;
        tsps->city_count = _N;
        tsps->sbest_route = malloc(_N * sizeof(int));
        tsps->sroute = calloc(_N, sizeof(int));
        tsps->svisited = calloc(_N, sizeof(bool));
        if (tsps->sbest_route == NULL || tsps->sroute == NULL ||
            tsps->svisited == NULL) {
            tsps_destroy(tsps);
            return NULL;
        }
        tsps->sroute[0] = 0;
        tsps->svisited[0] = true;
    }

    return tsps;
}

TSPParallelResult *tspp_init(unsigned _N, unsigned thread_count)
{
    if (thread_count == 0) {
        return NULL;
    }

    TSPParallelResult *tspp = malloc(sizeof(TSPParallelResult));

    if(tspp != NULL)
    {
        tspp->pbest_cost = DBL_MAX;
        tspp->city_count = _N;
        unsigned city_count = _N > 0 ? _N - 1 : 0;
        tspp->thread_count = thread_count < city_count
            ? thread_count
            : city_count;
        tspp->pbest_route = malloc(_N * sizeof(int));
        tspp->threads = malloc(tspp->thread_count * sizeof(pthread_t));
        tspp->threads_data = malloc(tspp->thread_count * sizeof(ThreadData));

        if (tspp->pbest_route == NULL || tspp->threads == NULL ||
            tspp->threads_data == NULL) {
            tspp_destroy(tspp);
            return NULL;
        }

        for(unsigned i = 0; i < tspp->thread_count; i++){
            tspp->threads_data[i].first_city =
                1 + (i * city_count) / tspp->thread_count;
            tspp->threads_data[i].end_city =
                1 + ((i + 1) * city_count) / tspp->thread_count;
            tspp->threads_data[i].distance = NULL;
            tspp->threads_data[i].best_cost = DBL_MAX;
            tspp->threads_data[i].city_count = _N;
            tspp->threads_data[i].completed = false;
        }
    }

    return tspp;
}

void tsps_destroy(TSPSequentialResult *result)
{
    if(result != NULL){
        free(result->sroute);
        free(result->sbest_route);
        free(result->svisited);
        free(result);
    }

}

void tspp_destroy(TSPParallelResult *result)
{
    if(result != NULL)
    {
        if (result->threads_data != NULL) {
            for (unsigned i = 0; i < result->thread_count; i++) {
                free(result->threads_data[i].best_route);
            }
        }
        free(result->threads);
        free(result->threads_data);
        free(result->pbest_route);

        free(result);
    }
}

void destroy(TSPSequentialResult *seq, TSPParallelResult *par){
    tsps_destroy(seq);
    tspp_destroy(par);
}