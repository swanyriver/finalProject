/***********************************************************
 * Author: Brandon Swanson
 * Date Created: 06-29-2014
 * Last Modification Date: 07-01-2014
 * Filename: swansonString.cpp
 *
 * Overview: A collection of static methods designed to process and
 *           modify strings
 *
 ***********************************************************/

#ifndef SWANSONSTRING_HPP_
#define SWANSONSTRING_HPP_

#include <string>
#include <list>
#include <cctype>
#include <sstream>

using namespace std;

class swansonString {
public:

   static bool IsALetter ( char character );
   static bool IsANumber ( char character );
   static bool AllNumbers ( string numberString );
   static bool AllNumbersFloat ( string numberString );
   static bool AllLetters ( string letterString );
   static bool AllLetters ( string letterString , char permitedChars[] ,
         int arraySize );

   static void SeperateWords ( string myString , list<string>& seperateWords );
   static string LowerCase ( string caseString );
   static void LowerCasePreserve ( string &caseString );
   static string UpperCase ( string caseString );

   static int NumOccurances(string main, string sub);

   template <typename Type>
   static string GetString ( Type element){
      ostringstream numToString;
      numToString << element; //convert things to string
      return numToString.str();
   }
};

int swansonString::NumOccurances(string main, string sub){
   int occurances = 0;
   int foundPos = 0;
   while(foundPos!=string::npos){
      foundPos = main.find(sub,foundPos);
      if(foundPos!=string::npos){
         foundPos++;
         occurances++;
      }
   }
   return occurances;
}


/******************************************************************************
 *    purpose: determines if a char is a letter
 *
 *    entry: none
 *
 *    exit: true if char is [a-z] or [A-Z]
 ******************************************************************************/
bool swansonString::IsALetter ( char character ) {
   return isalpha(character);
}

/******************************************************************************
 *    purpose: determines if a char is a number
 *
 *    entry: none
 *
 *    exit: true if char is [0-9]
 ******************************************************************************/
bool swansonString::IsANumber ( char character ) {
   return isdigit(character);
}

/******************************************************************************
 *    purpose: determines if a string represents a whole number, using IsANumber
 *
 *    entry: none
 *
 *    exit: true if all chars in string are [0-9]
 ******************************************************************************/
bool swansonString::AllNumbers ( string numberString ) {
   int i = 0;
   //allow for negative numbers
   if ( numberString.length() > 1 && numberString.at( 0 ) == '-' ) {
      i++;
   }
   for ( ; i < numberString.length() ; i++ ) {
      if ( !IsANumber( numberString.at( i ) ) )
         return false;
   }
   return true;
}

/******************************************************************************
 *    purpose: determines if a string represents a float point number
 *
 *    entry: non void string
 *
 *    exit: true if all chars are numeric and contains 0 or 1 '.'
 ******************************************************************************/
bool swansonString::AllNumbersFloat ( string numberString ) {
   string tempString;
   int numDots = 0; // num '.' in string
   int dotLocation; // position of '.' in string

   for ( int i = 0 ; i < numberString.length() ; i++ ) {
      if ( numberString.at( i ) == '.' ) {
         numDots++;
         dotLocation = i;
      }
   }
   if ( numDots > 1 )
      return false; //not valid float number
   else if ( numDots == 0 )
      return AllNumbers( numberString );
   else if ( numDots == 1 ) { //remove '.' and check if all else is numeric
      for ( int i = 0 ; i < numberString.length() ; i++ ) {
         if ( i != dotLocation )
            tempString += numberString.at( i );
      }
      return AllNumbers( tempString );
   }

   return false;
}

/******************************************************************************
 *    purpose: determines if a string is comprised of only letters
 *             using IsALetter
 *
 *    entry: non void string
 *
 *    exit: true if all chars are [a-z] or [A-Z]
 ******************************************************************************/
bool swansonString::AllLetters ( string letterString ) {
   for ( int i = 0 ; i < letterString.length() ; i++ ) {
      if ( !IsALetter( letterString.at( i ) ) )
         return false;
   }
   return true;
}

/******************************************************************************
 *    purpose: determines if a string is comprised of only letters and the
 *             characters in permitedChars
 *
 *    entry: non void string
 *
 *    exit: true if the string is alpha chars or the permitedChars
 ******************************************************************************/
bool swansonString::AllLetters ( string letterString , char permitedChars[] ,
      int arraySize ) {

   bool isAPermitedChar;

   for ( int i = 0 ; i < letterString.length() ; i++ ) {
      isAPermitedChar = false;

      for ( int j = 0 ; j < arraySize ; j++ ) {
         if ( letterString.at( i ) == permitedChars[j] ) {
            isAPermitedChar = true;
         }
      }

      if ( !(IsALetter( letterString.at( i ) ) || isAPermitedChar) ) {
         return false;
      }
   }
   return true;
}

/******************************************************************************
 *     purpose: create a list of individual words using anything other than
 *             alpha as a separation token
 *
 *    entry: non void string, empty list seperateWords
 *
 *    exit: seperateWords will contain strings of only alpha characters
 ******************************************************************************/
void swansonString::SeperateWords ( string myString ,
      list<string>& seperateWords ) {
   string currentWord;

   seperateWords.clear();

   int i = 0;
   while ( i < myString.length()
         && !swansonString::IsALetter( myString.at( i ) ) )
      i++; //find first letter;

   while ( i < myString.length() ) {
      if ( swansonString::IsALetter( myString.at( i ) ) ) {
         currentWord += myString.at( i );
         i++;
      } else {
         if ( !currentWord.empty() )
            seperateWords.push_back( currentWord );
         currentWord.clear();
         while ( i < myString.length()
               && !swansonString::IsALetter( myString.at( i ) ) )
            i++; //find next letter;
      }
   }
   if ( !currentWord.empty() )
      seperateWords.push_back( currentWord );

}

/******************************************************************************
 *   purpose: convert all letters in string to lower case
 *
 *    entry: non void string
 *
 *    exit: string with exclusivly alpha chars between [a-z]
 ******************************************************************************/
string swansonString::LowerCase ( string caseString ) {
   string lowerCaseString = "";
   for ( int i = 0 ; i < caseString.length() ; i++ ) {
      if ( swansonString::IsALetter( caseString.at( i ) ) ) {
         if ( caseString.at( i ) >= 'A' && caseString.at( i ) <= 'Z' ) {
            lowerCaseString += (caseString.at( i ) + ('a' - 'A'));
         } else
            lowerCaseString += caseString.at( i );
      }
   }
   return lowerCaseString;
}
/******************************************************************************
 *   purpose: convert all letters in string to lower case, while preserving non
 *             alpha characters
 *
 *    entry: non void string
 *
 *    exit: string with any alpha chars between [a-z]
 ******************************************************************************/
//todo implement uppercasePreserve
void swansonString::LowerCasePreserve ( string &caseString ) {
   //string lowerCaseString = "";
   for ( int i = 0 ; i < caseString.length() ; i++ ) {
      if ( swansonString::IsALetter( caseString.at( i ) ) ) {
         if ( caseString.at( i ) >= 'A' && caseString.at( i ) <= 'Z' ) {
            stringstream ss;
            ss << char ( (caseString.at( i ) + ('a' - 'A')) );
            caseString.replace(i,1,ss.str());
         }
      }
   }
}

/******************************************************************************
 *   purpose: convert all letters in string to upper case
 *
 *    entry: non void string
 *
 *    exit: string with any alpha chars between [A-Z]
 ******************************************************************************/
string swansonString::UpperCase ( string caseString ) {
   string upperCaseString = "";
   for ( int i = 0 ; i < caseString.length() ; i++ ) {
      if ( swansonString::IsALetter( caseString.at( i ) ) ) {
         if ( caseString.at( i ) >= 'a' && caseString.at( i ) <= 'z' ) {
            upperCaseString += (caseString.at( i ) + ('A' - 'a'));
         } else
            upperCaseString += caseString.at( i );
      }
   }
   return upperCaseString;
}
#endif
