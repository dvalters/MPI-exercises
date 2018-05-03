# Makefile for Collective Communications code

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
OBJ=	collcomms.o 

#
# Compile
#
collcomms:	$(OBJ)
	$(CC) -o $@ $(OBJ) $(LIB)

.c.o:
	$(CC) -c $<

#
# Clean out object files and the executable.
#
clean:
	rm *.o collcomms

