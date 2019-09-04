CXX = g++
FLAGS = -Wall -Wextra -Wvla -std=c++14 -g

testint: my_set.o big_int_tester.o big_integer.o
	$(CXX) $(FLAGS) my_set.o big_int_tester.o big_integer.o -o testint

testset: my_set_tester.o my_set.o big_integer.o
	$(CXX) $(FLAGS) my_set_tester.o my_set.o big_integer.o -o testset

all: testint testset

my_set.o: my_set.cpp my_set.h
	$(CXX) -c my_set.cpp

big_integer.o: big_integer.cpp big_integer.h
	$(CXX) -c big_integer.cpp

my_set_tester.o: my_set_tester.cpp
	$(CXX) -c my_set_tester.cpp

big_int_tester.o: big_int_tester.cpp
	$(CXX) -c big_int_tester.cpp

clean:
	rm -f *.o testint testset
