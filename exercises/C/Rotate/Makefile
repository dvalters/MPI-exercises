# Makefile for Rotate code

#
# Fortran compiler and options
#

#
# C compiler and options
#
CC=	mpicc
LIB=	-lm

#
# Object files
#
OBJ=	rotate.o 

#
# Compile
#
rotate:	$(OBJ)
	$(CC) -o $@ $(OBJ) $(LIB)

.c.o:
	$(CC) -c $<

#
# Clean out object files and the executable.
#
clean:
	rm *.o rotate core

