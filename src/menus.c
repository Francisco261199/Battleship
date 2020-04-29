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
  mvprintw(2,0,"                         #                Amadeu Marques                 #\n");
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

void prints_ships(){
initscr();

start_color();
init_pair(1,COLOR_CYAN,COLOR_BLACK);
attron(COLOR_PAIR(1));

    mvprintw(0,18,"Select Ships:");
    mvprintw(2,0,"2-Destroyer");   mvprintw(2,20,"3-Cruiser");       mvprintw(2,40,"4-Battleship");
  
    mvprintw(4,0,"0 0 0 0 0");     mvprintw(4,20,"0 0 0 0 0");       mvprintw(4,40,"0 0 1 0 0");     
    mvprintw(5,0,"0 0 1 0 0");     mvprintw(5,20,"0 0 1 0 0");       mvprintw(5,40,"0 0 1 0 0");     
    mvprintw(6,0,"0 0 X 0 0");     mvprintw(6,20,"0 0 X 0 0");       mvprintw(6,40,"0 0 X 0 0");     
    mvprintw(7,0,"0 0 0 0 0");     mvprintw(7,20,"0 0 1 0 0");       mvprintw(7,40,"0 0 1 0 0");     
    mvprintw(8,0,"0 0 0 0 0");     mvprintw(8,20,"0 0 0 0 0");       mvprintw(8,40,"0 0 0 0 0");     
   
  
    mvprintw(10,0,"5-Carrier");    mvprintw(10,20,"7-Sigma");        mvprintw(10,40,"9-Pickaxe");
   
    mvprintw(12,0,"0 0 1 0 0");    mvprintw(12,20,"0 0 1 1 0");      mvprintw(12,40,"0 1 1 1 0");   
    mvprintw(13,0,"0 0 1 0 0");    mvprintw(13,20,"0 0 1 0 0");      mvprintw(13,40,"1 0 1 0 1");   
    mvprintw(14,0,"0 0 X 0 0");    mvprintw(14,20,"0 0 X 0 0");      mvprintw(14,40,"0 0 X 0 0");   
    mvprintw(15,0,"0 0 1 0 0");    mvprintw(15,20,"0 0 1 0 0");      mvprintw(15,40,"0 0 1 0 0");   
    mvprintw(16,0,"0 0 1 0 0");    mvprintw(16,20,"0 1 1 0 0");      mvprintw(16,40,"0 0 1 0 0");    
    


    mvprintw(18,0,"Rotation:");
    mvprintw(20,0,"0-None");     mvprintw(20,10,"1-Left");
    mvprintw(22,0,"2-Reverse"); mvprintw(22,10,"3-Right");

    mvprintw(24,0,"Please insert the ship size and orientation:");
  
    getch();
    endwin();
}
int main(){
  menu_select();
  return 0;
}
