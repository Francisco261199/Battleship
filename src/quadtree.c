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

//create point
POINT * make_point(int x,int y){    //constrói o ponto (x,y) e o retorna;
  POINT* p = (POINT*) malloc (sizeof(POINT));
  p->x = x;
  p->y = y;
  return p;
}

void print_tree(QD_NODE* root, int l){
  if(root->type == QDLEAF){
    return;
  }
  printf("1:%p,type:%d, level = %f, cx = %f , cy = %f, parent : %p \n2:%p,type:%d, level = %f, cx = %f , cy = %f, parent : %p \n3:%p,type:%d, level = %f, cx = %f , cy = %f, parent : %p \n4:%p,type:%d, level = %f, cx = %f , cy = %f, parent : %p \n"
  ,root->node.quadrants[0],root->node.quadrants[0]->type,root->node.quadrants[0]->level,root->node.quadrants[0]->cx,root->node.quadrants[0]->cy,root->node.quadrants[0]->parent
  ,root->node.quadrants[1],root->node.quadrants[1]->type,root->node.quadrants[1]->level,root->node.quadrants[1]->cx,root->node.quadrants[1]->cy,root->node.quadrants[1]->parent
  ,root->node.quadrants[2],root->node.quadrants[2]->type,root->node.quadrants[2]->level,root->node.quadrants[2]->cx,root->node.quadrants[2]->cy,root->node.quadrants[2]->parent
  ,root->node.quadrants[3],root->node.quadrants[3]->type,root->node.quadrants[3]->level,root->node.quadrants[3]->cx,root->node.quadrants[3]->cy,root->node.quadrants[3]->parent);
  printf("_______\n");
  printf("tree0\n");
  print_tree(root->node.quadrants[0],l/2);
  printf("tree1\n");
  print_tree(root->node.quadrants[1],l/2);
  printf("tree2\n");
  print_tree(root->node.quadrants[2],l/2);
  printf("tree3\n");
  print_tree(root->node.quadrants[3],l/2);

  printf("------\n");
}
//get node q's parent
QD_NODE* get_parent(QD_NODE* q){
  return q->parent;
}
//create node that is empty
QD_NODE* create_node(float level){
    QD_NODE * new_node = (QD_NODE*)malloc(sizeof(QD_NODE));

    new_node->type = QDLEAF;
    new_node->nodes_inside = 0;
    new_node->cx = new_node->cy = level;
    new_node->level = level;
    new_node->parent = (QD_NODE*) malloc(sizeof(QD_NODE));
    new_node->node.leaf.p = (POINT*) malloc(sizeof(POINT));
    new_node->node.leaf.ship = (SHIP*) malloc(sizeof(SHIP));
    new_node->node.leaf.p->x = -1;
    new_node->node.leaf.p->y = -1;
    //new_node->node.leaf.p = NULL;
    //new_node->node.leaf.ship = NULL;

    return new_node;
}

//expand tree(it is going to have atleast 2 nodes with data)
void expand_tree(QD_NODE* root){
  //printf("root.cx.cy(%f,%f)\n",root->cx,root->cy);
  root->type = QDNODE;
  root->nodes_inside = 2;
  root->node.quadrants[0] = create_node(((root->level)/2.0));
  root->node.quadrants[1] = create_node(((root->level)/2.0));
  root->node.quadrants[2] = create_node(((root->level)/2.0));
  root->node.quadrants[3] = create_node(((root->level)/2.0));

  root->node.quadrants[0]->cx = (root->cx) - (root->node.quadrants[0]->level)/2.0;
  root->node.quadrants[0]->cy = (root->cy) - (root->node.quadrants[0]->level)/2.0;
  //printf("-0.x) %f-%f\n",(root->cx),(root->node.quadrants[0]->level)/2.0);
  //printf("-0.y) %f-%f\n",(root->cy),(root->node.quadrants[0]->level)/2.0);
  root->node.quadrants[1]->cx = (root->cx) - (root->node.quadrants[1]->level)/2.0;
  root->node.quadrants[1]->cy = (root->cy) + (root->node.quadrants[1]->level)/2.0;
  //printf("-1.x) %f-%f\n",(root->cx),(root->node.quadrants[1]->level)/2.0);
  //printf("-1.y) %f+%f\n",(root->cy),(root->node.quadrants[1]->level)/2.0);
  root->node.quadrants[2]->cx = (root->cx) + (root->node.quadrants[2]->level)/2.0;
  root->node.quadrants[2]->cy = (root->cy) - (root->node.quadrants[2]->level)/2.0;
  //printf("-2.x) %f+%f\n",(root->cx),(root->node.quadrants[2]->level)/2.0);
  //printf("-2.y) %f-%f\n",(root->cy),(root->node.quadrants[2]->level)/2.0);
  root->node.quadrants[3]->cx = (root->cx) + (root->node.quadrants[3]->level)/2.0;
  root->node.quadrants[3]->cy = (root->cy) + (root->node.quadrants[3]->level)/2.0;
  //printf("-3.x) %f+%f\n",(root->cx),(root->node.quadrants[3]->level)/2.0);
  //printf("-3.y) %f+%f\n",(root->cy),(root->node.quadrants[3]->level)/2.0);

  root->node.quadrants[0]->parent = root;
  root->node.quadrants[1]->parent = root;
  root->node.quadrants[2]->parent = root;
  root->node.quadrants[3]->parent = root;

}


QD_NODE* get_next(int x,int y,QD_NODE* save,QD_NODE* root){
  //printf("rootcx,cy(%f,%f)\n",root->cx,root->cy);
  if(x<=root->cx){
    if(y<=root->cy){
      //printf("root.cx,root.cy(%.3f,%.3f)\n",save->cx,save->cy);
      save->cx -= save->level/2.0;
      save->cy -= save->level/2.0;
      save->level/=2;
      save->parent = root;
      //printf("type:%d\n", root->type);
      printf("x,y:(%d,%d)\n",x,y);
      printf("id:%p\n",root->node.quadrants[0]);


      return root->node.quadrants[0];
     }
    //y>l
    else{
      //printf("root.cx,root.cy(%.3f,%.3f)\n",save->cx,save->cy);
      save->cx -= save->level/2.0;
      save->cy += save->level/2.0;
      save->level/=2;
      save->parent = root;
      //printf("type:%d\n", root->type);
      printf("x,y:(%d,%d)\n",x,y);
      printf("id:%p\n",root->node.quadrants[1]);


      return root->node.quadrants[1];
   }
  }
  else{ //x>l
    if(y<=root->cy){
      //printf("root.cx,root.cy(%.3f,%.3f)\n",save->cx,save->cy);
      save->cx += save->level/2.0;
      save->cy -= save->level/2.0;
      save->level/=2;
      save->parent = root;
      //printf("type:%d\n", root->type);
      printf("x,y:(%d,%d)\n",x,y);
      printf("id:%p\n",root->node.quadrants[2]);


    return root->node.quadrants[2];
   }
    //y>l
    else{
      save->cx += save->level/2.0;
      save->cy += save->level/2.0;
      save->level/=2;
      save->parent = root;
      //printf("type:%d\n", root->type);
      printf("x,y:(%d,%d)\n",x,y);
      printf("id:%p\n",root->node.quadrants[3]);


      return root->node.quadrants[3];
     }
  }
}

//quadrants [‘NW’,‘NE’,‘SW’,‘SE’] x=lines y=cols
QD_NODE * get_subdivision(int x,int y,float cx,float cy,float l,QD_NODE* root){
  //printf("before.l:%d\n",l);
  //printf("type:%d\n", root->type);
  printf("x,y:(%d,%d)\n",x,y);
  printf("id:%p\n",root);
  //printf("cx,cy:(%.3f,%.3f),root.cx,root.cy(%.3f,%.3f)\n",cx,cy,root->cx,root->cy);
  //printf("l:%f\n",l);

  if(root->type == QDLEAF) return root;
  if(root == NULL){
    printf("Not found!\n");
    return NULL;
  }

  if(x<=cx){
    if(y<=cy) return get_subdivision(x,y,cx-l/2.0,cy-l/2.0,l/2.0,root->node.quadrants[0]);
    //y>l
    else return get_subdivision(x,y,cx-l/2.0,cy+l/2.0,l/2.0,root->node.quadrants[1]);
  }
  else{ //x>l
    if(y<=cy) return get_subdivision(x,y,cx+l/2.0,cy-l/2.0,l/2.0,root->node.quadrants[2]);
    //y>l
    else return get_subdivision(x,y,cx+l/2.0,cy+l/2.0,l/2.0,root->node.quadrants[3]);
  }
  return NULL;
}

//divide sections in 4 sub-sections until 2 points dont match the same division
void divide_insert(QD_NODE* root, QD_NODE* copy, QD_NODE* n){
  QD_NODE* q1 = create_node(root->level);
  QD_NODE* q2 = create_node(root->level);
  q2 = q1 = root;

  //printf("--q1.level=%f, q2.level=%f--\n",copy->level,n->level);
  //printf("--q1.cx.cy(%f,%f)--\n",copy->cx,copy->cy);
  while(1){
    expand_tree(q1);

    q1 = get_next(copy->node.leaf.p->x,copy->node.leaf.p->y,copy,q1);
    //  printf("q1:%p\n",q1);
    printf("-------------\n");

    q2 = get_next(n->node.leaf.p->x,n->node.leaf.p->y,n,q2);
    //  printf("q2:%p\n",q2);
    printf("-------------\n");

    print_tree(root,root->level);
    printf("END--------------\n");
    //q1->level = get_parent(q1)->level/2;
    if(q1 != q2) break;

  }

  *q1->node.leaf.ship = *copy->node.leaf.ship;
  *q1->node.leaf.p = *copy->node.leaf.p;
  //printf("q1_level = %f\n",q1->level);
  //*q1->node.leaf.p->y = *copy->node.leaf.p->y;
  *q2->node.leaf.ship = *n->node.leaf.ship;
  *q2->node.leaf.p = *n->node.leaf.p;
  //printf("q2_level = %f\n",q2->level);

}

int node_insert(QD_NODE* root,QD_NODE* n){
  if(root == NULL){ //tree is empty
    printf("Error. Node not found");
    return 0;
  }

  if(root->type == QDLEAF){ //at node leaf to insert
    //printf("flag1\n");
    //no node inside leaf(insert)
    if(root->node.leaf.p->x == -1){
      //printf("flag1.1\n");
      *root = *n;
      //n = root;
      //free(n);
      //n = NULL;
      root->parent = get_parent(root);
      root->nodes_inside = 1;
      printf("Valid insert.(%d,%d),p:%p\n",root->node.leaf.p->x,root->node.leaf.p->y,root);
      return 1;
    }
    else{ //create sub-tree divisions
      //printf("flag1.2\n");
      QD_NODE* copy = create_node(root->level/2);
      *copy->node.leaf.ship = *root->node.leaf.ship;
      *copy->node.leaf.p = *root->node.leaf.p;
      copy->level =root->level;
      copy->cx = root->cx;
      copy->cy = root->cy;
      divide_insert(root,copy,n);
      return 1;
    }
  }
  else{ //not at leaf node
    //printf("flag2\n");
    QD_NODE* aux = create_node(19.0);
    aux = get_subdivision(n->node.leaf.p->x, n->node.leaf.p->y,root->level/2.0,root->level/2.0,root->level/2.0, root);
    n->cx = aux->cx;
    n->cy = aux->cy;
    n->level = aux->level;
    //printf("LEVEL:%f\n",aux->level);
    node_insert(aux,n);
    //free(aux);
    //aux = NULL;
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
    root->node.leaf.ship = NULL;
    root->node.leaf.p = NULL;
    free(root->node.leaf.ship);
    free(root->node.leaf.p);

    root = NULL;
    free(root);

    return 1;
  }
  printf("flag2:\n");
  //printf("l(%d,%d)\n",x,y);
  //locate node where point(x,y) belongs
  QD_NODE* e = get_subdivision(x,y,(root->level)/2.0,(root->level)/2.0,(root->level)/2.0,root);

  printf("e:%p\n",e);
  printf("rootad:%p\n",root->node.quadrants[0]->node.quadrants[0]->node.quadrants[2]->node.quadrants[3]->node.quadrants[1]);
  //printf("-(%d,%d)\n",root->node.quadrants[0]->node.quadrants[2]->node.quadrants[3]->node.quadrants[3]->node.leaf.p->x,root->node.quadrants[0]->node.quadrants[2]->node.quadrants[3]->node.quadrants[3]->node.leaf.p->y);

  printf("e:(%d,%d)\n",root->node.quadrants[0]->node.quadrants[0]->node.quadrants[2]->node.quadrants[3]->node.quadrants[1]->node.leaf.p->x,root->node.quadrants[0]->node.quadrants[0]->node.quadrants[2]->node.quadrants[3]->node.quadrants[1]->node.leaf.p->y);
  //printf("e:%p\n",e);

  if(e == NULL){
    printf("That node doesn't exist.\n");
    return -1;
  }

  QD_NODE* e_parent = create_node(e->level*4.0);
  e_parent = get_parent(e);

  printf("%d\n",e_parent->type);
  printf("ddd\n");
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


int main(){
  int l = 19;
  POINT* p = make_point(3,1);
  POINT* p1 = make_point(7,3);
  POINT* p2 = make_point(7,4);
  POINT* p3 = make_point(7,5);
  POINT* p4 = make_point(7,6);


  SHIP* boat =(SHIP*) malloc (sizeof(SHIP));
  create_ship(boat,2,7);
  SHIP* boat1 =(SHIP*) malloc (sizeof(SHIP));
  create_ship(boat1,2,4);
  SHIP* boat2 =(SHIP*) malloc (sizeof(SHIP));
  create_ship(boat2,1,9);
  SHIP* boat3 =(SHIP*) malloc (sizeof(SHIP));
  create_ship(boat3,1,9);
  SHIP* boat4 =(SHIP*) malloc (sizeof(SHIP));
  create_ship(boat4,1,9);

  QD_NODE* root = create_node(l);
  root->node.leaf.p = p;
  root->node.leaf.ship = boat;
  root->cx/=2;
  root->cy/=2;


  QD_NODE* d = create_node(l);
  d->node.leaf.p = p1;
  d->node.leaf.ship = boat1;

  QD_NODE* d1 = create_node(l);
  d1->node.leaf.p = p2;
  d1->node.leaf.ship = boat2;

  QD_NODE* d2 = create_node(l);
  d2->node.leaf.p = p3;
  d2->node.leaf.ship = boat3;

  QD_NODE* d3 = create_node(l);
  d3->node.leaf.p = p4;
  d3->node.leaf.ship = boat4;

  node_insert(root,d);
  print_tree(root,l);
  printf("END.LLL.%d\n",l);

  node_insert(root,d1);
  print_tree(root,l);
  printf("END.LLL.%d\n",l);

  node_insert(root,d2);
  print_tree(root,l);
  printf("END.LLL.%d\n",l);

  node_insert(root,d3);
  print_tree(root,l);
  printf("END.LLL.%d\n",l);

  printf("where to insert:\n");

  return 0;
}
