options = -Wall -Wextra

input = DataBase.txt

run : Akinator.exe
	Akinator.exe $(input)

Akinator.exe : Akinator.cpp Akinator.hpp StackUnsave.o
	g++ Akinator.cpp StackUnsave.o -o Akinator.exe $(options)

StackUnsave.o : Stack.cpp Stack.hpp
	g++ -c Stack.cpp -o StackUnsave.o $(options)

