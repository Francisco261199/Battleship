#include<stdio.h>
#include<stdlib.h>
//#include "quadtree.h"
#include "ships.h"
#include<time.h>

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

typedef struct{
  QD_NODE* root;
  char shot;
} CELL;

typedef struct{
    CELL **map1;
    CELL **map2;
    int size;
    int state1,state2;
} GAME;

// create board in memory
GAME * init_board(int size){
    GAME * pGame = (GAME*)malloc(sizeof(GAME));

    pGame->size = size;
    pGame->map1 = (CELL**)malloc(size*sizeof(CELL));
    pGame->map2 = (CELL**)malloc(size*sizeof(CELL));

    for(int i=0;i<size;i++){
        pGame->map1[i] = (CELL*)malloc(size*sizeof(CELL));
        pGame->map2[i] = (CELL*)malloc(size*sizeof(CELL));
        for(int j=0;j<size;j++){

          pGame->map1[i][j].shot= _NO_SHOT;
          pGame->map1[i][j].ship = NULL;
          pGame->map2[i][j].shot= _NO_SHOT;
          pGame->map2[i][j].ship = NULL;
        }
      }
  return pGame;
}

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
  if(x<=root->cx){
    if(y<=root->cy){
      save->cx -= save->level/2.0;
      save->cy -= save->level/2.0;
      save->level/=2;
      save->parent = root;
      //printf("root(%f,%f)\n", root->cx,root->cy);
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
      //printf("root(%f,%f)\n", root->cx,root->cy);
      printf("x,y:(%d,%d)\n",x,y);
      printf("id:%p\n",root->node.quadrants[1]);
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
      //printf("root(%f,%f)\n", root->cx,root->cy);
      printf("x,y:(%d,%d)\n",x,y);
      printf("id:%p\n",root->node.quadrants[2]);
      if(flag == 1)root->nodes_inside+=1;

    return root->node.quadrants[2];
   }
    //y>=root.cx
    else{
      save->cx += save->level/2.0;
      save->cy += save->level/2.0;
      save->level/=2;
      save->parent = root;
      //printf("root(%f,%f)\n", root->cx,root->cy);
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
  //printf("x,y:(%d,%d)\n",x,y);
  //printf("id:%p\n",root);
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
  //int i = 1;
  while(1){
    //i++;
    expand_tree(q1);

    q1 = get_next(copy->node.leaf.p->x,copy->node.leaf.p->y,copy,q1,0);
    //  printf("q1:%p\n",q1);
    printf("-------------\n");

    q2 = get_next(n->node.leaf.p->x,n->node.leaf.p->y,n,q2,1);
    //  printf("q2:%p\n",q2);
    printf("-------------\n");

    //print_tree(root,root->level);
    //printf("END--------------\n");
    print_tree(q1,n->level);
    if(q1 != q2) break;
    //if(i == 10) exit(1);
  }
  *q1->node.leaf.ship = *copy->node.leaf.ship;
  *q1->node.leaf.p = *copy->node.leaf.p;
  q1->nodes_inside = 1;

  *q2->node.leaf.ship = *n->node.leaf.ship;
  *q2->node.leaf.p = *n->node.leaf.p;
  q2->nodes_inside = 1;

  //q1 = NULL;
  //q2 = NULL;
  //free(q1);
  //free(q2);
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
      *root->node.leaf.ship = *n->node.leaf.ship;
      *root->node.leaf.p = *n->node.leaf.p;
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
      //printf("copy(%d,%d), n(%d,%d)\n",copy->node.leaf.p->x,copy->node.leaf.p->y,n->node.leaf.p->x,n->node.leaf.p->y);
      copy->level = root->level;
      copy->cx = root->cx;
      copy->cy = root->cy;
      divide_insert(root,copy,n);
      //copy = NULL;
      //free(copy);
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
    //aux = NULL;
    //free(aux);
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

int search_point(POINT* p,QD_NODE* root){
  QD_NODE* compare = (QD_NODE*) malloc(sizeof(QD_NODE));
  compare = get_subdivision(p->x,p->y,(root->level)/2.0,(root->level)/2.0,(root->level)/2.0,root,2);
  if(compare->node.leaf.p->x == p->x && compare->node.leaf.p->y == p->y){
    compare = NULL;
    return -1;
  }

  return 1;
}


/*POINT* create_points(POINT* p, SHIP* ship){
  int index=0;
  POINT* ans= (POINT*) malloc(ship->size*sizeof(POINT));
  for(int i=0;i<5;i++){
    for(int j=0;j<5;j++){
      if(ship->bitmap[i][j] == NOT_HIT){
        ans[index].x = ((p->x) + i - 2);
        ans[index].y = ((p->y) + j - 2);
        index++;
      }
    }
  }
  return ans;
}*/

// inserção do navio no tabuleiro
int verify_insert(QD_NODE* insert, QD_NODE* root, POINT* points){
  int x = insert->node.leaf.p->x;
  int y = insert->node.leaf.p->y;
  if(x>(root->level) || y>(root->level)) return 1;
  int flag,clear;
  clear = 0;
  QD_NODE* aux = create_node(root->level/2.0);
  for(int i=0;i<5;i++){
    for(int j=0;j<5;j++){
      flag = 1;
      //able to place boats in extremity of map even if bitmap doesn't fit
      if( ((x+i-2) > root->level || (y+j-2) > root->level || (x+i-2) < 0 || (y+j-2) < 0 ) &&
            insert->node.leaf.ship->bitmap[i][j] == EMPTY) flag=0;
      //out of bounds
      else if( ((x+i-2) > root->level || (y+j-2) > root->level || (x+i-2) < 0 || (y+j-2) < 0 ) &&
                insert->node.leaf.ship->bitmap[i][j] != EMPTY) return 1;
      //insert is possible for given position
       aux = get_subdivision((x+i-2),(y+j-2),root->level/2.0,root->level/2.0,root->level/2.0,root,2);
       //printf("POINT to verify:(%d,%d)\n",x+i-2,y+j-2);
      if(flag == 1 && insert->node.leaf.ship->bitmap[i][j] == NOT_HIT &&
        (aux->node.leaf.p->x != (x+i-2) || aux->node.leaf.p->y != (y+j-2))){
          points[clear].x = (x+i-2);
          points[clear].y = (y+j-2);
          clear++;
          //printf("clear=%d\n",clear);
        }
    }
  }
  return clear;
}

//insert_ship
void insert_ship(POINT* p,POINT* points, SHIP* ship, QD_NODE * root){
  QD_NODE* insert = create_node(root->level/2.0);
  *insert->node.leaf.p = *p;
  *insert->node.leaf.ship = *ship;
  //printf("Insert(%d,%d)\n",insert->node.leaf.p->x,insert->node.leaf.p->y);
  //verificar se pode ser inserido
  if(verify_insert(insert,root,points) != ship->size){
    printf("\033[1;31m");
    printf("ERROR! You can't insert the boat here!\n");
    printf("\033[1;33m");
    printf("Please choose another position.\n");
    printf("X: "); scanf("%d",&p->x);
    printf("Y: "); scanf("%d",&p->y);
    printf("\n");
    insert_ship(p,points,ship,root);
  }
  else{
    //POINT *point_array = (POINT*) malloc(ship->size*sizeof(POINT));
    //point_array=create_points(p,ship);
    //printf("point:(%d,%d)\n",insert->node.leaf.p->x,insert->node.leaf.p->y);
    for(int i=0 ;i<ship->size;i++){
      //printf("node to insert:(%d,%d)\n",points[i].x,points[i].y);
      *insert->node.leaf.p = points[i];
      (void)node_insert(root,insert);

    }
    //free(point_array);
    //point_array = NULL;
  }
}

int generate_number(int a,int b){
  return (a == 0)? rand()% ++b:rand() % ++b + a;
}

void rand_insert_ships(QD_NODE* root1,QD_NODE* root2){
  int boat_types[]={2,3,4,5,7,9};
  int map_size,rotation,verify;
  map_size = root1->level;
  int n_for_each_boat[6] = {1,1,1,1,1,1};

  //printf("flag 1");

  for(int i=0;i<6;i++){
    for(int j=0;j<n_for_each_boat[i];j++){
      //printf("i:%d\n",i);
      //create ships
      SHIP* newship1 = (SHIP*) malloc(sizeof(SHIP));
      SHIP* newship2 = (SHIP*) malloc(sizeof(SHIP));
      POINT* p1 = make_point(0,0);
      POINT* p2 = make_point(0,0);
      rotation = generate_number(0,3);
      create_ship(newship1,rotation,boat_types[i]);
      create_ship(newship2,rotation,boat_types[i]);
      print_boat(newship1);
      print_boat(newship2);

      //get random coordinates
      p1->x = generate_number(0,(map_size));
      p1->y = generate_number(0,(map_size));
      p2->x = generate_number(0,(map_size));
      p2->y = generate_number(0,(map_size));


      POINT *point_array1 = (POINT*) malloc(newship1->size*sizeof(POINT));
      POINT *point_array2 = (POINT*) malloc(newship2->size*sizeof(POINT));

      QD_NODE* n1 = create_node(root1->level);
       *n1->node.leaf.p = *p1;
       *n1->node.leaf.ship = *newship1;
      QD_NODE* n2 = create_node(root2->level);
       *n2->node.leaf.p = *p2;
       *n2->node.leaf.ship = *newship2;

      while((verify=verify_insert(n1,root1,point_array1)) != newship1->size){
        n1->node.leaf.p->x = generate_number(0,(map_size));
        n1->node.leaf.p->y = generate_number(0,(map_size));
      }

      while((verify=verify_insert(n2,root2,point_array2)) != newship2->size){
        n2->node.leaf.p->x = generate_number(0,(map_size));
        n2->node.leaf.p->y = generate_number(0,(map_size));
      }

      *p1 = *n1->node.leaf.p;
      *p2 = *n2->node.leaf.p;
      //p1->x = n1->node.leaf.p->x;
      //p1->y = n1->node.leaf.p->y;
      //p2->x = n2->node.leaf.p->x;
      //p2->y = n2->node.leaf.p->y;

      //insert
      insert_ship(p1,point_array1,newship1,root1);
      insert_ship(p2,point_array2,newship2,root2);

      free(p1);
      free(p2);
      p1 = NULL;
      p2 = NULL;
       //printf("flag 5");
    }
  }
}

//Users
void user_insert(GAME* g){
  int boat_types[]={2,3,4,5,7,9};
  char *boats[]={"Destroyer","Cruiser","Battleship","Carrier","Sigma","Pickaxe"};
  int x,y,boat,rotation,boat_rotation,player;
  player = 1;
  system("clear");
  printf("\033[1;31m");
  printf("Time to insert your boats!\nRemember: \nX->rows, Y->cols, (y,x)->boat's center\nFor rotations you have:\n");
  printf(" ___________\n");
  printf("|           |\n");
  printf("| 0 -> 0º   |\n");
  printf("| 1 -> 90º  |\n");
  printf("| 2 -> 180º |\n");
  printf("| 3 -> 270º |\n");
  printf("|___________|\n");
  printf("Press any key to ENTER");
  getchar();

  for(;player<=2;player++){
    for(int i=0;i<6;i++){
      for(int j=0;j<n_for_each_boat[i];j++){
        system("clear");
        SHIP* newship = (SHIP*) malloc(sizeof(SHIP));

        //print map during insertion
        if(player == 1) print_game(g->map1,g->size);
        else print_game(g->map2,g->size);

        printf("\033[1;35m");
        printf("               Player %d: \n",player);
        printf("\033[1;33m");
        printf("|--------------------------------------|\n");
        printf("%s\n",(char*)boats[i]);

        //get valid rotation
        printf("Select Rotation: ");
        scanf("%d",&boat_rotation);
        while(boat_rotation < 0 || boat_rotation > 3){
          printf("\033[1;31m");
          printf("Invalid rotation. Insert new one: ");
          printf("\033[1;33m");
          scanf("%d",&boat_rotation);
        }
        printf("X:");
        x = read_buffer();
        printf("Y:");
        y = read_buffer();

        //player 1 inserts('insert_ship' asks for new coordinates if needed)
        if(player == 1){
          create_ship(newship,boat_rotation,boat_types[i]);
          insert_ship(x,y,newship,g->map1,g->size);
        }
        //player2 inserts('insert_ship' asks for new coordinates if needed)
        else{
          create_ship(newship,boat_rotation,boat_types[i]);
          insert_ship(x,y,newship,g->map2,g->size);
        }
      }
    }
  }
}

int main(){
  srand(time(NULL));
  int l = 19;
  QD_NODE* root1 = create_node(l);
  root1->cx/=2;
  root1->cy/=2;

  QD_NODE* root2 = create_node(l);
  root2->cx/=2;
  root2->cy/=2;
  rand_insert_ships(root1,root2);

  print_tree(root1,l);
  printf("---------Second Tree---------\n");
  print_tree(root2,l);
  printf("END----------------\n");

  int x,y;
  printf("insert x: ");scanf("%d",&x);
  printf("insert y: ");scanf("%d",&y);
  node_delete(root2,x,y,l);
  //QD_NODE* node = get_subdivision(x,y,root2->level/2.0,root2->level/2.0,root2->level/2.0,root2,2);
  print_tree(root2,l);
  printf("END\n");


  /*main 1
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
  *d->node.leaf.p = *p1;
  *d->node.leaf.ship = *boat1;

  QD_NODE* d1 = create_node(l);
  *d1->node.leaf.p = *p2;
  *d1->node.leaf.ship = *boat2;

  QD_NODE* d2 = create_node(l);
  *d2->node.leaf.p = *p3;
  *d2->node.leaf.ship = *boat3;

  QD_NODE* d3 = create_node(l);
  *d3->node.leaf.p = *p4;
  *d3->node.leaf.ship = *boat4;

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
  node_delete(root,d3->node.leaf.p->x,d3->node.leaf.p->y,root->level);

  print_tree(root,l);
  printf("END\n");*/

  return 0;
}
