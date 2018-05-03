      program hello

      implicit none

      include 'mpif.h'
 
      integer ierr

      call MPI_INIT(ierr)

      write(*,*) 'Hello World!'

      call MPI_FINALIZE(ierr)

      end
