CC = gcc
CFLAGS = -Wall -g
OBJ = main.o hash_table.o proc_reader.o sort.o

all: systop

systop: $(OBJ)
	$(CC) $(CFLAGS) -o systop $(OBJ)

main.o: main.c process.h hash_table.h proc_reader.h sort.h
	$(CC) $(CFLAGS) -c main.c

hash_table.o: hash_table.c hash_table.h process.h
	$(CC) $(CFLAGS) -c hash_table.c

proc_reader.o: proc_reader.c proc_reader.h process.h hash_table.h
	$(CC) $(CFLAGS) -c proc_reader.c

sort.o: sort.c sort.h process.h
	$(CC) $(CFLAGS) -c sort.c

clean:
	rm -f *.o systop
