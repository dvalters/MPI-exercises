program hello

  use mpi

  implicit none

  integer :: ierr

  call MPI_Init(ierr)

  write(*,*) 'Hello World!'

  call MPI_Finalize(ierr)

end program hello
