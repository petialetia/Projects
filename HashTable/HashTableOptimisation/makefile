options = -Wall -Wextra -pedantic

run: EnglishTranslator.out
	valgrind --tool=callgrind ./EnglishTranslator.out in ../Dictionary.txt out Translated.html
	callgrind_annotate callgrind.out.47450

EnglishTranslator.out: src/EnglishTranslator.cpp src/EnglishTranslator.hpp ../bin/ProcessDictionary.o ../bin/TextFunctions.o ../bin/HashTable.o ../bin/HashFunctionsCollection.o ../bin/Vector.o
	g++ src/EnglishTranslator.cpp ../bin/ProcessDictionary.o ../bin/TextFunctions.o ../bin/HashTable.o ../bin/HashFunctionCollection.o ../bin/Vector.o $(options) -o EnglishTranslator.out

../bin/ProcessDictionary.o: ../src/ProcessDictionary.cpp ../include/ProcessDictionary.hpp ../include/ForHashTable.hpp
	g++ -c ../src/ProcessDictionary.cpp $(options) -o ../bin/ProcessDictionary.o

../bin/TextFunctions.o: ../src/TextFunctions.cpp ../include/TextFunctions.hpp
	g++ -c ../src/TextFunctions.cpp $(options) -o ../bin/TextFunctions.o

../bin/HashTable.o: ../src/HashTable.cpp ../include/HashTable.hpp
	g++ -c ../src/HashTable.cpp $(options) -o ../bin/HashTable.o

../bin/Vector.o: ../src/Vector.cpp ../include/Vector.hpp
	g++ -c ../src/Vector.cpp $(options) -o ../bin/Vector.o

../bin/HashFunctionsCollection.o: ../src/HashFunctionsCollection.cpp ../include/HashFunctionsCollection.hpp ../include/ForHashTable.hpp
	g++ -c ../src/HashFunctionsCollection.cpp $(options) -o ../bin/HashFunctionsCollection.o