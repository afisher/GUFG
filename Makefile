keytest:	interface.h buttonconfig.cc keytest.cc hooks.cc move.cc character.cc
	g++ `sdl-config --cflags --libs` -g -O2 keytest.cc buttonconfig.cc hooks.cc move.cc character.cc -o keytest


clean:
	rm keytest
