//
// perform a global sum of data stored on each process by rotating each piece of data all the
// way round the ring. At each iteration, a process receives some data from the left, adds the 
// value to its running total, then passes the data it has just received on to the right.
//
// previous version in ../rotate/rotate.c
//
// Use virtual topologies, here cartesian 1D
//

#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <mpi.h>

#define TRUE  1
#define FALSE 0
#define ndims 1    

int main(){
  // New variables required
  int comm1d,direction,disp;
  int dims[ndims];
  int period[ndims];
  int reorder;
  // Variables as before
  int rank, size,tag;
  int start,stop,i;
  int left, right, addon, passon, sum;
  MPI_Comm comm;
  MPI_Status status;
  MPI_Request request;

  comm  = MPI_COMM_WORLD;
  tag =1;

  MPI_Init(NULL,NULL);

  MPI_Comm_size(comm, &size); 

  // Cartesian topology
  dims[0] = 0;
  period[0] = TRUE;    // TRUE, Cyclic
  reorder = FALSE;      // FALSE
  direction = 0;       // shift along the first index
  disp = 1;            // Shift by 1

  MPI_Dims_create(size,ndims,dims);
  MPI_Cart_create(comm,ndims,dims,period,reorder,&comm1d);
  MPI_Comm_rank(comm1d,&rank);
  MPI_Cart_shift(comm1d,direction,disp,&left,&right);

  sum = 0;

  // Initialise local values to:
  passon = (rank+1)*(rank+1);

  //  Alternatively use
  // passon = rank;

  // Use non-blocking point-to-point communication

  for(i=0;i<size;i++){
    MPI_Issend(&passon,1,MPI_INT,right,tag,comm1d,&request);
    MPI_Recv(&addon,1,MPI_INT,left,tag,comm1d,&status);
    MPI_Wait(&request,&status);

    sum = sum + addon;
    passon = addon;

  }


  printf( "The sum is: %d on processor %d \n",sum, rank);
  MPI_Finalize();

}
