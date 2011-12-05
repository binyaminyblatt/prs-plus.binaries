OBJS=LnkParser.o unicode.o mkcramfs.o
CC=cc
CFLAGS=-lz

all : mkcramfs cramfsck

mkcramfs : ${OBJS}
	${CC} ${CFLAGS} ${OBJS} -o $@

cramfsck : 
	${CC} ${CFLAGS} $@.c -o $@

%.o : %.c
	${CC} -c $^
	
clear:
	rm -f *.o mkcramfs cramfsck
