#include <stdlib.h>
#include <string.h>
#include <math.h>

#include "../headers/distance.h"

void dgenerate_euclidian_distances(Distance *distance, unsigned _N)
{
    if(distance == NULL) return;

    for(unsigned i = 0; i < _N; i++)
    {
        for(unsigned j = 0; j < _N; j++)
        {
            if(i == j){
                distance->eu_distances[i][j] = 0.0f;
                continue;
            }

            float x1 = distance->coordinates[i][0];
            float y1 = distance->coordinates[i][1];

            float x2 = distance->coordinates[j][0];
            float y2 = distance->coordinates[j][1];

            float diffX = x1 - x2, diffY = y1 - y2;
            float dist = sqrtf((diffX * diffX) + (diffY * diffY));

            distance->eu_distances[i][j] = dist;

        }
    }
}

void alloc_coordinates(Distance *distance, unsigned _N)
{
    distance->coordinates = malloc(_N * sizeof(float *));
    float *_ = malloc(_N * _N * sizeof(float));

    if(distance->coordinates != NULL && _ != NULL){

        for(unsigned i = 0; i < _N; i++){
            distance->coordinates[i] = &_[i * _N];
        }
    }

    distance->eu_distances = malloc(_N * sizeof(float *));
    float *__ = malloc(_N * _N * sizeof(float));

    if(distance->eu_distances != NULL && __ != NULL)
    {
        for(unsigned i = 0; i<_N; i++)
        {
            distance->eu_distances[i] = &__[i * _N];
        }
    }
}
Distance* dcreate(unsigned _N)
{
    Distance *distance = malloc(sizeof(Distance));
    if(distance != NULL){

        alloc_coordinates(distance, _N);

        for(unsigned i = 0; i<_N; i++)
        {
            distance->coordinates[i][0] = (rand() % 10000) / 100.0f;
            distance->coordinates[i][1] = (rand() % 10000) / 100.0f;
        }

        dgenerate_euclidian_distances(distance, _N);
    }

    return distance;
}

double dcalculate_cost(Distance *distance, int route[], unsigned _N)
{
    double cost = 0.00;
    for(unsigned i = 0; i + 1 < _N; i++)
    {
        cost += distance->eu_distances[route[i]][route[i + 1]];
    }

    cost += distance->eu_distances[route[_N - 1]][route[0]];
    return cost;
}

size_t dgenerate_permutations(
    Distance *distance, 
    int position, 
    int route[],
    bool visited[],
    double *best_cost,
    int best_route[],
    unsigned _N
){
    if(position == (int)_N){
        double actually_cost = dcalculate_cost(distance, route, _N);

        if(actually_cost < *best_cost){
            *best_cost = actually_cost;

            for(unsigned i = 0; i < _N; i++){
                best_route[i] = route[i];
            }
        }

        return 1;
    }

    size_t routes_evaluated = 0;
    for(unsigned ci = 1; ci < _N; ci++)
    {
        if(!visited[ci]){
            route[position] = ci;
            visited[ci] = true;

            routes_evaluated += dgenerate_permutations(
                distance,
                position + 1,
                route,
                visited,
                best_cost,
                best_route,
                _N
            );
            visited[ci] = false;
        }

    }

    return routes_evaluated;
}

void ddestroy(Distance *distance)
{
    free(distance->coordinates[0]);
    free(distance->coordinates);

    free(distance->eu_distances[0]);
    free(distance->eu_distances);

    free(distance);
}