//
//  Program in which 2 processes repeatedly pass M columns of a matrix back and forth
//
// The number/array swapped changes from swap to swap; from back and forth.
//

#include <stdio.h>
#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <mpi.h>

#define N  4    // For an NxN matrix
#define M  1    // Number of columns to be sent together
#define ColStart  2    // where ColStart is the start column
#define itmax 4 // Number of iterations = itmax - 1

int main(int argc, char* argv[]){
  int rank, size, ierr,length;
  int start,stop,i,j,iter;    
  double rbuffer,sbuffer;
  MPI_Comm comm;
  MPI_Status status;

  double sendmatrix[N][N], recvmatrix[N][N];
  MPI_Datatype mcoltype;


  comm  = MPI_COMM_WORLD;

  ierr = MPI_Init(&argc, &argv);
  MPI_Comm_rank(comm, &rank);            
  MPI_Comm_size(comm, &size); 

  // Abort if not run on 2 processors.
  if(size != 2){
    if(rank == 0){
      printf("The code can only be run on 2 processors.\n");
    }
    MPI_Finalize();
    exit(1);
  }

  // Number of rows must not exceed number of coulumns, and start row to be sent must not exceed N.
  if(M > N || ColStart > N ||(ColStart+M) > (N+1)){
    printf("The number of rows MUST NOT be bigger than the number of coulumns: M=%d, N=%d, ColStart=%d \n",M,N,ColStart);
    MPI_Finalize();
    exit(1);
  }

  // Create new datatype mrowtype to send M columns and commit it.
  MPI_Type_vector(N,M,N,MPI_DOUBLE,&mcoltype);
  MPI_Type_commit(&mcoltype);

  // Initialize matrix
  for(i=0;i<N;i++){
    for(j=0;j<N;j++){
      sendmatrix[i][j] = (i+1)*pow(10,j)+rank;
      printf("Matrix intialized on rank %d: i=%d, j=%d, element=%f \n",rank,i,j,sendmatrix[i][j]);
    }
  }

  //E.g. sending columns 4, 5 and 6 of a matrix 10x10 back and forth between rank 0 and rank 1
  // corresponds to ColStart = 4, M = 3, N = 10

  for(iter=1;iter<itmax;iter++){
   if(rank == 0){

    for(i=0;i<N;i++){
     for(j=ColStart;j<(ColStart+M);j++){
     }
    }

    MPI_Ssend(&(sendmatrix[0][ColStart-1]),1,mcoltype,1,50,comm);
    MPI_Recv(&(recvmatrix[0][ColStart-1]),1,mcoltype,1,60,comm,&status);
   }
   else{

    for(i=0;i<N;i++){
     for(j=ColStart;j<(ColStart+M);j++){
     }
    }

    MPI_Recv(&(recvmatrix[0][ColStart-1]),1,mcoltype,0,50,comm,&status);
    MPI_Ssend(&(sendmatrix[0][ColStart-1]),1,mcoltype,0,60,comm);

   }

   for(i=0;i<N;i++){
    for(j=ColStart;j<(ColStart+M);j++){
     sendmatrix[i][j-1] = recvmatrix[i][j-1];
    }
   }
  }


  // Matrix after itmax ping-pongs                                                                                   
  for(i=0;i<N;i++){
   for(j=0;j<N;j++){
    printf("Matrix in the end on rank %d: i=%d, j=%d, element=%f \n",rank,i,j,sendmatrix[i][j]);
   }
  }

  if(rank == 0){
    printf("Sending %d column(s) of an %d x %d matrix, starting with column %d and sending it %d times.\n",M,N,N,ColStart,itmax-1);
  }

  MPI_Finalize();

}
