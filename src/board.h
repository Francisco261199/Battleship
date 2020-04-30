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

//Estruturas

typedef struct{
  SHIP *ship;
  char shot;
  //int shot_count;
} CELL;


// estrutura do jogo com 2 tabuleiros, tamanho do tabuleiro e estado do jogo ( nº de navios)
typedef struct{
    CELL **map1;
    CELL **map2;
    int size;
    int state1,state2;
} GAME;


// funções de jogo

GAME * init_board(int size);

void insert_ship(int x,int y,SHIP* ship,CELL** map, int map_size);

void rand_insert_ships(GAME* gameboard,int nships);

void attack(int x,int y, CELL **map,int size,int state);

void print_game(int player,GAME * b);

void print_secret_board(CELL **map,int size);

GAME* erase_game(GAME* board);
#endif
