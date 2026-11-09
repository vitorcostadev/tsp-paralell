#include <float.h>
#include <string.h>
#include <stdlib.h>

#include "../headers/result.h"

TSPSequentialResult *tsps_init()
{
    TSPSequentialResult *tsps = malloc(sizeof(TSPSequentialResult));

    if(tsps != NULL){
    
        tsps->sbest_cost = DBL_MAX;
        memset(tsps->svisited, 0, sizeof(tsps->svisited));

        tsps->sroute[0] = 0;
        tsps->svisited[0] = true;
    }

    return tsps;
}

TSPParallelResult *tspp_init()
{
    TSPParallelResult *tspp = malloc(sizeof(TSPParallelResult));

    if(tspp != NULL)
    {
        tspp->pbest_cost = DBL_MAX;

        for(unsigned i = 0; i < NUM_THREADS; i++){
            tspp->threads_data[i].initial_city = i + 1;
            tspp->threads_data[i].distance = NULL;
            tspp->threads_data[i].best_cost = DBL_MAX;
        }
    }

    return tspp;
}