#include<unistd.h>
#include<stdlib.h>
#include<ncurses.h>


int Start_menu(){
  refresh();
  //noecho();
  //system("clear");
  mvprintw(0,85,"Welcome to:");
  mvprintw(1,80,"   XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX");
  mvprintw(2,80," XX   ___  ____ ___ ___      ____ ____ _  _ _ ___  XX");
  mvprintw(3,80,"XXX   |__] |__|  |   |  |    |___ [__  |__| | |__]  XXX");
  mvprintw(4,80,"XXX   |__] |  |  |   |  |___ |___ ___] |  | | |     XXX");
  mvprintw(5,80," XX                                                XX");
  mvprintw(6,80,"   XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX");

  mvprintw(53,0,"1-Start Game");
  mvprintw(54,0,"2-Credits");
  mvprintw(55,0,"3-Leave");

  while(!getch()){
    if(getch()==1){ return 1;}
    else if(getch()==2){ return 2;}
    else if(getch()==3){ return 3;}
  }
  return 0;
}
int Credits(){
  refresh();
  system("clear");
  //noecho();
  mvprintw(0,0,"                                            Made By:\n");
  mvprintw(1,0,"                         ##################################################\n");
  mvprintw(2,0,"                         #                Amadeus Marques                 #\n");
  mvprintw(3,0,"                         ##                     &&                       ##\n");
  mvprintw(4,0,"                         #               Francisco Ribeiro                #\n");
  mvprintw(5,0,"                         ##################################################\n");
  mvprintw(6,0,"                         All rights reserved to the developers 2020.\n");

  mvprintw(20,0,"1-Back");
  //int *p=(int*)malloc(sizeof(int));
  while(getch()!=1) getch();
  return 1;
}

void Leave(){
  refresh();
  //noecho();
  system("clear");
  mvprintw(0,40,"Thank you for playing the game.\nHope you comeback for more incredible fights.");
  exit(0);
}


void menu_select(){
  initscr();
  int state = Start_menu();
  for(;;){
    //if(state == 1) Start_game();
    if(state == 2){
      Credits();
      state = Start_menu();
      continue;
    }
    if(state == 3) Leave();
  }
  //stop ncurses screen
  endwin();
}
int main(){
  menu_select();
  return 0;
}
