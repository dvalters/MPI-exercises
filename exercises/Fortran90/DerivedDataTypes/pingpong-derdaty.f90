!
! Program in which 2 processes repeatedly pass a M rows of a NxN matrix back and forth
!
!

program pingpong
implicit none
include 'mpif.h'

integer :: ierr, size, rank, comm, i, length, j
integer :: status(MPI_STATUS_SIZE)
real :: buffer(100000)
integer, parameter :: iter =3      ! Number of ping pongs to be executed
integer, parameter :: M=2,N=4,ColStart=2  ! M number of rows to be sent together, N number of coulumns, ColStart start row
integer :: mrowtype                ! new type to be created
double precision, dimension(N,N) :: sendmatrix, recvmatrix

comm = MPI_COMM_WORLD

call MPI_INIT(ierr)
call MPI_COMM_RANK(comm,rank,ierr)
call MPI_COMM_SIZE(comm,size,ierr)

! Code  must only run on 2 processors.
if(size.ne.2)then
 if(rank.eq.0) write(*,*) ' The code can only be run on 2 processors.'
 call MPI_FINALIZE(ierr)
 stop
endif

! Number of rows must not exceed number of coulumns, and start row to be sent must not exceed N.
if(M.gt.N.or.ColStart.gt.N.or.(ColStart+M).gt.(N+1))then
 write(*,*) 'The number of rows MUST NOT be bigger than the number of coulumns: ',M, N, ColStart
 call MPI_FINALIZE(ierr)
 stop
endif

! Create new datatype mrowtype to send M rows and commit it.
call MPI_TYPE_VECTOR(N,M,N,MPI_DOUBLE_PRECISION,mrowtype,ierr)
call MPI_TYPE_COMMIT(mrowtype,ierr)

! Initialize matrix.
do j = 1, N
 do i = 1, N
  sendmatrix(i,j) = i*10**(j-1)+rank
  write(*,*)'Intitialized matrix on rank ',rank,':',i,j, sendmatrix(i,j)
 enddo
enddo

! E.g. sending rows 4, 5 and 6 of a matrix back and forth between rank 0 and rank 1
! corresponds to ColStart = 4, M = 3

do i = 1, iter
 if (rank.eq.0)then

  call MPI_SSEND(sendmatrix(ColStart,1),1,mrowtype,1,50,comm,ierr)
  call MPI_RECV(recvmatrix(ColStart,1),1,mrowtype,1,60,comm,status,ierr)

 else

  call MPI_RECV(recvmatrix(ColStart,1),1,mrowtype,0,50,comm,status,ierr)
  call MPI_SSEND(sendmatrix(ColStart,1),1,mrowtype,0,60,comm,ierr)

 endif

 sendmatrix(ColStart:ColStart+M,1:N) = recvmatrix(ColStart:ColStart+M,1:N)

enddo

! Matrix after iter ping-pongs
do j=1,N
 do i=1,N
  write(*,*) 'Matrix in the end on rank ',rank,':', i,j,sendmatrix(i,j)
 enddo
enddo

if(rank.eq.0)then
 write(*,30) M,N,N,ColStart,iter
endif

30 format (1x, 'Sending ', i3 ,'  column(s) of an ', i3, ' x ', i3, ' matrix, starting with column ', i3 ,' and sending it ',i6,' times.')

call MPI_FINALIZE(ierr)

end program pingpong
