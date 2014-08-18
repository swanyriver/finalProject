/***********************************************************
 * Author: Brandon Swanson
 * Date Created: 06-29-2014
 * Last Modification Date: 07-03-2014
 * Filename: swansonUtils.cpp
 *
 * Overview: A collections of array and random number utility functions
 ***********************************************************/

#ifndef SWANSONUTIL_HPP_
#define SWANSONUTIL_HPP_

#include <cstdlib>
#include <ctime>
#include <cmath>
#include <list>
#include <set>
#include <string>
using std::list;
using std::set;
using std::string;

class swansonUtil {
public:

   static void SeedRandom ();
   static int GetRandomInRange ( int max );
   static int GetRandomInRange ( int min , int max );
   static void GetMappedRandomInts ( int valuesOut[] , int rangeBegining ,
         int rangeEnd , const int numGenerateValues );
   static bool IsEqual ( float value1 , float value2 );
   static bool IsEqual ( double value1 , double value2 );
   static void ClearScreen ();
   static void HackClearScreen ();

   //contains template method must be declared in class
   template<typename Type>
   static bool Contains ( Type var , Type values[] , int range ) {
      for ( int i = 0 ; i < range ; i++ ) {
         if ( values[i] == var )
            return true;
      }
      return false;
   }
   static bool Contains ( float var , float values[] , int range );
   static bool Contains ( double var , double values[] , int range );

   //insert template method must be declared in class
   template<typename TypeIns>
   static void InsertElement ( TypeIns val , TypeIns sortArray[] ,
         int numHolding ) {
      int i = 0;
      for ( ; i < numHolding ; i++ ) {
         if ( val < sortArray[i] ) {
            for ( int j = numHolding ; j > i ; j-- ) {
               sortArray[j] = sortArray[j - 1];
            }
            break;
         }
      }
      sortArray[i] = val;
   }

   template<typename TypeElement>
   static bool ExistsInSet(TypeElement element, set<TypeElement> mySet){
      return (mySet.count(element) > 0);
   }

};

/******************************************************************************
 *    purpose: determins if the array values of size range contains the item var
 *             for types: bool, int, long, double, float, char, string
 *
 *    entry: an array of size range
 *
 *    exit: true if the array values[] has at least one instance of var
 ******************************************************************************/

bool swansonUtil::Contains ( float var , float values[] , int range ) {
   for ( int i = 0 ; i < range ; i++ ) {
      if ( swansonUtil::IsEqual( values[i] , var ) )
         return true;
   }
   return false;
}
bool swansonUtil::Contains ( double var , double values[] , int range ) {
   for ( int i = 0 ; i < range ; i++ ) {
      if ( swansonUtil::IsEqual( values[i] , var ) )
         return true;
   }
   return false;
}

/******************************************************************************
 *    purpose: seeds the random number generator creating non repeatable output
 *
 *    entry: none
 *
 *    exit: rand() is seeded with time()
 ******************************************************************************/
void swansonUtil::SeedRandom () {
   srand( time( NULL ) );
}

/******************************************************************************
 *    purpose:produce a random number
 *
 *    entry: none
 *
 *    exit: random int in range [0,max]
 ******************************************************************************/
int swansonUtil::GetRandomInRange ( int max ) {
   return GetRandomInRange( 0 , max );
}

/******************************************************************************
 *    purpose:produce a random number
 *
 *    entry: none
 *
 *    exit: random int in range [min,max]
 ******************************************************************************/
int swansonUtil::GetRandomInRange ( int min , int max ) {

   int random;
   int range = max - min + 1;
   if ( range == 1 )
      return min;
   random = (rand() % range) + min;
   return random;
}

/******************************************************************************
 *    purpose: produce an amount (numGeneratedValues) of numbers across a given
 *             range avoiding duplicate values
 *
 *    entry: empty array
 *
 *    exit: a sorted array of non repeated random values
 ******************************************************************************/
void swansonUtil::GetMappedRandomInts ( int valuesOut[] , int rangeBegining ,
      int rangeEnd , const int numGenerateValues ) {

   int nextSelection;
   list<int> selectionsList;

   //first value doesnt need special consideration
   valuesOut[0] = swansonUtil::GetRandomInRange( rangeBegining , rangeEnd );
   rangeEnd--;

   for ( int i = 1 ; i < numGenerateValues ; ++i ) {
      nextSelection = swansonUtil::GetRandomInRange( rangeBegining , rangeEnd );
      rangeEnd--;
      selectionsList.clear();
      for ( int j = 0 ; j < i ; j++ ) {
         selectionsList.push_back( valuesOut[j] );
      }
      do {
         int increment = 0;
         while ( !selectionsList.empty()
               && selectionsList.front() <= nextSelection ) {
            increment++;
            selectionsList.pop_front();
         }
         nextSelection += increment;
      } while ( !selectionsList.empty()
            && selectionsList.front() <= nextSelection );
      swansonUtil::InsertElement( nextSelection , valuesOut , i );
   }

}

/**************************************************************
 *
 *  Entry:2 float/double values
 *
 *  Exit: true if two values are within defined epsilon of each other, false
 *  otherwise
 *
 *  Purpose: to compare the "equality" of imperiously stored floating point
 *  numbers
 *
 * ***************************************************************/
bool swansonUtil::IsEqual ( float value1 , float value2 ) {
   return IsEqual( static_cast<double>( value1 ) ,
         static_cast<double>( value2 ) );
}
bool swansonUtil::IsEqual ( double value1 , double value2 ) {
   static const float EPSILON = .0005;
   if ( abs( value1 - value2 ) < EPSILON )
      return true;
   else
      return false;
}

/**************************************************************
 *
 * Entry: running on unix based system
 *
 * Exit: A cleared screen
 *
 * Purpose: Clear screen for a refresh
 *
 * ***************************************************************/
void swansonUtil::ClearScreen () {
   system( "clear && printf '\e[3J'" );
}

/**************************************************************
 *
 * * Entry:none
 *
 * * Exit: 100 return characters to output stream
 *
 * * Purpose: obscure secret word
 *
 * ***************************************************************/
void swansonUtil::HackClearScreen () {
   string clearScrn = "";
   clearScrn.append( 100 , '\n' );
   std::cout << clearScrn;
}

#endif
