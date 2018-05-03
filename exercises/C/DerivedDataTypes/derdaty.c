//
// Rotate exercise from ../Rotate/rotate.c with derived data types                              
//
// Using MPI_GET_ADDRESS and MPI_TYPE_CREATE_STRUCT (MPI2)                                             
// instead of MPI_ADDRESS and MPI_TYPE_COMMIT (MPI1)    
//

#include <stdlib.h>
#include <stdio.h>
#include <mpi.h>

struct compound{
  int ival;
  double dval;
};


int main(){

  struct compound passon,addon,sum;
  int rank, size,tag;
  int start,stop,i;
  int left, right;
  MPI_Comm comm;
  MPI_Status status;
  MPI_Request request;
  // introduce new variables related to derived datatypes
  int length[2] = {1,1};
  MPI_Aint disp[2],base;
  MPI_Datatype type[2], newtype;

  comm  = MPI_COMM_WORLD;
  tag = 1;

  MPI_Init(NULL,NULL);
  MPI_Comm_rank(comm, &rank);            
  MPI_Comm_size(comm, &size); 

  // Must be run on more than 1 procesor.
  if (size == 1)
    {
      printf("Must be run on more than 1 processor.\n");
      MPI_Finalize();
      exit(1);
    }

  // Create all necessary info for the derived datatype
  MPI_Get_address(&passon.ival, &disp[0]);
  MPI_Get_address(&passon.dval, &disp[1]);

  base = disp[0];
  disp[0] = disp[0] - base;
  disp[1] = disp[1] -base;

  type[0] = MPI_INT;
  type[1] = MPI_DOUBLE;

  // Create the new datatype, called 'newtype' and commit it
  MPI_Type_create_struct(2,length,disp,type,&newtype);
  MPI_Type_commit(&newtype);

  //Send current number to the right and receive from the left
  left = rank -1;
  right = rank + 1;
  if ((rank+1) == size){
    right = 0;
  }
  else if(rank == 0){
    left = size - 1;
  }

  sum.ival = 0;
  sum.dval = 0.0;

  // Initialise local values to:
  passon.ival = rank;
  passon.dval = (rank+1)*(rank+1);

  // Use non-blocking point-to-point communication

  for(i=0;i<size;i++){
    MPI_Issend(&passon,1,newtype,right,tag,comm,&request);
    MPI_Recv(&addon,1,newtype,left,tag,comm,&status);
    MPI_Wait(&request,&status);

    sum.ival = sum.ival + addon.ival;
    sum.dval = sum.dval + addon.dval;
    passon.ival = addon.ival;
    passon.dval = addon.dval;

  }


   printf( "The sums are: %d %f on processor %d \n",sum.ival,sum.dval, rank);
  MPI_Finalize();

}
