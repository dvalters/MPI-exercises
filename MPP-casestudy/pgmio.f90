!
!  This file contains Fortran routines for the MPI Casestudy.
!
!  "pgmread" reads in a PGM picture and can be called as follows:
!
!     real buf(M,N)
!     call pgmread('edge.pgm', buf)
!
!  "pgmwrite" writes an array as PGM picture and can be called as follows:
!
!     real buf(M,N)
!     call pgmwrite('picture.pgm', buf)
!
!  "pgmsize" returns the size of a PGM picture and can be called as follows:
!
!     integer :: nx, ny
!     call pgmsize('edge.pgm', nx, ny)
!
!  To access these routines, add the following to your program:
!
!     use pgmio
!

module pgmio

implicit none

integer, parameter, private :: iounit = 10
integer, parameter, private :: realnumber=kind(1.0e0)

real(kind=realnumber), parameter, private :: thresh = 255.0


contains


!  Routine to get the size of a PGM image
!  Note that this assumes a single line comment and no other white space

subroutine pgmsize(filename, nx, ny)

  integer :: nx, ny

  character*(*) :: filename

  open(unit=iounit, file=filename)

  read(iounit,*)
  read(iounit,*)
  read(iounit,*) nx, ny

  close(unit=iounit)

end subroutine pgmsize


!  Routine to read a PGM file into a 2D floating-point array
!  Note that this assumes a single line comment and no other white space


subroutine pgmread(filename, x)

  character*(*) :: filename
  integer :: nx, ny, nxt, nyt
  real(kind=realnumber), dimension(:,:) :: x

  integer i, j

  nx = size(x,1)
  ny = size(x,2)

  write(*,*) 'Reading ', nx, ' x ', ny, ' picture from: ', filename

  open(unit=iounit, file=filename)

  read(iounit,*)
  read(iounit,*)
  read(iounit,*) nxt, nyt

  if (nx .ne. nxt .or. ny .ne. nyt) then
    write(*,*) 'pgmread: size mismatch, (nx,ny) = (', nxt, ',', nyt, &
               ') expected (', nx, ',', ny, ')'
    stop
  end if

  read(iounit,*)
  read(iounit,*) ((x(i,ny-j+1), i=1,nx), j=1,ny)

  close(unit=iounit)

end subroutine pgmread


!  Routine to write a PGM image file from a 2D floating-point array
!  To cope with negative numbers we take the absolute value
!  Output values are scaled to lie between 0 and thresh

subroutine pgmwrite(filename, x)

  implicit none

  character*(*) :: filename
  integer :: nx, ny

  real(kind=realnumber), dimension(:,:) :: x

  real(kind=realnumber) :: tmin, tmax

  integer :: i, j

  nx = size(x,1)
  ny = size(x,2)

  write(*,*) 'Writing ', nx, ' x ', ny, ' picture into: ', filename

  open(unit=iounit, file=filename)

  tmin = minval(abs(x(:,:)))
  tmax = maxval(abs(x(:,:)))

  if (tmin == tmax) tmin = tmax-1

  write(iounit,fmt='(''P2''/''# Written by pgmio::pgmwrite'')')
  write(iounit,fmt='(i4, '' '', i4)') nx, ny

  write(iounit,*) int(thresh)
  write(iounit,fmt='(18(i3,'' ''))') &
       ((int(thresh*(abs(x(i,ny-j+1))-tmin)/(tmax-tmin)+0.5), i=1,nx), j=1,ny)

  close(unit=iounit)

end subroutine pgmwrite

end module pgmio
