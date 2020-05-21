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
  int nodes_inside;
  union {
    struct QD_Node_*quadrants[4];    // internal node
    struct {
      SHIP* ship;  // apontador barco
      POINT* p;     // (x,y) board
    } leaf;        //  cell version for qtree
  } node;
} QD_NODE;

POINT make_point(int x,int y){    //constrói o ponto (x,y) e o retorna;
  POINT p;
  p.x = x;
  p.y = y;
  return p;
}
//quadrants [‘NW’,‘NE’,‘SW’,‘SE’]
QD_NODE * get_subdivision(int x,int y, int l, QD_NODE* root){
  if(root->type == QDLEAF) return q;
  l/=2;
  if(x>=l){
    if(y>=l) return get_subdivision(x,y,l,root->node.quadrants[3]);
    //y<l
    else return get_subdivision(x,y,l,root->node.quadrants[2]);
  }
  else{ //x<l
    if(y>=l) return get_subdivision(x,y,l,root->node.quadrants[1]);
    //y<l
    else return get_subdivision(x,y,l,root->node.quadrants[0]);
  }
}

//divide sections in 4 sub-sections until 2 points dont match the same division
void divide_insert(POINT p1,POINT p2,QD_NODE* root, int x, int y, int l){
  QD_NODE* q1 = (QD_NODE*) malloc (sizeof(QD_NODE));
  QD_NODE* q2 = (QD_NODE*) malloc (sizeof(QD_NODE));
  do{
    l/=2;
    
    q1 = get_subdivision(x,y,l,root);
    q2 = get_subdivision(x,y,l,root);
  }while(q1 == q2);
  free(q1);
  free(q2);
}



int node_insert(POINT p,QD_NODE* root,int x,int y,int l){
  //tree is empty
  if(root == NULL){r=p;return 1;}
  //at node leaf to insert
  else if(root->type == QDLEAF){
    if(root->node.leaf->p == null){
      root->node.leaf->p.x = p.x;
      root->node.leaf->p.y = p.y;
      printf("Valid insert.(%d,%d)\n",x,y);
      return 1;
    }
    else{ //create sub-tree divisions
      divide_insert(p,root->node.leaf->p,root,x,y,l);
      root->type = QDNODE;
      root->nodes_inside += 1;
    }
  }

  return 0;
}

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
