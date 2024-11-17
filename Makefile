CC = gcc
CFLAGS = -Wall -g
OBJ = main.o proc_reader.o sort.o system_reader.o

all: systop

systop: $(OBJ)
	$(CC) $(CFLAGS) -o systop $(OBJ)

main.o: main.c process.h proc_reader.h sort.h system_reader.h
	$(CC) $(CFLAGS) -c main.c

proc_reader.o: proc_reader.c proc_reader.h process.h
	$(CC) $(CFLAGS) -c proc_reader.c

sort.o: sort.c sort.h process.h
	$(CC) $(CFLAGS) -c sort.c

system_reader.o: system_reader.c system_reader.h
	$(CC) $(CFLAGS) -c system_reader.c

clean:
	rm -f *.o systop
