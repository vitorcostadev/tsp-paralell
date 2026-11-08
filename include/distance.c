#include <stdlib.h>
#include <string.h>
#include <math.h>

#include "../headers/distance.h"

void dgenerate_euclidian_distances(Distance *distance)
{
    if(distance == NULL) return;

    for(unsigned i = 0; i < N; i++)
    {
        for(unsigned j = 0; j < N; j++)
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

Distance* dcreate()
{
    Distance *distance = malloc(sizeof(Distance));
    if(distance != NULL){

        memset(distance, 0, sizeof(Distance));

        for(unsigned i = 0; i<N; i++)
        {
            distance->coordinates[i][0] = (rand() % 10000) / 100.0f;
            distance->coordinates[i][1] = (rand() % 10000) / 100.0f;
        }

        dgenerate_euclidian_distances(distance);
    }

    return distance;
}

float dcalculate_cost(Distance *distance, int route[N])
{
    double cost = 0.00;
    for(unsigned i = 0; i<N-1; i++)
    {
        cost += distance->eu_distances[route[i]][route[i + 1]];
    }

    cost += distance->eu_distances[route[N - 1]][route[0]];
    return cost;
}

void dgenerate_permutations(
    Distance *distance, 
    int position, 
    int route[N], 
    bool visited[N],
    double *best_cost,
    int best_route[N]
){
    if(position == N){
        double actually_cost = dcalculate_cost(distance, route);

        if(actually_cost < *best_cost){
            *best_cost = actually_cost;

            for(unsigned i = 0; i < N; i++){
                best_route[i] = route[i];
            }
        }

        return;
    }

    for(unsigned ci = 1; ci < N; ci++)
    {
        if(!visited[ci]){
            route[position] = ci;
            visited[ci] = true;

            dgenerate_permutations(
                distance,
                position + 1,
                route,
                visited,
                best_cost,
                best_route
            );
            visited[ci] = false;
        }

    }
}