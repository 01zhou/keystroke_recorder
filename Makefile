CFLAGS = -Wall -std=c++11 -O3

all: keystroke

keystroke: key.o
	g++ -o keystroke $(CFLAGS) key.o

key.o: key.cpp
	g++ -c key.cpp $(CFLAGS) -fPIC

clean:
	rm -f keystroke
	rm -f *.o

