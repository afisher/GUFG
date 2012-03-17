headers = interface.h  move.h character.h player.h frame.h red.h hitstun.h
source = keytest.cc interface.cc hooks.cc move.cc character.cc frame.cc player.cc draw.cc red.cc hitstun.cc
flags = -g -O2 -Wno-write-strings `sdl-config --cflags`
libs = `sdl-config --libs`
OBJS = \
  keytest.o \
  interface.o \
  hooks.o \
  move.o \
  hitstun.o \
  character.o \
  frame.o \
  player.o \
  draw.o \
  red.o

FLAGS = -g -O2 -Wno-write-strings `sdl-config --cflags`

LIBS = `sdl-config --libs`

keytest: $(OBJS)
	g++ -o keytest $(OBJS) $(FLAGS) $(LIBS)

keytest.o: keytest.cc
	g++ -c keytest.cc $(FLAGS)

interface.o: interface.h interface.cc
	g++ -c interface.cc $(FLAGS)

hooks.o: hooks.cc
	g++ -c hooks.cc $(FLAGS)

move.o: move.h move.cc
	g++ -c move.cc $(FLAGS)

character.o: character.h character.cc
	g++ -c character.cc $(FLAGS)

frame.o: frame.h frame.cc
	g++ -c frame.cc $(FLAGS)

player.o: player.h player.cc
	g++ -c player.cc $(FLAGS)

draw.o: draw.cc
	g++ -c draw.cc $(FLAGS)

red.o: red.h red.cc
	g++ -c red.cc $(FLAGS)

hitstun.o: hitstun.h hitstun.cc
	g++ -c hitstun.cc $(FLAGS)

# -f Is needed to avoid errors with missing files.
clean:
	rm -f keytest *.o
