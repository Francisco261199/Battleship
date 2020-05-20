#include<stdio.h>
#include<stdlib.h>
//#include "quadtree.h"
#include "ships.h"

//Node identifiers
#define NON_LEAF -1
#define LEAF 1
#define EMPTY 2
#define NOT_EMPTY 3

typedef struct POINT{
  int x,y;
}POINT;

typedef enum { QDNODE, QDLEAF} QD_TNODE;

typedef struct QD_Node_ {
  QD_TNODE  type;    // defines the type of node (internal node or leaf)
  union {
    struct QD_Node_ *quadrants[4];    // internal node
    struct {
      SHIP* ship;  // apontador barco
      POINT p;     // coords of the board cell
    } leaf;        //  almost a board cell together with its coordinates
  } node;
} QD_NODE;

/*POINT make_point(int x,int y)  //constrói o ponto (x,y) e o retorna;

POINT add_point(POINT p, POINT q) //retorna a adição de p a q;

int isEqual(POINT p,POINT q) //verificar se as coordenadas coincidem;

inSquare(POINT p, POINT corner, int side) //verificar se p está no quadrado definido por (corner,side)
*/


int main(){
  //QTNode** map = (QTNode**) malloc(QT);
  //int
  //QTree* qtree = (QTree*) malloc(sizeof(QTree));
   //*qtree = *inicial;

  return 0;
}
