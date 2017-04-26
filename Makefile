all: obj.o heap.o

obj.o:
	gcc -c obj.c

heap.o:
	gcc -c heap.c `pkg-config --libs --cflags glib-2.0`

clean:
	rm *.o

