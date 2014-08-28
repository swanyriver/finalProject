/*
 * cursedGameOfLife.cpp
 *
 *  Created on: Aug 20, 2014
 *      Author: brandon
 */

#include <ncurses.h>

#include "SwansonLibs/swansonInput.hpp"
#include "SwansonLibs/swansonUtils.hpp"
#include "GOL/GameOfLife.hpp"
#include "GOL/God.hpp"
#include "GOL/WalkerMaker.hpp"
#include "GOL/WorldTools.hpp"
#include "GOL/MapSetWorld.hpp"


using namespace std;

const char FASTER = 'w';
const char SLOWER = 's';
const char QUIT = 'q';

const int START_TENTHS_OF_SECOND = 3;
const int MAX_TENTHS_OF_SECOND = 10;

int delayTenthsOfASecond = START_TENTHS_OF_SECOND;

void outputWorldINT ( WorldDisplayInterface* display );
bool input();

int main(){

   //initializations
   swansonUtil::SeedRandom();

   GOL::LivingCellStartSet genesis;


   initscr();  //start curses

   cbreak();
   halfdelay(delayTenthsOfASecond);
   noecho();

   start_color();
   init_pair(2,COLOR_GREEN,COLOR_GREEN);


   ////////pass by refrence
   int row,col;
   getmaxyx(stdscr,row,col);

   const int WORLD_WIDTH = col;
   const int WORLD_HEIGHT = row;
   const float SATURATION = .3f;

   ///find screen size
   /*endwin();

   cout << "row:" << row << " col:" << col << endl;

   exit(0);*/

   Walker myCreator(WORLD_WIDTH,WORLD_HEIGHT);

   int max = WORLD_HEIGHT*WORLD_WIDTH;
   myCreator.getSet(genesis,RandomWalker::GetWalkString(max,SATURATION),
           WORLD_WIDTH/2,WORLD_HEIGHT/2);

   God myGod( new MpSWorldBuilder( WORLD_WIDTH , WORLD_HEIGHT , genesis ) );
   WorldDisplayInterface* VoiceOfGod = myGod.GetWorldDisplayInt();

   do {

      //put world in window
      outputWorldINT(VoiceOfGod);
      //get next generation
      myGod.Generation();
      //refresh window after delay

      refresh();



   } while (input());


   endwin();  /* End curses mode        */

}

/////////////////////////////////////////////////////
///USES WORLD DISPLAY INTERFACE//////////////////////
//////CREATES A STRING TO REPRESENT LIVING CELLS/////
/////////////////////////////////////////////////////

void outputWorldINT ( WorldDisplayInterface* display ){

   clear();
   display->LivingCellBegin();
   while ( !display->LivingCellsEnd()) {
      GOL::cordinate nextCord = display->NextLivingCellLoc();
      mvaddch(nextCord.y,nextCord.x,' ' | COLOR_PAIR(2));

   }
}


//////////////////input

bool input(){
   char input = getch();

   if(input != ERR){
      switch (input) {
         case FASTER:
            halfdelay(--delayTenthsOfASecond);
            break;
         case SLOWER:
            halfdelay(--delayTenthsOfASecond);
            break;
         case QUIT:
            return false;

            break;
         default:
            break;
      }
   }
   return true;
}




