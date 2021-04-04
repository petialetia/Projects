options = -Wall -Wextra -pedantic -O3

run: MathBeauty.out
	./MathBeauty.out

MathBeauty.out: MathBeauty.cpp MathBeauty.hpp
	g++ MathBeauty.cpp -mavx -lSDL2 $(options) -o MathBeauty.out
