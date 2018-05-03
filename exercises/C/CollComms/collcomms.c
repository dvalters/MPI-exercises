//
// perform a global sum of data stored on each process by rotating each piece of data all the
// way round the ring. At each iteration, a process receives some data from the left, adds the 
// value to its running total, then passes the data it has just received on to the right.
//
// Start from code in ../Rotate/rotate.c
//
//  Now using an MPI reduction operation to perform the global sum
//

#include <stdlib.h>
#include <stdio.h>
#include <mpi.h>

int main(){
  int rank, size;
  int start,stop,i;
  int passon, sum;
  MPI_Comm comm;
  MPI_Status status;
  MPI_Request request;

  comm  = MPI_COMM_WORLD;

  MPI_Init(NULL,NULL);
  MPI_Comm_rank(comm, &rank);            
  MPI_Comm_size(comm, &size); 

  sum = 0;

  // Initialise local values to:
  passon = (rank+1)*(rank+1);

  //  Alternatively use
  // passon = rank;


  // Use MPI reduction operation

  //MPI_Reduce(&passon,&sum,1,MPI_INT,MPI_SUM,0,comm);
  MPI_Allreduce(&passon,&sum,1,MPI_INT,MPI_SUM,comm);

  printf( "The sum is: %d on processor %d\n",sum,rank);

  MPI_Finalize();

}
