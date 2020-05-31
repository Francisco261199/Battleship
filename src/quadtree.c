#include<stdio.h>
#include<stdlib.h>
//#include "quadtree.h"
#include "ships.h"

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
  printf("1:%p,type:%d, level = %f, cx = %f , cy = %f, parent : %p , nodes inside:%d\n2:%p,type:%d, level = %f, cx = %f , cy = %f, parent : %p , nodes inside:%d\n3:%p,type:%d, level = %f, cx = %f , cy = %f, parent : %p , nodes inside:%d\n4:%p,type:%d, level = %f, cx = %f , cy = %f, parent : %p , nodes inside:%d\n"
  ,root->node.quadrants[0],root->node.quadrants[0]->type,root->node.quadrants[0]->level,root->node.quadrants[0]->cx,root->node.quadrants[0]->cy,root->node.quadrants[0]->parent,root->node.quadrants[0]->nodes_inside
  ,root->node.quadrants[1],root->node.quadrants[1]->type,root->node.quadrants[1]->level,root->node.quadrants[1]->cx,root->node.quadrants[1]->cy,root->node.quadrants[1]->parent,root->node.quadrants[1]->nodes_inside
  ,root->node.quadrants[2],root->node.quadrants[2]->type,root->node.quadrants[2]->level,root->node.quadrants[2]->cx,root->node.quadrants[2]->cy,root->node.quadrants[2]->parent,root->node.quadrants[2]->nodes_inside
  ,root->node.quadrants[3],root->node.quadrants[3]->type,root->node.quadrants[3]->level,root->node.quadrants[3]->cx,root->node.quadrants[3]->cy,root->node.quadrants[3]->parent,root->node.quadrants[3]->nodes_inside);

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

QD_NODE* get_last_child(QD_NODE* e){
  for(int i=0;i<=3;i++){
    if(e->node.quadrants[i]->node.leaf.ship != NULL) return e->node.quadrants[i];
  }
  return NULL;
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
    return new_node;
}

void node_clean(QD_NODE* e){
  if(e->type == QDLEAF){
    e->node.leaf.ship = NULL;
    free(e->node.leaf.ship);
    e->node.leaf.p = NULL;
    free(e->node.leaf.p);
    e->parent = NULL;
    free(e->parent);
    e = NULL;
    free(e);
  }
  else{
    e = NULL;
    free(e);
  }
}

//expand tree(it is going to have atleast 2 nodes with data)
void expand_tree(QD_NODE* root){
  root->type = QDNODE;
  root->nodes_inside = 1;
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

//get next node
//flag specifies if we are searching for a point during boat insertion or deletion
QD_NODE* get_next(int x,int y,QD_NODE* save,QD_NODE* root, int flag){
  //printf("rootcx,cy(%f,%f)\n",root->cx,root->cy);
  if(x<=root->cx){
    if(y<=root->cy){
      save->cx -= save->level/2.0;
      save->cy -= save->level/2.0;
      save->level/=2;
      save->parent = root;
      //printf("type:%d\n", root->type);
      printf("x,y:(%d,%d)\n",x,y);
      printf("id:%p\n",root->node.quadrants[0]);
      if(flag == 1)root->nodes_inside+=1;

      return root->node.quadrants[0];
     }
    //y>l
    else{
      save->cx -= save->level/2.0;
      save->cy += save->level/2.0;
      save->level/=2;
      save->parent = root;
      //printf("type:%d\n", root->type);
      printf("x,y:(%d,%d)\n",x,y);
      printf("id:%p\n",root->node.quadrants[1]);
      if(flag == 1)root->nodes_inside+=1;

      return root->node.quadrants[1];
   }
  }
  else{ //x>l
    if(y<=root->cy){
      save->cx += save->level/2.0;
      save->cy -= save->level/2.0;
      save->level/=2;
      save->parent = root;
      //printf("type:%d\n", root->type);
      printf("x,y:(%d,%d)\n",x,y);
      printf("id:%p\n",root->node.quadrants[2]);
      if(flag == 1)root->nodes_inside+=1;

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
      if(flag == 1)root->nodes_inside+=1;

      return root->node.quadrants[3];
     }
  }
}

//quadrants [‘NW’,‘NE’,‘SW’,‘SE’] x=lines y=cols
//flag specifies if we are searching for a point during boat insertion or deletion
QD_NODE * get_subdivision(int x,int y,float cx,float cy,float l,QD_NODE* root,int flag){
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
  //searching to insert
  if(flag == 1)root->nodes_inside+=1;
  //searching to remove
  else if(flag == 0) root->nodes_inside-=1;

  //find next subdivision
  if(x<=cx){
    if(y<=cy) return get_subdivision(x,y,cx-l/2.0,cy-l/2.0,l/2.0,root->node.quadrants[0],flag);
    //y>l
    else return get_subdivision(x,y,cx-l/2.0,cy+l/2.0,l/2.0,root->node.quadrants[1],flag);
  }
  else{ //x>l
    if(y<=cy) return get_subdivision(x,y,cx+l/2.0,cy-l/2.0,l/2.0,root->node.quadrants[2],flag);
    //y>l
    else return get_subdivision(x,y,cx+l/2.0,cy+l/2.0,l/2.0,root->node.quadrants[3],flag);
  }
  return NULL;
}

//divide sections in 4 sub-sections until 2 points dont match the same division
void divide_insert(QD_NODE* root, QD_NODE* copy, QD_NODE* n){
  QD_NODE* q1 = create_node(root->level);
  QD_NODE* q2 = create_node(root->level);
  q2 = q1 = root;
  while(1){
    expand_tree(q1);

    q1 = get_next(copy->node.leaf.p->x,copy->node.leaf.p->y,copy,q1,0);
    //  printf("q1:%p\n",q1);
    printf("-------------\n");

    q2 = get_next(n->node.leaf.p->x,n->node.leaf.p->y,n,q2,1);
    //  printf("q2:%p\n",q2);
    printf("-------------\n");

    print_tree(root,root->level);
    printf("END--------------\n");
    if(q1 != q2) break;
  }
  *q1->node.leaf.ship = *copy->node.leaf.ship;
  *q1->node.leaf.p = *copy->node.leaf.p;
  q1->nodes_inside = 1;

  *q2->node.leaf.ship = *n->node.leaf.ship;
  *q2->node.leaf.p = *n->node.leaf.p;
  q2->nodes_inside = 1;

  q1 = NULL;
  q2 = NULL;
  free(q1);
  free(q2);
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
      root->parent = get_parent(root);
      root->nodes_inside = 1;
      //printf("Valid insert.(%d,%d),p:%p\n",root->node.leaf.p->x,root->node.leaf.p->y,root);
      return 1;
    }
    else{ //create sub-tree divisions
      //printf("flag1.2\n");
      QD_NODE* copy = create_node(root->level/2);
      copy->node.leaf.ship = root->node.leaf.ship;
      copy->node.leaf.p = root->node.leaf.p;
      copy->level = root->level;
      copy->cx = root->cx;
      copy->cy = root->cy;
      divide_insert(root,copy,n);
      copy = NULL;
      free(copy);
      return 1;
    }
  }
  else{ //not at leaf node
    //printf("flag2\n");
    QD_NODE* aux = create_node(19.0);
    aux = get_subdivision(n->node.leaf.p->x, n->node.leaf.p->y,root->level/2.0,root->level/2.0,root->level/2.0, root,1);
    n->cx = aux->cx;
    n->cy = aux->cy;
    n->level = aux->level;
    //printf("LEVEL:%f\n",aux->level);
    node_insert(aux,n);
    aux = NULL;
    free(aux);
    return 1;
  }
  return -1;
}

//find and delete a node
int node_delete(QD_NODE* root, int x, int y, int l){
  if(root == NULL){
    printf("No tree found\n");
    return -1;
  }

  if(root->nodes_inside == 1){ //tree/sub-tree constains only one node
    //printf("flag1:\n");
    root->node.leaf.ship = NULL;
    root->node.leaf.p = NULL;
    free(root->node.leaf.ship);
    free(root->node.leaf.p);

    root = NULL;
    free(root);

    return 1;
  }
  //printf("flag2:\n");

  //locate node where point(x,y) belongs
  QD_NODE* child = get_subdivision(x,y,(root->level)/2.0,(root->level)/2.0,(root->level)/2.0,root,0);
  //printf("after remove\n");
  //print_boat(child->node.leaf.ship);

  if(child == NULL){
    //printf("That node doesn't exist.\n");
    return -1;
  }

  QD_NODE* parent = get_parent(child);
  //printf("parent:%p, nodes_inside: %d\n",parent,parent->nodes_inside);

  while(parent != NULL && parent->nodes_inside == 1){
    parent->type = QDLEAF;
    *parent->node.leaf.ship = *child->node.leaf.ship;
    *parent->node.leaf.p = *child->node.leaf.p;
    node_clean(child);
    child = parent;
    parent = get_parent(child);
  }

  child = NULL;
  free(child);
  node_clean(parent);
  return 1;
}


int search_point(POINT * p,QD_NODE* root){
  QD_NODE* compare = (QD_NODE*) malloc(sizeof(QD_NODE));
  compare = get_subdivision(p->x,p->y,(root->level)/2.0,(root->level)/2.0,(root->level)/2.0,root,2);
  if(compare->node.leaf.p->x == p->x && compare->node.leaf.p->y == p->y){
    compare = NULL;
    free(compare);
    return 1;
  }
  return -1;
}


POINT* [] create_points(POINT* p, SHIP* ship){
  int index=0;
  POINT * ans[ship->size] = (POINT*) malloc (sizeof((ship->size)*POINT));
  for(int i=0;i<5;i++){
    for(int j=0;j<5;j++){
      if(ship->bitmap[i][j] == 1){
        ans[index]->x = ((p->x) + i - 2);
        ans[index]->y = ((p->y) + j - 2);
        index++;
      }
    }
  }
  return ans;
}

int verify( POINT * points[],int size,int map_size, QD_NODE * root){
  int check = 0;
  float cx = (float) map_size-1/2;
  float cy = (float) map_size-1/2;

  for(int i = 0; i<size;i++){
    check+=search_point(points[i],cx,cy,(float)map_size-1,root);
  }
return check;
}

void insert_ship(POINT* p,SHIP * ship,QD_NODE * root,int size){
  POINT * points[] = create_points(p,ship); 

   //verificar se pode ser inserido
if(verify(points,ship->size,root) != ship->size){
    check:
    printf("\033[1;31m");
    printf("ERROR! You can't insert the boat here!\n");
    printf("\033[1;33m");
    printf("Please choose another position.\n");
    printf("X: "); scanf("%d",&x);
    printf("Y: "); scanf("%d",&y);
    printf("\n");
    free(points);
    POINT = NULL;
    POINT * newpoint = (POINT*) malloc(sizeof(POINT));
    newpoint->x = x;
    newpoint->y = y;

    insert_ship(newpoint,ship,map,map_size);

  }

else

  int flg;
  for(int i=0;i<ship->size,i++){
    QD_NODE* n = create_node(size-1);
    n->node.leaf.point = point[i];
    n->node.leaf.ship = ship;
  if((flg = node_insert(root,n)) == -1) goto check;


}
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
  //print_tree(root,l);
  //printf("END.LLL.%d\n",l);

  node_insert(root,d1);
  //print_tree(root,l);
  //printf("END.LLL.%d\n",l);

  node_insert(root,d2);
  //print_tree(root,l);
  //printf("END.LLL.%d\n",l);

  node_insert(root,d3);
  //print_tree(root,l);
  //printf("END.LLL.%d\n",l);

  print_tree(root,l);
  printf("END\n");

  printf("nodes inside root:%d\n",root->nodes_inside);
  printf("%d", node_delete(root,d3->node.leaf.p->x,d3->node.leaf.p->y,root->level));

  print_tree(root,l);
  printf("END\n");

  return 0;
}
