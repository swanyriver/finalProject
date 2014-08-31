This program demonstrates the Game of Life utilizing the NCurses library for
an output with animation and user input.

The provided makefile will compile this program for you simply type:
"make"

This game will attempt to match the size of your terminal.  However terminal
sizes larger than 80x22 over SSH sometimes present lag problems and visual 
aborations will be observed.  A large simulation of the Game Of Life is
best viewed on a local machine.


Contols of the game are as follows
w - speed up animation
s - slow down animation
q - quit program

The following command line arguments will modify the programs behavior

"-s" allows for the manual input of screen resoltuion

"-a" uses an alternate subclass for storing the state of the world that uses
     two dynamic two dimensional boolean arrays instead of a set of cordinates
     
"-c" will disable color mode, use this if your terminal emulator is not
     displaying the game correctly
     
"--help" will display the contents of this document
