battleship: main.o menus.o board.o ships.o
	gcc -o battleship main.o board.o ships.o -lncurses
main.o: main.c menus.h board.h ships.h
	gcc -c main.c
menus.o: menus.c board.h ships.h
	gcc -c menus.c -lncurses
board.o: board.c ships.h
	gcc -c board.c
ships.o: ships.c
	gcc -c ships.c
clean:
	rm battleship main.o menus.o board.o ships.o 
