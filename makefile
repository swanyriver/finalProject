#Makefile template from http://mrbook.org/tutorials/make
#Uses sources and executable variables to build a binary
#first by building objects when they need updating
#then by linking them togethr

##----------------------------------------------------------------------
## 20. Demonstrates at least one makefile
##----------------------------------------------------------------------

CC=g++
CFLAGS=-c
LDFLAGS=-lncurses
SOURCES=cursedGameOfLife.cpp
OBJECTS=$(SOURCES:.cpp=.o)
EXECUTABLE=FUNEX

all: $(SOURCES) $(EXECUTABLE)
	
$(EXECUTABLE): $(OBJECTS)
	$(CC)  $(OBJECTS) -o $@ $(LDFLAGS)

.cpp.o:
	$(CC) $(CFLAGS) $< -o $@


