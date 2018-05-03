#include <stdio.h>
#include <mpi.h> 

void mpstart(int *numproc, int *procid)
{ 
  MPI_Init(NULL, NULL);
  MPI_Comm_rank(MPI_COMM_WORLD, procid);
  MPI_Comm_size(MPI_COMM_WORLD, numproc);
}
  
void mpstop(void)
{ 
  MPI_Finalize();
}
 
void mpgsum(int *ival)
{
  int itmp;
 
  MPI_Allreduce(ival, &itmp, 1, MPI_INT, MPI_SUM, MPI_COMM_WORLD);

  *ival = itmp;
} 

void mpscatter(int *inbuff, int *outbuff, int ndata)
{
  MPI_Scatter(inbuff,  ndata, MPI_INT,
	      outbuff, ndata, MPI_INT,
	      0, MPI_COMM_WORLD);
}

void mpsendrecv(int *senddata, int sendproc, int *recvdata, int recvproc)
{
  MPI_Status status;
    
  MPI_Sendrecv(senddata, 1, MPI_INT, sendproc, 1,
	       recvdata, 1, MPI_INT, recvproc, 1,
               MPI_COMM_WORLD, &status);
}
  
double gettime(void)
{ 
  return MPI_Wtime(); 
} 
