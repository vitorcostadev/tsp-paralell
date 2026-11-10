#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>

#include "headers/result.h"
#include "headers/thread.h"
#include "headers/utils.h"


int main(void)
{
    Distance *distance = dcreate();
    TSPSequentialResult *sr = tsps_init();
    TSPParallelResult *pr = tspp_init();

    if (distance == NULL || sr == NULL || pr == NULL) {
        free(pr);
        free(sr);
        free(distance);
        fprintf(stderr, "Failed to allocate TSP data.\n");
        return EXIT_FAILURE;
    }
    print_matrix(distance);
    size_t sequential_routes = run_sequential_tsp(distance, sr);
    size_t parallel_routes = run_paralell_tsp(distance, pr);

    printf("Sequential routes evaluated: %zu\n", sequential_routes);
    printf("Parallel routes evaluated: %zu\n", parallel_routes);

    export_distances(distance);
    export_coordinates(distance);
    export_route(sr->sbest_route, "output/sequential_route.csv");
    export_route(pr->pbest_route, "output/parallel_route.csv");

    double Ts = get_elapsed_time(sr->start, sr->end);
    double Tp = get_elapsed_time(pr->start, pr->end);

    print_result(
        "Sequential Execution",
        sr->sbest_cost,
        sr->sbest_route,
        Ts
    );

    print_result(
        "Parallel Execution",
        pr->pbest_cost,
        pr->pbest_route,
        Tp
    );

    printf("\n========== COMPARISON ==========\n");

    printf("Tempo sequencial (Ts): %.9f segundos\n", Ts);
    printf("Tempo paralelo (Tp):   %.9f segundos\n", Tp);

    if (Tp > 0.0) {
        double Speedup = calculate_speedup(Ts, Tp);
        double Efficiency = calculate_efficiency(Speedup, NUM_THREADS);

        printf("Speedup: %.4f\n", Speedup);
        printf("Efficiency: %.2f%%\n", Efficiency * 100.0);
    }

    printf("\n========== VALIDATION ==========\n");

    if(sequential_routes != parallel_routes) {
        printf("Different number of routes evaluated.\n");
        return EXIT_FAILURE;
    } 
    if (fabs(sr->sbest_cost - pr->pbest_cost) < 1e-6) {
        printf("Both executions found the same optimal cost.\n");
    } else {
        printf("Different optimal costs found.\n");
    }
    
    free(pr);
    free(sr);
    free(distance);

    return EXIT_SUCCESS;
}
