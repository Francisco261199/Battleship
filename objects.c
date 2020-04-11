#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <ncurses.h>
#include "templates.h"

//Flags for bitmap
#define EMPTY '0'
#define NOT_HIT '1'
#define HIT '2'
#define MISSED_SHOT '3'

//flags for CELL maps
#define _NO_SHOT 0
#define _NO_HIT 1
#define _HIT_CELL 2


typedef struct{
  SHIP *ship;
  int shot;
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

SHIP* create_ship(int orient,int size){
  SHIP *newship = (SHIP*)malloc(sizeof(SHIP));
  switch(size){
    case 2: newship = Destroyer;break;
    case 3: newship = Cruiser;break;
    case 4: newship = Battleship;break;
    case 5: newship = Carrier;break;
    case 7: newship = Sigma;break;
    case 9: newship = Pickaxe;break;
    default : printf("Error. Boat not found!\n"),exit(1);
  }

  switch(orient){
   case 1: rotation_90(newship);break;
   case 2: rotation_180(newship);break;
   case 3: rotation_270(newship);break;
   default : printf("Error. Rotation not possible!\n");exit(1);
 }
 return newship;
}

void insert_ship(int x,int y,SHIP *ship,CELL **map){
  int clear=0; // serve para verificar se todas as posições em que queremos inserir estão desocupadas
  ship->x=x;
  ship->y=y;

  for(int i=0;i<5;i++)
    for(int j=0;j<5;i++)
      if(ship->bitmap[i][j] == NOT_HIT && map[x+i-2][y+j-2].shot == _NO_SHOT) clear++;

  if(clear>=ship->size){
    for(int i=0;i<5;i++){
      for(int j=0;j<5;i++){
        if(ship->bitmap[i][j] == NOT_HIT ){
          map[x+i-2][y+j-2].ship = ship;
          map[x+i-2][y+j-2].shot = _NO_HIT;
        }
      }
    }
  }
  else
  printf("You can't insert your ship here");
}

/*void attack(int x,int y, CELL **map){
  int bitmap_x = 2+(x-map[y][x].ship->x);
  int bitmap_y = 2+(y-map[y][x].ship->y);
  if(map[y][x].ship->bitmap[bitmap_y][bitmap_x] == EMPTY){
    //atualiza o bitmap
    map[y][x].ship->bitmap[bitmap_y][bitmap_x] = MISSED_SHOT;
    //atualiza CELL
    map[y][x].shot = _NO_HIT;
    mvprintw(0,20,"MISS");
    return;
   }
  else if(map[y][x].ship->bitmap[bitmap_y][bitmap_x] == NOT_HIT){
      //update value in bitmap
      map[y][x].ship->bitmap[bitmap_y][bitmap_x] = HIT;
      //update value in CELL map
      map[y][x].shot = _HIT_CELL;
      map[y][x].ship->size -= 1;
      mvprintw(0,20,"HIT");
    }
}*/
/*void insert_ship(int player,SHIP *ship,BOARD * game){
if (player == 1){
    game->map1[x][y]=ship;
}
else
{
    game->map2[x][y]=ship;
}
}
*/

/*void print_game(int player,GAME * b){

    if(player == 1){
      for(int i=0;i<b->size;i++){
          for(int j=0;j<b->size;j++){
              printf("%d ",b->map1[i][j].shot);
            }
            printf("\n");
          }
        }
else if(player == 2){
    for(int i=0;i<b->size;i++){
        for(int j=0;j<b->size;j++){
            printf("%d ",b->map2[i][j].shot);
        }
        printf("\n");
    }
  }

  else printf("Player does not exist");
}*/

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
  int n;
  printf("Selecione o tamanho do mapa\n");
	scanf("%d",&n);
	system("clear");

//GAME* gameboard = init_board(n);

SHIP* boat1 = create_ship(1,7);
//SHIP* boat2 = Carrier;
//SHIP* boat3 = Battleship;
//SHIP* boat4 = Sigma;
//SHIP* boat5 = Cruiser;
//SHIP* boat6 = Destroyer;

//rotation_270(boat1);
print_boat(boat1);

//insert_ship(1,2,3,'x',gameboard);
//insert_ship(2,3,4,'*',gameboard);
//print_game(1,gameboard);

printf("\n");

//print_game(2,gameboard);

//erase_game(gameboard);

//print_game(1,gameboard);

return 0;

}
