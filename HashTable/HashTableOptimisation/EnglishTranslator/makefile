options = -Wall -Wextra -pedantic

recompile:
	g++ -c ../src/HashFunctionsCollection.cpp $(options) -o ../bin/HashFunctionsCollection.o
	g++ -c ../src/Bucket.cpp $(options) -mavx -o ../bin/Bucket.o
	g++ -c ../src/HashTable.cpp $(options) -mavx -mavx2 -o ../bin/HashTable.o
	g++ -c ../src/TextFunctions.cpp $(options) -o ../bin/TextFunctions.o
	g++ -c ../src/ProcessDictionary.cpp $(options) -o ../bin/ProcessDictionary.o
	g++ src/EnglishTranslator.cpp ../bin/ProcessDictionary.o ../bin/TextFunctions.o ../bin/HashTable.o ../bin/HashFunctionsCollection.o ../bin/Bucket.o ../bin/FindHashTable.o $(options) -o EnglishTranslator.out
#	g++ src/EnglishTranslator.cpp ../bin/ProcessDictionary.o ../bin/TextFunctions.o ../bin/HashTable.o ../bin/HashFunctionsCollection.o ../bin/Bucket.o $(options) -o EnglishTranslator.out

test:
	./EnglishTranslator.out in ../../Dictionary.txt out Translated.html
	./EnglishTranslator.out in ../../Dictionary.txt out Translated.html
	./EnglishTranslator.out in ../../Dictionary.txt out Translated.html
	./EnglishTranslator.out in ../../Dictionary.txt out Translated.html
	./EnglishTranslator.out in ../../Dictionary.txt out Translated.html

run: EnglishTranslator.out
#	valgrind --tool=callgrind ./EnglishTranslator.out in ../../Dictionary.txt out Translated.html
	./EnglishTranslator.out in ../../Dictionary.txt out Translated.html

EnglishTranslator.out: src/EnglishTranslator.cpp src/EnglishTranslator.hpp ../bin/ProcessDictionary.o ../bin/TextFunctions.o ../bin/HashTable.o ../bin/HashFunctionsCollection.o ../bin/Bucket.o ../bin/FindHashTable.o
	g++ src/EnglishTranslator.cpp ../bin/ProcessDictionary.o ../bin/TextFunctions.o ../bin/HashTable.o ../bin/HashFunctionsCollection.o ../bin/Bucket.o ../bin/FindHashTable.o $(options) -o EnglishTranslator.out

#EnglishTranslator.out: src/EnglishTranslator.cpp src/EnglishTranslator.hpp ../bin/ProcessDictionary.o ../bin/TextFunctions.o ../bin/HashTable.o ../bin/HashFunctionsCollection.o ../bin/Bucket.o
#	g++ src/EnglishTranslator.cpp ../bin/ProcessDictionary.o ../bin/TextFunctions.o ../bin/HashTable.o ../bin/HashFunctionsCollection.o ../bin/Bucket.o $(options) -o EnglishTranslator.out


../bin/ProcessDictionary.o: ../src/ProcessDictionary.cpp ../include/ProcessDictionary.hpp ../include/ForHashTable.hpp
	g++ -c ../src/ProcessDictionary.cpp $(options) -o ../bin/ProcessDictionary.o

../bin/TextFunctions.o: ../src/TextFunctions.cpp ../include/TextFunctions.hpp
	g++ -c ../src/TextFunctions.cpp $(options) -o ../bin/TextFunctions.o

../bin/HashTable.o: ../src/HashTable.cpp ../include/HashTable.hpp
	g++ -c ../src/HashTable.cpp $(options) -mavx -mavx2 -o ../bin/HashTable.o

../bin/Bucket.o: ../src/Bucket.cpp ../include/Bucket.hpp
	g++ -c ../src/Bucket.cpp $(options) -mavx -o ../bin/Bucket.o

../bin/HashFunctionsCollection.o: ../src/HashFunctionsCollection.cpp ../include/HashFunctionsCollection.hpp ../include/ForHashTable.hpp
	g++ -c ../src/HashFunctionsCollection.cpp $(options) -o ../bin/HashFunctionsCollection.o

../bin/FindHashTable.o: ../src/FindHashTable.asm
	nasm -f elf64 ../src/FindHashTable.asm -o ../bin/FindHashTable.o 