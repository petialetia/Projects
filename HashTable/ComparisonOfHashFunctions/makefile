options = -Wall -Wextra -pedantic

run: ComparisonOfHashFunctions.out
	./ComparisonOfHashFunctions.out in ../Dictionary.txt out ResultTable.csv

ComparisonOfHashFunctions.out: src/ComparisonOfHashFunctions.cpp src/ComparisonOfHashFunctions.hpp ../bin/TextFunctions.o ../bin/HashTable.o ../bin/HashFunctionCollection.o ../bin/ProcessDictionary.o
	g++ src/ComparisonOfHashFunctions.cpp ../bin/TextFunctions.o ../bin/HashTable.o ../bin/Vector.o ../bin/HashFunctionCollection.o ../bin/ProcessDictionary.o $(options) -o ComparisonOfHashFunctions.out

../bin/TextFunctions.o: ../src/TextFunctions.cpp ../include/TextFunctions.hpp
	g++ -c ../src/TextFunctions.cpp $(options) -o ../bin/TextFunctions.o

../bin/ProcessDictionary.o: ../src/ProcessDictionary.cpp ../include/ProcessDictionary.hpp
	g++ -c ../src/ProcessDictionary.cpp ../bin/TextFunctions.o $(options) -o ../bin/ProcessDictionary.o

../bin/HashTable.o: ../src/HashTable.cpp ../include/HashTable.hpp ../bin/Vector.o
	g++ -c ../src/HashTable.cpp ../bin/Vector.o $(options) -o ../bin/HashTable.o

../bin/Vector.o: ../src/Vector.cpp ../include/Vector.hpp
	g++ -c ../src/Vector.cpp $(options) -o ../bin/Vector.o

../bin/HashFunctionCollection.o: ../src/HashFunctionCollection.cpp ../include/HashFunctionCollection.hpp
	g++ -c ../src/HashFunctionCollection.cpp $(options) -o ../bin/HashFunctionCollection.o
