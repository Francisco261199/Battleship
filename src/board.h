#ifndef BOARD_H
#define BOARD_H

#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <ncurses.h>
#include "ships.h"
#include <time.h>

/*MACROS*/


//Flags for bitmap
#define EMPTY '0'
#define NOT_HIT '1'
#define HIT '2'

//flags for CELL maps
#define _NO_SHOT '0'
#define _NO_HIT '1'
#define _HIT_CELL '2'
#define _MISSED_SHOT '3'

//Structs

typedef struct{
  SHIP *ship;
  char shot;
} CELL;


// both boards,state for each player(nº of boats remaining),size of map
typedef struct{
    CELL **map1;
    CELL **map2;
    int size;
    int state1,state2;
} GAME;


// game functions

GAME * init_board(int size);
int verify_insert(int x,int y,SHIP* ship,CELL** map, int map_size);

void insert_ship(int x,int y,SHIP* ship,CELL** map, int map_size);

void rand_insert_ships(GAME* gameboard);

void user_insert(GAME* gameboard);

int attack(int x,int y, CELL **map,int size);

void print_game(CELL** map,int size);

int read_buffer();

void print_secret_board(CELL** map, int size);

GAME* erase_game(GAME* board);

#endif
