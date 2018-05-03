#include <cstdlib>
#include <cmath>

#include "mpi.h"

#include <iostream>


#define N 900

int main ()
{
  double pi, exactpi;

  double tstart, tstop;

  int i;

  // MPI variables
  MPI_Status status;
  MPI_Comm comm;

  int rank, size, source, tag;

  // Other variables
  int istart, istop;
  double partialpi, recvpi;

  std::cout << "Computing approximation to pi using N = " << N << std::endl;

  // Initialise MPI and compute number of processes and local rank
  comm = MPI_COMM_WORLD;
  MPI_Init(NULL, NULL);

  MPI_Comm_size(comm, &size);
  MPI_Comm_rank(comm, &rank);
  
  // OOP method is now deprecated - use the C-style call above 
  // rank = comm.Get_rank();
  // size = comm.Get_size();

  std::cout << "Hello from rank " << rank << std::endl;

  // Now make sure output only comes from one process
  if (rank == 0) std::cout << "Running on " << size << " process(es)" << std::endl;

  tstart = MPI_Wtime();

  partialpi = 0.0;

  //
  // Compute an approximation to pi using a simple series expansion for pi/4
  // Ensure each process computes a separate section of the summation
  // NOTE: here I assume that N is exactly divisible by the number of processes
  //
  
  // Deal with case of N not divisible by size
  
  // For cases where rank < size -1 
  if (rank != size-1)
  {
    istart = (N / size) * rank + 1;
    istop  = istart + (N / size) - 1;
  }
  // Final case may not be equal (remainder N)
  else if (rank == size -1)
  {
	istart = (N / size) * rank + 1;
	istop = istart + std::remainder(N, size) -1;
  }

  std::cout << "On rank " << rank << " istart = " << istart
       << ", istop = " << istop << std::endl;

  for (i=istart; i<=istop; i++)
  {
    partialpi = partialpi +
	  1.0/( 1.0 + std::pow( (((double) i)-0.5)/((double) N), 2.0) );
  }

  std::cout << "On rank " << rank << " partialpi = " << partialpi << std::endl;

  //
  // Compute global value of pi by sending partial values to rank 0 
  // NOTE: this would be more efficiently done using MPI_REDUCE 
  //

  if (rank == 0)
  {
    // Initialise pi to locally computed parial sum
    pi = partialpi;

    // Add in contribution from other processes
    for (source = 1; source < size; source++)
	{
	  // receive partialpi from rank=source and place value in recvpi
	  // all messages are tagged as zero
	  tag = 0;

	  MPI_Recv(&recvpi, 1, MPI_DOUBLE, MPI_ANY_SOURCE,
			       tag, comm, &status);

	  std::cout << "rank 0 receiving from rank " << status.MPI_SOURCE << std::endl;

	  // add to running total
	  pi = pi + recvpi;
	}
  }
  else
  {
	// all other processes send their partial value to rank 0
	tag = 0;

	std::cout << "rank " << rank << " sending to rank 0" << std::endl;

	MPI_Ssend(&partialpi, 1, MPI_DOUBLE, 0, tag, comm);
  }

  pi = pi * 4.0/((double) N);

  exactpi = 4.0*atan(1.0);
  
  tstop = MPI_Wtime();

  if (rank == 0)
  {
    std::cout << "pi = " << pi
	   << ", % error = " << fabs(100.0*(pi-exactpi)/exactpi) << std::endl;

    std::cout << "Time taken was " << (tstop - tstart) << " seconds" << std::endl;
  }

  MPI_Finalize();

  return 0;
}
