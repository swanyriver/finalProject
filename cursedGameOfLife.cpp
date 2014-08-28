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

#include <ctime>
#include <cstdlib>

using namespace std;

void outputWorldINT ( WorldDisplayInterface* display );

int main(){
   /*const int WORLD_WIDTH = 80;
   const int WORLD_HEIGHT = 22;
   const float SATURATION = .3f;*/

   //const float SECOND_PER_FRAME = .25;
   const float SECOND_PER_FRAME = .25 / 4;
   const clock_t TICKS_PER_FRAME = CLOCKS_PER_SEC*SECOND_PER_FRAME;


   swansonUtil::SeedRandom();


   //Walker myCreator(WORLD_WIDTH,WORLD_HEIGHT);
   GOL::LivingCellStartSet genesis;

   /*int max = WORLD_HEIGHT*WORLD_WIDTH;
   myCreator.getSet(genesis,RandomWalker::GetWalkString(max,SATURATION),
           WORLD_WIDTH/2,WORLD_HEIGHT/2);

   God myGod( new MpSWorldBuilder( WORLD_WIDTH , WORLD_HEIGHT , genesis ) );
   WorldDisplayInterface* VoiceOfGod = myGod.GetWorldDisplayInt();*/



   initscr();  //start curses

   start_color();
   init_pair(2,COLOR_GREEN,COLOR_GREEN);

   int row,col;            /* to store the number of rows and *
                   * the number of colums of the screen */
   getmaxyx(stdscr,row,col);    /* get the number of rows and columns */

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

   while(true){
      clock_t timeout=clock() + TICKS_PER_FRAME;
      //put world in window
      outputWorldINT(VoiceOfGod);
      //get next generation
      myGod.Generation();
      //refresh window after delay
      while(clock() < timeout){
         //nothing, just wait
      }
      refresh();

   }


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




