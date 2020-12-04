#include <stdio.h>
#include <math.h>
#include <ctype.h>
#include <string.h>

const double PSEUDO_ZERO = 1E-6;

struct dependencies
{
    const char*       string    = nullptr;
    size_t            position  = 0;
};

double GetG (const char* string);

double GetE (dependencies* dep);

double GetT (dependencies* dep);

double GetPWR (dependencies* dep);

double GetP (dependencies* dep);

char GetF (dependencies* dep);

double GetB (dependencies* dep);

double GetN (dependencies* dep);

void SkipSpaces (dependencies* dep);

bool Require (dependencies* dep, char symbol);

void SyntaxError (dependencies* dep);

bool IsZero (double val);

void DivByZero (dependencies* dep);

int main ()
{
    char string[] = " ( cos ( 1 ) ^ ( 3 - 1 ) + sin ( 1 ) ^ 2 ) * ( ( 35 + 5 ^ 2 ) * 2 ) / 2 * 5";

    double val = GetG (string);

    printf ("%lg\n", val);

    return NULL;
}

double GetG (const char* string)
{
    dependencies dep = {};

    dep.string = string;

    SkipSpaces (&dep);

    double val = GetE (&dep);

    if (isnan (val))
    {
        return NAN;
    }

    SkipSpaces (&dep);

    if (Require (&dep, '\0')) return NAN;

    return val;
}

double GetE (dependencies* dep)
{
    double val = GetT (dep);

    SkipSpaces (dep);

    if (isnan (val)) return NAN;

    while ((dep->string[dep->position] == '+') || (dep->string[dep->position] == '-'))
    {
        char op = dep->string[dep->position];
        dep->position++;

        SkipSpaces (dep);
        double val2 = GetT (dep);
        if (isnan (val2)) return NAN;

        if (op == '+') val += val2;
        else           val -= val2;
    }

    return val;
}

double GetT (dependencies* dep)
{
    double val = GetPWR (dep);

    SkipSpaces (dep);

    if (isnan (val)) return NAN;

    while ((dep->string[dep->position] == '*') || (dep->string[dep->position] == '/'))
    {
        char op = dep->string[dep->position];
        dep->position++;

        SkipSpaces (dep);
        double val2 = GetPWR (dep);
        if (isnan (val2)) return NAN;

        if (op == '/')
        {
            if (IsZero (val2))
            {
                DivByZero (dep);
                return NAN;
            }
            else val /= val2;
        }
        else val *= val2;
    }

    return val;
}

double GetPWR (dependencies* dep)
{
    double val = GetP (dep);

    SkipSpaces (dep);

    if (isnan (val)) return NAN;

    while (dep->string[dep->position] == '^')
    {
        dep->position++;

        SkipSpaces (dep);
        double val2 = GetP (dep);

        if (isnan (val2)) return NAN;

        val = pow (val, val2);
    }

    return val;
}

#define DEF_FUNCTION(name, num, equivalent)\
                                           \
if (func_num == num)                       \
{                                          \
    equivalent                             \
    return val;                            \
}

double GetP (dependencies* dep)
{
    double val = NAN;

    if (islower (dep->string[dep->position]))
    {
        char func_num = GetF(dep);
        if (func_num == 0) return NAN;

        SkipSpaces (dep);

        val = GetB (dep);
        if (isnan (val)) return NAN;

        SkipSpaces (dep);

        #include "Functions.hpp"

        #undef DEF_FUNCTION
    }

    if (dep->string[dep->position] == '(')
    {
        val = GetB (dep);
        if (isnan (val)) return NAN;

        SkipSpaces (dep);
    }
    else
    {
        val = GetN (dep);
        if (isnan (val)) return NAN;

        SkipSpaces (dep);
    }

    return val;
}

char GetF (dependencies* dep)
{
    size_t length = 0;

    #define DEF_FUNCTION(name, num, equivalent)                   \
                                                                  \
    length = strlen (#name);                                      \
                                                                  \
                                                                  \
    if (strncmp (#name, dep->string + dep->position, length) == 0)\
    {                                                             \
        dep->position += length;                                  \
        return num;                                               \
    }

    #include "Functions.hpp"

    #undef DEF_FUNCTION

    SyntaxError (dep);

    return 0;
}

double GetB (dependencies* dep)
{
    if (Require (dep, '(')) return NAN;
    dep->position++;

    SkipSpaces (dep);

    double val = GetE (dep);
    if (isnan (val)) return NAN;

    SkipSpaces (dep);

    if (Require (dep, ')')) return NAN;
    dep->position++;

    return val;
}

double GetN (dependencies* dep)
{
    double val = NAN;

    char* end_ptr = nullptr;

    val = strtod (dep->string + dep->position, &end_ptr);

    if (end_ptr == dep->string + dep->position)
    {
        SyntaxError (dep);
        return NAN;
    }

    dep->position += end_ptr - (dep->string + dep->position);

    return val;
}

void SkipSpaces (dependencies* dep)
{
    if (dep->string[dep->position] == ' ')
    {
        for (size_t i = 1;; i++)
        {
            if (dep->string[dep->position + i] != ' ')
            {
                dep->position += i;
                break;
            }
        }
    }
}

bool Require (dependencies* dep, char symbol)
{
    if (dep->string[dep->position] != symbol)
    {
        SyntaxError (dep);
        return 1;
    }

    return 0;
}

void SyntaxError (dependencies* dep)
{
    printf ("Error is found\n");

    printf ("%s\n", dep->string);
    for (size_t i = 0; i < dep->position; i ++)
    {
        printf ("-");
    }
    printf ("^\n");
}

bool IsZero (double val)
{
    return (fabs (val) <= PSEUDO_ZERO);
}

void DivByZero (dependencies* dep)
{
    SyntaxError (dep);
    printf ("DIVISION BY 0\n");
}
