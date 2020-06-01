battleship: main.o board.o ships.o menus.o
	gcc -o battleship main.o board.o ships.o menus.o -lncurses
quadtree: mainq.o boardq.o quadtree.o ships.o menus.o
	gcc -o battleship mainq.o boardq.o quadtree.o ships.o menus.o -lncurses
main.o: main.c board.h ships.h menus.h
	gcc -c main.c
mainq.o: mainq.c boardq.h ships.h quadtree.h menus.h
	gcc -c mainq.c
boardq.o: boardq.c quadtree.h
	gcc -c boardq.c
board.o: board.c menus.h ships.h
	gcc -c board.c
quadtree.o: quadtree.c
	gcc -c quadtree.c
ships.o: ships.c
	gcc -c ships.c
menus.o: menus.c
	gcc -c menus.c -lncurses
clean:
	rm -rf battleship *.o 
