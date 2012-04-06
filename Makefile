# 4/5/2012 - Cleaned up makefile, added 'all' and 'clobber' targets.

#Environment

MKDIR=mkdir
CP=cp
RM=rm
GREP=grep
NM=nm
CC=gcc
CCC=g++
CXX=g++
AS=as

# Macros
CND_PLATFORM=GNU-Linux-x86
CND_CONF=Beta
CND_DISTDIR=
CND_BUILDDIR=

# Object Files
OBJS = \
	keytest.o \
  	interface.o \
  	move.o \
  	moveTrie.o \
	hitstun.o \
	character.o \
	player.o \
	draw.o \
	red.o

# CC Compiler Flags
CCFLAGS = -g -O2 -Wno-write-strings `sdl-config --cflags`
CXXFLAGS = -g -O2 -Wno-write-strings `sdl-config --cflags`

# Link Libraries and Options
LIBS = `sdl-config --libs`

# Header Files (all)
ALL_HEAD = interface.h move.h character.h player.h red.h hitstun.h special.h trie.h

# Source Files (all)
ALL_SRC = keytest.cc interface.cc move.cc special.cc character.cc player.cc draw.cc red.cc hitstun.cc trie.cc

# Build Targets
all: keytest

keytest: $(ALL_HEAD) $(ALL_SRC)
	$(CXX) -o keytest $(ALL_SRC) $(CXXFLAGS) $(LIBS)

keytest.o: $(ALL_SRC) $(ALL_HEAD)
	$(CXX) -c keytest.cc $(CXXFLAGS)

interface.o: interface.h interface.cc player.h player.cc character.h character.cc
	$(CXX) -c interface.cc $(CXXFLAGS)

move.o: move.h move.cc moveTrie.h moveTrie.cc
	$(CXX) -c move.cc moveTrie.cc $(CXXFLAGS)

character.o: character.h character.cc move.h move.cc hitstun.h hitstun.cc 
	$(CXX) -c character.cc $(CXXFLAGS)

player.o: player.h player.cc character.h character.cc
	$(CXX) -c player.cc $(CXXFLAGS)

draw.o: draw.cc player.h interface.h
	$(CXX) -c draw.cc $(CXXFLAGS)

red.o: red.h red.cc character.h character.cc
	$(CXX) -c red.cc $(CXXFLAGS)

hitstun.o: hitstun.h hitstun.cc move.h move.cc special.h special.cc
	$(CXX) -c hitstun.cc special.cc $(CXXFLAGS)

# -f Is needed to avoid errors with missing files.
clean:
	$(RM) -f *.o

clobber:
	$(RM) -f keytest *.o
