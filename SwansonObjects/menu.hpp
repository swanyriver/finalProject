/**********************************************************
 *
 *  Author: Brandon Swanson
 *
 *  Date Created: 07-06-2014
 *
 *  Last Modification Date: 07-07-2014
 *
 *  Filename: menu.hpp
 *
 *  Overview: Holds a list of Menu Items which contain output strings and a
 *  single void() function to be executed upon selection
 *
 *  Input: an integer from [1-Number of selections available]
 *
 *  Output:selected function is executed
 *
 * updated 7/22 for subtree git
 * updated from FOO 5:45pm
 * **********************************************************/

#ifndef MENU_HPP_
#define MENU_HPP_

#include <iostream>
#include <string>
#include <vector>
#include <cstdlib>
#include "../SwansonLibs/swansonString.hpp"
#include "../SwansonLibs/swansonInput.hpp"
#include "../SwansonLibs/swansonUtils.hpp"
using namespace std;

//strings for menu
#define DFLT_REPEART "would you like to do that again"
#define EXIT "EXIT MENU"
#define SHOW_ALL "DEMONSTRATE ALL FUNCTIONS"
#define NUM_IN "Please choose by number:"

//items to be added to menu
class MenuItem {

public:
   string title, intro, repeatpromt;
   bool itemRepeat;
   bool hasIntro;

   //constructor for menu item
   MenuItem ( string itemTitle , string itemIntro ,
         string itemRepeatprompt = DFLT_REPEART ) {
      itemRepeat = true;
      hasIntro = true;
      title = itemTitle;
      intro = itemIntro;
      repeatpromt = itemRepeatprompt;
      //myFunction = itemFunction;
   }

   //introduces and executes the function held by menu item
   virtual void ItemSelected () =0;
};

class Menu {
private:
   vector<MenuItem*> menuItems;
   string menuIntro;
   void (*MenuClear)();


public:

   bool clearScreenON;
   bool menuRepeat; //controls if menu repeats
   bool demoAllItem, exitMenuItem; //include items for menu functions



   //constructor for menu
   Menu ( string intro ) :
         menuIntro( intro ), menuRepeat( true ), demoAllItem( false ), exitMenuItem(
               true ),clearScreenON(true){
      MenuClear = swansonUtil::ClearScreen;
      //todo add default constructor for this
   }

   void setClear(void (*clear) ()){
      MenuClear = clear;
   }

   void SetIntro(string intro){
      menuIntro = intro;
   }

   //add an item to the menu
   void addItem ( MenuItem *item ) {
      menuItems.push_back( item );
   }

   MenuItem* GetLastItem(){return menuItems.back();}

   void RunSelection(MenuItem *selected){
      do {
            if ( selected->hasIntro )
               cout << endl << selected->intro << endl;
            selected->ItemSelected();
         } while ( selected->itemRepeat
               && swansonInput::yesNo( selected->repeatpromt ) );
   }

   //display menu options and prompt for selection
   void showMenu ( bool withIntro = true ) {
      int demoItemNumber = -1;
      int exitItemNumber = -1;
      do {
         ///////////display menu////////////////////
        if(clearScreenON) MenuClear();
         if ( withIntro )
            cout << endl << menuIntro << endl;
         int i = 0;
         for ( ; i < menuItems.size() ; i++ ) {
            cout << "[" << i + 1 << "] ";
            cout << menuItems.at( i )->title;
            cout << endl;
         }
         ////////////display menu function items/////
         i++;
         if ( demoAllItem ) {
            cout << "[" << i << "] " << SHOW_ALL << endl;
            demoItemNumber = i;
            i++;
         }
         if ( exitMenuItem ) {
            cout << "[" << i << "] " << EXIT << endl;
            exitItemNumber = i;
         }

         //get selection//////////
         int selection = swansonInput::GetInt( NUM_IN , 1 ,
               menuItems.size() + demoAllItem + exitMenuItem );

         //execute selection
         if ( selection == demoItemNumber ) { //demo all
            for ( int i = 0 ; i < menuItems.size() ; i++ )
               RunSelection(menuItems.at( i ));
         } else if ( selection == exitItemNumber ) {
            menuRepeat = false;
         } else {
            RunSelection(menuItems.at( selection - 1 ));
         }

      } while ( menuRepeat ); //repeat menu
   }

   //used for passing in the number you wish selected
   void runFromCommandLine ( int argc , char* argv[] ) {

      for ( int i = 1 ; i < argc ; i++ ) {

         if ( swansonString::AllNumbers( argv[i] ) ) {

            int selectionNumber = strtol( argv[i] , NULL , 0 );

            if ( selectionNumber == 0 ) { //demo all
               for ( int i = 0 ; i < menuItems.size() ; i++ )
                  RunSelection(menuItems.at( i ));
            } else if ( selectionNumber > 0
                  && selectionNumber <= menuItems.size() ) {
               RunSelection(menuItems.at( selectionNumber - 1 ));
            }

         }
      }
   }

};



////////////////////////////////////////////MENU ITEM TYPES
/////////////////////////////////////////////////////////////////////

class GoItem:public MenuItem{
private:
   void (*myFunction) ();
public:
   GoItem ( void (*itemFunction) () , string itemTitle , string itemIntro ,
         string itemRepeatprompt = DFLT_REPEART ):MenuItem(itemTitle,itemIntro,itemRepeatprompt) {

      myFunction = itemFunction;
   }

   void ItemSelected(){
      myFunction();
   }
};


///////////boolean item

class BoolItem:public MenuItem{
private:
   bool *myBool;
   string trueStr, falseStr, titleBase;

   string getStateString(){
      if(*myBool) return " [" + trueStr + "]";
      else return " [" + falseStr + "]";
   }

public:
   BoolItem (bool &boolIn, string titleIn, string tstr, string fstr):
      MenuItem("",""),myBool(&boolIn),
      titleBase(titleIn),trueStr(tstr),falseStr(fstr){

      itemRepeat=false;
      hasIntro=false;

      this->title=titleBase+getStateString();
   }

   void ItemSelected(){
      *myBool = !*myBool;
      title=titleBase+getStateString();
   }

};

//itterator menu item

class ListItem:public MenuItem{
private:
   vector<string> myList;
   int position;
   string titleBase;

   string getListString(){
      return " [" + myList.at(position) + "]";
   }

public:
   ListItem(vector<string> listIncoming, string titleB ,int pos)
      :MenuItem("",""), myList(listIncoming),position(pos),
       titleBase(titleB){

      itemRepeat=false;
      hasIntro=false;

      this->title=titleBase+getListString();
   }

   void ItemSelected(){
      if(position==myList.size()-1) position=0;
      else position++;
      this->title=titleBase+getListString();
   }

   string GetSelection(){
      return myList.at(position);
   }

};

#endif /*MENU_HPP_ */
