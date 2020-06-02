#include<stdio.h>
#include<stdlib.h>


int main(){
  char *ships[5]={"Carriers","Battleship","Cruiser","Submarines","Destryoers"};
  int board_size=0;
  {
  //while(1){
    int *n_ships= malloc (5*sizeof(int));
    printf("\tWelcome to Battleship\n");

    printf("        .First select your board's size: ");
    scanf("%d",&board_size);


    printf("\n\n");
    printf("        .Now lets choose your ships.\n");
    for(int i=0;i<5;i++){
      printf("      .Type number of %s: ", ships[i]);
      scanf("%d",n_ships++);
    }
    start_game(&n_ships,board_size);
    //free(n_ships);
  }
  printf("\n\nThank you for playing the game.\nHope you comeback for more incredible fights.\n");
  printf("\nMade by:  Amadeus Marques && Francisco Ribeiro\n\n");
  printf("All rights reserved to the developers.2020.\n");
  return 0;
}
