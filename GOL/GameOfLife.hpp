/*
 * GameOfLife.hpp
 *
 *  Created on: Aug 11, 2014
 *      Author: brandon
 */

#ifndef GAMEOFLIFE_HPP_
#define GAMEOFLIFE_HPP_

#include <set>

namespace GOL{

   //-----------------------------------------------------------------------
   // Requirement #16. Demonstrates definition and use of struct
   //------------------------------------------------------------------------
   struct cordinate{
      int x,y;


      bool operator() ( const cordinate &lhs ,
            const cordinate &rhs ) const {
         //lhs<rhs return true > return false
         //coords are sorted with y highest priority to facilitate console output

         if ( lhs.y < rhs.y ){
            //cerr << "y " << lhs.y << " less than " << rhs.y << endl;
            return true;
         }
         else if((lhs.y == rhs.y) && (lhs.x < rhs.x)){
            //cerr << "x " << lhs.x << " less than " << rhs.x << endl;
            return true;
         }
         else
            return false;

      }

   };

   struct cell{
      bool alive;
      cordinate location;
      int numNeighbors;
   };

   cordinate GetCord(int x, int y){
      cordinate result;
      result.x = x;
      result.y = y;
      return result;
   }

   typedef std::set<GOL::cordinate,GOL::cordinate> LivingCellStartSet;



};



#endif /* GAMEOFLIFE_HPP_ */
