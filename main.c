#include <stdio.h>  
#include <stdlib.h>
#include <math.h>  

#include "mpi.h"

#define ROOT_PROCESS 0

/**
 * N = Número de elementos do vetor
 */
#define N 40 

int main(int argc, char* argv[])
{
    int current_process, number_of_processes;
    int *vector = NULL;

    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &current_process);
    MPI_Comm_size(MPI_COMM_WORLD, &number_of_processes);

    if (current_process == ROOT_PROCESS) {
        vector = (int*) malloc(N * sizeof(int));
        
        if (vector == NULL) {
            fprintf(stderr, "Memory allocation failed!\n");
            MPI_Abort(MPI_COMM_WORLD, 1);
        }
        
        for (int i = 0; i < N; i++) {
            vector[i] = i + 1;
        }

        printf("Created vector on root process: [ ");
        for (int i = 0; i < N; i++) {
            printf("%d ", vector[i]);
        }
        printf("]\n");
    }

    int chunk_size = N / number_of_processes;
    int *chunk = (int*) malloc(chunk_size * sizeof(int));

    if (chunk == NULL) {
        fprintf(stderr, "Memory allocation failed on process %d!\n", current_process);
        MPI_Abort(MPI_COMM_WORLD, 1);
    }

    MPI_Scatter(vector, chunk_size, MPI_INT, chunk, chunk_size, MPI_INT, ROOT_PROCESS, MPI_COMM_WORLD);

    int local_sum = 0;

    printf("Process %d received chunk: [ ", current_process);
    for (int i = 0; i < chunk_size; i++) {
        printf("%d ", chunk[i]);
        local_sum += chunk[i] * chunk[i];
    }
    printf("]\n");

    int global_sum = 0;
    MPI_Reduce(&local_sum, &global_sum, 1, MPI_INT, MPI_SUM, ROOT_PROCESS, MPI_COMM_WORLD);

    if (current_process == ROOT_PROCESS) {
        printf("Sum of squares: %d\n", global_sum);

        int expected_sum = N * (N + 1) * (2 * N + 1) / 6;
        
        if (global_sum == expected_sum) {
            printf("Result is correct!\n");
        } else {
            printf("Result is incorrect! Expected %d but got %d\n", expected_sum, global_sum);
        }

        free(vector);
    }

    MPI_Finalize();
    return EXIT_SUCCESS;
}