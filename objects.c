#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <ncurses.h>
#include "templates.h"
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

#define NORTH 0
#define SOUTH 1
#define WEST  2
#define EAST  3

typedef struct{
  SHIP *ship;
  char shot;
  //int shot_count;
} CELL;


typedef struct{
    CELL **map1;
    CELL **map2;
    int size;
    int state1,state2;
} GAME;


/*typedef struct Coordinate{
    int x;
    int y;
} COORD;*/


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

void rotation_90(SHIP *ship){
//rotatçao por camadas externas do cubo
////////////////////////////
//   i=0      i=1    i=2 //
//  11111                //
//  1...1     222        //
//  1...1 --> 2.2 --> 3  //
//  1...1     222        //
//  11111                //
//                       //
///////////////////////////
for(int i=0;i<5/2;i++){
  for(int j=i;j<5-i-1;j++){
    int temp = ship->bitmap[i][j];
    ship->bitmap[i][j] = ship->bitmap[5-1-j][i];
    ship->bitmap[5-1-j][i] = ship->bitmap[5-1-i][5-1-j];
    ship->bitmap[5-1-i][5-1-j] = ship->bitmap[j][5-1-i];
    ship->bitmap[j][5-1-i] = temp;
    }
  }
}
void rotation_180(SHIP* ship){
  for(int i=0;i<5/2;i++){
    for(int j=0;j<3;j++){
      int temp1 = ship->bitmap[i][j];
      int temp2 = ship->bitmap[i][5-1-j];
      ship->bitmap[i][j] = ship->bitmap[5-i-1][j];
      ship->bitmap[i][5-1-j] = ship->bitmap[5-i-1][5-1-j];
      ship->bitmap[5-i-1][j]=temp1;
      ship->bitmap[5-i-1][5-1-j]=temp2;
    }
  }
}

void rotation_270(SHIP* ship){
  for(int i=0;i<5/2;i++){
    for(int j=i;j<5-i-1;j++){
      int temp = ship->bitmap[i][j];
      ship->bitmap[i][j] = ship->bitmap[j][5-1-i];
      ship->bitmap[j][5-1-i] = ship->bitmap[5-1-i][5-1-j];
      ship->bitmap[5-1-i][5-1-j] = ship->bitmap[5-1-j][i];
      ship->bitmap[5-1-j][i] = temp;
    }
  }
}

void create_ship(SHIP* newship,int orient,int size){
  //SHIP *newship = (SHIP*)malloc(sizeof(SHIP));
  switch(size){
    case 2: *newship = *Destroyer;break;
    case 3: *newship = *Cruiser;break;
    case 4: *newship = *Battleship;break;
    case 5: *newship = *Carrier;break;
    case 7: *newship = *Sigma;break;
    case 9: *newship = *Pickaxe;break;
    default : printf("Error. Boat not found!\n"),exit(1);
  }

  switch(orient){
   case 0: break;
   case 1: rotation_90(newship);break;
   case 2: rotation_180(newship);break;
   case 3: rotation_270(newship);break;
   default : printf("Error. Rotation not possible!\n");exit(1);
 }
 //return newship;
}
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

int generate_number(int a,int b){
  return (a == 0)? rand()% ++b:rand() % ++b + a;
}

void rand_insert_ship(int map_size,CELL** map,int nships, SHIP** boat){
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

/*void * erase_game(GAME * game){

  for(int i =0;i<game->size;i++){
    CELL* map1Pos=game->map1[i];
    CELL* map2Pos = game->map2[i];
    free(map1Pos);
    free(map2Pos);
  }


//free(game->map1);
//free(game->map2);

  free(game);

  game = NULL;
  game->map1 = NULL;
  game->map2 = NULL;

  assert(game == NULL);
  assert(game->map2 == NULL);
}*/

 //inicializar um tabuleiro;
///void board_insert(ship ships[],BOARD board,int player)




/*char **init_map(){
char **map= (char **)malloc(n*sizesizeof(char*));
for(int i=0;i<size;i++){
	map[i]=(char*)malloc(size*sizeof(char));
for(int j=0;j<size;j++){
	map[i][j]='+';
}
}
S
return map;
	}
*/
void print_boat(SHIP* ship){
  for(int i=0;i<5;i++){
     for(int j=0;j<5;j++){
         printf("%c ",ship->bitmap[i][j]);
      }
      printf("\n");
  }
  printf("\n");
}



int main(){
srand(time(0));
int n;
printf("Selecione o tamanho do mapa\n");
scanf("%d",&n);
system("clear");

GAME* gameboard = init_board(n);
//SHIP* boat1 = create_ship(1,7);
//SHIP* boat2 = (SHIP*) malloc(sizeof(SHIP));
//SHIP* boat3 = (SHIP*) malloc(sizeof(SHIP));
//create_ship(boat2,1,2);
//create_ship(boat3,0,2);
SHIP** boat;

rand_insert_ship(gameboard->size,gameboard->map1,7,boat);
//print_boat(boat2);
//print_boat(boat3);

//insert_ship(12,15,boat1,gameboard->map1,gameboard->size,1);
//insert_ship(9,3,boat2,gameboard->map1,gameboard->size);
//insert_ship(10,13,boat3,gameboard->map1,gameboard->size);
print_game(1,gameboard);

//attack(10,3,gameboard->map1,gameboard->size);
//printf("...\n");
//print_game(1,gameboard);

//attack(10,4,gameboard->map1,gameboard->size);
//printf("...\n");
//print_game(1,gameboard);

//attack(10,4,gameboard->map1,gameboard->size);
//print_game(1,gameboard);
//attack(13,21,gameboard->map1);
//print_game(1,gameboard);

printf("\n");

//erase_game(gameboard);

return 0;

}
