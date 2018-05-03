!
! Prints 'Hello World' from rank 0 and                                                                       ! prints what processor it is out of the total number of processors from all ranks   
!

program helloworld
implicit none
include  "mpif.h"

integer :: comm, rank, size, ierr

comm = MPI_COMM_WORLD

call MPI_INIT(ierr)

call MPI_COMM_RANK(comm, rank, ierr)
call MPI_COMM_SIZE(comm, size, ierr)

! Each process prints out its rank
write(*,*) 'I am ', rank, 'out of ', size,' processors.'

! Only processor 0 prints 
if(rank.eq.0) write(*,*) 'Hello World!.'

call MPI_FINALIZE(ierr)

end program helloworld
