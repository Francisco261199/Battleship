#include <stdio.h>
#include "ships.h"
#include <stdlib.h>


//Create variables in memory
static SHIP  Pickaxe_data =
{
.x=0,
.y=0,
.size = 9,
.bitmap = { {'0', '1', '1', '1', '0'}, //
            {'1', '0', '1', '0', '1'}, //
            {'0', '0', '1', '0', '0'}, //
            {'0', '0', '1', '0', '0'}, //
            {'0', '0', '1', '0', '0'}  //
          }
};

static SHIP  Sigma_data =
{
.x=0,
.y=0,
.size = 7,
.bitmap = { {'0', '0', '1', '1', '0'}, //
            {'0', '0', '1', '0', '0'}, //
            {'0', '0', '1', '0', '0'}, //
            {'0', '0', '1', '0', '0'}, //
            {'0', '1', '1', '0', '0'}  //
          }
};

static SHIP  Carrier_data =
{
.x=0,
.y=0,
.size = 5,
.bitmap = { {'0', '0', '1', '0', '0'}, //
            {'0', '0', '1', '0', '0'}, //
            {'0', '0', '1', '0', '0'}, //
            {'0', '0', '1', '0', '0'}, //
            {'0', '0', '1', '0', '0'}  //
          }
};

static SHIP  Battleship_data =
{
.x=0,
.y=0,
.size = 4,
.bitmap = { {'0', '0', '1', '0', '0'}, //
            {'0', '0', '1', '0', '0'}, //
            {'0', '0', '1', '0', '0'}, //
            {'0', '0', '1', '0', '0'}, //
            {'0', '0', '0', '0', '0'}  //
          }
};

static SHIP  Cruiser_data =
{
.x=0,
.y=0,
.size = 3,
.bitmap = { {'0', '0', '0', '0', '0'}, //
            {'0', '0', '1', '0', '0'}, //
            {'0', '0', '1', '0', '0'}, //
            {'0', '0', '1', '0', '0'}, //
            {'0', '0', '0', '0', '0'}  //
          }
};

static SHIP  Destroyer_data =
{
.x =0,
.y =0,
.size = 2,
.bitmap = { {'0', '0', '0', '0', '0'}, //
            {'0', '0', '1', '0', '0'}, //
            {'0', '0', '1', '0', '0'}, //
            {'0', '0', '0', '0', '0'}, //
            {'0', '0', '0', '0', '0'}  //
          }
};

//assign data to each respective SHIP
 SHIP*  Pickaxe = &Pickaxe_data;
 SHIP*  Sigma = &Sigma_data;
 SHIP*  Carrier = &Carrier_data;
 SHIP*  Battleship = &Battleship_data;
 SHIP*  Cruiser = &Cruiser_data;
 SHIP*  Destroyer = &Destroyer_data;





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
   printf("\033[1;36m");
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

void print_boat(SHIP* ship){
   printf("\033[1;36m");
  for(int i=0;i<5;i++){
     for(int j=0;j<5;j++){
         printf("%c ",ship->bitmap[i][j]);
      }
      printf("\n");
  }
  printf("\n");
}
