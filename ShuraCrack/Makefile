options = -Wall -Wextra

run: Crack.exe
	Crack.exe

Crack.exe : Crack.cpp ForCrack\TextFunctions.o 
	g++ Crack.cpp ForCrack\TextFunctions.o -mconsole c:\Winmm\winmm.dll -o Crack.exe $(options)

TextFunctions.o : ForCrack\TextFunctions.cpp ForCrack\TextFunctions.hpp
	g++ -c ForCrack\TextFunctions.cpp -o ForCrack\TextFunctions.o $(options)