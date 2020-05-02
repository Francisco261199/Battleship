
#ifndef SHIPS_H

#define SHIPS_H

#include <stdio.h>
#include <stdlib.h>

//estrutura do navio

typedef struct{
    //center coordinates of the boat in CELL
    int x;
    int y;
    char bitmap[5][5];
    int size;
}SHIP;

//boat types

extern SHIP* Pickaxe;
extern SHIP* Carrier;
extern SHIP* Battleship;
extern SHIP* Sigma;
extern SHIP* Cruiser;
extern SHIP* Destroyer;


void create_ship(SHIP* newship,int orient,int size);
void print_boat(SHIP* ship);

#endif

