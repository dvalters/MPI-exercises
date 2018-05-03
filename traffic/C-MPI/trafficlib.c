#include "uni.h"
#include "mplib.h"

int initroad(int *road, int n, float density, int seed)
{
  int i, ncar;
  float rng;

  // seed random number generator

  rinit(seed);

  ncar = 0;

  for (i=0; i < n; i++)
    {
      rng = uni();

      if (rng < density)
	{
	  road[i] = 1;
	}
      else
	{
	  road[i] = 0;
	}

      ncar += road[i];
    }

  return ncar;
}


int updateroad(int *newroad, int *oldroad, int n)
{
  int i, nmove;

  nmove = 0;

  for (i=1; i<=n; i++)
    {
      if (oldroad[i] == 1)
	{
	  if (oldroad[i+1] == 1)
	    {
	      newroad[i] = 1;  
	    }
	  else
	    {
	      newroad[i] = 0; 
	      nmove++; 
	    }
	}
      else
	{
	  if (oldroad[i-1] == 1)
	    { 
	      newroad[i] = 1;  
	    }
	  else
	    {
	      newroad[i] = 0; 
	    }
	}
    }

  return nmove;
}


void updatebcs(int *road, int n)
{
  road[0]   = road[n];
  road[n+1] = road[1];
}


void mpupdatebcs(int *road, int n, int procid, int nproc)
{
  int procup, procdown;

  procup   = (procid + 1) % nproc;
  procdown = (procid + nproc - 1) % nproc;

  mpsendrecv(&road[n], procup,   &road[0],   procdown);
  mpsendrecv(&road[1], procdown, &road[n+1], procup  );
}
