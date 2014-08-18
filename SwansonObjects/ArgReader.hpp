/*
 * ArgReader.hpp
 *
 *  Created on: Jul 15, 2014
 *      Author: Brandon
 */

#ifndef ARGREADER_HPP_
#define ARGREADER_HPP_

#include <string>
#include <set>
#include <list>
#include <iostream> // for testing

using namespace std;

class ARGinAttor {
public:

   struct ControlFlag {
      string argument;
      bool *controlFlag;
   };

   bool ArgumentPassedIn ( string argument );

private:
   set<string> arguments;

   void CheckFlag ( ControlFlag flag ) {
      if ( ArgumentPassedIn( flag.argument ) )
         *flag.controlFlag = true;
      else
         *flag.controlFlag = false;
   }

   void Initialize ( int argc , char* argv[] );
   void Initialize ( int argc , char* argv[] , list<ControlFlag> flags );

public:
   ARGinAttor ( int argc , char* argv[] , bool InitonConstruction = true ) {

      if ( InitonConstruction )
         Initialize( argc , argv );
   }

   ARGinAttor ( int argc , char* argv[] , list<ControlFlag> flags ,
         bool InitonConstruction = true ) {

      if ( InitonConstruction )
         Initialize( argc , argv , flags);
   }

};

void ARGinAttor::Initialize ( int argc , char* argv[] ) {

   for ( int index = 0 ; index < argc ; index++ ) {
      arguments.insert( string( argv[index] ) );
   }

}

void ARGinAttor::Initialize ( int argc , char* argv[] ,
      list<ControlFlag> flags ) {

   Initialize(argc, argv);

   while ( !flags.empty() ) {
      CheckFlag( flags.back() );
      flags.pop_back();
   }
}

bool ARGinAttor::ArgumentPassedIn ( string argument ) {
   if ( arguments.count( argument ) > 0 )
      return true;
   else
      return false;
}


#endif /* ARGREADER_HPP_ */
