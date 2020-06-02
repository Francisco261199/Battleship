#ifndef BOARDQ_H
#define BOARDQ_H

#include <stdio.h>
#include <stdlib.h>
#include "quadtree.h"
//#include "ships.h"
#include <time.h>
//#include "menus.h"


//Flags for bitmap
#define EMPTY '0'
#define NOT_HIT '1'
#define HIT '2'

//flags for CELL maps
#define _NO_SHOT '0'
#define _NO_HIT '1'
#define _HIT_CELL '2'
#define _MISSED_SHOT '3'


typedef struct{
    QD_NODE* root1;
    QD_NODE* root2;

} GAME;





#ifdef INFO
char * info1;
char * info2;
#else
extern char *info1;
extern char *info2;
#endif


int read_buffer();
GAME* init_board(int size);
int verify_insert(QD_NODE* insert, QD_NODE* root, POINT* points);
void insert_ship(POINT* p,POINT* points, SHIP* ship, QD_NODE * root,char* info);
void rand_insert_ships(GAME *g);
void user_insert(GAME* g);
void print_secret_board(char* info,int size);
void wallhack(char *info,int size);
void user_insert(GAME* g);
int attack(int x, int y, QD_NODE* root, char *info);
GAME* erase_game(GAME* game);


#endif
