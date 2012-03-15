headers = interface.h  move.h character.h player.h frame.h red.h
source = keytest.cc interface.cc hooks.cc move.cc character.cc frame.cc player.cc draw.cc red.cc
flags = -g -O2 -Wno-write-strings `sdl-config --cflags`
libs = `sdl-config --libs` 

keytest: $(headers) $(source)
	g++ $(flags) $(source) -o keytest $(libs)


clean:
	rm keytest
