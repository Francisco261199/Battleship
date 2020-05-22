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

//create point
POINT * make_point(int x,int y){    //constrói o ponto (x,y) e o retorna;
  POINT* p = (POINT*) malloc (sizeof(POINT));
  p->x = x;
  p->y = y;
  return p;
}

//quadrants [‘NW’,‘NE’,‘SW’,‘SE’]
QD_NODE * get_subdivision(int x,int y, int l, QD_NODE* root){
  if(root == NULL){
    printf("Not found!\n");
    return NULL;
  }
  if(root->type == QDLEAF) return root;
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

//create node that is empty
QD_NODE* create_node(){
    QD_NODE * new_node = (QD_NODE*)malloc(sizeof(QD_NODE));

    new_node->type = LEAF;
    new_node->nodes_inside = 0;
    new_node->node.leaf.p = NULL;
    new_node->node.leaf.ship = NULL;
    return new_node;
}

//expand tree(it is going to have atleast 2 nodes with data)
void expand_tree(QD_NODE* root){
  root->type = QDNODE;
  root->node.quadrants[0] = create_node();
  root->node.quadrants[1] = create_node();
  root->node.quadrants[2] = create_node();
  root->node.quadrants[3] = create_node();
  /*{
    free(root->node.leaf.ship);
    free(root->node.leaf.p);
    root->node.leaf.ship = NULL;
    root->node.leaf.p = NULL;
  }*/
}


//divide sections in 4 sub-sections until 2 points dont match the same division
void divide_insert(QD_NODE* daddy, QD_NODE* copy, QD_NODE* n, int l){
  QD_NODE* q1 = (QD_NODE*) malloc(sizeof(QD_NODE));
  QD_NODE* q2 = (QD_NODE*) malloc(sizeof(QD_NODE));
  do{
    expand_tree(daddy);
    l/=2;
    printf("copy(%d,%d)\n",copy->node.leaf.p->x,copy->node.leaf.p->y);
    printf("n(%d,%d)\n",n->node.leaf.p->x,n->node.leaf.p->y);
    q1 = get_subdivision(copy->node.leaf.p->x,copy->node.leaf.p->y,l,daddy);
    q2 = get_subdivision(n->node.leaf.p->x,n->node.leaf.p->y,l,daddy);
  }while(q1 == q2);
  *q1 = *copy;
  *q2 = *n;
  print_boat(q1->node.leaf.ship);
  print_boat(q2->node.leaf.ship);
  free(q1);
  free(q2);
  q1 = NULL;
  q2 = NULL;
}



int node_insert(QD_NODE* daddy,QD_NODE* n,int l){

  if(daddy == NULL){ //tree is empty
    printf("Error. Node not found");
    return 0;
  }

  if(daddy->type == QDLEAF){ //at node leaf to insert
    //no node inside leaf(insert)
    if(daddy->node.leaf.p == NULL){
      *daddy = *n;
      free(n);
      n = NULL;

      daddy->nodes_inside = 1;
      printf("Valid insert.(%d,%d)\n",daddy->node.leaf.p->x,daddy->node.leaf.p->y);
      return 1;
    }
    else{ //create sub-tree divisions
      QD_NODE* copy =(QD_NODE*) malloc(sizeof(QD_NODE));
      *copy = *daddy;
      divide_insert(daddy,copy,n,l);
      daddy->nodes_inside += 1;
      free(n);
      free(copy);
      n = NULL;
      copy = NULL;
      return 1;
    }
  }
  else{ //not at leaf node
    QD_NODE* aux = (QD_NODE*) malloc(sizeof(QD_NODE));
    aux = get_subdivision(n->node.leaf.p->x, n->node.leaf.p->y, l, daddy);
    l/=2;
    node_insert(aux,n,l);
    free(aux);
    free(n);
    aux = NULL;
    n = NULL;
    return 1;
  }

  return -1;
}

/*POINT make_point(int x,int y)  //constrói o ponto (x,y) e o retorna;

POINT add_point(POINT p, POINT q) //retorna a adição de p a q;

int isEqual(POINT p,POINT q) //verificar se as coordenadas coincidem;

inSquare(POINT p, POINT corner, int side) //verificar se p está no quadrado definido por (corner,side)
*/

int main(){
  //int l = 20;
  POINT* p = make_point(3,1);
  POINT* p1 = make_point(7,4);
  SHIP* boat1 =(SHIP*) malloc (sizeof(SHIP));
  create_ship(boat1,2,7);
  SHIP* boat2 =(SHIP*) malloc (sizeof(SHIP));
  create_ship(boat2,2,4);

  //print_boat(boat1);
  //printf("(%d,%d)\n",p->x,p->y);
  QD_NODE* root = create_node();

  root->node.leaf.p = p;
  root->node.leaf.ship = boat1;
  QD_NODE* d = create_node();

  d->node.leaf.p = p1;
  d->node.leaf.ship = boat2;
  node_insert(root,d,10);
  //printf("root:%p\n",root);
  //printf("d:%p\n",d);
  printf("root1:%p,\nroot2:%p,\nroot3:%p,\nroot4:%p\n",root->node.quadrants[0],root->node.quadrants[1],root->node.quadrants[2],root->node.quadrants[3]);
  print_boat(root->node.quadrants[0]->node.leaf.ship);
  print_boat(root->node.quadrants[2]->node.leaf.ship);

  printf("d:%p\n",d);
  //expand_tree(root);
  //QD_NODE* node = get_subdivision(p->x,p->y,l,root);
  //node_insert(root,node,l);


  return 0;
}
