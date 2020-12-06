options = -Wall -Wextra

run: Derivative.exe
	Derivative.exe

Derivative.exe : Derivative.cpp Derivative.hpp TextFunctions.o TreeFunctions.o
	g++ Derivative.cpp TreeFunctions.o TextFunctions.o -o Derivative.exe

TextFunctions.o : TextFunctions.cpp TextFunctions.hpp
	g++ -c TextFunctions.cpp -o TextFunctions.o $(options)

TreeFunctions.o : TreeFunctions.cpp TreeFunctions.hpp
	g++ -c TreeFunctions.cpp -o TreeFunctions.o $(options)