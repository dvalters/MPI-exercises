//
// perform a global sum of data stored on each process by rotating each piece of data all the
// way round the ring. At each iteration, a process receives some data from the left, adds the 
// value to its running total, then passes the data it has just received on to the right.
//
// Use virtual topologies, here cartesian 1D, like in rotate.c and MPI_Scan
//

#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <mpi.h>


int main(){

  int rank, size;
  int passon, sum;
  MPI_Comm comm;

  comm  = MPI_COMM_WORLD;

  MPI_Init(NULL,NULL);

  MPI_Comm_size(comm, &size); 
  MPI_Comm_rank(comm,&rank);

  sum = 0;

  // Initialise local values to:
  passon = (rank+1)*(rank+1);

  //  Alternatively use
  // passon = rank;

  // Use MPI_scan

  MPI_Scan(&passon,&sum,1,MPI_INT,MPI_SUM,comm);


  printf( "The sum is: %d on processor %d \n",sum, rank);
  MPI_Finalize();

}
