!
! perform a global sum of data stored on each process by rotating each piece of data all the
! way round the ring. At each iteration, a process receives some data from the left, adds the 
! value to its running total, then passes the data it has just received on to the right.
!
! Start from code in ../rotate.rotate.f90
!
! Now using an MPI reduction operation to perform the global sum
!

program rotate
implicit none
include 'mpif.h'

integer :: comm, ierr, rank, size, request
integer :: send_status(MPI_STATUS_SIZE), recv_status(MPI_STATUS_SIZE)
integer :: sum, i, passon

comm = MPI_COMM_WORLD

call MPI_INIT(ierr)
call MPI_COMM_RANK(comm,rank,ierr)
call MPI_COMM_SIZE(comm,size,ierr)

sum = 0
! Initialise local values to:
passon = (rank+1)**2
! Alternatively use
! passon = rank

! Use collective reduction operation to perform global sum

!call MPI_REDUCE(passon,sum,1,MPI_INTEGER,MPI_SUM,0,comm,ierr)
call MPI_ALLREDUCE(passon,sum,1,MPI_INTEGER,MPI_SUM,comm,ierr)

write(*,*)'The sum is:', sum,' on processor ',rank

call MPI_FINALIZE(ierr)

end program rotate
