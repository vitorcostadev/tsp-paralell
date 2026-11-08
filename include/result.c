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
    }

    return tspp;
}