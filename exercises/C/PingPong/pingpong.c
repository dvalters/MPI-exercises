//
//  Program in which 2 processes repeatedly pass a message back and forth
//
// The same data is sent from A to B, then returned from B to A.
//
// Corrected July 2010 ($HOME/mail/EPCC/NAIS/DSH)
//

#include <stdlib.h>
#include <stdio.h>
#include <mpi.h>

int main(int argc, char* argv[]){
  int rank, size, numiter,length;
  int start,stop,i,iter;
  int tag1, tag2,extent;
  double *sbuffer;
  double t1, t2,totmess;
  MPI_Comm comm;
  MPI_Status status;

  comm  = MPI_COMM_WORLD;
  tag1= 1;
  tag2 = 2;

  MPI_Init(&argc, &argv);
  MPI_Comm_rank(comm, &rank);            
  MPI_Comm_size(comm, &size); 

  // Abort if run on less than 2 processors.
  if(size < 2){
    if(rank == 0){
      printf("The code must be run on at least 2 processors.\n");
    }
    MPI_Finalize();
    exit(1);
  }
  
  if(argc < 2) {
    if(rank == 0){
     printf("Code requires 2 input arguments: The array length and the number of iterations. \n");
    }
    MPI_Finalize();
  exit(1);
  }

  if(rank > 1){
    printf( "Rank %d not participating \n",rank);
  }

  if (rank == 0) {
    length = atoi(argv[1]);
    numiter = atoi(argv[2]);
    printf("Array length = %d , number of iterations = %d\n",length,numiter);
  }

  MPI_Bcast(&length,1,MPI_INT,0,comm);
  MPI_Bcast(&numiter,1,MPI_INT,0,comm);


  // Allocate array
  sbuffer = malloc(length*sizeof(double));
 
  // Send 'buffer' back and forth between rank 0 and rank 1.
  for(i=0;i<length;i++)
    sbuffer[i] = (double)rank + 10.0;

  // Time the parallel execution.
  MPI_Barrier(comm);
  t1 = MPI_Wtime();

  // Swap back and forth for iter times

  for(iter=1;iter<numiter;iter++){

  if(rank == 0){

  // Timings only make sense when no print statements are includes!

   MPI_Ssend(&sbuffer[0],length,MPI_DOUBLE,1,tag1,comm);
   MPI_Recv(&sbuffer[0],length,MPI_DOUBLE,1,tag2,comm,&status);
  }

 else if(rank == 1){

   MPI_Recv(&sbuffer[0],length,MPI_DOUBLE,0,tag1,comm,&status);
   MPI_Ssend(&sbuffer[0],length,MPI_DOUBLE,0,tag2,comm);

   }
  }   //end of for loop



  t2 = MPI_Wtime();

  // Extent of an MPI_DOUBLE on this machine

  MPI_Type_size(MPI_DOUBLE,&extent);

  if(rank == 0){
    // Each iteration comprises two messages
    // A MB os 1024*1024 bytes
   totmess = 2.0*extent*length/1024*numiter/1024;
   printf( "Ping-Pong of twice %d bytes, for %d times \n",extent*length,numiter);
   printf( "Total computing time is %f [s]\n", t2 - t1 );
   printf( "Total message size is %f [MB] \n", totmess );
   printf( "Latency (time per message) is %f [s]\n",  0.5*(t2-t1)/numiter);
   printf( "Bandwidth (message size per time) is %f [MB/s]\n",  totmess/(t2-t1));
   
   if((t2-t1)<1.0)
     
     printf( "WARNING: The time is too short to be meaningful, increase the number of iterations and/or the array size so time is at least one second!\n");
  }

  free(sbuffer);

  MPI_Finalize();

}
