/*
 * WalkerMaker.hpp
 *
 *  Created on: Aug 14, 2014
 *      Author: brandon
 */

#ifndef WALKERMAKER_HPP_
#define WALKERMAKER_HPP_

#include "GameOfLife.hpp"
#include <string>
#include <set>
#include <utility>
#include <istream>
#include <sstream>


#include "../SwansonFunctions.hpp"


using namespace std; //todo find needed refrences to this

class Walker {
private:

   GOL::LivingCellStartSet mySet;
   set<char> inputKeys;

   const int WORLD_WIDTH, WORLD_HEIGHT;




   void up ( GOL::cordinate &pos ) {
      pos.y = (pos.y + WORLD_HEIGHT - 1) % WORLD_HEIGHT;
   }
   void down ( GOL::cordinate &pos ) {
      pos.y = (pos.y + 1) % WORLD_HEIGHT;
   }
   void left ( GOL::cordinate &pos ) {
      pos.x = (pos.x + WORLD_WIDTH - 1) % WORLD_WIDTH;
   }
   void right ( GOL::cordinate &pos ) {
      pos.x = (pos.x + 1) % WORLD_WIDTH;
   }

public:

   static const char North;
   static const char South;
   static const char West;
   static const char East;
   static const char NW;
   static const char SW;
   static const char NE;
   static const char SE;

   static const int upper = 'a' - 'A';
   static const char terminate;

   static const char inputChars[];
   static const string glider;
   static const int GLIDER_SIZE = 3;

   Walker ( int width , int height );

   void getSet ( GOL::LivingCellStartSet &cellSet, std::istream &myStream,
         int startx = 0 , int starty = 0);

   void getSet ( GOL::LivingCellStartSet &cellSet, string input,
         int startx = 0 , int starty = 0 ) {
      if(input.at(input.length()-1)!=terminate)input.append(1,terminate);
      istringstream mystream(input);
      getSet(cellSet,mystream, startx, starty);
   }

   void getGliderWorld(GOL::LivingCellStartSet &cellSet);
   static const string Tumbler;
};

///outside definitions
const char Walker::North = 'w';
const char Walker::South = 's';
const char Walker::West = 'a';
const char Walker::East = 'd';
const char Walker::NW = 'q';
const char Walker::SW = 'z';
const char Walker::NE = 'e';
const char Walker::SE = 'c';
const char Walker::terminate = '!';

const char Walker::inputChars[] = {North,South,West,East,SW,SE,NW,NE};


Walker::Walker ( int width , int height ) :
      WORLD_WIDTH( width ), WORLD_HEIGHT( height ) {
   //myKeys.insert(walkChar('w',true));
   inputKeys.insert( Walker::North );
   inputKeys.insert( South );
   inputKeys.insert( East );
   inputKeys.insert( West );
   inputKeys.insert( NW );
   inputKeys.insert( NE );
   inputKeys.insert( SE );
   inputKeys.insert( SW );

   for ( set<char>::iterator it = inputKeys.begin() ; it != inputKeys.end() ;
         it++ ) {
      inputKeys.insert( *it - upper );
   }

}

void Walker::getSet ( GOL::LivingCellStartSet &cellSet, std::istream &myStream,
      int startx, int starty) {

   char nextchar;

   GOL::cordinate position = GOL::GetCord( startx , starty );

   while ( nextchar != terminate ) {
      myStream >> nextchar;

      if ( inputKeys.count( nextchar ) ) {
         bool cursorOn = (nextchar < 'a');
         if ( cursorOn )
            nextchar += upper;

         switch (nextchar) {
         case Walker::North:
            up( position );
            break;
         case Walker::South:
            down( position );
            break;
         case Walker::East:
            right( position );
            break;
         case Walker::West:
            left( position );
            break;
         case NW:
            up( position );
            left( position );
            break;
         case NE:
            up( position );
            right( position );
            break;
         case SE:
            down( position );
            right( position );
            break;
         case SW:
            down( position );
            left( position );
            break;
         default:
            break;
         }

         if ( cursorOn ){
            cellSet.insert(position);
         }

      } //end of if
   }//end of while
}//end of function

class RandomWalker{
private:
   static const float DEFAULT_CLUSTERING;

   static string Walk(int &cellNumber, float clustering){

      //----------------------------------------------------------------------
      // Requirement #5: demonstrate a Random Number
      //----------------------------------------------------------------------

      char nextStep = Walker::inputChars[swansonB::GetRandomInRange(7)];
      if(swansonB::GetRandomInRange(100)>(clustering*100)){
         nextStep -= Walker::upper;
         cellNumber++;
      }
      return swansonB::GetString(nextStep);
   }

public:
   static string GetWalkString(int maxCells, float saturation,
         float clustring = RandomWalker::DEFAULT_CLUSTERING){
      string walkStr;
      int cellNumber = 0;
      int saturationNumber = maxCells * saturation;
      while(cellNumber<saturationNumber){
         walkStr += Walk(cellNumber,clustring);
      }

      walkStr += swansonB::GetString(Walker::terminate);
      return walkStr;


   }

   //----------------------------------------------------------------------
   // Requirement #12: demonstrate cstring and string
   //----------------------------------------------------------------------

   static string GetWalkString(int maxCells, int steps,
         float clustring = RandomWalker::DEFAULT_CLUSTERING){
      string walkStr;
      int cellNumber = 0;
      int takenSteps = 0;

      while(takenSteps<steps && cellNumber<maxCells){
         walkStr += Walk(cellNumber,clustring);
         takenSteps++;
      }
      walkStr += swansonB::GetString(Walker::terminate);
      return walkStr;

   }


};

const float RandomWalker::DEFAULT_CLUSTERING = .5;


#endif /* WALKERMAKER_HPP_ */
