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
  QD_TNODE  type;
  union {
    struct QD_Node_ *quadrants[4];    // internal node
    struct {
      SHIP* ship;  // apontador barco
      POINT p;     // (x,y) board
    } leaf;        //  cell version for qtree
  } node;
} QD_NODE;


//quadrants [‘NW’,‘NE’,‘SW’,‘SE’]
QD_NODE * get_quadrant(int x,int y, int l, QD_NODE* q){
  if(q->type == QDLEAF) return q;
  l/=2;
  if(x>=l){
    if(y>=l) return get_quadrant(x,y,l,q->node.quadrants[3]);
    //y<l
    else return get_quadrant(x,y,l,q->node.quadrants[2]);
  }
  else{ //x<l
    if(y>=l) return get_quadrant(x,y,l,q->node.quadrants[1]);
    //y<l
    else return get_quadrant(x,y,l,q->node.quadrants[0]);
  }
}
POINT make_point(int x,int y){    //constrói o ponto (x,y) e o retorna;
  POINT p;
   p.x = x;
   p.y=y;
  
  return p;
}
/*POINT add_point(POINT p, POINT q) //retorna a adição de p a q;

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
