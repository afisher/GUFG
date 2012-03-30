headers = interface.h move.h character.h player.h red.h hitstun.h special.h
source = keytest.cc interface.cc hooks.cc move.cc special.cc character.cc player.cc draw.cc red.cc hitstun.cc
flags = -g -O2 -Wno-write-strings `sdl-config --cflags`
libs = `sdl-config --libs`
OBJS = \
  keytest.o \
  interface.o \
  hooks.o \
  move.o \
  hitstun.o \
  character.o \
  player.o \
  draw.o \
  red.o

FLAGS = -g -O2 -Wno-write-strings `sdl-config --cflags`

LIBS = `sdl-config --libs`

keytest: $(headers) $(source)
	g++ -o keytest $(source) $(FLAGS) $(LIBS)

keytest.o: $(source) $(headers)
	g++ -c keytest.cc $(FLAGS)

interface.o: interface.h interface.cc player.h player.cc character.h character.cc
	g++ -c interface.cc $(FLAGS)

hooks.o: hooks.cc player.h player.cc move.h move.cc 
	g++ -c hooks.cc $(FLAGS)

move.o: move.h move.cc
	g++ -c move.cc $(FLAGS)

character.o: character.h character.cc move.h move.cc hitstun.h hitstun.cc 
	g++ -c character.cc $(FLAGS)

player.o: player.h player.cc character.h character.cc
	g++ -c player.cc $(FLAGS)

draw.o: draw.cc player.h interface.h
	g++ -c draw.cc $(FLAGS)

red.o: red.h red.cc character.h character.cc
	g++ -c red.cc $(FLAGS)

hitstun.o: hitstun.h hitstun.cc move.h move.cc special.h special.cc
	g++ -c hitstun.cc special.cc $(FLAGS)

# -f Is needed to avoid errors with missing files.
clean:
	rm -f keytest *.o
