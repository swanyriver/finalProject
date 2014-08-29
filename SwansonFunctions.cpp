/*
 * SwansonFunctions.cpp
 *
 *  Created on: Aug 28, 2014
 *      Author: brandon
 */

#include "SwansonFunctions.hpp"

#include <errno.h>
#include <string>
#include <iostream>
#include <cstdlib>
#include <climits>
#include <cctype>

using namespace std;

/******************************************************************************
 *    purpose: receive a string including spaces from the user
 *
 *    entry: a string to be used to clarify desired input to user
 *
 *    exit: a string of user input
 ******************************************************************************/
string swansonB::GetString ( string prompt ) {

   string str_rtrn = "";

   //request input
   cout << endl << prompt;

   ///Clearing errors and NewLine char
   cin.clear();
   if ( cin.peek() == '\n' ) {
      cin.ignore( 1000 , '\n' );
   }

   //----------------------------------------------------------------------
   // Requirement #1: demonstrate simple IO
   //----------------------------------------------------------------------
   // base method for get int gets a string from the cin stream
   getline( cin , str_rtrn );

   return str_rtrn;

}

/******************************************************************************
 *    purpose: receive a value from the user, screen for alpha input
 *             and overflow
 *
 *    entry: a string to be used to clarify desired input to user
 *
 *    exit: a value between TYPE_MIN and TYPE_MAX
 ******************************************************************************/

//----------------------------------------------------------------------
// Requirement #9: demonstrate function and overloading
//----------------------------------------------------------------------
int swansonB::GetInt ( string prompt ) {
   return swansonB::GetInt( prompt , INT_MIN , INT_MAX );
}

/******************************************************************************
 *    purpose: receive a value from the user, screen for alpha input
 *             and overflow, restrict input to specific range
 *
 *    entry: a string to be used to clarify desired input to user
 *
 *    exit: a value between rangeMin and rangeMax
 ******************************************************************************/
int swansonB::GetInt ( string prompt , int rangeMin , int rangeMax ) {

   //----------------------------------------------------------------------
   // Requirement #2: demonstrate explicit typecasting
   //----------------------------------------------------------------------
   return static_cast<int>( swansonB::GetLong( prompt , rangeMin , rangeMax ) );
}

long int swansonB::GetLong ( string prompt ) {
   return swansonB::GetLong( prompt , LONG_MIN , LONG_MAX );

}
long int swansonB::GetLong ( string prompt , long int rangeMin ,
      long int rangeMax ) {
   long int int_rtrn;
   string parse_string;
   bool firstTimeThrough = true;

   do {
      if ( !firstTimeThrough ) {
         cout << "Please keep the input within these bounds [" << rangeMin
               << " - " << rangeMax << "]";
      }
      firstTimeThrough = false;
      parse_string = swansonB::GetString( prompt );

      while ( !swansonB::AllNumbers( parse_string ) || parse_string.empty() ) {

         cout << "lets try to restrain ourselves to only whole numbers";
         parse_string = swansonB::GetString( prompt );
      }

      int_rtrn = strtol( parse_string.c_str() , NULL , 0 );

      //----------------------------------------------------------------------
      // Requirement #03: demonstrate logical operators ! && || ==
      //----------------------------------------------------------------------
   } while ( !(int_rtrn >= rangeMin && int_rtrn <= rangeMax)
         || (errno == ERANGE && (int_rtrn == LONG_MAX || int_rtrn == LONG_MIN)) );

   return int_rtrn;
}

/******************************************************************************
 *    purpose: determines if a string represents a whole number, using IsANumber
 *
 *    entry: none
 *
 *    exit: true if all chars in string are [0-9]
 ******************************************************************************/
bool swansonB::AllNumbers ( string numberString ) {
   int i = 0;
   //allow for negative numbers
   if ( numberString.length() > 1 && numberString.at( 0 ) == '-' ) {
      i++;
   }
   for ( ; i < numberString.length() ; i++ ) {
      if ( !isdigit( numberString.at( i ) ) )
         return false;
   }
   return true;
}



/******************************************************************************
 *    purpose: seeds the random number generator creating non repeatable output
 *
 *    entry: none
 *
 *    exit: rand() is seeded with time()
 ******************************************************************************/
void swansonB::SeedRandom () {
   srand( time( NULL ) );
}

/******************************************************************************
 *    purpose:produce a random number
 *
 *    entry: none
 *
 *    exit: random int in range [0,max]
 ******************************************************************************/
int swansonB::GetRandomInRange ( int max ) {
   return swansonB::GetRandomInRange( 0 , max );
}

/******************************************************************************
 *    purpose:produce a random number
 *
 *    entry: none
 *
 *    exit: random int in range [min,max]
 ******************************************************************************/
int swansonB::GetRandomInRange ( int min , int max ) {

   int random;
   int range = max - min + 1;
   if ( range == 1 )
      return min;

   //----------------------------------------------------------------------
   // Requirement #5: demonstrate a Random Number
   //----------------------------------------------------------------------
   //
   // see RandomWalker in WalkerMaker.hpp class
   // for call to random number function
   //
   random = (rand() % range) + min;
   return random;
}

