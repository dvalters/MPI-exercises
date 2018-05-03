module mplib

  use mpi

  implicit none

  integer :: ierror

contains

subroutine mpstart(numproc, procid)

  integer :: numproc, procid

  call MPI_Init(ierror)
  call MPI_Comm_rank(MPI_COMM_WORLD, procid, ierror)
  call MPI_Comm_size(MPI_COMM_WORLD, numproc, ierror)

end subroutine mpstart

subroutine mpstop

  call MPI_Finalize(ierror)

end subroutine mpstop
 
subroutine mpgsum(ival)

  integer :: ival, itmp
 
  call MPI_Allreduce(ival, itmp, 1, MPI_INTEGER, MPI_SUM, &
                     MPI_COMM_WORLD, ierror)

  ival = itmp

end subroutine mpgsum

subroutine mpscatter(inbuff, outbuff, ndata)

  integer :: ndata

  integer, dimension(ndata) :: outbuff
  integer, dimension(:)     :: inbuff

  call MPI_Scatter(inbuff,  ndata, MPI_INTEGER, &
	           outbuff, ndata, MPI_INTEGER, &
                   0, MPI_COMM_WORLD, ierror)

end subroutine mpscatter

subroutine mpsendrecv(senddata, sendproc, recvdata, recvproc)

  integer :: senddata, sendproc, recvdata, recvproc
  integer, dimension(MPI_STATUS_SIZE) :: status

  call MPI_Sendrecv(senddata, 1, MPI_INTEGER, sendproc, 1, &
                    recvdata, 1, MPI_INTEGER, recvproc, 1, &
                    MPI_COMM_WORLD, status, ierror)
 
end subroutine mpsendrecv

double precision function gettime()

  gettime = MPI_Wtime()

end function gettime

end module mplib
