/*
 * CompileMenuItem.hpp
 *
 *  Created on: Jul 21, 2014
 *      Author: root
 */

#ifndef COMPILEMENUITEM_HPP_
#define COMPILEMENUITEM_HPP_

#include "menu.hpp"
#include "../SwansonLibs/swansonInput.hpp"

#include "../SwansonLibs/swansonString.hpp"

#include <iostream>
#include <string>
#include <cstdlib>
#include <list>
using namespace std;

class CompileMenuItem: public MenuItem {
private:

   string compileCommand, runCommand, myFilename;
   bool compiled;
   void ItemSelected () {
      if ( compiled )
         system( runCommand.c_str() );
   }
public:

   static const string N0T_COMP;

   bool compile () {
      if ( system( compileCommand.c_str() ) == 0 ) {
         cout << myFilename << " sucessfully compiled" << endl;
         return true;
      } else {
         //         if ( !swansonInput::yesNo(
         //               "something has gone wrong with <" + filename
         //                     + ">  continue anyways " ) )
         //            exit( 1 );

         this->title += CompileMenuItem::N0T_COMP;
         this->itemRepeat = false;
         this->hasIntro = false;
         return false;
      }
   }

   CompileMenuItem ( string filename , string args = "" ) :
         MenuItem( filename , "welcome to " + filename ) {
      myFilename = filename;
      string binary = "BIN" + filename;
      compileCommand = "g++ -o " + binary + " " + filename + ".cpp";
      runCommand = "./" + binary + args;



      compiled=this->compile();

   }

};

const string CompileMenuItem::N0T_COMP = " NOT COMPILED";

class AutoCompileMenu: public Menu {
public:

   AutoCompileMenu ( list<string> items , string title =
         "all of the compiled programs" ) :
         Menu( title ) {
      this->clearScreenON = false;

      while ( !items.empty() ) {
         if ( swansonString::NumOccurances( items.front() , " " ) > 0 ) {
            string temp = items.front();
            int pos = temp.find_first_of( ' ' );
            string filename = temp.substr( 0 , pos );
            string args = temp.substr( pos , temp.size() );

//            cout << "file is:" << filename << " args are:" << args;
//            swansonInput::yesNo("is this right");
            this->addItem( new CompileMenuItem( filename , args ) );

         } else
            this->addItem( new CompileMenuItem( items.front() ) );
         items.pop_front();
      }

      this->menuRepeat = true;
      this->clearScreenON = false;
      this->demoAllItem = true;

   }
};

#endif /* COMPILEMENUITEM_HPP_ */
