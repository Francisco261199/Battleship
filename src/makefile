battleship: main.o board.o ships.o menus.o
	gcc -o battleship main.o board.o ships.o menus.o -lncurses
main.o: main.c board.h ships.h menus.h
	gcc -c main.c
board.o: board.c menus.h ships.h
	gcc -c board.c
ships.o: ships.c
	gcc -c ships.c
menus.o: menus.c
	gcc -c menus.c -lncurses
clean:
	rm battleship main.o menus.o board.o ships.o 