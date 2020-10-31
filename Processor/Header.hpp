#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <string.h>
#include <math.h>
#include <windows.h>

#include "Safe stack.cpp"
#include "Onegin.cpp"

const size_t size_of_header = 7;

const size_t max_num_of_labels = 10;

const size_t max_length_of_label = 10;




enum Commands
{

    #define DEF_CMD(name, num, asm_cmd, disasm_cmd, cpu_cmd)\
    CMD_##name = num,


    #include "Commands.hpp"

    #undef DEF_CMD
};

struct CPU_struct
{
    Stack_t stack;
    Stack_t stack_of_calls;
    char* code = nullptr;
    size_t ofs = 0;
    double reg[4];
};

struct label
{
    char addr = -1;
    char* name = nullptr;
};
