/*
 * A simple solution to the Case Study exercise from the EPCC MPI course.
 *
 * Communications is done using the sendrecv routine; a proper solution
 * would use non-blocking communications (ie some combination of issend/recv
 * and ssend/irecv).
 *
 * Note that the special rank of MPI_PROC_NULL is a "black hole" for
 * communications (similar to /dev/null in Unix). The use of this value
 * for processes off the edges of the image means we do not need any
 * additional logic to ensure that processes at the edges do not attempt
 * to send to or receive from invalid ranks (ie rank = -1 and rank = P).
 */

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <mpi.h>

void datread (char *filename, void *vx, int nx, int ny);
void pgmwrite(char *filename, void *vx, int nx, int ny);

#define M 192
#define N 360

#define P 4

#define MP M/P
#define NP N

#define MAXITER   1500
#define PRINTFREQ  200

int main (int argc, char **argv)
{
  float old[MP+2][NP+2], new[MP+2][NP+2], edge[MP+2][NP+2];

  float masterbuf[M][N];
  float buf[MP][NP];

  int i, j, iter, maxiter;
  char *filename;

  int rank, size, next, prev;
  MPI_Status status;

  MPI_Init(&argc, &argv);

  MPI_Comm_size(MPI_COMM_WORLD, &size);
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);

  if(size != P)
    {
     if (rank == 0) printf("ERROR: size = %d, P = %d\n", size, P);
      MPI_Finalize();
      exit(-1);
    }

  next = rank + 1;
  prev = rank - 1;

  if (next >= size)
    {
      next = MPI_PROC_NULL;
    }

  if (prev < 0)
    {
      prev = MPI_PROC_NULL;
    }

  if(rank == 0)
    {
      printf("Processing %d x %d image on %d processes\n", M, N, P);
      printf("Number of iterations = %d\n", MAXITER);

      filename = "edge192x360.dat";
      printf("\nReading <%s>\n", filename);
      datread(filename, masterbuf, M, N);
      printf("\n");
    }

  MPI_Scatter(masterbuf, MP*NP, MPI_FLOAT, buf, MP*NP, MPI_FLOAT,
              0, MPI_COMM_WORLD);

  for (i=0;i<MP+2;i++)
    {
      for (j=0;j<NP+2;j++)
	{
	  edge[i][j]=0.0;
	}
    }
      
  for (i=1;i<MP+1;i++)
    {
      for (j=1;j<NP+1;j++)
	{
	  edge[i][j]=buf[i-1][j-1];
	}
    }

  for (i=0;i<MP+2;i++)
    {
      for (j=0;j<NP+2;j++)
	{
	  old[i][j]=edge[i][j];
	}
    }

  for (iter=1;iter<=MAXITER; iter++)
    {
      if(iter%PRINTFREQ==0)
	{
	  if(rank==0)
	    {
	      printf("Iteration %d\n", iter);
	    }
	}

   MPI_Sendrecv(&old[MP][1], NP, MPI_FLOAT, next, 1, 
		   &old[0][1],  NP, MPI_FLOAT, prev, 1,
		   MPI_COMM_WORLD, &status);

   MPI_Sendrecv(&old[1][1],    NP, MPI_FLOAT, prev, 2, 
		   &old[MP+1][1], NP, MPI_FLOAT, next, 2,
		   MPI_COMM_WORLD, &status);

   for (i=1;i<MP+1;i++)
     {
       for (j=1;j<NP+1;j++)
         {
           new[i][j]=0.25*(old[i-1][j]+old[i+1][j]+old[i][j-1]+old[i][j+1]
			      - edge[i][j]);
	 }
     }
	
   for (i=1;i<MP+1;i++)
     {
       for (j=1;j<NP+1;j++)
         {
           old[i][j]=new[i][j];
	 }
     }

   for (i=1;i<MP+1;i++)
     {
       for (j=1;j<NP+1;j++)
         {
           buf[i-1][j-1]=old[i][j];
	 }
     }
    }


  if (rank==0)
    {
      printf("\nFinished %d iterations\n", iter-1);
    }

  MPI_Gather(buf, MP*NP, MPI_FLOAT, masterbuf, MP*NP, MPI_FLOAT,
	     0, MPI_COMM_WORLD);

  if (rank == 0)
    {
      filename="image192x360.pgm";
      printf("\nWriting <%s>\n", filename); 
      pgmwrite(filename, masterbuf, M, N);
    }

  MPI_Finalize();
} 
