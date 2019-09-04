CXX = g++
FLAG = -Wall -Wextra -Wvla -std=c++14 -g

test: my_set.o my_set_tester.o
	$(CXX) $(FLAG) my_set.hpp my_set_tester.cpp -o test

testset: my_set.o my_set_tester.o
	$(CXX) $(FLAG) my_set.hpp my_set_tester.cpp -o testset

my_set.o: my_set.h my_set.hpp
	$(CXX) my_set.hpp

my_set_tester.o: my_set_tester.cpp
	$(CXX) my_set_tester.cpp

clean:
	rm -f *.o testset
