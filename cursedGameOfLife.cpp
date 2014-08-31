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
#include <iostream>
#include <cstdlib>

//#include "SwansonLibs/swansonInput.hpp"
//#include "SwansonLibs/swansonUtils.hpp"

#include "SwansonFunctions.hpp"

#include "GOL/GameOfLife.hpp"
#include "GOL/God.hpp"
#include "GOL/WalkerMaker.hpp"
#include "GOL/WorldTools.hpp"
#include "GOL/MapSetWorld.hpp"
#include "GOL/arrayWorld.hpp"


using namespace std;

// Control keys for game of life
const char FASTER = 'w';
const char SLOWER = 's';
const char QUIT = 'q';

// controls for halfDelay(),  sets speed of animation
const int START_TENTHS_OF_SECOND = 3;
const int MAX_TENTHS_OF_SECOND = 10;

//----------------------------------------------------------------------
// Requirement #10: Demonstrate scope
//----------------------------------------------------------------------
// this variable is defined globally, therefore it is accessible to
// all functions in this .cpp file
int delayTenthsOfASecond = START_TENTHS_OF_SECOND;


//----------------------------------------------------------------------
// Requirement #12: demonstrate cstring and string    C-STRING
//----------------------------------------------------------------------
const char logFileName[9] = {'l','i','f','e','.','l','o','g','\0'};


////////////////////////////////////////////////////////
//////////////command line arguments////////////////////
////////////////////////////////////////////////////////
const string ARRAY_WORLD_ARG = "-a";      // uses array world class
const string MANUAL_SCREEN_SIZE = "-s";   // allows for input of screen size
const string NO_COLOR_ARG = "-c";         // disables color mode
const string HELP_ARG = "--help";         // display readme.txt

const string HELP_FILENAME = "readme.txt"; // filename of help file


////////////////////////////////////////////////////////
////////////Function Definitions////////////////////////
////////////////////////////////////////////////////////

//manual input of screen size
void getScreenSize(int &row, int &col);

//display the state of the game of life world
void outputWorldINT ( WorldDisplayInterface* display , int numberOfGenerations,
      int height );

//get keyboard input between screen updates
bool input();

//display contents of readme.txt
void help();



///////////////////////////////////////////////////////////////////////
//////////Int main()     main program//////////////////////////////////
///////////////////////////////////////////////////////////////////////
int main( int argc , char **argv){

   ///////////////////////////////////////////////////////////////////////
   ///////////////////read command line arguments ////////////////////////
   ///////////////////////////////////////////////////////////////////////
   bool ManualScreenSize = false;
   bool ArrayWorld = false;
   bool ColorOn = true;

   //----------------------------------------------------------------------
   // Requirement #15: demonstrate command line arguments
   //----------------------------------------------------------------------
   // checks for -s and -a and --help
   // affects the setting of manual screen size and usage of array world
   // --help displays readme.txt and then exit programs

   //check all arguments except program name
   for(int i=1;i<argc;i++){

      //check if -a is passed in
      if(string(argv[i]) == ARRAY_WORLD_ARG){
         ArrayWorld = true;
      }

      //check if -s is passed in
      if(string(argv[i]) == MANUAL_SCREEN_SIZE){
         ManualScreenSize = true;
      }

      //check if -c is passed in
      if(string(argv[i]) == NO_COLOR_ARG){
         ColorOn = false;
      }

      //check if --help is passed in
      if(string(argv[i]) == HELP_ARG){
         help();
      }
   }

   swansonB::SeedRandom();

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
      getScreenSize(row,col); // input screen size, as per command arg
   }else{

      //----------------------------------------------------------------------
      // Requirement #25: Demonstrate exception
      //----------------------------------------------------------------------
      class dimensions_missing{};
      try{
         getmaxyx(stdscr,row,col);     //ncurses checks terminal size

         if( row == 0 || col == 0 ){   //unsuccessful read of terminal size
            dimensions_missing exep;
            throw (exep);
         }
      }
      catch(dimensions_missing&){
         getScreenSize(row,col);       //respond to exception, manual input
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

   GOL::LivingCellStartSet genesis;

   Walker myCreator(WORLD_WIDTH,WORLD_HEIGHT);

   int max = WORLD_HEIGHT*WORLD_WIDTH;

   //randomly create a starting set, using a "walking" random algorithm
   //pass in total available cells, desired saturation level, and center cords
   myCreator.getSet(genesis,RandomWalker::GetWalkString(max,SATURATION),
           WORLD_WIDTH/2,WORLD_HEIGHT/2);


   //----------------------------------------------------------------------
   // Requirement #24: demonstrate polymorphism and inheritance
   //----------------------------------------------------------------------
   //
   //The map and Set based world or Array based world can both be used
   // Interchangeably,  they both implement the display and reaping interfaces
   // the God controller class and the Display function can interact with them
   // both through pointers to the interface classes.
   God myGod( new MpSWorldBuilder( WORLD_WIDTH , WORLD_HEIGHT , genesis ) );

   if(ArrayWorld){  //use array world as per command line argument
      myGod =
            God( new ArrayWorldBuilder( WORLD_WIDTH , WORLD_HEIGHT , genesis ));
   }


   //-----------------------------------------------------------------------
   // Requirement #16. Demonstrates definition and use of class
   //-----------------------------------------------------------------------
   // using custom classes God, and WorldDisplayInterface
   WorldDisplayInterface* VoiceOfGod = myGod.GetWorldDisplayInt();


   ////////////////////////////////////////////////////////////////////////
   ////////////////initializing window settings///////////////////////////
   ////////////////////////////////////////////////////////////////////////

   //setup input, characters don't echo, and are available before return key
   // pressed,  halfdelay allows to wait x tenthes of second for input before
   // carrying on with program,  returning ERR character
   cbreak();
   halfdelay(delayTenthsOfASecond);
   noecho();

   //begin color mode and define color scheme
   if(ColorOn) start_color();
   init_pair(2,COLOR_GREEN,COLOR_GREEN);



///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
/////////////////////PRIMARY PROGRAM LOOP//////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////

   //----------------------------------------------------------------------
   // Requirement #04: demonstrate a loop
   //----------------------------------------------------------------------
   do {

      //----------------------------------------------------------------------
      // Requirement #09: demonstrate Functional Decomposition
      //----------------------------------------------------------------------
      // this is the entire repeated operations of the game of life
      // broken down into output, calculations, and input

      //put world in window
      outputWorldINT(VoiceOfGod, myGod.GenerationsPassed(), WORLD_HEIGHT);
      refresh();

      //advance game of life world
      myGod++;

   } while (input());  ///change speed of animation or quit

   endwin(); //exit curses window

   return 0; //end program

}

void getScreenSize(int &row, int &col){

   endwin();  //disable curses mode

   //----------------------------------------------------------------------
   // Requirement #01: demonstrate simple IO
   //----------------------------------------------------------------------
   // here we have a cout statement
   // follow the call to GetInt to see the use of getline()

   cout << "Please manually enter your screen resolution" << endl;

   //get ints between 0 and 150
   row =swansonB::GetInt("Rows: ",0,150);
   col =swansonB::GetInt("Columns: ",0,150);

   initscr();  //return to ncuses mode

}

/////////////////////////////////////////////////////
///USES WORLD DISPLAY INTERFACE//////////////////////
//////CREATES A STRING TO REPRESENT LIVING CELLS/////
/////////////////////////////////////////////////////

void outputWorldINT ( WorldDisplayInterface* display , int numberOfGenerations,
      int height){

   clear();                         //empty window contents
   display->LivingCellBegin();      //set display iterator to beginning;

   while ( !display->LivingCellsEnd()) {

      //use display interface to retrieve next coordinate
      GOL::cordinate nextCord = display->NextLivingCellLoc();

      //add single character to window at y,x
      mvaddch(nextCord.y,nextCord.x,'#' | COLOR_PAIR(2));
   }
   /////////////////////////////////////////////////////////////////
   ///////build a string to add to bottom of display////////////////
   /////generations counts/////////////////////controls/////////////
   /////////////////////////////////////////////////////////////////
   string bottomLine = "Generations:";
   bottomLine+= swansonB::GetString(numberOfGenerations);
   bottomLine+= "  Speed up:";
   bottomLine+= swansonB::GetString(FASTER);
   bottomLine+= " down:";
   bottomLine+= swansonB::GetString(SLOWER);
   bottomLine+= " Quit:";
   bottomLine+= swansonB::GetString(QUIT);

   mvaddstr(height,0,bottomLine.c_str());    //add string to window

   //----------------------------------------------------------------------
   // Requirement #11: Demonstrate Passing methods - Pass by value
   //----------------------------------------------------------------------
   // the following modification to the variable numberOfGenerations will have
   // no lasting effect on the program because this variable was passed by value
   // and therefore is local to this function, it is about to pass out of scope
   // if there were a copy of the variable passed in to this function
   // it would be unaffected by this modification
   //
   // in this case it was passed in "anonymously" as the return of a function
   // call
   numberOfGenerations++;
}


//////////////////input

bool input(){
   char input = getch();  // use curses input to get character

   if(input != ERR){      //if input was received, halfdelay not passed
      switch (input) {
         case FASTER:
            if(delayTenthsOfASecond > 1){
               halfdelay(--delayTenthsOfASecond);  //speed up animation
            }
            break;
         case SLOWER:
            if(delayTenthsOfASecond < MAX_TENTHS_OF_SECOND){
               halfdelay(++delayTenthsOfASecond);  //slow down animation
            }
            break;
         case QUIT:
            return false;     //will end main loop

            break;
         default:
            break;
      }

      //----------------------------------------------------------------------
      // Requirement #07: Demonstrate debugging trick
      //----------------------------------------------------------------------
      // This allows me to check the functioning of this input function
      // I can observe the output live using 'tail filename -f'

      //----------------------------------------------------------------------
      // Requirement #23: Demonstrate File IO
      //----------------------------------------------------------------------

      ////logfile output
      ofstream log(logFileName, ios::app);   //open in append mode
      log << "input received: " << input
            << " , updating every " << delayTenthsOfASecond
            << " tenths of a second" << endl;
      log.close();                           //close file

   }

   return true;
}

void help(){

   //----------------------------------------------------------------------
   // Requirement #23: Demonstrate File IO
   //----------------------------------------------------------------------
   ifstream helpin(HELP_FILENAME.c_str());

   if(helpin.fail()){
      cout << "\ncommand line arguments are "
            << "\n -a  for alternate sublcass using arrays"
            << "\n -s  for the manual input of screen resolution"
            << "\n -c  to disable color mode"
            << "\n\nplease see the contents of readme.txt for more" << endl;
   } else{

      while(helpin){
         string helptext;
         getline(helpin,helptext);
         cout << helptext << "\n";
      }
      cout << endl;
   }
   exit(0);
}



