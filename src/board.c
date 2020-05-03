#include <stdio.h>
#include <stdlib.h>
#include "ships.h"
#include "board.h"
#include "menus.h"

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

// inserção do navio no tabuleiro
int verify_insert(int x, int y, SHIP* ship, CELL**map, int map_size){
  if(x>(map_size-1) || y>(map_size-1)) return 1;
  int flag,clear;
  clear = 0;
  for(int i=0;i<5;i++){
    for(int j=0;j<5;j++){
      flag = 1;
      //able to place boats in extremity of map even if bitmap doesn't fit
      if( ((x+i-2) >= map_size || (y+j-2) >= map_size || (x+i-2) < 0 || (y+j-2) < 0 ) && ship->bitmap[i][j] == EMPTY) flag=0;
      //out of bounds
      else if( ((x+i-2) >= map_size || (y+j-2) >= map_size || (x+i-2) < 0 || (y+j-2) < 0 ) && ship->bitmap[i][j] != EMPTY) return 1;
      //insert is possible for given position
      if(flag == 1 && ship->bitmap[i][j] == NOT_HIT && map[x+i-2][y+j-2].shot == _NO_SHOT) clear++;
    }
  }
  return clear;
}

void insert_ship(int x,int y,SHIP* ship,CELL** map, int map_size){
  int check = verify_insert(x,y,ship,map,map_size);
  if(check != ship->size){

    printf("\033[1;31m");
    printf("ERROR! You can't insert the boat here!\n");
    printf("\033[1;33m");
    printf("Please choose another position.\n");
    printf("X: "); x = read_buffer();
    printf("Y: "); y = read_buffer();
    printf("\n");

    insert_ship(x,y,ship,map,map_size);
  }
  else{
    for(int i=0;i<5;i++){
      for(int j=0;j<5;j++){
        if(ship->bitmap[i][j] == NOT_HIT ){
          map[x+i-2][y+j-2].ship = ship;
          map[x+i-2][y+j-2].shot = _NO_HIT;
          map[x+i-2][y+j-2].ship->x = x;
          map[x+i-2][y+j-2].ship->y = y;
        }
      }
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
//inserçáo aleatória no tabuleiro
int generate_number(int a,int b){
  return (a == 0)? rand()% ++b:rand() % ++b + a;
}

void rand_insert_ships(GAME* b){
  int boat_types[]={2,3,4,5,7,9};
  int x1,x2,y1,y2,map_size,rotation,verify;
  map_size = b->size;

  for(int i=0;i<6;i++){
    for(int j=0;j<n_for_each_boat[i];j++){

      //create ships
      SHIP* newship1 = (SHIP*) malloc(sizeof(SHIP));
      SHIP* newship2 = (SHIP*) malloc(sizeof(SHIP));
      rotation = generate_number(0,3);
      create_ship(newship1,rotation,boat_types[i]);
      create_ship(newship2,rotation,boat_types[i]);

      //get random coordinates
      x1 = generate_number(0,(map_size-1));
      y1 = generate_number(0,(map_size-1));
      x2 = generate_number(0,(map_size-1));
      y2 = generate_number(0,(map_size-1));

      //verify if insertion is possible
      while((verify=verify_insert(x1,y1,newship1,b->map1,map_size)) != newship1->size){
        x1 = generate_number(0,(map_size-1));
        y1 = generate_number(0,(map_size-1));
      }

      while((verify=verify_insert(x2,y2,newship2,b->map2,map_size)) != newship2->size){
        x2 = generate_number(0,(map_size-1));
        y2 = generate_number(0,(map_size-1));
      }

      //insert
      insert_ship(x1,y1,newship1,b->map1,map_size);
      insert_ship(x2,y2,newship2,b->map2,map_size);
    }
  }
}

//atack ship
int attack(int x,int y, CELL **map,int size){
  //ask for new coordinates if user selects out of bounds position
  if(x>(size-1) || y>(size-1)){
    do{
      printf("\033[1;31m");
      printf("Out of bounds. Insert new position:\n");
      printf("\033[1;36m");
      printf("X: ");x = read_buffer();
      printf("Y: ");y = read_buffer();
      printf("\n");
    }while(x>(size-1) || y>(size-1));
  }

  printf("\033[1;36m");

  //has ship
  if(map[x][y].ship != NULL){

    //conversion to bitmap coordinates(map(x,y)->bitmap(x,y))
    int bitmap_x = 2+(x-map[x][y].ship->x);
    int bitmap_y = 2+(y-map[x][y].ship->y);

    //boat piece not hit
    if(map[x][y].ship->bitmap[bitmap_x][bitmap_y] == NOT_HIT){
      //update bitmap
      map[x][y].ship->bitmap[bitmap_x][bitmap_y] = HIT;
      //update CELL
      map[x][y].shot = _HIT_CELL;
      map[x][y].ship->size -= 1;
      printf("Hit!\n");

      //ship fully destroyed
      if(map[x][y].ship->size == 0){
        printf("Ship Destroyed!\n");
        return 1;
      }
      return 0;
    }

    //boat piece already hit
    if(map[x][y].ship->bitmap[bitmap_x][bitmap_y] == HIT){
      printf("Already hit(with boat)! Please try again\n");
      //get new coodinates
      printf("X: "); x = read_buffer();
      printf("Y: "); y = read_buffer();
      printf("\n");
      attack(x,y,map,size);
      return 0;
    }
  }

  //no boat(water)
  else{
    //non-attacked position
    if(map[x][y].ship == NULL && map[x][y].shot == _NO_SHOT){
      printf("Miss!No Boat!\n");
      printf("\n");
      //update CELL
      map[x][y].shot = _MISSED_SHOT;
      return 0;
    }
    //already attacked position
    if(map[x][y].shot == _MISSED_SHOT){
      printf("Already hit(without boat)! Please try again\n");
      //get new coodinates
      printf("X: "); x = read_buffer();
      printf("Y: "); y = read_buffer();
      printf("\n");
      attack(x,y,map,size);
      return 0;
    }
  }
}

//imprime o tabuleiro no seu estado atual
void print_game(CELL** map,int size){
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
      if(map[i][j].shot == _NO_HIT){
        //green
        printf("\033[1;32m");
        printf(" %c ",map[i][j].shot);
        printf("\033[1;36m");
      }
      else printf(" %c ",map[i][j].shot);
    }
    printf("\n");
  }
}


void print_secret_board(CELL **map,int size){
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
      if(map[i][j].shot == _NO_HIT || map[i][j].shot == _NO_SHOT){
        //cyan
        printf("\033[1;36m");
        printf(" %c ",EMPTY);
      }
      else if(map[i][j].shot == _HIT_CELL){
        //green
        printf("\033[1;32m");
        printf(" %c ",map[i][j].shot);
        printf("\033[1;36m");
      }
      else if(map[i][j].shot == _MISSED_SHOT){
        //red
        printf("\033[1;31m");
        printf(" %c ",map[i][j].shot);
        printf("\033[1;36m");
      }
    }
    printf("\n");
  }
}

GAME* erase_game(GAME* board){
  for(int i=0;i<board->size;i++){
    free(board->map1[i]);
    free(board->map2[i]);
    board->map1[i]=NULL;
    board->map2[i]=NULL;
  }

  free(board->map1);
  free(board->map2);

  board->map1=NULL;
  board->map2=NULL;

  free(board);
  return NULL;
}
