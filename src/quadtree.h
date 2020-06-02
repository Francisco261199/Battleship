#ifndef BATTLESHIP_H
#define BATTLESHIP_H

#include <stdio.h>
#include <stdlib.h>
#include "ships.h"
#include <time.h>

//Flags for bitmap
#define EMPTY '0'
#define NOT_HIT '1'
#define HIT '2'

//flags for CELL maps
#define _NO_SHOT '0'
#define _NO_HIT '1'
#define _HIT_CELL '2'
#define _MISSED_SHOT '3'

typedef struct POINT{
  int x,y;
}POINT;

typedef enum { QDNODE, QDLEAF} QD_TNODE;

typedef struct QD_Node_ {
  QD_TNODE  type;
  int nodes_inside;
  float cx,cy,level;
  struct QD_Node_* parent;
  union {
    struct QD_Node_*quadrants[4];    // internal node
    struct {
      SHIP* ship;  // apontador barco
      POINT* p;     // (x,y) board
    } leaf;        //  cell version for qtree
  } node;
} QD_NODE;







POINT * make_point(int x,int y);
void print_tree(QD_NODE* root, int l);
QD_NODE* get_parent(QD_NODE* q);
QD_NODE* get_last_child(QD_NODE* e);
QD_NODE* create_node(float level);
void node_clean(QD_NODE* e);
void expand_tree(QD_NODE* root);
QD_NODE* get_next(int x,int y,QD_NODE* save,QD_NODE* root, int flag);
QD_NODE * get_subdivision(int x,int y,float cx,float cy,float l,QD_NODE* root,int flag);
void divide_insert(QD_NODE* root, QD_NODE* copy, QD_NODE* n);
int node_insert(QD_NODE* root,QD_NODE* n);
int node_delete(QD_NODE* root, int x, int y);
int search_point(POINT* p,QD_NODE* root);

#endif
