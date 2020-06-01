#ifndef BOARDQ_H
#define BOARDQ_H

#include <stdio.h>
#include <stdlib.h>
#include "quadtree."
#include "ships.h"
#include "time.h"



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







extern char *info1;
extern char *info2;
