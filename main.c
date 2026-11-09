#include <stdio.h>
#include <stdlib.h>

#include "headers/result.h"
#include "headers/thread.h"
#include "headers/utils.h"


int main(void)
{
    srand(time(NULL));
    Distance *distance = dcreate();
    TSPSequentialResult *sr = tsps_init();
    TSPParallelResult *pr = tspp_init();

    export_coordinates(distance);

    if (distance == NULL || sr == NULL || pr == NULL) {
        free(pr);
        free(sr);
        free(distance);
        fprintf(stderr, "Failed to allocate TSP data.\n");
        return EXIT_FAILURE;
    }
    print_matrix(distance);
    run(distance, sr, pr);
    export_distances(distance);

    export_route(sr->sbest_route, "output/sequential_route.csv");

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

    if (sr->sbest_cost == pr->pbest_cost) {
        printf("The sequential and parallel executions found the same optimal cost.\n");
    } else {
        printf("The sequential and parallel executions found different optimal costs.\n");
    }

    free(pr);
    free(sr);
    free(distance);

    return EXIT_SUCCESS;
}
