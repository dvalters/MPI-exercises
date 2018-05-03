program traffic

  use trafficlib

  implicit none

! Set the size of the road

  integer :: ncell = 100000
  integer :: maxiter, printfreq

  integer procid, nproc, nlocal

  integer :: i, iter, nmove, ncars
  real    :: density

  integer, allocatable, dimension(:) :: newroad, oldroad, bigroad

  double precision :: tstart, tstop

  maxiter = 200000000/ncell
  printfreq = maxiter/10

! Set target density of cars

  density = 0.52

! Start message passing system

  call mpstart(nproc, procid)

  nlocal = NCELL/nproc

  if (procid == 0) then

     write(*,*) 'Length of road is ', NCELL
     write(*,*) 'Number of iterations is ', maxiter
     write(*,*) 'Target density of cars is ', density
     write(*,*) 'Running on ', nproc, ' processes'

  end if

! Allocate arrays

  allocate(newroad(0:nlocal+1))
  allocate(oldroad(0:nlocal+1))

  if (procid == 0) then

     allocate(bigroad(NCELL))

     ! Initialise road accordingly using random number generator

     write(*,*) 'Initialising ...'

     ncars = initroad(bigroad, NCELL, density, seed)

     write(*,*) '... done'

     write(*,*) 'Actual density of cars is ', float(ncars)/float(NCELL)
     write(*,*) 'Scattering data ...'

  end if

  call mpscatter(bigroad, oldroad(1:nlocal), nlocal)

  if (procid == 0) then

     write(*,*) '... done'
     write(*,*)

  end if

  tstart = gettime()

  do iter = 1, maxiter

     call mpupdatebcs(oldroad, nlocal, procid, nproc)

     nmove = updateroad(newroad, oldroad, nlocal)

! Globally sum the value

     call mpgsum(nmove)

! Copy new to old array

     do i = 1, nlocal

        oldroad(i) = newroad(i)

     end do

     if (mod(iter, printfreq) == 0) then

        if (procid == 0) write(*,*) 'At iteration ', iter, &
             ' average velocity is ', float(nmove)/float(ncars)
     end if

  end do

  tstop = gettime()

  deallocate(oldroad)
  deallocate(newroad)

  if (procid == 0) then

     deallocate(bigroad)

     write(*,*)
     write(*,*) 'Finished'
     write(*,*)
     write(*,*) 'Time taken was  ', tstop - tstart, ' seconds'
     write(*,*) 'Update rate was ', &
                 1.d-6*float(NCELL)*float(maxiter)/(tstop-tstart), ' MCOPs'
     write(*,*)

  end if

  call mpstop()

end program traffic
