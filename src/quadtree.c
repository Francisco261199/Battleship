
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
//struct QD_Node_;

typedef struct QD_Node_ {
  QD_TNODE  type;
  int nodes_inside;
  struct QD_Node_* parent;
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

//quadrants [‘NW’,‘NE’,‘SW’,‘SE’] x=lines y=cols
QD_NODE * get_subdivision(int x,int y, int l,int cx,int cy, QD_NODE* root){
  //printf("address:%p\nl:%d\n",root,l);
  //printf("type:%d\n", root->type);
  //printf("(%d,%d)\n",x,y);

  if(root == NULL){
    printf("Not found!\n");
    return NULL;
  }
  if(root->type == QDLEAF) return root;
  l/=2;
  //printf("l:%d\n",l);
  if(x>cx){
    if(y>cy){
      //printf("(%d,%d)\n",cx,cy);
      cx = cx+l;
      cy = cy+l;
      //printf("(%d,%d)\n",cx,cy);
      return get_subdivision(x,y,cx,cy,l,root->node.quadrants[3]);
   }
    //y<=l
    else{
      //printf("(%d,%d)\n",cx,cy);
      cx = cx+l;
      cy = cy-l;
      //printf("(%d,%d)\n",cx,cy);
      return get_subdivision(x,y,cx,cy,l,root->node.quadrants[2]);
    }
  }
  else{ //x<=l
    if(y>cy){
      //printf("(%d,%d)\n",cx,cy);
      cx = cx-l;
      cy = cy+l;
     //printf("(%d,%d)\n",cx,cy);
      return get_subdivision(x,y,cx,cy,l,root->node.quadrants[1]);
   }
    //y<=l
    else{
      //printf("(%d,%d)\n",cx,cy);
      cx = cx-l;
      cy = cy-l;
      //printf("(%d,%d)\n",cx,cy);
      return get_subdivision(x,y,cx,cy,l,root->node.quadrants[0]);
     }
  }
}

//create node that is empty
QD_NODE* create_node(){
    QD_NODE * new_node = (QD_NODE*)malloc(sizeof(QD_NODE));

    new_node->type = QDLEAF;
    new_node->nodes_inside = 0;
    new_node->parent = (QD_NODE*) malloc(sizeof(QD_NODE));
    new_node->node.leaf.p = (POINT*) malloc(sizeof(POINT));
    new_node->node.leaf.ship = (SHIP*) malloc(sizeof(SHIP));
    return new_node;
}

//expand tree(it is going to have atleast 2 nodes with data)
void expand_tree(QD_NODE* root){
  root->type = QDNODE;
  root->nodes_inside = 2;
  root->node.quadrants[0] = create_node();
  root->node.quadrants[1] = create_node();
  root->node.quadrants[2] = create_node();
  root->node.quadrants[3] = create_node();

  root->node.quadrants[0]->parent = root;
  root->node.quadrants[1]->parent = root;
  root->node.quadrants[2]->parent = root;
  root->node.quadrants[3]->parent = root;
}


//divide sections in 4 sub-sections until 2 points dont match the same division
void divide_insert(QD_NODE* root, QD_NODE* copy, QD_NODE* n, int l){
  QD_NODE* q1 = (QD_NODE*) malloc(sizeof(QD_NODE));
  QD_NODE* q2 = (QD_NODE*) malloc(sizeof(QD_NODE));
  q2 = q1 = root;
  l/=2;
  //int i = 1;
  while(q1 == q2){
    //printf("i:%d\n",i);
    //i++;
    expand_tree(q1);
    q2->type = QDNODE;
    q2->parent = q1;

    q1 = get_subdivision(copy->node.leaf.p->x,copy->node.leaf.p->y,l,l,l,root);
    q2 = get_subdivision(n->node.leaf.p->x,n->node.leaf.p->y,l,l,l,root);
    printf("(%d,%d),(%d,%d)\n",copy->node.leaf.p->x,copy->node.leaf.p->y,n->node.leaf.p->x,n->node.leaf.p->y);
    printf("q1:%p\n",q1);
    printf("q2:%p\n",q2);
    printf("---------\n");
  }
  printf("(%d,%d),(%d,%d)\n",copy->node.leaf.p->x,copy->node.leaf.p->y,n->node.leaf.p->x,n->node.leaf.p->y);
  printf("q1:%p\n",q1);
  printf("q2:%p\n",q2);
  //printf("ddd\n");
  *q1->node.leaf.ship = *copy->node.leaf.ship;
  *q1->node.leaf.p = *copy->node.leaf.p;

  *q2->node.leaf.ship = *n->node.leaf.ship;
  *q2->node.leaf.p = *n->node.leaf.p;
}

//get node q's parent
QD_NODE* get_parent(QD_NODE* q){
  return q->parent;
}

int node_insert(QD_NODE* root,QD_NODE* n,int l){
  if(root == NULL){ //tree is empty
    printf("Error. Node not found");
    return 0;
  }

  if(root->type == QDLEAF){ //at node leaf to insert
    //printf("flag1\n");
    //no node inside leaf(insert)
    if(root->node.leaf.p == NULL){
      //printf("flag1.1\n");
      *root = *n;
      n = root;
      //free(n);
      //n = NULL;
      root->parent = get_parent(root);
      root->nodes_inside = 1;
      printf("Valid insert.(%d,%d)\n",root->node.leaf.p->x,root->node.leaf.p->y);
      return 1;
    }
    else{ //create sub-tree divisions
      //printf("flag1.2\n");
      QD_NODE* copy =(QD_NODE*) malloc(sizeof(QD_NODE));
      *copy = *root;
      divide_insert(root,copy,n,l);
      root->type = QDNODE;
      free(copy);
      copy = NULL;
      return 1;
    }
  }
  else{ //not at leaf node
    //printf("flag2\n");
    QD_NODE* aux = (QD_NODE*) malloc(sizeof(QD_NODE));
    aux = get_subdivision(n->node.leaf.p->x, n->node.leaf.p->y,l/2,l/2,l/2, root);
    node_insert(aux,n,l/2);
    free(aux);
    aux = NULL;
    return 1;
  }
  return -1;
}

//eliminate subdivision
void colapse_division(QD_NODE* parent){
  for(int i=0;i<4;i++){
    //if we find last node on subdivision place it in root of that subdivision
    if(parent->node.quadrants[i]!=NULL && !(parent->node.quadrants[i]->type == QDNODE)){
      *parent->node.leaf.ship = *parent->node.quadrants[i]->node.leaf.ship;
      *parent->node.leaf.p = *parent->node.quadrants[i]->node.leaf.p;
      free(parent->node.quadrants[i]->node.leaf.ship);
      free(parent->node.quadrants[i]->node.leaf.p);
      parent->node.quadrants[i]->node.leaf.ship = NULL;
      parent->node.quadrants[i]->node.leaf.p = NULL;
      free(parent->node.quadrants[i]);
      parent->node.quadrants[i] = NULL;
      printf("eliminated, in i:%d\n",i);
      break;
    }
  }
}

int node_delete(QD_NODE* root, int x, int y, int l){
  if(root == NULL){
    printf("No tree found\n");
    return -1;
  }

  if(root->nodes_inside == 1){ //tree/sub-tree constains only one node
    printf("flag1:\n");
    free(root->node.leaf.ship);
    free(root->node.leaf.p);
    root->node.leaf.ship = NULL;
    root->node.leaf.p = NULL;

    free(root);
    root = NULL;
    return 1;
  }
  printf("flag2:\n");
  //locate node where point(x,y) belongs
  QD_NODE* e = get_subdivision(x,y,l/2,l/2,l/2,root);
  printf("(%d,%d)\n",x,y);
  printf("q:%p\n",e);
  printf("(%d,%d)\n",e->node.leaf.p->x,e->node.leaf.p->y);
  printf("e:%p\n",e);

  if(e == NULL){
    printf("That node doesn't exist.\n");
    return -1;
  }

  QD_NODE* e_parent = get_parent(e);
  printf("%d\n",e_parent->type);
  while(e_parent != root){
    printf("flag2.1:\n");
    e_parent->nodes_inside -= 1;

    //remove node
    free(e->node.leaf.ship);
    free(e->node.leaf.p);
    e->node.leaf.ship = NULL;
    e->node.leaf.p = NULL;
    free(e);
    e = NULL;


    if(e_parent->nodes_inside == 1){ //only one node left
      colapse_division(e_parent);
    }
    e_parent = get_parent(e_parent);
  }

  return 1;
}

void print_tree(QD_NODE* root, int l){
  if(root->type == QDLEAF){
    return;
  }
  printf("1:%p,\n2:%p,\n3:%p,\n4:%p,\n",root->node.quadrants[0],root->node.quadrants[1],root->node.quadrants[2],root->node.quadrants[3]);
  printf("\n");
  printf("tree0\n");
  print_tree(root->node.quadrants[0],l/2);
  printf("tree1\n");
  print_tree(root->node.quadrants[1],l/2);
  printf("tree2\n");
  print_tree(root->node.quadrants[2],l/2);
  printf("tree3\n");
  print_tree(root->node.quadrants[3],l/2);

  printf("_______\n");
}

int main(){
  int l = 19;
  POINT* p = make_point(3,1);
  POINT* p1 = make_point(7,3);
  POINT* p2 = make_point(7,4);
  SHIP* boat1 =(SHIP*) malloc (sizeof(SHIP));
  create_ship(boat1,2,7);
  SHIP* boat2 =(SHIP*) malloc (sizeof(SHIP));
  create_ship(boat2,2,4);
  SHIP* boat3 =(SHIP*) malloc (sizeof(SHIP));
  create_ship(boat2,1,9);


  QD_NODE* root = create_node();
  root->node.leaf.p = p;
  root->node.leaf.ship = boat1;

  QD_NODE* d = create_node();
  d->node.leaf.p = p1;
  d->node.leaf.ship = boat2;

  QD_NODE* d1 = create_node();
  d1->node.leaf.p = p2;
  d1->node.leaf.ship = boat3;

  node_insert(root,d,l);
  node_insert(root,d1,l);


  printf("Level1\n");
  printf("%p\n",root->node.quadrants[0]);
  printf("%p\n",root->node.quadrants[1]);
  printf("%p\n",root->node.quadrants[2]);
  printf("%p\n",root->node.quadrants[3]);
  printf("Level2\n");
  printf("%p\n",root->node.quadrants[0]->node.quadrants[0]);
  printf("%p\n",root->node.quadrants[0]->node.quadrants[1]);
  printf("%p\n",root->node.quadrants[0]->node.quadrants[2]);
  printf("%p\n",root->node.quadrants[0]->node.quadrants[3]);
  printf("%p\n",root->node.quadrants[1]->node.quadrants[0]);
  printf("%p\n",root->node.quadrants[1]->node.quadrants[1]);
  printf("%p\n",root->node.quadrants[1]->node.quadrants[2]);
  printf("%p\n",root->node.quadrants[1]->node.quadrants[3]);
  printf("%p\n",root->node.quadrants[2]->node.quadrants[0]);
  printf("%p\n",root->node.quadrants[2]->node.quadrants[1]);
  printf("%p\n",root->node.quadrants[2]->node.quadrants[2]);
  printf("%p\n",root->node.quadrants[2]->node.quadrants[3]);
  printf("%p\n",root->node.quadrants[3]->node.quadrants[0]);
  printf("%p\n",root->node.quadrants[3]->node.quadrants[1]);
  printf("%p\n",root->node.quadrants[3]->node.quadrants[2]);
  printf("%p\n",root->node.quadrants[3]->node.quadrants[3]);
  printf("Level 3\n");
  printf("%p\n",root->node.quadrants[0]->node.quadrants[0]->node.quadrants[0]);
  printf("%p\n",root->node.quadrants[0]->node.quadrants[0]->node.quadrants[1]);
  printf("%p\n",root->node.quadrants[0]->node.quadrants[0]->node.quadrants[2]);
  printf("%p\n",root->node.quadrants[0]->node.quadrants[0]->node.quadrants[3]);
  printf("%p\n",root->node.quadrants[0]->node.quadrants[1]->node.quadrants[0]);
  printf("%p\n",root->node.quadrants[0]->node.quadrants[1]->node.quadrants[1]);
  printf("%p\n",root->node.quadrants[0]->node.quadrants[1]->node.quadrants[2]);
  printf("%p\n",root->node.quadrants[0]->node.quadrants[1]->node.quadrants[3]);
  printf("%p\n",root->node.quadrants[0]->node.quadrants[2]->node.quadrants[0]);
  printf("%p\n",root->node.quadrants[0]->node.quadrants[2]->node.quadrants[1]);
  printf("%p\n",root->node.quadrants[0]->node.quadrants[2]->node.quadrants[2]);
  printf("%p\n",root->node.quadrants[0]->node.quadrants[2]->node.quadrants[3]);
  printf("%p\n",root->node.quadrants[0]->node.quadrants[3]->node.quadrants[0]);
  printf("%p\n",root->node.quadrants[0]->node.quadrants[3]->node.quadrants[1]);
  printf("%p\n",root->node.quadrants[0]->node.quadrants[3]->node.quadrants[2]);
  printf("%p\n",root->node.quadrants[0]->node.quadrants[3]->node.quadrants[3]);
  printf("%p\n",root->node.quadrants[0]->node.quadrants[2]->node.quadrants[2]->node.quadrants[2]);
  printf("%p\n",root->node.quadrants[0]->node.quadrants[2]->node.quadrants[2]->node.quadrants[3]);
  //printf("-(%d,%d)\n",d1->node.leaf.p->x,d1->node.leaf.p->y);
  //printf("-(%d,%d)\n",root->node.quadrants[0]->node.quadrants[2]->node.quadrants[2]->node.quadrants[3]->node.leaf.p->x,root->node.quadrants[0]->node.quadrants[2]->node.quadrants[2]->node.quadrants[3]->node.leaf.p->y);
  printf("where to insert:\n");
  //printf("%p\n",root->node.quadrants[0]->node.quadrants[0]);
  print_boat(root->node.quadrants[0]->node.quadrants[0]->node.leaf.ship);
  //printf("%p\n",root->node.quadrants[0]->node.quadrants[2]);
  print_boat(root->node.quadrants[0]->node.quadrants[2]->node.quadrants[2]->node.quadrants[2]->node.leaf.ship);
  print_boat(root->node.quadrants[0]->node.quadrants[2]->node.quadrants[2]->node.quadrants[3]->node.leaf.ship);

  //print_tree(root,l);

  //node_delete(root,d1->node.leaf.p->x,d1->node.leaf.p->y,l);
  //print_tree(root,l);
  return 0;
}

/*print boats
printf("(%d,%d)\n",root->node.quadrants[0]->node.quadrants[0]->node.leaf.p->x,root->node.quadrants[0]->node.quadrants[0]->node.leaf.p->y);
print_boat(root->node.quadrants[0]->node.quadrants[2]->node.leaf.ship);
printf("(%d,%d)\n",root->node.quadrants[0]->node.quadrants[2]->node.leaf.p->x,root->node.quadrants[0]->node.quadrants[2]->node.leaf.p->y);
print_boat(root->node.quadrants[0]->node.quadrants[0]->node.leaf.ship);*/

/*print tree
printf("Level1\n");
printf("%p\n",root->node.quadrants[0]);
printf("%p\n",root->node.quadrants[1]);
printf("%p\n",root->node.quadrants[2]);
printf("%p\n",root->node.quadrants[3]);
printf("Level2\n");
printf("%p\n",root->node.quadrants[0]->node.quadrants[0]);
printf("%p\n",root->node.quadrants[0]->node.quadrants[1]);
printf("%p\n",root->node.quadrants[0]->node.quadrants[2]);
printf("%p\n",root->node.quadrants[0]->node.quadrants[3]);
printf("%p\n",root->node.quadrants[1]->node.quadrants[0]);
printf("%p\n",root->node.quadrants[1]->node.quadrants[1]);
printf("%p\n",root->node.quadrants[1]->node.quadrants[2]);
printf("%p\n",root->node.quadrants[1]->node.quadrants[3]);
printf("%p\n",root->node.quadrants[2]->node.quadrants[0]);
printf("%p\n",root->node.quadrants[2]->node.quadrants[1]);
printf("%p\n",root->node.quadrants[2]->node.quadrants[2]);
printf("%p\n",root->node.quadrants[2]->node.quadrants[3]);
printf("%p\n",root->node.quadrants[3]->node.quadrants[0]);
printf("%p\n",root->node.quadrants[3]->node.quadrants[1]);
printf("%p\n",root->node.quadrants[3]->node.quadrants[2]);
printf("%p\n",root->node.quadrants[3]->node.quadrants[3]);
printf("Level 3\n");
printf("%p\n",root->node.quadrants[0]->node.quadrants[0]->node.quadrants[0]);
printf("%p\n",root->node.quadrants[0]->node.quadrants[0]->node.quadrants[1]);
printf("%p\n",root->node.quadrants[0]->node.quadrants[0]->node.quadrants[2]);
printf("%p\n",root->node.quadrants[0]->node.quadrants[0]->node.quadrants[3]);
printf("%p\n",root->node.quadrants[0]->node.quadrants[1]->node.quadrants[0]);
printf("%p\n",root->node.quadrants[0]->node.quadrants[1]->node.quadrants[1]);
printf("%p\n",root->node.quadrants[0]->node.quadrants[1]->node.quadrants[2]);
printf("%p\n",root->node.quadrants[0]->node.quadrants[1]->node.quadrants[3]);
printf("%p\n",root->node.quadrants[0]->node.quadrants[2]->node.quadrants[0]);
printf("%p\n",root->node.quadrants[0]->node.quadrants[2]->node.quadrants[1]);
printf("%p\n",root->node.quadrants[0]->node.quadrants[2]->node.quadrants[2]);
printf("%p\n",root->node.quadrants[0]->node.quadrants[2]->node.quadrants[3]);
printf("%p\n",root->node.quadrants[0]->node.quadrants[3]->node.quadrants[0]);
printf("%p\n",root->node.quadrants[0]->node.quadrants[3]->node.quadrants[1]);
printf("%p\n",root->node.quadrants[0]->node.quadrants[3]->node.quadrants[2]);
printf("%p\n",root->node.quadrants[0]->node.quadrants[3]->node.quadrants[3]);
*/
