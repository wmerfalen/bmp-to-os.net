GCC = g++
FLAGS = -std=c++14 -Wall 

all: bmp.cpp
	$(GCC) $(FLAGS) bmp.cpp -o bmp
