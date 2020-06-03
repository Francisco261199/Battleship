#include<stdio.h>
#include<stdlib.h>
#include "quadtree.h"
#include "ships.h"


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
    free(e->node.leaf.p);
    e->node.leaf.p = NULL;
    e->parent = NULL;
    e = NULL;
  }
  else e = NULL;
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
  if(x<=root->cx){
    if(y<=root->cy){
      save->cx -= save->level/2.0;
      save->cy -= save->level/2.0;
      save->level/=2;
      save->parent = root;
      if(flag == 1)root->nodes_inside+=1;

      return root->node.quadrants[0];
     }
    //y>l
    else{
      save->cx -= save->level/2.0;
      save->cy += save->level/2.0;
      save->level/=2;
      save->parent = root;
      if(flag == 1)root->nodes_inside+=1;

      return root->node.quadrants[1];
   }
  }
  else{ //x>root.cx
    if(y<=root->cy){
      save->cx += save->level/2.0;
      save->cy -= save->level/2.0;
      save->level/=2;
      save->parent = root;
      if(flag == 1)root->nodes_inside+=1;

    return root->node.quadrants[2];
   }
    //y>=root.cx
    else{
      save->cx += save->level/2.0;
      save->cy += save->level/2.0;
      save->level/=2;
      save->parent = root;
      if(flag == 1)root->nodes_inside+=1;

      return root->node.quadrants[3];
     }
  }
}

//quadrants [‘NW’,‘NE’,‘SW’,‘SE’] x=lines y=cols
//flag specifies if we are searching for a point during boat insertion or deletion
QD_NODE * get_subdivision(int x,int y,float cx,float cy,float l,QD_NODE* root,int flag){

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

    q2 = get_next(n->node.leaf.p->x,n->node.leaf.p->y,n,q2,1);

    if(q1 != q2) break;

  }
  q1->node.leaf.ship = copy->node.leaf.ship;
  *q1->node.leaf.p = *copy->node.leaf.p;
  q1->nodes_inside = 1;

  q2->node.leaf.ship = n->node.leaf.ship;
  *q2->node.leaf.p = *n->node.leaf.p;
  q2->nodes_inside = 1;
}

int node_insert(QD_NODE* root,QD_NODE* n){
  if(root == NULL){ //tree is empty
    printf("Error. Node not found");
    return 0;
  }

  if(root->type == QDLEAF){ //at node leaf to insert
    //no node inside leaf(insert)
    if(root->node.leaf.p->x == -1){
      root->node.leaf.ship = n->node.leaf.ship;
      *root->node.leaf.p = *n->node.leaf.p;
      root->parent = get_parent(root);
      root->nodes_inside = 1;
      return 1;
    }
    else{ //create sub-tree division
      QD_NODE* copy = create_node(root->level/2);
      copy->node.leaf.ship = root->node.leaf.ship;
      *copy->node.leaf.p = *root->node.leaf.p;
      copy->level = root->level;
      copy->cx = root->cx;
      copy->cy = root->cy;
      divide_insert(root,copy,n);
      return 1;
    }
  }
  else{ //not at leaf node
    QD_NODE* aux = create_node(19.0);
    aux = get_subdivision(n->node.leaf.p->x, n->node.leaf.p->y,root->level/2.0,root->level/2.0,root->level/2.0, root,1);
    n->cx = aux->cx;
    n->cy = aux->cy;
    n->level = aux->level;
    node_insert(aux,n);
    return 1;
  }
  return -1;
}

//find and delete a node
int node_delete(QD_NODE* root, int x, int y){
  if(root == NULL){
    printf("No tree found\n");
    return -1;
  }

  if(root->nodes_inside == 1){ //tree/ constains only one node
    root->node.leaf.ship = NULL;
    root->node.leaf.p = NULL;
    root->nodes_inside-=1;

    return 1;
  }


  //locate node where point(x,y) belongs
  QD_NODE* child = get_subdivision(x,y,(root->level)/2.0,(root->level)/2.0,(root->level)/2.0,root,0);

  if(child == NULL){
    printf("That node doesn't exist.\n");
    return -1;
  }

  QD_NODE* parent = get_parent(child);

  while(parent != NULL && parent->nodes_inside == 1){
    parent->type = QDLEAF;
    parent->node.leaf.ship = child->node.leaf.ship;
    *parent->node.leaf.p = *child->node.leaf.p;
    node_clean(child);
    child = parent;
    parent = get_parent(child);
  }

  child = NULL;
  //node_clean(parent);
  return 1;
}
