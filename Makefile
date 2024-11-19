CC = gcc
CFLAGS = -Wall -g
OBJ = main.o proc_reader.o print.o sort.o control.o system_reader.o

all: systop

systop: $(OBJ)
	$(CC) $(CFLAGS) -o systop $(OBJ)

main.o: main.c process.h proc_reader.h print.h sort.h system_reader.h control.h
	$(CC) $(CFLAGS) -c main.c

proc_reader.o: proc_reader.c proc_reader.h process.h
	$(CC) $(CFLAGS) -c proc_reader.c

print.o: print.c print.h process.h
	$(CC) $(CFLAGS) -c print.c

sort.o: sort.c sort.h process.h
	$(CC) $(CFLAGS) -c sort.c

control.o: control.c control.h process.h
	$(CC) $(CFLAGS) -c control.c

system_reader.o: system_reader.c system_reader.h
	$(CC) $(CFLAGS) -c system_reader.c

clean:
	rm -f *.o systop
