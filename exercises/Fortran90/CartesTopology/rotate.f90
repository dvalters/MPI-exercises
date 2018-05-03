!
! perform a global sum of data stored on each process by rotating each piece of data all the
! way round the ring. At each iteration, a process receives some data from the left, adds the 
! value to its running total, then passes the data it has just received on to the right.
!
! Previous version in ../Rotate/rotate.f90
!
! Use virtual topologies, here cartesian 1D
!

program rotate
implicit none
include 'mpif.h'

integer :: comm, ierr, rank, size, request,tag
integer :: send_status(MPI_STATUS_SIZE), recv_status(MPI_STATUS_SIZE)
integer :: left, right, addon, sum, i, passon
! New variables required
integer :: ndims, comm1d, direction,disp
integer :: dims(1)
logical :: periods(1), reorder

comm = MPI_COMM_WORLD
tag = 1

call MPI_INIT(ierr)
call MPI_COMM_SIZE(comm,size,ierr)

! Cartesian topology
ndims = 1
dims(1) = 0
periods(1) = .true.       ! Cyclic
reorder = .false.
direction = 0             ! Shift along the first index
disp = 1                  ! Shift by 1

call MPI_DIMS_CREATE(size,1,dims,ierr)
call MPI_CART_CREATE(comm,ndims,dims,periods,reorder,comm1d,ierr)
call MPI_COMM_RANK(comm1d,rank,ierr)
call MPI_CART_SHIFT(comm1d,direction,disp,left,right,ierr)

sum = 0
! Initialise local values to:
passon = (rank+1)**2
! Alternatively use
! passon = rank

! Use non-blocking point-to-point communication 
do i=1,size

 call MPI_ISSEND(passon,1,MPI_INTEGER,left,tag,comm1d,request,ierr)
 call MPI_RECV(addon,1,MPI_INTEGER,right,tag,comm1d,recv_status,ierr)
 call MPI_WAIT(request,send_status,ierr)

sum = sum + addon
passon = addon
enddo

write(*,*)'The sum is:', sum,' on processor', rank

call MPI_FINALIZE(ierr)

end program rotate
