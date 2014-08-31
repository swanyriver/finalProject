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



//-----------------------------------------------------------------------
// Requirement #16. Demonstrates definition and use of class
//------------------------------------------------------------------------
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


   //----------------------------------------------------------------------
   // Requirement #22: demonstrate overloaded operator
   //----------------------------------------------------------------------
   //
   // provides a syntax sugar way to call the Generation() method
   //
   God operator ++(){

      this->Generation();

      return *this;
   }

   God operator ++(int i){

      this->Generation();

      return *this;
   }
};


#endif /* GOD_HPP_ */
