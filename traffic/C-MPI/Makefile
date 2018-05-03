MF=	Makefile

# For morar
#CC=	mpicc

# For ARCHER
CC=	cc

CFLAGS=
LFLAGS=	-lm

EXE=	traffic

INC= \
	traffic.h \
	uni.h \
	mplib.h

SRC= \
	traffic.c \
	trafficlib.c \
	uni.c \
	mplib.c

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
	$(CC) $(CFLAGS) -o $@ $(OBJ) $(LFLAGS)

$(OBJ):	$(MF)

clean:
	rm -f $(OBJ) $(EXE) core
