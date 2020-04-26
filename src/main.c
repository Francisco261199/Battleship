#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <ncurses.h>
#include "ships.h"
#include "board.h"
#include <time.h>

int main(){
srand(time(0));
int n;
printf("Selecione o tamanho do mapa\n");
scanf("%d",&n);
system("clear");

GAME* gameboard = init_board(n);
//SHIP* boat1 = create_ship(1,7);
//SHIP* boat2 = (SHIP*) malloc(sizeof(SHIP));
//SHIP* boat3 = (SHIP*) malloc(sizeof(SHIP));
//create_ship(boat2,1,2);
//create_ship(boat3,0,2);
SHIP** boat;

//rand_insert_ship(gameboard->size,gameboard->map1,7,boat);
//print_boat(boat2);
//print_boat(boat3);

//insert_ship(12,15,boat1,gameboard->map1,gameboard->size,1);
//insert_ship(9,3,boat2,gameboard->map1,gameboard->size);
//insert_ship(10,13,boat3,gameboard->map1,gameboard->size);
print_game(1,gameboard);

//attack(10,3,gameboard->map1,gameboard->size);
//printf("...\n");
//print_game(1,gameboard);

//attack(10,4,gameboard->map1,gameboard->size);
//printf("...\n");
//print_game(1,gameboard);

//attack(10,4,gameboard->map1,gameboard->size);
//print_game(1,gameboard);
//attack(13,21,gameboard->map1);
//print_game(1,gameboard);

printf("\n");

//erase_game(gameboard);

return 0;
}