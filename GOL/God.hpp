/*
 * God.hpp
 *
 *  Created on: Aug 11, 2014
 *      Author: brandon
 */

#ifndef GOD_HPP_
#define GOD_HPP_

#include "WorldTools.hpp"
#include "World.hpp"
#include "Angel.hpp"

class God {
private:
   WORLD *myWORLD;
   ANGELofLIFE *myAngel;

   long int NumGenerations;

public:

   God ( WorldBuilder *creator ):NumGenerations(1){
      myWORLD = creator->buildWord();
      myAngel = new ANGELofLIFE(myWORLD->GetReapingInterface());

   }

   void Generation () {
      NumGenerations++;
      myWORLD->CountNeighbors();
      myAngel->ReapandSow();
      myWORLD->generation();
   }

   long int GenerationsPassed(){ return NumGenerations; };

   WorldDisplayInterface* GetWorldDisplayInt(){
      return myWORLD->GetDisplayInterface();
   }
};


#endif /* GOD_HPP_ */
