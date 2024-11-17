CC = gcc
CFLAGS = -Wall -g
OBJ = main.o proc_reader.o tree.o sort.o

all: systop

systop: $(OBJ)
	$(CC) $(CFLAGS) -o systop $(OBJ)

main.o: main.c process.h proc_reader.h tree.h sort.h
	$(CC) $(CFLAGS) -c main.c

proc_reader.o: proc_reader.c proc_reader.h process.h
	$(CC) $(CFLAGS) -c proc_reader.c

tree.o: tree.c tree.h process.h
	$(CC) $(CFLAGS) -c tree.c

sort.o: sort.c sort.h process.h
	$(CC) $(CFLAGS) -c sort.c

clean:
	rm -f *.o systop