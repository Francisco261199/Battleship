#ifndef MENUS_H
#define MENUS_H


#include <unistd.h>
#include <stdlib.h>
#include <ncurses.h>
#include "ships.h"
//#include "board.h"


// MACROS

#define START_MENU 0
#define START_GAME 1
#define CREDITS 2
#define LEAVE 3
#define BACK 4
#define START_GAME_END 5
#define RANDOM_INSERTION 6
#define SELECTED_INSERTION 7
#define ERROR 404


//variables
#ifdef VARS
int n_boats,map_size,rand_flag;
int n_for_each_boat[6];
#else
extern int n_boats,map_size,rand_flag;
extern int n_for_each_boat[6];
#endif
//functions
int menu();

#endif