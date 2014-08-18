/*
 * World.hpp
 *
 *  Created on: Aug 11, 2014
 *      Author: brandon
 */

#ifndef WORLD_HPP_
#define WORLD_HPP_

#include "GameOfLife.hpp"

class WorldDisplayInterface;
class WorldReapingInterface;

class WORLD {

protected:

   const int WORLD_WIDTH;
   const int WORLD_HEIGHT;

   GOL::LivingCellStartSet startLivingCells;

public:
   //////necesary conditions///////////
   /*
    * 2 Generations tracked,
    * living is mapped onto next generation
    * next generations starts off blank
    *    (required to avoid the unchecked isolated cells persisting)
    *
    */

   WORLD ( int width , int height , GOL::LivingCellStartSet start ) :
         WORLD_WIDTH( width ), WORLD_HEIGHT( height ),
         startLivingCells(start){};


   ////////////////////////////
   //called by GOD ////////////
   ////////////////////////////
   virtual void generation () = 0; ///switch generaton data set pointers
   virtual void CountNeighbors ()=0; //mutator method, increments neighbor counts

   //factory methods
   virtual WorldDisplayInterface* GetDisplayInterface() = 0;
   virtual WorldReapingInterface* GetReapingInterface() = 0;

   ////////////////////////////
   //called by Angel//////////
   ////////////////////////////


   //called by Angel after Calculations
   virtual void Live ( const GOL::cordinate &loc )=0;
   virtual void Birth ( const GOL::cordinate &loc )=0; //under current design dupes Live
   virtual void Die ( const GOL::cordinate &loc )=0;  //under current design does nothing

};



#endif /* WORLD_HPP_ */
