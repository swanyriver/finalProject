/*
 * SwansonFunctions.hpp
 *
 *  Created on: Aug 28, 2014
 *      Author: brandon
 */

#ifndef SWANSONFUNCTIONS_HPP_
#define SWANSONFUNCTIONS_HPP_

//----------------------------------------------------------------------
// Requirement #19: demonstrate Header
//----------------------------------------------------------------------

#include <climits>
#include <string>
#include <sstream>


//----------------------------------------------------------------------
// Requirement #18: demonstrate namespace
//----------------------------------------------------------------------

namespace swansonB{

   std::string GetString ( std::string prompt );

   int GetInt ( std::string prompt );
   int GetInt ( std::string prompt , int rangeMin ,
         int rangeMax = INT_MAX );
   long int GetLong ( std::string prompt );
   long int GetLong ( std::string prompt , long int rangeMin ,
         long int rangeMax = LONG_MAX );

   //string functions
   bool AllNumbers ( std::string numberString );

   template <typename Type>
   std::string GetString ( Type element);


   //random functions
   void SeedRandom ();
   int GetRandomInRange ( int max );
   int GetRandomInRange ( int min , int max );
}


///must be defined in same file because it is template
template <typename Type>
std::string swansonB::GetString ( Type element){
  std::ostringstream numToString;
  numToString << element; //convert things to string
  return numToString.str();
}


#endif /* SWANSONFUNCTIONS_HPP_ */
