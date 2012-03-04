keytest:	interface.h buttonconfig.cc keytest.cc moveHooks.cc
	g++ `sdl-config --cflags --libs` keytest.cc buttonconfig.cc moveHooks.cc -o keytest

clean:
	rm keytest
