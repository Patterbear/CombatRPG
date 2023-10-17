# Specify C++ compiler
CXX     = g++

# Specify compiler options
# Optimisation level, outputs debugging info for gdb, and C++ version.
CXXFLAGS = -O0 -g3 -std=c++17

All: all
all: main

main: main.cpp Game.o
	$(CXX) $(CXXFLAGS) main.cpp Game.o -o main

# -c command produces object file
Game.o: Game.cpp Game.h
	$(CXX) $(CXXFLAGS) -c Game.cpp -o Game.o

# Cleanup functions (make clean or make deepclean)
deepclean:
	rm -f *~ *.o main main.exe *.stackdump

clean:
	rm -f *~ *.o *.stackdump

