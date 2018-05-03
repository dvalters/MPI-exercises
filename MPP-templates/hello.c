#include <stdio.h>
#include <stdlib.h>

#include <mpi.h>

int main(void)
{
  MPI_Init(NULL, NULL);

  printf("Hello World!\n");

  MPI_Finalize();
}
