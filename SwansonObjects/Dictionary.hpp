/***********************************************************
 * Author: Brandon Swanson
 * Date Created: 07-13-2014
 * Last Modification Date: 07-13-2014
 * Filename: Dictionary.hpp
 *
 * Overview: Create a searchable map of words
 *
 * Input: a file with alpha only, lower case, return character separated words
 *
 * Output: a dictionary object that can be polled for number of words contained
 * and if a single string element exists in the dictionary
 *
 ***********************************************************/

#ifndef DICTIONARY_HPP_
#define DICTIONARY_HPP_

#include <fstream>
#include <string>
#include <set>
#include <algorithm> //for string compare less<string>, used by set<>
#include <iterator>
#include <ctime>
#include "../SwansonLibs/swansonUtils.hpp"
#include "../SwansonLibs/swansonString.hpp"


using namespace std;

class Dictionary {

private:
   set<string> wordSet;
   set<string>::iterator lookup;
   int MAX_WORD_LENGTH;

public:
   void AddAlphabet(){
      for(char letter='a';letter<='z';letter++){
         wordSet.insert(swansonString::GetString(letter));
      }
   }
   void AddWord(string word){
      wordSet.insert(word);
   }
   int getMaxWordLenght(){
      return MAX_WORD_LENGTH;
   }

protected:

   bool constructionWasSuccesfull;

   void DetermineLongestWord(){
      MAX_WORD_LENGTH=0;
      for(set<string>::iterator it=wordSet.begin(); it!=wordSet.end(); it++){
         if(it->length()>MAX_WORD_LENGTH) MAX_WORD_LENGTH = it->length();
      }
   }

   /**************************************************************
    *
    * * Entry: a file with only alpha, lower case, return character separated
    *          words
    *
    * * Exit: wordSet is filled with unique and sorted strings
    *          alpha only, lowercase, excluding ones greater than max length
    *
    * * Purpose: to parse a file into WORDS, for dictionary data set
    *
    *
    * ***************************************************************/
   virtual void ReadFromFile ( fstream &instream , set<string> &wordSet ,
         const int MaxWordLength ) {

      string nextWord;

      while ( !instream.eof() ) {
         getline( instream , nextWord ); // Retrieve next word
         if ( MaxWordLength == UNRESTRICTED
               || nextWord.size() <= MaxWordLength ) {
            wordSet.insert( nextWord );
         }
      }

   }

   /**************************************************************
    *
    * * Entry: an existing file at <filename>
    *
    * * Exit: an inflated map
    *
    * * Purpose:  open and close dictionary file, report failures to inflate
    *    data set
    *
    * ***************************************************************/
   bool InflateDict ( string filename , const int MaxWordLength ) {

      fstream instream;

      //open dictionary file
      instream.open( filename.c_str() );

      if ( instream.fail() ) {
         return false;
      }

      //read words from file
      this->ReadFromFile( instream , wordSet , MaxWordLength );

      //words added  //close file
      instream.close();

      if(MaxWordLength==UNRESTRICTED){
         DetermineLongestWord();
      }

      if ( wordSet.empty() )
         return false;

      return true;

   }

public:
   static const int UNRESTRICTED = -1;
   static const string DICTIONARY_EMPTY;
   static const string OUT_OF_BOUNDS;

   ///constructors
   Dictionary ( bool dummy ){
      constructionWasSuccesfull = false; //used to instantiate an empty dictionary

   }
   Dictionary ( int maxWordLength = UNRESTRICTED , string filename =
         "dictionary.txt" , bool construct = true ):MAX_WORD_LENGTH(maxWordLength) {
      //inflates on construction by default
      //sub classes can override ReadFromFile() method, and must call inflate
      //in their own constructors
      if ( construct )
         constructionWasSuccesfull = InflateDict( filename , maxWordLength );
      else
         constructionWasSuccesfull = false;
   }
   //constructor for a pre-fab set of words,  primarily for using error version
   //in a polymorphic manner.
   Dictionary ( set<string> PreFabSet ) :
         wordSet( PreFabSet ), constructionWasSuccesfull( true ) {
      if(wordSet.size()==0) constructionWasSuccesfull = false;
      DetermineLongestWord();
   }

   bool Filled () {
      return constructionWasSuccesfull;
   }

   /**************************************************************
    *
    * * Entry:an inflated set
    *
    *
    * * Exit:true if map contains 1 or more of element, false otherwise
    *
    *
    * * Purpose: determine if passed in word is a word in dictionary
    *
    *
    * ***************************************************************/
   bool IsAWord ( string word ) {
      if ( wordSet.count( word ) > 0 )
         return true;
      else
         return false;
   }

   //returns number of words
   long int NumWords () {
      return wordSet.size();
   }

   //returns string at a given position
   //only useful for a specialized generation of random numbers for access
   string GetWordAt ( int position ) {

      if ( !Filled() )
         return DICTIONARY_EMPTY;
      if ( position < 0 || position > NumWords() - 1 )
         return OUT_OF_BOUNDS;

      lookup = wordSet.begin();
      advance( lookup , position );
      return *lookup;

   }

   string GetRandomWord () {
      if ( !Filled() )
         return DICTIONARY_EMPTY;

      int position = swansonUtil::GetRandomInRange( NumWords() - 1 );
      return GetWordAt( position );
   }

};

///PUBLIC ERROR CODES////////////////////////////////////////////
const string Dictionary::DICTIONARY_EMPTY = "DICTIONARY_EMPTY";
const string Dictionary::OUT_OF_BOUNDS = "OUT_OF_BOUNDS";
///PUBLIC ERROR CODES///////////////////////////////////////////

/**************************************************************
 * * Purpose: A dictionary object with a non optimized input stream file
 *            Considerably longer input time but possibly more fun
 * ***************************************************************/

class ThemeDictionary: public Dictionary {
public:

   ///constructors
   ThemeDictionary ( bool dummy ) :
         Dictionary( dummy ) {
   }  //call parent constructor
   ThemeDictionary ( int maxWordLenght = UNRESTRICTED , string filename =
         "dictionary.txt" , bool construct = false ) :
         Dictionary( maxWordLenght , filename , false ) {
      /*must use this two staged inflation,  base class will by default
       * inflate upon construction, but then the overridden subclass method
       * is not called during the inflate method, because the 'this' pointer
       * is still referencing the base class,  in this constructor the construct
       * =false flag is sent to the super constructor to prevent it from
       * inflating using parent method default arguments, then the derived
       * class inflates with its specialized word parsing method
       */
      constructionWasSuccesfull = InflateDict( filename , maxWordLenght );
   }

private:
   /**************************************************************
    *
    * * Entry: a file with something in it, preferably at some point in it
    *          combinations of alpha characters separated by spaces on one or
    *          both sides, and comprised of intelligible words.
    *
    * * Exit: wordSet is filled with unique and sorted strings
    *          alpha only, lowercase, excluding ones greater than max length
    *
    * * Purpose: to parse a file into WORDS, for dictionary data set
    *
    *
    * ***************************************************************/
   static const int BAIL = 5; //time until we give up on file
   void ReadFromFile ( fstream &instream , set<string> &wordSet ,
         const int MaxWordLength ) {

      string nextWord;
      int timein = time(NULL);

      while ( !instream.eof() && time(NULL)-timein<BAIL) { //x second bailout
         while ( !swansonString::IsALetter( (instream.peek()) ) ) {
            instream.ignore( 1 );
         }
         string nextWord;
         getline( instream , nextWord , ' ' );
         //check last character for alpha // for !.?"
         while ( !swansonString::IsALetter( nextWord.at( nextWord.size() - 1 ) ) ) {
            nextWord.erase( nextWord.size() - 1 , 1 ); // delete last character
         }

         if ( (!nextWord.empty() && swansonString::AllLetters( nextWord ))
               && (MaxWordLength == UNRESTRICTED
                     || nextWord.size() < MaxWordLength) ) {

            nextWord = swansonString::LowerCase( nextWord );

            wordSet.insert( nextWord );
         }
      }

      //cout << "time out is " << time(NULL) << " file took " << time(NULL)-timein;

   }

};

#endif /* DICTIONARY_HPP_ */

