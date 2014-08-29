/*
 * MapSetWorld.hpp
 *
 *  Created on: Aug 11, 2014
 *      Author: brandon
 */

#ifndef MAPSETWORLD_HPP_
#define MAPSETWORLD_HPP_

#include "World.hpp"
#include "WorldTools.hpp"
#include "GameOfLife.hpp"

#include <map>
#include <set>
#include <utility>
#include <iterator>
#include <cassert>

#include <cstdio>

using namespace std;

class MpSWorldDisplay;
class MpSWorldReap;

class MapSetWorld: public WORLD{
   friend class MpSWorldDisplay;
   friend class MpSWorldReap;
protected:

   //typedefs
   typedef std::set<GOL::cordinate,GOL::cordinate> cordSet;
   typedef std::pair<GOL::cordinate,int> NbCountPair;
   typedef std::map<GOL::cordinate,int,GOL::cordinate> neighborMap;


   //member variables
   cordSet mLivingCellsA;
   cordSet mLivingCellsB;
   cordSet *pThisGen;
   cordSet *pNextGen;

   neighborMap mNeigborNums;

public:

   MapSetWorld
   ( int width , int height , GOL::LivingCellStartSet start ) :
         WORLD(width,height,start){
      //assign start to be current gen
      pThisGen = &mLivingCellsA;
      pNextGen = &mLivingCellsB;
      *pThisGen = start;

   }



   //used internally, but does not depend on internal state, so made static
   //for ease of testing and re-usability.

   //----------------------------------------------------------------------
   // Requirement #11: Demonstrate various passing methods
   //----------------------------------------------------------------------
   //
   // this function utilizes passing by array which is similar to passing by
   // pointer but is distinguished in the textbook as being a special separate
   // method for passing arrays,  this function signature could also be written
   // GOL::cordinate* mooreNB and when an array was passed at the parameter
   // the pointer would point to first item in the array and the [] would
   // dereference with an offset
   //
   static void YourNeighbors ( const GOL::cordinate &loc ,
         GOL::cordinate mooreNB[],
         const int &width, const int &height ){
      //return [8] cords
      //order changed to match print/set-sort order
      //y increases downward

      //row 1
      mooreNB[0].x = (loc.x+width-1)%width; mooreNB[0].y = (loc.y+height-1)%height;
      mooreNB[1].x = loc.x; mooreNB[1].y = (loc.y+height-1)%height;
      mooreNB[2].x = (loc.x+1)%width; mooreNB[2].y = (loc.y+height-1)%height;

      //row 2
      mooreNB[3].x = (loc.x+width-1)%width; mooreNB[3].y = loc.y;
      mooreNB[4].x = (loc.x+1)%width; mooreNB[4].y = loc.y;

      //row 3
      mooreNB[5].x = (loc.x+width-1)%width; mooreNB[5].y = (loc.y+1)%height;
      mooreNB[6].x = loc.x; mooreNB[6].y = (loc.y+1)%height;
      mooreNB[7].x = (loc.x+1)%width; mooreNB[7].y = (loc.y+1)%height;


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

      for(cordSet::iterator st = pThisGen->begin(); st!=pThisGen->end();st++){
         YourNeighbors(*st,mooreNB);

         for(int i=0; i<8; i++){
            //increment all of the moore neighborhood
            //insertion is only successful if key not already in map
            //if key(cord) exist in map, returned iterator points to value
            //this indicates a cell with at least one other neighbor
            pair<neighborMap::iterator,bool> element =
                  mNeigborNums.insert(NbCountPair(mooreNB[i], 0));
            //new element made to 1, or element incremented
            element.first->second++;

         }
      }
   }

   ///switch generaton data set pointers
   void generation (){

      //----------------------------------------------------------------------
      // Requirement #17: demonstrate pointers  -- to object
      //----------------------------------------------------------------------
      //
      // the pointers pThisGen, and pNextGen point to two persistent map
      // map objects and the operations performed on the maps alternate
      // each generations
      cordSet *pTemp = pThisGen;
      pThisGen = pNextGen;
      pNextGen = pTemp;

      pNextGen->clear();
   }


   ////////////////////////////
   //called by ANGLE //////////
   ////////////////////////////

   //called by Angel after Calculations
   void Live ( const GOL::cordinate &loc ){
      pNextGen->insert(loc);
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

class MpSWorldDisplay: public WorldDisplayInterface{
private:
   MapSetWorld *mpsWorld;
   MapSetWorld::cordSet::iterator mCellLookUp;
public:
   MpSWorldDisplay(MapSetWorld *world):mpsWorld(world){};
   bool LivingCellsEnd (){
      return(mCellLookUp == mpsWorld->pThisGen->end());
   }
   void LivingCellBegin(){
      mCellLookUp = mpsWorld->pThisGen->begin();
   }
   GOL::cordinate NextLivingCellLoc(){
      //----------------------------------------------------------------------
      // Requirement #17: demonstrate pointers  -- to struct
      //----------------------------------------------------------------------
      //
      // mCellLookUp is an iterator that points to a set of structs of type
      // GOL::cordinate, the dereference operator * allows it for the return
      // of the struct being pointed to by the iterator
      //
      return *mCellLookUp++;
   }
   long int NumLiving(){
      return mpsWorld->pThisGen->size();
   }
};

class MpSWorldReap: public WorldReapingInterface{
private:
   MapSetWorld *mpsWorld;
   MapSetWorld::neighborMap::iterator mNbLookUp;
public:
   MpSWorldReap( MapSetWorld *mpsworld ):
      WorldReapingInterface(mpsworld),mpsWorld(mpsworld){};

   virtual void NeighborCellBegin(){
      mNbLookUp = mpsWorld->mNeigborNums.begin();
   };
   virtual bool NeighborCellsEnd (){
      return(mNbLookUp == mpsWorld->mNeigborNums.end());
   }
   virtual GOL::cell NextNeighbor (){
      GOL::cell result;
      result.location=mNbLookUp->first;
      result.numNeighbors = mNbLookUp->second;
      result.alive = mpsWorld->pThisGen->count(mNbLookUp->first);
      mNbLookUp++;

      //----------------------------------------------------------------------
      // Requirement #07: Demonstrate debugging trick
      //----------------------------------------------------------------------
      //using assert to catch logic errors
      assert(result.numNeighbors<9);

      //----------------------------------------------------------------------
      // Requirement #06: Demonstrate 3 kinds of errors  --Logic
      //----------------------------------------------------------------------
      //This assert was caught because I had along the chain of function calls
      //Passed width twice instead of width and height, and therefore the
      //the calculations were off
      // The program was able to compile, and even run without hitting any
      // runtime errors,  but the behavior of the cells did not match test
      // cases,

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

class MpSWorldBuilder: public WorldBuilder{
public:
   MpSWorldBuilder( int width , int height, GOL::LivingCellStartSet &inStart ):
      WorldBuilder(width,height,inStart){};

   WORLD* buildWord (){
      return new MapSetWorld(this->WORLD_WIDTH,this->WORLD_HEIGHT,this->start);
   }

};

WorldDisplayInterface* MapSetWorld::GetDisplayInterface(){
   return new MpSWorldDisplay(this);
}
WorldReapingInterface* MapSetWorld::GetReapingInterface(){
   return new MpSWorldReap(this);
}



#endif /* MAPSETWORLD_HPP_ */
