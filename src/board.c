#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include "ships.h"
#include "board.h"
#include <time.h>

// função de criação dos tabuleiros a partir do tamanho indicado

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

  int clear = 0;
  for(int i=0;i<5;i++){
    for(int j=0;j<5;j++){
      int flag = 1;

      //able to place boats in extremity of map even if bitmap doesnt fit
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
    if(check == 1) printf("Boat out of bounds!\n");
    else printf("ERROR! You can't insert boat here!\n");

    printf("Please insert othr position.\n");
    printf("X: "); scanf("%d",&x);
    printf("Y: "); scanf("%d",&y); printf("\n");
    insert_ship(x,y,ship,map,map_size);
  }
  else{
    ship->x=x;
    ship->y=y;
    for(int i=0;i<5;i++){
      for(int j=0;j<5;j++){
        if(ship->bitmap[i][j] == NOT_HIT ){
          map[x+i-2][y+j-2].ship = ship;
          map[x+i-2][y+j-2].shot = _NO_HIT;
        }
      }
    }
  }
}
void user_insert(GAME* g,int nships){
  SHIP newship;
  int boat_types[]={2,3,4,5,7,9};
  char *boats[]={"Destryoer","Cruiser","Battleship","Carrier","Sigma","Pickaxe"};
  int x,y,boat,rotation;
  system("clear");
  printf("Time to insert your boats!\n");
  printf("Remember: X->rows, Y->cols, (y,x)->boat's center\n");
  printf("For rotations you have:\n");
  printf(" ___________\n");
  printf("|           |\n");
  printf("| 0 -> 0º   |\n");
  printf("| 1 -> 90º  |\n");
  printf("| 2 -> 180º |\n");
  printf("| 3 -> 270º |\n");
  printf("|___________|\n");
  int player = 1;
  int boat_rotation;
  for(;player<=2;player++){
    boat_rotation = 0;
    printf("Player %d: \n",player);
    for(int i=0;i<6;i++){
      for(int j=0;j<n_for_each_boat[i];j++){
        if(player == 1){
          printf("%s\n",(char*)boats[i]);
          printf("Select boat rotation: ");scanf("%d",&boat_rotation);
          while(boat_rotation < 0 || boat_rotation > 3){printf("Invalid rotation. Insert new one: ");scanf("%d",&boat_rotation);}
          switch(i){
            case 0: printf("X:");scanf("%d",&x);printf("Y:");scanf("%d",&y);create_ship(&newship,boat_rotation,boat_types[i]);insert_ship(x,y,&newship,g->map1,g->size);break;
            case 1: printf("X:");scanf("%d",&x);printf("Y:");scanf("%d",&y);create_ship(&newship,boat_rotation,boat_types[i]);insert_ship(x,y,&newship,g->map1,g->size);break;
            case 2: printf("X:");scanf("%d",&x);printf("Y:");scanf("%d",&y);create_ship(&newship,boat_rotation,boat_types[i]);insert_ship(x,y,&newship,g->map1,g->size);break;
            case 3: printf("X:");scanf("%d",&x);printf("Y:");scanf("%d",&y);create_ship(&newship,boat_rotation,boat_types[i]);insert_ship(x,y,&newship,g->map1,g->size);break;
            case 4: printf("X:");scanf("%d",&x);printf("Y:");scanf("%d",&y);create_ship(&newship,boat_rotation,boat_types[i]);insert_ship(x,y,&newship,g->map1,g->size);break;
            case 5: printf("X:");scanf("%d",&x);printf("Y:");scanf("%d",&y);create_ship(&newship,boat_rotation,boat_types[i]);insert_ship(x,y,&newship,g->map1,g->size);break;
          }
        }
        else{
          printf("%s\n",(char*)boats[i]);
          printf("Select Rotation: ");scanf("%d",&boat_rotation);
          while(boat_rotation < 0 || boat_rotation > 3){printf("Invalid rotation. Insert new one: ");scanf("%d",&boat_rotation);}
          switch(i){
            case 0: printf("X:");scanf("%d",&x);printf("Y:");scanf("%d",&y);create_ship(&newship,boat_rotation,boat_types[i]);insert_ship(x,y,&newship,g->map2,g->size);break;
            case 1: printf("X:");scanf("%d",&x);printf("Y:");scanf("%d",&y);create_ship(&newship,boat_rotation,boat_types[i]);insert_ship(x,y,&newship,g->map2,g->size);break;
            case 2: printf("X:");scanf("%d",&x);printf("Y:");scanf("%d",&y);create_ship(&newship,boat_rotation,boat_types[i]);insert_ship(x,y,&newship,g->map2,g->size);break;
            case 3: printf("X:");scanf("%d",&x);printf("Y:");scanf("%d",&y);create_ship(&newship,boat_rotation,boat_types[i]);insert_ship(x,y,&newship,g->map2,g->size);break;
            case 4: printf("X:");scanf("%d",&x);printf("Y:");scanf("%d",&y);create_ship(&newship,boat_rotation,boat_types[i]);insert_ship(x,y,&newship,g->map2,g->size);break;
            case 5: printf("X:");scanf("%d",&x);printf("Y:");scanf("%d",&y);create_ship(&newship,boat_rotation,boat_types[i]);insert_ship(x,y,&newship,g->map2,g->size);break;
          }
        }
      }
    }
  }
}

//inserçáo aleatória no tabuleiro
int generate_number(int a,int b){
  return (a == 0)? rand()% ++b:rand() % ++b + a;
}


void rand_insert_ships(GAME* b,int nships){
  int boat_types[]={2,3,4,5,7,9};
  int x1,x2,y1,y2,map_size,boat,rotation;
  map_size = b->size;
  for(int i=0;i<nships;i++){
    x1 = x2 = y1 = y2 = 0;
    SHIP newship;
    rotation = generate_number(0,3);
    x1 = generate_number(0,(map_size-1));
    y1 = generate_number(0,(map_size-1));
    x2 = generate_number(0,(map_size-1));
    y2 = generate_number(0,(map_size-1));
    boat = generate_number(0,5);

    create_ship(&newship,rotation,boat_types[boat]);
    if(verify_insert(x1,y1,&newship,b->map1,map_size) != newship.size){
      while(verify_insert(x1,y1,&newship,b->map1,map_size) != newship.size){
        x1 = generate_number(0,(map_size-1));
        y1 = generate_number(0,(map_size-1));
      }
    }
    if(verify_insert(x2,y2,&newship,b->map2,map_size) != newship.size){
      while(verify_insert(x2,y2,&newship,b->map2,map_size) != newship.size){
        x2 = generate_number(0,(map_size-1));
        y2 = generate_number(0,(map_size-1));
      }
    }
    insert_ship(x1,y1,&newship,b->map1,map_size);
    insert_ship(x2,y2,&newship,b->map2,map_size);
  }
}


//atacar navio
void attack(int x,int y, CELL **map,int size,int state){
   printf("\033[1;36m");
  //permitir ao jogador selecionar nova posição
  //no caso de este ter inserido posição fora do board
  if(x>(size-1) || y>(size-1)){
    printf("check1\n");
    do{
      printf("Out of bounds. Insert new position:\n");
      printf("X: ");scanf("%d",&x);
      printf("Y: ");scanf("%d",&y);printf("\n");
    }while(x>(size-1) || y>(size-1));
  }


  //calculamos as coordenadas no bitmap somando à coordenada central
  //o valor da diferença entre a coordenada que queremos atacar e a
  // coordenada central do barco no mapa do jogo

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
      print_secret_board(map,size);
      if(map[x][y].ship->size == 0){
        printf("Ship Destroyed!");
        state-=1;
      }
      free(map[x][y].ship);
     map[x][y].ship=NULL; 
      return;
    }
    //boat piece hit
    if(map[x][y].ship->bitmap[bitmap_x][bitmap_y] == HIT){
      printf("Already hit(with boat)! Please try again\n");
      //get new coodinates
      printf("X: ");scanf("%d",&x);
      printf("Y: ");scanf("%d",&y);printf("\n");
      attack(x,y,map,size,state);
      return;
    }
  }
  //already hit position and no boat
  else{
    //water and non-attacked
    if(map[x][y].ship == NULL && map[x][y].shot == _NO_SHOT){
      printf("Miss!No Boat!\n");
      //update CELL
      map[x][y].shot = _MISSED_SHOT;
      print_secret_board(map,size);
      return;
    }
    //water and attacked
    if(map[x][y].shot == _MISSED_SHOT){
      printf("Already hit(without boat)! Please try again\n");
      //get new coodinates
      printf("X: ");scanf("%d",&x);
      printf("Y: ");scanf("%d",&y);printf("\n");
      attack(x,y,map,size,state);
      return;
    }
  }
}

void print_secret_board(CELL **map,int size){
  printf("\033[1;36m");
  printf("   ");
  for(int i=0;i<size;i++){
    if(i<10) printf(" %d ",i);

    else    
    printf("%d ",i);
  }
  printf("\n");
  printf("\n");

    for(int i=0;i<size;i++){
      if(i<10) printf(" %d ",i);
  else
      printf("%d ",i);
      for(int j=0;j<size;j++){
          if(map[i][j].shot == _NO_HIT){
              printf(" %c ",_NO_SHOT);
              
          }
          else
          printf(" %c ",map[i][j].shot);
      }
      printf("\n");
    }
  }


//imprime o tabuleiro no seu estado atual
void print_game(int map,GAME * b){
  printf("Gameboard:\n");
  if(map == 1){
    for(int i=0;i<b->size;i++){
      for(int j=0;j<b->size;j++){
        printf("%c ",b->map1[i][j].shot);
      }
      printf("\n");
    }
  }
  else if(map == 2){
    for(int i=0;i<b->size;i++){
      for(int j=0;j<b->size;j++){
        printf("%c ",b->map2[i][j].shot);
      }
      printf("\n");
    }
  }
  else printf("Player does not exist");
  printf("\n");
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
