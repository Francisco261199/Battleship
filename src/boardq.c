#include <stdio.h>
#include <stdlib.h>
#include "quadtree.h"
#include "ships.h"
#include "boardq.h"
#include "time.h"
#include "menus.h"

char *info1;
char *info2;
//int n_for_each_boat[6] = {1,1,1,1,1,1};

//read buffer to avoid errors when typing chars or strings instead of int
int read_buffer(){
  printf("\033[1;36m");
  char buffer[1024],*a;
  int number;
  while(fgets(buffer,sizeof(buffer),stdin)){
    number = (int)strtol(buffer,&a,10);
    if(a == buffer || *a !='\n'){
      printf("\033[1;31m");
      printf("Invalid type. Please type integer:");
      printf("\033[1;36m");
   }
    else break;
  }
  return number;
}



GAME* init_board(int size){

    float l = (float)size-1.0;

    GAME * pGame = (GAME*)malloc(sizeof(GAME));

    pGame->root1 = create_node(l);
    pGame->root2 = create_node(l);

    pGame->root1->cx /=2;
    pGame->root1->cy /=2;
    pGame->root2->cx /=2;
    pGame->root2->cy /=2;

    info1 = (char*) malloc(size*size*sizeof(char));
    info2 = (char*) malloc(size*size*sizeof(char));

    for(int i = 0; i<size*size; i++){
        info1[i] = _NO_SHOT;
        info2[i] = _NO_SHOT;
    }

  return pGame;
}

void wallhack(char *info,int size){
  printf("\033[1;36m");
  printf("   ");
  for(int i=0;i<size;i++){
    if(i<10) printf(" %d ",i);
    else printf("%d ",i);
  }
  printf("\n");
  printf("\n");

  for(int i=0;i<size;i++){
    if(i<10) printf(" %d ",i);
    else printf("%d ",i);
    for(int j=0;j<size;j++){
      if(info[i*size + j] == _NO_HIT){
        //green
        printf("\033[1;32m");
        printf(" %c ",info[i*size + j]);
        printf("\033[1;36m");
      }
      else printf(" %c ",info[i*size + j]);
    }
    printf("\n");
  }
}

void print_secret_board(char* info,int size){
    printf("\033[1;36m");
  printf("   ");
  for(int i=0;i<size;i++){
    if(i<10) printf(" %d ",i);
    else printf("%d ",i);
  }
  printf("\n");
  printf("\n");

  for(int i=0;i<size;i++){

    if(i<10) printf(" %d ",i);
    else printf("%d ",i);
    for(int j=0;j<size;j++){
      if(info[i*size + j] == _NO_HIT || info[i*size + j] == _NO_SHOT){
        //cyan
        printf("\033[1;36m");
        printf(" %c ",EMPTY);
      }

      else if(info[i*size + j] == _HIT_CELL){
        //green
        printf("\033[1;32m");
        printf(" %c ",info[i*size + j]);
        printf("\033[1;36m");
      }

      else if(info[i*size + j] == _MISSED_SHOT){
        //red
        printf("\033[1;31m");
        printf(" %c ",info[i*size + j]);
        printf("\033[1;36m");
      }
    }
    printf("\n");
  }
}

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

      //insert if possible for given position
      aux = get_subdivision((x+i-2),(y+j-2),root->level/2.0,root->level/2.0,root->level/2.0,root,2);
      if(flag == 1 && insert->node.leaf.ship->bitmap[i][j] == NOT_HIT &&
        (aux->node.leaf.p->x != (x+i-2) || aux->node.leaf.p->y != (y+j-2))){
          points[clear].x = (x+i-2);
          points[clear].y = (y+j-2);
          clear++;
        }
    }
  }
  return clear;
}

//insert_ship
void insert_ship(POINT* p,POINT* points, SHIP* ship, QD_NODE * root,char *info){
  int size = (int) root->level +1;
  QD_NODE* insert = create_node(root->level/2.0);
  *insert->node.leaf.p = *p;
  insert->node.leaf.ship = ship;

  //verify insertion
  if(verify_insert(insert,root,points) != ship->size){
    printf("\033[1;31m");
    printf("ERROR! You can't insert the boat here!\n");
    printf("\033[1;33m");
    printf("Please choose another position.\n");
    printf("X: "); p->x = read_buffer();
    printf("Y: "); p->y = read_buffer();
    printf("\n");
    insert_ship(p,points,ship,root,info);
  }
  else{
    //insert centers
    insert->node.leaf.ship->x = p->x;
    insert->node.leaf.ship->y = p->y;
    for(int i=0 ;i<ship->size;i++){
      *insert->node.leaf.p = points[i];
      (void)node_insert(root,insert);
      info[points[i].x*size + points[i].y]=_NO_HIT;
    }
  }
  free(points);
  points = NULL;
}

int generate_number(int a,int b){
  return (a == 0)? rand()% ++b:rand() % ++b + a;
}

void rand_insert_ships(GAME* g){
  int boat_types[]={2,3,4,5,7,9};
  int map_size,rotation,verify;
  map_size = (int)g->root1->level;


  for(int i=0;i<6;i++){
    for(int j=0;j<n_for_each_boat[i];j++){

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

      QD_NODE* n1 = create_node(g->root1->level);
       *n1->node.leaf.p = *p1;
       *n1->node.leaf.ship = *newship1;
      QD_NODE* n2 = create_node(g->root2->level);
       *n2->node.leaf.p = *p2;
       *n2->node.leaf.ship = *newship2;

      while((verify=verify_insert(n1,g->root1,point_array1)) != newship1->size){
        n1->node.leaf.p->x = generate_number(0,(map_size));
        n1->node.leaf.p->y = generate_number(0,(map_size));
      }

      while((verify=verify_insert(n2,g->root2,point_array2)) != newship2->size){
        n2->node.leaf.p->x = generate_number(0,(map_size));
        n2->node.leaf.p->y = generate_number(0,(map_size));
      }

      *p1 = *n1->node.leaf.p;
      *p2 = *n2->node.leaf.p;

      //insert
      insert_ship(p1,point_array1,newship1,g->root1,info1);
      insert_ship(p2,point_array2,newship2,g->root2,info2);

      free(n1);
      free(n2);
      n1 = NULL;
      n2 = NULL;
    }
  }
}

//Users
void user_insert(GAME* g){
  int size = (int)g->root1->level + 1;
  int boat_types[]={2,3,4,5,7,9};
  char *boats[]={"Destroyer","Cruiser","Battleship","Carrier","Sigma","Pickaxe"};
  int boat_rotation,player;
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
        POINT* p = (POINT*) malloc(sizeof(POINT));
        POINT* points = (POINT*) malloc(boat_types[i]*sizeof(POINT));

        //print map during insertion
        if(player == 1) wallhack(info1,size);
        else wallhack(info2,size);

        printf("\033[1;35m");
        printf("               Player %d: \n",player);
        printf("\033[1;33m");
        printf("|--------------------------------------|\n");
        printf("%s\n",(char*)boats[i]);

        //get valid rotation
        printf("Select Rotation: ");
        boat_rotation = read_buffer();
        while(boat_rotation < 0 || boat_rotation > 3){
          printf("\033[1;31m");
          printf("Invalid rotation. Insert new one: ");
          printf("\033[1;33m");
          boat_rotation = read_buffer();
        }
        printf("X:");
        p->x = read_buffer();
        printf("Y:");
        p->y = read_buffer();

        //player 1 inserts('insert_ship' asks for new coordinates if needed)
        if(player == 1){
          create_ship(newship,boat_rotation,boat_types[i]);
          insert_ship(p,points,newship,g->root1,info1);
          free(p);
          p = NULL;
        }
        //player2 inserts('insert_ship' asks for new coordinates if needed)
        else{
          create_ship(newship,boat_rotation,boat_types[i]);
          insert_ship(p,points,newship,g->root2,info2);
          free(p);
          p = NULL;
        }
      }
    }
  }
}

//atack ship
int attack(int x, int y, QD_NODE* root, char* info){
  //ask for new coordinates if user selects out of bounds position
  if(x>root->level || y>root->level){
    do{
      printf("\033[1;31m");
      printf("Out of bounds. Insert new position:\n");
      printf("\033[1;36m");
      printf("X: ");x = read_buffer();
      printf("Y: ");y = read_buffer();
      printf("\n");
    }while(x>root->level || y>root->level);
  }
  int size = (int)root->level + 1;
  printf("\033[1;36m");
  //boat piece already hit
  if(info[x*size+y] == HIT){
    printf("Already hit(with boat)! Please try again\n");
    //get new coodinates
    printf("X: "); x = read_buffer();
    printf("Y: "); y = read_buffer();
    printf("\n");
    attack(x,y,root,info);
    return 0;
  }

  //has ship
  if(info[x*size+y] == _NO_HIT){
    QD_NODE* get = get_subdivision(x,y,root->level/2.0,root->level/2.0,root->level/2.0,root,2);
    int x1,y1;
    x1 = get->node.leaf.p->x;
    y1 = get->node.leaf.p->y;

    //conversion to bitmap coordinates(map(x,y)->bitmap(x,y))
    int bitmap_x = 2+(x-get->node.leaf.ship->x);
    int bitmap_y = 2+(y-get->node.leaf.ship->y);

    //boat piece not hit
    //update bitmap
    get->node.leaf.ship->bitmap[bitmap_x][bitmap_y] = HIT;
    //update CELL
    info[x*size+y] = _HIT_CELL;
    get->node.leaf.ship->size -= 1;
    //printf("ship.size:%d\n",get->node.leaf.ship->size);
    //ship fully destroyed
    if(get->node.leaf.ship->size == 0){
      printf("Ship Destroyed!\n");
      node_delete(root,get->node.leaf.p->x,get->node.leaf.p->y);
      return 1;
    }
    node_delete(root,get->node.leaf.p->x,get->node.leaf.p->y);
    printf("Hit!\n");
    return 0;
  }

  //no boat(water)
  else{
    //non-attacked position
    if(info[x*size+y] == _NO_SHOT){
      printf("Miss!No Boat!\n");
      printf("\n");
      //update CELL
      info[x*size+y] = _MISSED_SHOT;
      return 0;
    }
    //already attacked position
    if(info[x*size+y] == _MISSED_SHOT){
      printf("Already hit(without boat)! Please try again\n");
      //get new coodinates
      printf("X: "); x = read_buffer();
      printf("Y: "); y = read_buffer();
      printf("\n");
      attack(x,y,root,info);
      return 0;
    }
  }
  return -1;
}

GAME *erase_game(GAME* g){
    free(info1);
    info1=NULL;
    free(info2);
    info2=NULL;

    free(g->root1);
    g->root1 = NULL;
    free(g->root2);
    g->root2 = NULL;

    free(g);
    g=NULL;

    return NULL;
}
