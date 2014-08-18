/***********************************************************
 * Author: Brandon Swanson
 * Date Created: 06-29-2014
 * Last Modification Date: 07-01-2014
 * Filename: swansonInput.cpp
 *
 * Overview: A collection of static methods designed to facilitate getting input
 *           from the user, all methods take in a prompt that will be given to
 *           clarify desired input to user
 *  7/22/ updated for seperate git tracking
 *  11:00pm  git in git
 * 11:10 git fake submodule
 ***********************************************************/


#ifndef SWANSONINPUT_HPP_
#define SWANSONINPUT_HPP_

#include <string>
#include <iostream>
#include <climits>
#include <cstdlib> 
#include <cfloat>
#include <errno.h>

#include "swansonString.hpp"

//todo, change these to static constants, belonging to the class
#define RETRY_GET_WORD 	"Lets try that again"
#define THIS_WORD	 	"Is this what you wanted to input"

using std::string;
using std::cin;
using std::cout;
using std::endl;

static const int MAX_ATTEMPTS = 6; //maximum number of attempts from user

class swansonInput {
public:
   static string GetString ( string prompt );
   static string GetOneWord ( string prompt );

   static int GetInt ( string prompt );
   static int GetInt ( string prompt , int rangeMin , int rangeMax = INT_MAX );
   static long int GetLong ( string prompt );
   static long int GetLong ( string prompt , long int rangeMin ,
         long int rangeMax = LONG_MAX );

   static float GetFloat ( string prompt );
   static float GetFloat ( string prompt , float rangeMin , float rangeMax =
   FLT_MAX );
   static double GetDouble ( string prompt );
   static double GetDouble ( string prompt , double rangeMin , double rangeMax =
   DBL_MAX );

   static bool yesNo ( string prompt );
};

/******************************************************************************
 *    purpose: receive a string including spaces from the user
 *
 *    entry: a string to be used to clarify desired input to user
 *
 *    exit: a string of user input
 ******************************************************************************/
string swansonInput::GetString ( string prompt ) {

   string str_rtrn = "";

   //request input
   cout << endl << prompt;

   ///Clearing errors and NewLine char
   cin.clear();
   if ( cin.peek() == '\n' ) {
      cin.ignore( 1000 , '\n' );
   }

   //grabEntireLine
   getline( cin , str_rtrn );

   return str_rtrn;

}

/******************************************************************************
 *    purpose: receive a string excluding spaces from the user
 *
 *    entry: a string to be used to clarify desired input to user
 *
 *    exit: a string of user input including no spaces
 ******************************************************************************/
string swansonInput::GetOneWord ( string prompt ) {
   string candaditeWord;
   bool retry = false;

   do {
      if ( retry ) {
         cout << RETRY_GET_WORD;
      }

      candaditeWord = swansonInput::GetString( prompt );

      if ( swansonString::AllLetters( candaditeWord ) ) {
         break;
      }

      list<string> seperatedWords;
      swansonString::SeperateWords( candaditeWord , seperatedWords );

      string allLetterWord;
      string nextWord;
      while ( !seperatedWords.empty() && allLetterWord.empty() ) {

         nextWord = seperatedWords.front();
         seperatedWords.pop_front();

         if ( swansonString::AllLetters( nextWord ) ) {
            allLetterWord = nextWord;
         } else {
            for ( int i = 0 ; i < nextWord.size() ; i++ )
               if ( swansonString::IsALetter( nextWord.at( i ) ) )
                  allLetterWord += nextWord.at( i );
         }
      }

      candaditeWord = allLetterWord;

      if ( !candaditeWord.empty() ) {
         cout << endl << candaditeWord << endl;
         if ( !swansonInput::yesNo( THIS_WORD ) ) {
            candaditeWord.clear();
         }
      }

      retry = true;

   } while ( candaditeWord.empty() );

   return candaditeWord;
}

/******************************************************************************
 *    purpose: receive a value from the user, screen for alpha input
 *             and overflow
 *
 *    entry: a string to be used to clarify desired input to user
 *
 *    exit: a value between TYPE_MIN and TYPE_MAX
 ******************************************************************************/
int swansonInput::GetInt ( string prompt ) {
   return GetInt( prompt , INT_MIN , INT_MAX );
}

/******************************************************************************
 *    purpose: receive a value from the user, screen for alpha input
 *             and overflow, restrict input to specific range
 *
 *    entry: a string to be used to clarify desired input to user
 *
 *    exit: a value between rangeMin and rangeMax
 ******************************************************************************/
int swansonInput::GetInt ( string prompt , int rangeMin , int rangeMax ) {
   return static_cast<int>( GetLong( prompt , rangeMin , rangeMax ) );
}

long int swansonInput::GetLong ( string prompt ) {
   return GetLong( prompt , LONG_MIN , LONG_MAX );

}
long int swansonInput::GetLong ( string prompt , long int rangeMin ,
      long int rangeMax ) {
   long int int_rtrn;
   int attempts = 0;
   string parse_string;
   bool firstTimeThrough = true;

   do {
      if ( !firstTimeThrough ) {
         cout << "Please keep the input within these bounds [" << rangeMin
               << " - " << rangeMax << "]";
      }
      firstTimeThrough = false;
      parse_string = swansonInput::GetString( prompt );

      while ( !swansonString::AllNumbers( parse_string ) || parse_string.empty() ) {
         attempts++;
         if ( attempts > MAX_ATTEMPTS )
            return 0;

         cout << "lets try to restrain ourselves to only whole numbers";
         parse_string = swansonInput::GetString( prompt );
      }

      //int_rtrn = atol(parse_string.c_str());
      //changed to strtol for overflow erno
      int_rtrn = strtol( parse_string.c_str() , NULL , 0 );

   } while ( !(int_rtrn >= rangeMin && int_rtrn <= rangeMax)
         || (errno == ERANGE && (int_rtrn == LONG_MAX || int_rtrn == LONG_MIN)) );

   return int_rtrn;
}

//////////////////////////////////////////////floating point input ///////
float swansonInput::GetFloat ( string prompt ) {
   return GetFloat( prompt , FLT_MIN , FLT_MAX );
}
float swansonInput::GetFloat ( string prompt , float rangeMin ,
      float rangeMax ) {
   return static_cast<float>( GetDouble( prompt , rangeMin , rangeMax ) );
}
double swansonInput::GetDouble ( string prompt ) {
   return GetDouble( prompt , DBL_MIN , DBL_MAX );

}
double swansonInput::GetDouble ( string prompt , double rangeMin ,
      double rangeMax ) {
   double double_rtrn;
   int attempts = 0;
   string parse_string;
   bool firstTimeThrough = true;

   do {
      if ( !firstTimeThrough ) {
         cout << "Please keep the input within these bounds [" << rangeMin
               << " - " << rangeMax << "]";
      }
      firstTimeThrough = false;
      parse_string = swansonInput::GetString( prompt );

      while ( !swansonString::AllNumbersFloat( parse_string )
            || parse_string.empty() ) {
         attempts++;
         if ( attempts > MAX_ATTEMPTS )
            return 0;

         cout << "lets try to restrain ourselves to only "
               << "valid floating point numbers";
         parse_string = swansonInput::GetString( prompt );
      }

      double_rtrn = strtod( parse_string.c_str() , NULL );

   } while ( !(double_rtrn >= rangeMin && double_rtrn <= rangeMax)
         || (errno == ERANGE
               && (double_rtrn == DBL_MAX || double_rtrn == DBL_MIN)) );

   return double_rtrn;
}

/******************************************************************************
 *   purpose: get an affirmative or negative answer from the user
 *
 *    entry: a string to be used to clarify desired input to user
 *
 *    exit: true if entered 'y' or 'yes' in upper or lower case
 *          flase if entered 'n' or 'no' in upper or lower case
 ******************************************************************************/
bool swansonInput::yesNo ( string prompt ) {
   string yesNoStr;

   for ( int i = 0 ; i < MAX_ATTEMPTS ; i++ ) { //give them X trys at  input
      yesNoStr = swansonInput::GetString( prompt + " (y/n)?:" );
      yesNoStr = swansonString::LowerCase( yesNoStr );

      if ( yesNoStr == "y" || yesNoStr == "yes" )
         return true;
      else if ( yesNoStr == "n" || yesNoStr == "no" )
         return false;
   }
   return false; //give up

}
#endif
