/*
 * arrayWorld.hpp
 *
 *  Created on: Aug 28, 2014
 *      Author: brandon
 */

#ifndef ARRAYWORLD_HPP_
#define ARRAYWORLD_HPP_

#include <map>
#include <utility>
#include <cassert>
#include <list>

#include "World.hpp"
#include "WorldTools.hpp"
#include "GameOfLife.hpp"
#include "MapSetWorld.hpp"


class ArrayWorldDisplay;
class ArrayWorldReap;


class ArrayWorld: public WORLD{
   friend class ArrayWorldDisplay;
   friend class ArrayWorldReap;
protected:

   typedef std::pair<GOL::cordinate,int> NbCountPair;
   typedef std::map<GOL::cordinate,int,GOL::cordinate> neighborMap;
   typedef bool** liveCells;
   //member variables


   //----------------------------------------------------------------------
   // Requirement #17: demonstrate pointers  -- to array
   //----------------------------------------------------------------------
   //
   // in this version of the world class the live cells are stored in two
   // dimensional arrays of bools that are defined as bool** a pointers to
   // pointers
   liveCells pThisGen;
   liveCells pNextGen;

   neighborMap mNeigborNums;

private:

   const bool ALIVE;
   const bool DEAD;

   //----------------------------------------------------------------------
   // Requirement #13: demonstrate recursion
   //----------------------------------------------------------------------
   void ClearLiveCells(liveCells cells, int row = 0, int col = 0){


      //----------------------------------------------------------------------
      // Requirement #06: demonstrate error categories   -RUNTIME
      //----------------------------------------------------------------------
      //----------------------------------------------------------------------
      // Requirement #07: demonstrate debugging trick
      //----------------------------------------------------------------------
      //
      // At this line I was receiving a segfault error on runtime
      // I was able to locate this using GDB and the where command
      // It was due to the wrong variable being compared for termination
      //       if(++col<this->WORLD_HEIGHT){
      //
      cells[row][col] = DEAD;

      if(++col<this->WORLD_WIDTH){
         ClearLiveCells(cells,row,col);
      } else if(++row < this->WORLD_HEIGHT){
         ClearLiveCells(cells,row,0);
      }

      //----------------------------------------------------------------------
      // Requirement #06: demonstrate error categories   -SYNTAX
      //----------------------------------------------------------------------
      //
      // these kind of errors occur less often, mostly because i use an IDE
      // and obvious errors like a missing ; or an unclosed ( { are caught
      // before compile and underlined red
      // examples would be for the above line
      //      ClearLiveCells(cells,row,0)    missing semicolon
      //      Clearlivecells(cells,row,0);   misspelled function call
      //      ClearLiveCells(cells,0);       no matching funciton signature
      //
      //  these kind of errors would prevent a succesful compile
      //

   }

public:

   ArrayWorld
   ( int width , int height , GOL::LivingCellStartSet start ) :
         WORLD(width,height,start), ALIVE(true), DEAD(false){

      //----------------------------------------------------------------------
      // Requirement #14: demonstrate Dynamic multi-dimensional arrays
      //----------------------------------------------------------------------

      pThisGen = new bool*[height];
      pNextGen = new bool*[height];
      for(int i = 0; i < height; i++){
         pThisGen[i] = new bool[width];
         pNextGen[i] = new bool[width];
      }

      ClearLiveCells(pThisGen);
      ClearLiveCells(pNextGen);


      for( GOL::LivingCellStartSet::const_iterator it = start.begin();
            it != start.end(); it++){
         pThisGen[it->y][it->x] = ALIVE;
      }

   }


private:
   void YourNeighbors ( const GOL::cordinate &loc , GOL::cordinate mooreNB[]){
      MapSetWorld::YourNeighbors
         (loc,mooreNB,this->WORLD_WIDTH,this->WORLD_HEIGHT);
   }

public:
   ////////////////////////////
   //called by GOD ////////////
   ////////////////////////////

   //mutator method, increments neighbor counts
   void CountNeighbors (){
      mNeigborNums.clear();

      GOL::cordinate mooreNB[8];

      for (int row = 0; row < this->WORLD_HEIGHT; row++){
         for (int col = 0; col < this->WORLD_WIDTH; col++){
            if(pThisGen[row][col] == ALIVE){
               YourNeighbors(GOL::GetCord(col,row),mooreNB);

               for(int i=0; i<8; i++){
                  pair<neighborMap::iterator,bool> element =
                        mNeigborNums.insert(NbCountPair(mooreNB[i], 0));
                  //new element made to 1, or element incremented
                  element.first->second++;

               }

            }
         }
      }


   }

   ///switch generaton data set pointers
   void generation (){
      liveCells pTemp = pThisGen;
      pThisGen = pNextGen;
      pNextGen = pTemp;

      ClearLiveCells(pNextGen);

   }


   ////////////////////////////
   //called by ANGLE //////////
   ////////////////////////////

   //called by Angel after Calculations
   void Live ( const GOL::cordinate &loc ){
      pNextGen[loc.y][loc.x] = ALIVE;
   }
   void Birth ( const GOL::cordinate &loc ){
      //duplicate behavior of life (for now)
      Live(loc);
   }

   void Die ( const GOL::cordinate &loc ){
      //intentionally left blank in this implementation
   };

   //factory methods
   WorldDisplayInterface* GetDisplayInterface();
   WorldReapingInterface* GetReapingInterface();
};



////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////
//////////interfaces and builder//////////////////////////
////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////


#include <fstream>
#include <iostream>
#include <string>
using namespace std;



class ArrayWorldDisplay: public WorldDisplayInterface{
private:
   ArrayWorld *aWorld;

   list<GOL::cordinate> mAlive;
   list<GOL::cordinate>::const_iterator mLookup;


public:
   ArrayWorldDisplay(ArrayWorld *world):aWorld(world){};

   void LivingCellBegin(){
      mAlive.clear();

      //add all living cells to list
      for (int row = 0; row < aWorld->WORLD_HEIGHT; row++){
         for (int col = 0; col < aWorld->WORLD_WIDTH; col++){
            if(aWorld->pThisGen[row][col] == aWorld->ALIVE){
               mAlive.push_back(GOL::GetCord(col,row));
            }
         }
      }


      mLookup = mAlive.begin();

   }

   bool LivingCellsEnd (){
      return (mLookup==mAlive.end());
   }

   GOL::cordinate NextLivingCellLoc(){
      return *mLookup++;
   }
   long int NumLiving(){
      return mAlive.size();
   }
};

class ArrayWorldReap: public WorldReapingInterface{
private:
   ArrayWorld *aWorld;
   ArrayWorld::neighborMap::iterator mNbLookUp;

public:
   ArrayWorldReap(ArrayWorld *world ):
      WorldReapingInterface(world),aWorld(world){};

   virtual void NeighborCellBegin(){
      mNbLookUp = aWorld->mNeigborNums.begin();
   };
   virtual bool NeighborCellsEnd (){
      return(mNbLookUp == aWorld->mNeigborNums.end());
   }
   virtual GOL::cell NextNeighbor (){
      GOL::cell result;
      result.location=mNbLookUp->first;
      result.numNeighbors = mNbLookUp->second;
      result.alive =
            (aWorld->pThisGen[result.location.y][result.location.x] ==
                  aWorld->ALIVE);
      mNbLookUp++;

      assert(result.numNeighbors<9);

      return result;
   }

   void Live ( const GOL::cordinate &loc ){
      mWorld->Live(loc);
   }
   void Birth ( const GOL::cordinate &loc ){
      mWorld->Birth(loc);
   }
   void Die ( const GOL::cordinate &loc ){
      mWorld->Die(loc);
   }
};

class ArrayWorldBuilder: public WorldBuilder{
public:
   ArrayWorldBuilder( int width , int height, GOL::LivingCellStartSet &inStart ):
      WorldBuilder(width,height,inStart){};

   WORLD* buildWord (){
      return new ArrayWorld(this->WORLD_WIDTH,this->WORLD_HEIGHT,this->start);
   }

};

WorldDisplayInterface* ArrayWorld::GetDisplayInterface(){
   return new ArrayWorldDisplay(this);
}
WorldReapingInterface* ArrayWorld::GetReapingInterface(){
   return new ArrayWorldReap(this);
}

#endif /* ARRAYWORLD_HPP_ */
