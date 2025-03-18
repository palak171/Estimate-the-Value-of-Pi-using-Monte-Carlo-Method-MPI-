#include <mpi.h>
#include <stdio.h>
#include <stdlib.h>

#define NUM_POINTS 1000000  // Total number of points

int main(int argc, char** argv) {
    int rank, size;
    long long local_count = 0, global_count;
    
    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    long long points_per_proc = NUM_POINTS / size;
    srand(rank);  // Different seed for each process

    for (long long i = 0; i < points_per_proc; i++) {
        double x = (double)rand() / RAND_MAX;
        double y = (double)rand() / RAND_MAX;
        if (x * x + y * y <= 1.0) local_count++;
    }

    // Reduce all counts to rank 0
    MPI_Reduce(&local_count, &global_count, 1, MPI_LONG_LONG, MPI_SUM, 0, MPI_COMM_WORLD);

    if (rank == 0) {
        double pi_estimate = (4.0 * global_count) / NUM_POINTS;
        printf("Estimated PI = %f\n", pi_estimate);
    }

    MPI_Finalize();
    return 0;
}
