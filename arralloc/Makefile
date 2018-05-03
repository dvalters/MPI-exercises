MF=	Makefile

CC=	pgcc
CFLAGS=

LFLAGS= $(CFLAGS)

EXE=	arralloctest

INC= \
	arralloc.h

SRC= \
	arralloctest.c \
	arralloc.c \

#
# No need to edit below this line
#

.SUFFIXES:
.SUFFIXES: .c .o

OBJ=	$(SRC:.c=.o)

.c.o:
	$(CC) $(CFLAGS) -c $<

all:	$(EXE)

$(OBJ):	$(INC)

$(EXE):	$(OBJ)
	$(CC) $(LFLAGS) -o $@ $(OBJ)

$(OBJ):	$(MF)

clean:
	rm -f $(EXE) $(OBJ) core
