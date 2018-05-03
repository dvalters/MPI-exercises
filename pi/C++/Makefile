MF=	Makefile

CC=	mpicxx
CFLAGS=

LFLAGS=	-lm

EXE=	piparallel

SRC= \
	piparallel.cc

#
# No need to edit below this line
#

.SUFFIXES:
.SUFFIXES: .cc .o

OBJ=	$(SRC:.cc=.o)

.cc.o:
	$(CC) $(CFLAGS) -c $<

all:	$(EXE)

$(EXE):	$(OBJ)
	$(CC) $(CFLAGS) -o $@ $(OBJ) $(LFLAGS)

$(OBJ):	$(MF)

clean:
	rm -f $(OBJ) $(EXE) core
