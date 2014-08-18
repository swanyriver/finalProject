/*
 * WorldBuilder.hpp
 *
 *  Created on: Aug 11, 2014
 *      Author: brandon
 */

#ifndef WORLDBUILDER_HPP_
#define WORLDBUILDER_HPP_

#include "GameOfLife.hpp"
#include "World.hpp"

/*
 * ALL SUBCLASSES OF WORLD MUST HAVE AN IMPLEMENTATION OF
 *    -WorldDisplayInterface
 *    -WorldReapingInterface
 *    -WorldBuilder
 */

class WorldDisplayInterface{
   //private mSubclassWorld,  pass in on construction
   //each instance need to itterate seperatly
public:

   //used to iterate over living cells
   virtual bool LivingCellsEnd ()=0;
   virtual void LivingCellBegin() = 0;
   virtual GOL::cordinate NextLivingCellLoc()= 0;
   virtual long int NumLiving() = 0;



};

class WorldReapingInterface{
protected:
   WORLD *mWorld;
   //private mSubclassWorld,  pass in on construction
public:

   WorldReapingInterface(WORLD *world):mWorld(world){};

   virtual void NeighborCellBegin() = 0;
   virtual bool NeighborCellsEnd () = 0;
   virtual GOL::cell NextNeighbor () = 0;

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

//only delivers constructed world to GOD, needs same arguments
//as abstract world class, allows for additional conditions for specific
//implementations and for the sole posesions of a world instance by the
//god controller.
class WorldBuilder {
protected:
   const int WORLD_WIDTH;
   const int WORLD_HEIGHT;
   GOL::LivingCellStartSet start;

public:
   WorldBuilder ( int width , int height, GOL::LivingCellStartSet &inStart  ) :
         WORLD_WIDTH( width ), WORLD_HEIGHT( height ), start(inStart) {
      //todo implement two methods for out of bounds start set, WRAP or PRUNE,
      //pass in t/f default PRUNE

      //prune fix
      for(GOL::LivingCellStartSet::iterator it = start.begin();
            it!=start.end(); it++){
         if(it->x>=WORLD_WIDTH || it->y>=WORLD_HEIGHT){
            start.erase(it);
         }
      }
   }
   virtual WORLD* buildWord () = 0;
};


#endif /* WORLDBUILDER_HPP_ */
