!
! Rotate exercise from ../Rotate/rotate.f90 with derived data types
!
! Using MPI_GET_ADDRESS and MPI_TYPE_CREATE_STRUCT (MPI2)
! instead of MPI_ADDRESS and MPI_TYPE_COMMIT (MPI1)
!

program derdaty
implicit none
include 'mpif.h'

type compound
 integer :: ival
 double precision :: dval
end type compound 

! introduce new variables related to derived datatypes
type(compound) :: passon,addon,sum
integer :: blocklen(2), type(2)
integer (MPI_ADDRESS_KIND), dimension(2) :: disp
integer (MPI_ADDRESS_KIND) :: base
integer :: newtype

! old variables as already used for the rotate.f90 code
integer :: comm, ierr, rank, size, request
integer :: send_status(MPI_STATUS_SIZE), recv_status(MPI_STATUS_SIZE)
integer :: left, right, i, tag

comm = MPI_COMM_WORLD
tag = 1

call MPI_INIT(ierr)
call MPI_COMM_RANK(comm,rank,ierr)
call MPI_COMM_SIZE(comm,size,ierr)

! Must be run on more than 1 processor
if(size.eq.1)then
 write(*,*) 'Must be run on more than 1 processor.'
 call MPI_FINALIZE(ierr)
 stop
endif
 
! Create all necessary info for the derived datatype
call MPI_GET_ADDRESS(passon%ival,disp(1),ierr)
call MPI_GET_ADDRESS(passon%dval,disp(2),ierr)

base = disp(1)
disp(1) = disp(1) - base
disp(2) = disp(2) - base

blocklen(1) = 1
blocklen(2) = 1

type(1) = MPI_INTEGER
type(2) = MPI_DOUBLE_PRECISION

! Create the new datatype, called 'newtype' and commit it
call MPI_TYPE_CREATE_STRUCT(2,blocklen,disp,type,newtype,ierr)
call MPI_TYPE_COMMIT(newtype,ierr)

! Send current number to the right and receive from the left
left = rank - 1
right = rank + 1

if ((rank+1).eq.size)then
 right = 0
elseif(rank.eq.0)then
 left = size -1
endif

sum%ival = 0
sum%dval = 0.0

! Initialise local values to:
passon%ival = rank
passon%dval = (rank+1)**2

! Use non-blocking point-to-point communication 
do i=1,size

 call MPI_ISSEND(passon,1,newtype,right,tag,comm,request,ierr)
 call MPI_RECV(addon,1,newtype,left,tag,comm,recv_status,ierr)
 call MPI_WAIT(request,send_status,ierr)

sum%ival = sum%ival + addon%ival
sum%dval = sum%dval + addon%dval
passon%ival = addon%ival
passon%dval = addon%dval

enddo

write(*,*)'The sum is:', sum,' on processor', rank

call MPI_FINALIZE(ierr)

end program derdaty
