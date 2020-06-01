#include <stdio.h>
#include <stdlib.h>
#include "quadtree."
#include "ships.h"
#include "boardq.h"
#include "time.h"

//read buffer to avoid errors when typing chars or strings instead of int
int read_buffer(){
  printf("\033[1;36m");
  char buffer[1024],*a;
  int number;
  while(fgets(buffer,sizeof(buffer),stdin)){
    number = (int)strtol(buffer,&a,10);
    if(a == buffer || *a !='\n') printf("Invalid type. Please type integer:");
    else break;
  }
  return number;
}


GAME* init_board(int size){

    float l = (float) size;

    GAME * pGame = (GAME*)malloc(sizeof(GAME));

    pGame->root1 = create_node(l);
    pGame->root2 = create_node(l);

    pGame->root1->cx /=2;
    pGame->root1->cy /=2;
    pGame->root2->cx /=2;
    pGame->root2->cy /=2;
    
    info1 = (CHAR*) malloc(size*size*sizeof(CHAR));
    info2 = (CHAR*) malloc(size*size*sizeof(CHAR));

    
  return pGame;
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
        if(player == 1) print_game(map1);
        else print_game(map2);

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
        p->x = read_buffer();
        printf("Y:");
        p->y = read_buffer();

        //player 1 inserts('insert_ship' asks for new coordinates if needed)
        if(player == 1){
          create_ship(newship,boat_rotation,boat_types[i]);
          insert_ship(p,points,newship,g->root1);
        }
        //player2 inserts('insert_ship' asks for new coordinates if needed)
        else{
          create_ship(newship,boat_rotation,boat_types[i]);
          insert_ship(p,points,newship,g->root2);
        }
      }
    }
  }
}

//atack ship
int attack(int x, int y, QD_NODE* root, char map){
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

  printf("\033[1;36m");

  QD_NODE* get = get_subdivision(x,y,root->level/2.0,root->level/2.0,root->level/2.0,root,2);
  int x1,y1,size;
  x1 = (int)get->node.leaf.p->x;
  y1 = (int)get->node.leaf.p->y;
  size = (int)root->level + 1;
  //has ship
  if(get->node.leaf.ship != NULL){

    //conversion to bitmap coordinates(map(x,y)->bitmap(x,y))
    int bitmap_x = 2+(x-get->node.leaf.ship->x);
    int bitmap_y = 2+(y-get->node.leaf.ship->y);

    //boat piece not hit
    if(get->node.leaf.ship->bitmap[bitmap_x][bitmap_y] == NOT_HIT){
      //update bitmap
      get->node.leaf.ship->bitmap[bitmap_x][bitmap_y] = HIT;
      //update CELL
      map[x1*size+y1] = _HIT_CELL;
      get->node.leaf.ship->size -= 1;
      node_delete(root,get->node.leaf.p->x,get->node.leaf.p->y);
      printf("Hit!\n");
      //ship fully destroyed
      if(get->node.leaf.ship->size == 0){
        printf("Ship Destroyed!\n");
        return 1;
      }
      return 0;
    }

    //boat piece already hit
    if(map[x1*size+y1] == HIT){
      printf("Already hit(with boat)! Please try again\n");
      //get new coodinates
      printf("X: "); x = read_buffer();
      printf("Y: "); y = read_buffer();
      printf("\n");
      attack(x,y,root,map);
      return 0;
    }
  }

  //no boat(water)
  else{
    //non-attacked position
    if(get->node.leaf.ship == NULL && map[x1*size+y1] == _NO_SHOT){
      printf("Miss!No Boat!\n");
      printf("\n");
      //update CELL
      map[x1*size+y1] = _MISSED_SHOT;
      return 0;
    }
    //already attacked position
    if(map[x1*size+y1] == _MISSED_SHOT){
      printf("Already hit(without boat)! Please try again\n");
      //get new coodinates
      printf("X: "); x = read_buffer();
      printf("Y: "); y = read_buffer();
      printf("\n");
      attack(x,y,root,map);
      return 0;
    }
  }
  return -1;
}
