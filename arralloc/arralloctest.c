#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h> 

#include "arralloc.h"

void printlinearbuffer(float *x, int n);
void print3Darray(float ***x, int ni, int nj, int nk);

int main(void)
{
  int nx, ny, nz;
  int i, j, k;

  float ***array;

  nx = 2;
  ny = 4;
  nz = 3;

  array = arralloc(sizeof(float), 3, nx, ny, nz);

  for (i=0; i < nx; i++)
    {
      for (j=0; j < ny; j++)
	{
	  for (k=0; k < nz; k++)
	    {
	      array[i][j][k] = k + j*nz + i*nz*ny;
	    }
	}
    }

  printf("array[%d][%d][%d] = %f\n", nx/2,ny/2,nz/2, array[nx/2][ny/2][nz/2]);

  /*
   *  The following call is correct - pass address of first data element
   */

  printf("\nArray passed correctly to a function expecting a linear buffer\n\n");

  printlinearbuffer(&(array[0][0][0]), nx*ny*nz);

  /*
   *  The following call is INCORRECT - you should NOT pass the array itself
   */

  printf("\nArray passed INCORRECTLY to a function expecting a linear buffer\n\n");

  printlinearbuffer((float *) array, nx*ny*nz);

  /*
   *  The following call is correct - can pass address of array provided that
   *  the function being called expects a 3D array and not a linear buffer
   */

  printf("\nArray passed correctly to a function expecting a 3D array\n\n");

  print3Darray(array, nx, ny, nz);

  /*
   *  Entire multidimensional array is deallocated with a single free
   */

  free(array);
}
      
void printlinearbuffer(float *x, int n)
{
  int i;

  for (i=0; i < n; i++)
    {
      printf("x[%d] = %f\n", i, x[i]);
    }
}

void print3Darray(float ***x, int ni, int nj, int nk)
{
  int i, j, k;

  for (i=0; i < ni; i++)
    {
    for (j=0; j < nj; j++)
      {
      for (k=0; k < nk; k++)
        {
  	  printf("x[%d][%d][%d] = %f\n", i, j, k, x[i][j][k]);
        }
      }
    }
}
