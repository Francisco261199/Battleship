#include <stdio.h>
#include <stdlib.h>
//#include "ships.h"
#include "boardq.h"
#include <time.h>
#include "menus.h"



void game(){
  int player,n;
  int x,y;
  player = 1;
  //color Cyan
  printf("\033[1;36m");

  if( START_GAME_END != menu()){
    printf("Error!");
    exit(1);
  }
  GAME* gameboard = init_board(map_size);


  if(rand_flag==1) rand_insert_ships(gameboard);
  else user_insert(gameboard);

  system("clear");
  printf("Boats inserted successfully! When you're both ready press ENTER to start!\n");
  getchar();

  while(TRUE){
    n=0;
    sleep(1);
    system("clear");
    printf("\033[1;36m");
    printf("PLAYER %d's Turn\n",player);
    printf("Positions hit:\n");

    //print boats in map
    if(player == 1) print_secret_board(info2,map_size);
    else print_secret_board(info1,map_size);


    //get coordinates
    printf("Select coordinates to attack:\n");
    printf("X: ");x = read_buffer();
    printf("Y: ");y = read_buffer();

  if(x>(map_size-1) || y>(map_size-1) || x < 0 || y < 0){
    do{
      printf("\033[1;31m");
      printf("Out of bounds. Insert new position:\n");
      printf("\033[1;36m");
      printf("X: ");x = read_buffer();
      printf("Y: ");y = read_buffer();printf("\n");
    }while(x>(map_size-1) || y>(map_size-1) || x < 0 || y < 0);
  }
    //player x attacks selected position
    switch (player){
      case 1:
        n = attack(x,y,gameboard->root2,info2);
        player=2;
        break;

      case 2:
        n = attack(x,y,gameboard->root1,info1);
        player=1;
        break;
      }

      if(gameboard->root1->nodes_inside == 0){
        printf("Congratulations Player 2, you won!\n");
        break;
      }
      else if(gameboard->root2->nodes_inside == 0){
        printf("Congratulations Player 1,you won!\n");
        break;
      }
    }
    sleep(2);
   gameboard = erase_game(gameboard);
}


int main(){
  srand(time(0));
  while(TRUE) game();
  system("clear");
  return 0;
}
