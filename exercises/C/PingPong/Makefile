# Makefile for PingPong code

#
# Fortran compiler and options
#

#
# C compiler and options
#
CC=	/opt/usr/multi/multi_apps_bin/mpich2/bin/mpicc
LIB=	-lm

SRC=	pingpong.c 
#
# Object files
#
OBJ=$(SRC:.c=.o)

#
# Compile
#
pingpong:	$(OBJ)
	$(CC) -o $@ $(OBJ) $(LIB)

.c.o:
	$(CC) -c $<

#
# Clean out object files and the executable.
#
clean:
	rm *.o pingpong
