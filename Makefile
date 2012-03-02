keytest:	interface.h buttonconfig.cc keytest.cc
	g++ `sdl-config --cflags --libs` keytest.cc buttonconfig.cc -o keytest

clean:
	rm keytest
