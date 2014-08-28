/*
 * inputTest.cpp
 *
 *  Created on: Aug 27, 2014
 *      Author: brandon
 */


#include <ncurses.h>
#include <ctime>

struct Input{
   char FASTER;
   char SLOWER;
   char PAUSE;
   char QUIT;
};

const Input CONTROLS = {
      'w', //Faster
      's', //Slower
      ' ', //pause
      'q'  //quit
};

void getInput();

int main(){

   const float SECOND_PER_FRAME = .25;
   const clock_t TICKS_PER_FRAME = CLOCKS_PER_SEC*SECOND_PER_FRAME;



   int tenthsDelay = 4;

   char out = '#';

   initscr();

   cbreak();
   halfdelay(tenthsDelay);
   noecho();

   int i = 0;

   while(true){
      clear();
      mvaddch(i%10,i++%10,out);
      refresh();
      //delay
      /*clock_t timeout=clock() + TICKS_PER_FRAME;
      while(clock() < timeout){
         //nothing, just wait
      }*/

      char input = getch();

      if (input != ERR){
         out = input;

         if (input == UP){
            halfdelay(--tenthsDelay);
         }

         if( input == DOWN ){
            halfdelay(++tenthsDelay);
         }
      }



   }
}

void getInput(){

}
