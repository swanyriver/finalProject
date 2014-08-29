/*
 * cursedGameOfLife.cpp
 *
 *  Created on: Aug 20, 2014
 *      Author: brandon
 */

#include <ncurses.h>
#include <string>
#include <exception>
#include <fstream>

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


//----------------------------------------------------------------------
// Requirement #12: demonstrate cstring and string
//----------------------------------------------------------------------
//   See also WalkerMaker.hpp

const char logFileName[9] = {'l','i','f','e','.','l','o','g','\0'};

const string ARRAY_WORLD_ARG = "-a";
const string MANUAL_SCREEN_SIZE = "-s";

void getScreenSize(int &row, int &col);
void outputWorldINT ( WorldDisplayInterface* display , int numberOfGenerations,
      int height );
bool input();


int main( int argc , char **argv){

   ///////////////////////////////////////////////////////////////////////
   ///////////////////read command line arguments ////////////////////////
   ///////////////////////////////////////////////////////////////////////
   bool ManualScreenSize = false;
   bool ArrayWorld = false;

   //----------------------------------------------------------------------
   // Requirement #15: demonstrate command line arguments
   //----------------------------------------------------------------------
   // checks for -s and -a
   // affects the setting of manual screen size and usage of array world
   for(int i=1;i<argc;i++){

      if(ARRAY_WORLD_ARG == string(argv[i])){
         ArrayWorld = true;
      }
      if(MANUAL_SCREEN_SIZE == string(argv[i])){
         ManualScreenSize = true;
      }
   }

   //initializations
   swansonUtil::SeedRandom();

   GOL::LivingCellStartSet genesis;


   initscr();  //start curses


   ////////////////////////////////////////////////////////////////////////
   ////////////determine screen resolution/////////////////////////////////
   ////////////////////////////////////////////////////////////////////////

   //----------------------------------------------------------------------
   // Requirement #11: Demonstrate passing by reference
   //----------------------------------------------------------------------
   //row and col are passed by reference to either getmaxyx() or getScreenSize
   //these values are modified by the function and those modifications persist
   int row,col;


   if(ManualScreenSize){
      getScreenSize(row,col);
   }else{

      //----------------------------------------------------------------------
      // Requirement #25: Demonstrate exception
      //----------------------------------------------------------------------
      class dimensions_missing{};
      try{
         getmaxyx(stdscr,row,col);

         if( row == 0 || col == 0 ){
            dimensions_missing exep;
            throw (exep);
         }
      }
      catch(dimensions_missing&){
         getScreenSize(row,col);
      }
   }

   //----------------------------------------------------------------------
   // Requirement #10: Demonstrate scope
   //----------------------------------------------------------------------
   // these are defined locally hence they are only available locally
   // that is why height is passed to the display function
   const int WORLD_WIDTH = col;
   const int WORLD_HEIGHT = row-1;
   const float SATURATION = .3f;

   ////////////////////////////////////////////////////////////////////////
   ///////////////initialize game of life cells///////////////////////////
   ////////////////////////////////////////////////////////////////////////

   Walker myCreator(WORLD_WIDTH,WORLD_HEIGHT);

   int max = WORLD_HEIGHT*WORLD_WIDTH;
   myCreator.getSet(genesis,RandomWalker::GetWalkString(max,SATURATION),
           WORLD_WIDTH/2,WORLD_HEIGHT/2);

   God myGod( new MpSWorldBuilder( WORLD_WIDTH , WORLD_HEIGHT , genesis ) );
   WorldDisplayInterface* VoiceOfGod = myGod.GetWorldDisplayInt();


   ////////////////////////////////////////////////////////////////////////
   ////////////////initializing window settings///////////////////////////
   ////////////////////////////////////////////////////////////////////////
   cbreak();
   halfdelay(delayTenthsOfASecond);
   noecho();

   start_color();
   init_pair(2,COLOR_GREEN,COLOR_GREEN);


   do {

      //put world in window
      outputWorldINT(VoiceOfGod, myGod.GenerationsPassed(), WORLD_HEIGHT);

      //get next generation
      myGod.Generation();
      //refresh window after delay

      refresh();

   } while (input());


   endwin();

}

void getScreenSize(int &row, int &col){
   endwin();
   cout << "Please manually enter your screen resolution";

   row =swansonInput::GetInt("Rows: ",0,150);
   col =swansonInput::GetInt("Columns: ",0,150);

   initscr();

}

/////////////////////////////////////////////////////
///USES WORLD DISPLAY INTERFACE//////////////////////
//////CREATES A STRING TO REPRESENT LIVING CELLS/////
/////////////////////////////////////////////////////

void outputWorldINT ( WorldDisplayInterface* display , int numberOfGenerations,
      int height){

   clear();
   display->LivingCellBegin();
   while ( !display->LivingCellsEnd()) {
      GOL::cordinate nextCord = display->NextLivingCellLoc();
      mvaddch(nextCord.y,nextCord.x,' ' | COLOR_PAIR(2));
   }

   string bottomLine = "Generations:";
   bottomLine+= swansonString::GetString(numberOfGenerations);
   bottomLine+= "  Speed up:";
   bottomLine+= swansonString::GetString(FASTER);
   bottomLine+= " down:";
   bottomLine+= swansonString::GetString(SLOWER);
   bottomLine+= " Quit:";
   bottomLine+= swansonString::GetString(QUIT);

   mvaddstr(height,0,bottomLine.c_str());
}


//////////////////input

bool input(){
   char input = getch();

   if(input != ERR){
      switch (input) {
         case FASTER:
            if(delayTenthsOfASecond > 1){
               halfdelay(--delayTenthsOfASecond);
            }
            break;
         case SLOWER:
            if(delayTenthsOfASecond < MAX_TENTHS_OF_SECOND){
               halfdelay(++delayTenthsOfASecond);
            }
            break;
         case QUIT:
            return false;

            break;
         default:
            break;
      }

      //----------------------------------------------------------------------
      // Requirement #07: Demonstrate debugging trick
      //----------------------------------------------------------------------
      // This allows me to check the functioning of this input function
      // I can observe the output live using 'tail filename -f'

      ////logfile output
      ofstream log(logFileName, ios::app);
      log << "input received: " << input
            << " , updating every " << delayTenthsOfASecond
            << " tenths of a second" << endl;
      log.close();

   }

   return true;
}




