OBJS = \
  keytest.o \
  interface.o \
  hooks.o \
  move.o \
  character.o \
  frame.o \
  player.o \
  draw.o \
  red.o

FLAGS = -g -O2 -Wno-write-strings `sdl-config --cflags`

LIBS = `sdl-config --libs`

keytest: $(OBJS)
	g++ -o keytest $(OBJS) $(FLAGS) $(LIBS)

keytest.o:
	g++ -c keytest.cc $(FLAGS)

interface.o:
	g++ -c interface.cc $(FLAGS)

hooks.o:
	g++ -c hooks.cc $(FLAGS)

move.o:
	g++ -c move.cc $(FLAGS)

character.o:
	g++ -c character.cc $(FLAGS)

frame.o:
	g++ -c frame.cc $(FLAGS)

player.o:
	g++ -c player.cc $(FLAGS)

draw.o:
	g++ -c draw.cc $(FLAGS)

red.o:
	g++ -c red.cc $(FLAGS)

# -f Is needed to avoid errors with missing files.
clean:
	rm -f keytest *.o
