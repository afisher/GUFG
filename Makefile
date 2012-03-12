headers = interface.h  move.h character.h player.h frame.h red.h
source = keytest.cc interface.cc hooks.cc move.cc character.cc frame.cc player.cc red.cc
flags = `sdl-config --cflags --libs` -g -O2 -Wno-write-strings 

keytest: $(headers) $(source)
	g++ $(flags) $(source) -o keytest


clean:
	rm keytest
