keytest:	interface.h interface.cc keytest.cc hooks.cc move.cc character.cc frame.cc
	g++ `sdl-config --cflags --libs` -g -O2 -Wno-write-strings keytest.cc interface.cc hooks.cc move.cc character.cc frame.cc -o keytest


clean:
	rm keytest
