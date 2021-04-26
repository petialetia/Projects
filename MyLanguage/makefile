options = -Wall -Wextra -pedantic

#Compile: 

ParseCode: FrontEnd.out
	./FrontEnd.out

ProcessTree: BackEnd.out
	./BackEnd.out

Compile: FrontEnd.out BackEnd.out
	./FrontEnd.out
	./BackEnd.out

FrontEnd.out: src/FrontEnd.cpp include/FrontEnd.hpp	bin/TreeFunctions.o bin/TextFunctions.o
	g++ src/FrontEnd.cpp bin/TreeFunctions.o bin/TextFunctions.o $(options) -o FrontEnd.out

BackEnd.out: src/BackEnd.cpp include/BackEnd.hpp bin/TreeFunctions.o bin/TextFunctions.o
	g++ src/BackEnd.cpp bin/TreeFunctions.o bin/TextFunctions.o $(options) -o BackEnd.out

bin/TreeFunctions.o: src/TreeFunctions.cpp include/TreeFunctions.hpp include/TextFunctions.hpp
	g++ -c src/TreeFunctions.cpp $(options) -o bin/TreeFunctions.o

bin/TextFunctions.o: src/TextFunctions.cpp include/TextFunctions.hpp
	g++ -c src/TextFunctions.cpp $(options) -o bin/TextFunctions.o  