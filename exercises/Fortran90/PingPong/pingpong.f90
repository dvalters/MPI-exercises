!
! Program in which 2 processes repeatedly pass a message back and forth
!
! The same data is sent from A to B, then returned from B to A.
!

program pingpong
implicit none
include 'mpif.h'

integer :: ierr, size, rank, comm, i, length, numiter
integer :: status(MPI_STATUS_SIZE)
integer :: tag1, tag2, extent
character*10 temp_char10
integer :: iargc
real, allocatable :: sbuffer(:)
double precision :: tstart, tstop, time, totmess

comm = MPI_COMM_WORLD
tag1 = 1
tag2 = 2

call MPI_INIT(ierr)
call MPI_COMM_RANK(comm,rank,ierr)
call MPI_COMM_SIZE(comm,size,ierr)

if (iargc() /= 2) then
 if (rank .eq. 0) then
   write(*,*) 'Usage: pingpong <array length> <number of iterations>'
 end if

 call mpi_finalize(ierr)
 stop
end if


if (rank.gt.1) then
 print*, 'Rank not participating', rank
end if


if (rank .eq. 0) then 
 call getarg(1,temp_char10)
 read(temp_char10,*) length
 call getarg(2,temp_char10)
 read(temp_char10,*) numiter

 print*, 'Array length, number of iterations = '
 print*,  length, numiter
end if

call MPI_BCAST(length,1,MPI_INTEGER,0,comm,ierr)
call MPI_BCAST(numiter,1,MPI_INTEGER,0,comm,ierr)

! Must be run on at least 2 processors
if(size.lt.2)then
 if(rank.eq.0) write(*,*) ' The code must be run on at least 2 processors.'
 call MPI_FINALIZE(ierr)
 stop
endif

! Allocate array
allocate(sbuffer(length))

! Send 'buffer' back and forth between rank 0 and rank 1.
do i=1,length
 sbuffer(i) = rank + 10.d0
enddo

! Start timing the parallel part here.
call MPI_BARRIER(comm,ierr)                                           
tstart = MPI_Wtime()

do i=1,numiter
 if (rank.eq.0)then
  call MPI_SSEND(sbuffer(1),length,MPI_REAL,1,tag1,comm,ierr)
  call MPI_RECV(sbuffer(1),length,MPI_REAL,1,tag2,comm,status,ierr)

 else if (rank.eq.1)then
  call MPI_RECV(sbuffer(1),length,MPI_REAL,0,tag1,comm,status,ierr)
  call MPI_SSEND(sbuffer(1),length,MPI_REAL,0,tag2,comm,ierr)
 endif
enddo


tstop = MPI_Wtime()
time  = tstop - tstart

call MPI_TYPE_SIZE(MPI_REAL,extent,ierr)

if(rank.eq.0)then
 totmess = 2.d0*extent*length/1024.d0*numiter/1024.d0
 write(*,*) ' Ping-Pong of twice ',extent*length,' bytes, for ',numiter,' times.'
 write(*,*) 'Total computing time is ',time,' [s].'
 write(*,*) 'Total message size is ',totmess,' [MB].'
 write(*,*) 'Latency (time per message) is ', time/numiter*0.5d0,'[s].'
 write(*,*) 'Bandwidth (message per time) is ',totmess/time,' [MB/s].'

 if(time.lt.1.d0)then
  write(*,*) 'WARNING: The time is too short to be meaningful, increase the number of iterations and/or the array size so time is at least one second!'
    
    
 endif
endif

deallocate(sbuffer)

call MPI_FINALIZE(ierr)

end program pingpong
