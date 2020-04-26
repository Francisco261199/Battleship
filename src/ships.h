
#ifndef SHIPS_H

#define SHIPS_H

#include<stdio.h>
#include<stdlib.h>

//macros

#define NORTH 0
#define SOUTH 1
#define WEST  2
#define EAST  3

//estrutura do navio

typedef struct{
    //center coordinates of the boat in CELL
    int x;
    int y;
    char bitmap[5][5];
    int size;
}SHIP;

//tipos de navio

extern SHIP* Pickaxe;
extern SHIP* Carrier;
extern SHIP* Battleship;
extern SHIP* Sigma;
extern SHIP* Cruiser;
extern SHIP* Destroyer;

// criação do navio

void create_ship(SHIP* newship,int orient,int size);

#endif
