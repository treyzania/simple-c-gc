FLAGS=`pkg-config --libs --cflags glib-2.0`
OPTS=-O3

all: obj.o heap.o gc.o

obj.o:
	gcc ${OPTS} -c obj.c ${FLAGS}

heap.o:
	gcc ${OPTS} -c heap.c ${FLAGS}

gc.o:
	gcc ${OPTS} -c gc.c ${FLAGS}

clean:
	rm -f *.o
