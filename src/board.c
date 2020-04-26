#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <ncurses.h>
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
          pGame->map2[i][j].shot= _NO_SHOT;
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
      if(ship->bitmap[i][j] == NOT_HIT && map[x+i-2][y+j-2].shot == _NO_SHOT) clear++;
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

//inserçáo aleatória no tabuleiro
int generate_number(int a,int b){
  return (a == 0)? rand()% ++b:rand() % ++b + a;
}

/*void rand_insert_ship(int map_size,CELL** map,int nships, SHIP** boat){
  int boat_types[]={2,3,4,5,7,9};
  for(int i=0;i<nships;i++){
    int x=0;
    int y=0;
    while(TRUE){
      int rotation = generate_number(0,3);
      x = generate_number(0,(map_size-1));
      y = generate_number(0,(map_size-1));
      int boat = generate_number(0,5);
      create_ship(*boat[i],rotation,boat_types[boat]);
      if(verify_insert(x,y,*boat[i],map,map_size) != *boat[i]->size) continue;
      else break;
    }
    insert_ship(x,y,boat[i],map,map_size);
  }
}

*/

//atacar navio
void attack(int x,int y, CELL **map,int size){
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
      return;
    }
    //boat piece hit
    if(map[x][y].ship->bitmap[bitmap_x][bitmap_y] == HIT){
      printf("Already hit(with boat)! Please try again\n");
      //get new coodinates
      printf("X: ");scanf("%d",&x);
      printf("Y: ");scanf("%d",&y);printf("\n");
      attack(x,y,map,size);
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
      return;
    }
    //water and attacked
    if(map[x][y].shot == _MISSED_SHOT){
      printf("Already hit(without boat)! Please try again\n");
      //get new coodinates
      printf("X: ");scanf("%d",&x);
      printf("Y: ");scanf("%d",&y);printf("\n");
      attack(x,y,map,size);
      return;
    }
  }
}

//imprime o tabuleiro no seu estado atual
void print_game(int player,GAME * b){
  printf("Gameboard:\n");
  if(player == 1){
    for(int i=0;i<b->size;i++){
      for(int j=0;j<b->size;j++){
        printf("%c ",b->map1[i][j].shot);
      }
      printf("\n");
    }
  }
  else if(player == 2){
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
