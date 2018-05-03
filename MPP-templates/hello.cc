#include "mpi.h"
#include <iostream>

using namespace std;

int main()
{
  MPI::Init();

  cout << "Hello World!" << endl;

  MPI::Finalize();
}
