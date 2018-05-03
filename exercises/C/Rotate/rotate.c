//
// perform a global sum of data stored on each process by rotating each piece of data all the
// way round the ring. At each iteration, a process receives some data from the left, adds the 
// value to its running total, then passes the data it has just received on to the right.
//

#include <stdlib.h>
#include <stdio.h>
#include <mpi.h>

int main(){
  int rank, size, tag;
  int start,stop,i;
  int left, right, addon, passon, sum;
  MPI_Comm comm;
  MPI_Status status;
  MPI_Request request;

  comm  = MPI_COMM_WORLD;

  tag = 1;

  MPI_Init(NULL,NULL);
  MPI_Comm_rank(comm, &rank);            
  MPI_Comm_size(comm, &size); 

  //Send current number to the right and receive from the left
  left = rank -1;
  right = rank + 1;
  if ((rank+1) == size){
    right = 0;
  }
  if(rank == 0){
    left = size - 1;
  }

  sum = 0;

  // Initialise local values to:
  passon = (rank+1)*(rank+1);

  //  Alternatively use
  // passon = rank;

  // Use non-blocking point-to-point communication

  for(i=0;i<size;i++){
    MPI_Issend(&passon,1,MPI_INT,right,tag,comm,&request);
    MPI_Recv(&addon,1,MPI_INT,left,tag,comm,&status);
    MPI_Wait(&request,&status);

    sum = sum + addon;
    passon = addon;

  }


  printf( "The sum is: %d on processor %d \n",sum, rank);
  MPI_Finalize();

}
