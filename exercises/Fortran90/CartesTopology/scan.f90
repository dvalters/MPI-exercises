!
! perform a global sum of data stored on each process by rotating each piece of data all the
! way round the ring. At each iteration, a process receives some data from the left, adds the 
! value to its running total, then passes the data it has just received on to the right.
!
! Use virtual topologies, here cartesian 1D, like in rotate.f90  and MPI_SCAN
!

program rotate
implicit none
include 'mpif.h'

integer :: comm, ierr, rank, size
integer :: sum, passon

comm = MPI_COMM_WORLD

call MPI_INIT(ierr)
call MPI_COMM_SIZE(comm,size,ierr)
call MPI_COMM_RANK(comm,rank,ierr)

sum = 0
! Initialise local values to:
passon = (rank+1)**2
! Alternatively use
! passon = rank

! Use MPI_Scan
call MPI_SCAN(passon,sum,1,MPI_INTEGER,MPI_SUM,comm,ierr)

write(*,*)'The sum is:', sum,' on processor', rank

call MPI_FINALIZE(ierr)

end program rotate
