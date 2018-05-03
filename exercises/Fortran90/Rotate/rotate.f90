!
! perform a global sum of data stored on each process by rotating each piece of data all the
! way round the ring. At each iteration, a process receives some data from the left, adds the 
! value to its running total, then passes the data it has just received on to the right.
!

program rotate
implicit none
include 'mpif.h'

integer :: comm, ierr, rank, size, request,tag
integer :: send_status(MPI_STATUS_SIZE), recv_status(MPI_STATUS_SIZE)
integer :: left, right, addon, sum, i, passon

comm = MPI_COMM_WORLD
tag = 1

call MPI_INIT(ierr)
call MPI_COMM_RANK(comm,rank,ierr)
call MPI_COMM_SIZE(comm,size,ierr)

! Send current number to the right and receive from the left
left = rank - 1
right = rank + 1

if ((rank+1).eq.size)then
 right = 0
endif
if(rank.eq.0)then
 left = size -1
endif

sum = 0
! Initialise local values to:
passon = (rank+1)**2
! Alternatively use
! passon = rank

! Use non-blocking point-to-point communication 
do i=1,size

 call MPI_ISSEND(passon,1,MPI_INTEGER,right,tag,comm,request,ierr)
 call MPI_RECV(addon,1,MPI_INTEGER,left,tag,comm,recv_status,ierr)
 call MPI_WAIT(request,send_status,ierr)

 sum = sum + addon
 passon = addon

enddo

write(*,*)'The sum is:', sum,' on processor', rank

call MPI_FINALIZE(ierr)

end program rotate
