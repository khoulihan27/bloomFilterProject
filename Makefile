objs = bloom.o hashFunc.o main.o
CXX = g++
CFLAGS = -Wall -std=c++17 -O2 -IInc

all: $(objs) tests

tests: $(objs)
	$(CXX) $(CFLAGS) -o tests $(objs)

bloom.o: Src/bloom.cpp Inc/bloom.h Inc/hashFunc.h
	$(CXX) $(CFLAGS) -c -IInc Src/bloom.cpp -o bloom.o

hashFunc.o: Src/hashFunc.cpp Inc/hashFunc.h
	$(CXX) $(CFLAGS) -c -IInc Src/hashFunc.cpp -o hashFunc.o

main.o: Src/main.cpp Inc/bloom.h Inc/hashFunc.h
	$(CXX) $(CFLAGS) -c -IInc Src/main.cpp -o main.o

clean:
	rm -f $(objs)
