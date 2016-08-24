#Define special targets
.PHONY: clean, mrproper

#Desable implicit rules
.SUFFIXES:

#Variables
GXX = g++
CXXFLAGS = -std=c++11 -W -Wall -g
SFMLFLAGS = -lsfml-graphics -lsfml-window -lsfml-system

#Creates the executable
all: pathfinding.o
	$(GXX) pathfinding.o -o pathfinding $(SFMLFLAGS)
	$(MAKE) clean
	./pathfinding

#Builds dependecies
pathfinding.o: pathfinding.cpp
	$(GXX) -c pathfinding.cpp -o pathfinding.o $(CXXFLAGS)

#Removes temporary files
clean:
	rm -rf *.o

#Removes all files (except the sources) in order to rebuild
mrproper: clean
	rm -rf buddysAdventures
