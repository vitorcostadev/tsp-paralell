#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>
#include <getopt.h>

#include "headers/result.h"
#include "headers/thread.h"
#include "headers/utils.h"


int main(int argc, char *argv[])
{
    unsigned qty = 1;
    unsigned thread_count = 0;
    unsigned _seed = 0;
    int opt;

    struct option options[] = {
        {"N", required_argument, 0, 'n'},
        {"threads", required_argument, 0, 't'},
        {"seed", required_argument, 0, 's'},
        {0,0,0,0}
    };

    while((opt = getopt_long(argc, argv, "n:t:T:s:S:", options, NULL)) != -1){
        switch(opt){
            case 'n':
                qty = (unsigned)strtoul(optarg, NULL, 10);
                break;
            case 't':
            case 'T':
                thread_count = (unsigned)strtoul(optarg, NULL, 10);
                break;
            case 's':
            case 'S':
                _seed = (unsigned) strtoul(optarg, NULL, 10);
                break;
            default:
                fprintf(stderr,
                        "Use: %s [--N <number>] [--threads <number>]\n",
                        argv[0]);
                return 1;
        }
    }

    if (qty < 2) {
        fprintf(stderr, "N must be at least 2.\n");
        return EXIT_FAILURE;
    }

    if (thread_count == 0) {
        thread_count = qty / 2;
    }

    if(_seed == 0){
        srand(time(NULL));
    }else{
        srand(_seed);
    }

    Distance *distance = dcreate(qty);
    TSPSequentialResult *sr = tsps_init(qty);
    TSPParallelResult *pr = tspp_init(qty, thread_count);

    if (distance == NULL || sr == NULL || pr == NULL) {
        destroy(sr, pr);
        ddestroy(distance);
        fprintf(stderr, "Failed to allocate TSP data.\n");
        return EXIT_FAILURE;
    }
    print_matrix(distance, qty);
    size_t sequential_routes = run_sequential_tsp(distance, sr);
    size_t parallel_routes = run_paralell_tsp(distance, pr, qty);

    printf("Sequential routes evaluated: %zu\n", sequential_routes);
    printf("Parallel routes evaluated: %zu\n", parallel_routes);

    export_distances(distance, qty);
    export_coordinates(distance, qty);
    export_route(sr->sbest_route, "output/sequential_route.csv", qty);
    export_route(pr->pbest_route, "output/parallel_route.csv", qty);

    double Ts = get_elapsed_time(sr->start, sr->end);
    double Tp = get_elapsed_time(pr->start, pr->end);

    print_result(
        "Sequential Execution",
        sr->sbest_cost,
        sr->sbest_route,
        Ts,
        qty
    );

    print_result(
        "Parallel Execution",
        pr->pbest_cost,
        pr->pbest_route,
        Tp,
        qty
    );

    printf("\n========== COMPARISON ==========\n");

    printf("Tempo sequencial (Ts): %.9f segundos\n", Ts);
    printf("Tempo paralelo (Tp):   %.9f segundos\n", Tp);

    if (Tp > 0.0) {
        double Speedup = calculate_speedup(Ts, Tp);
        double Efficiency = calculate_efficiency(Speedup, (int)pr->thread_count);

        printf("Speedup: %.4f\n", Speedup);
        printf("Efficiency: %.2f%%\n", Efficiency * 100.0);
    }

    printf("\n========== VALIDATION ==========\n");

    if(sequential_routes != parallel_routes) {
        printf("Different number of routes evaluated.\n");
        destroy(sr, pr);
        ddestroy(distance);
        return EXIT_FAILURE;
    } 
    if (fabs(sr->sbest_cost - pr->pbest_cost) < 1e-6) {
        printf("Both executions found the same optimal cost.\n");
    } else {
        printf("Different optimal costs found.\n");

        destroy(sr, pr);
        ddestroy(distance);
        return EXIT_FAILURE;
    }
    
    destroy(sr, pr);
    ddestroy(distance);
    return EXIT_SUCCESS;
}
