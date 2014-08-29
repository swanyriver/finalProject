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

#include "World.hpp"
#include "WorldTools.hpp"
#include "GameOfLife.hpp"

class ArrayWorldDisplay;
class ArrayWorldReap;


class ArrayWorld: public WORLD{
   friend class ArrayWorldDisplay;
   friend class ArrayWorldReap;
protected:

   typedef std::map<GOL::cordinate,int,GOL::cordinate> neighborMap;
   typedef bool** liveCells;
   //member variables
   liveCells mLivingCellsA;
   liveCells mLivingCellsB;
   liveCells *pThisGen;
   liveCells *pNextGen;

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
   MpSWorldReap(WORLD *world, MapSetWorld *mpsworld ):
      WorldReapingInterface(world),mpsWorld(mpsworld){};

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

//todo make mapset worldbuilder
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
   return new MpSWorldReap(this,this);
}

#endif /* ARRAYWORLD_HPP_ */
