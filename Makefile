CC = gcc
CFLAGS = -Wall -g
OBJ = main.o proc_reader.o print.o sort.o control.o system_reader.o terminal_ui.o

all: systop

systop: $(OBJ)
	$(CC) $(CFLAGS) -o systop $(OBJ) -lncursesw
	@echo "시스템 우선순위 조정을 위해 Root 권한이 필요합니다: sudo ./systop"

main.o: main.c process.h proc_reader.h print.h sort.h system_reader.h control.h terminal_ui.h
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

terminal_ui.o: terminal_ui.c terminal_ui.h process.h
	$(CC) $(CFLAGS) -c terminal_ui.c

clean:
	rm -f $(OBJ) systop
