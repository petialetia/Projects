//#define SUPERPROTECT

/*#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <math.h>
#include <windows.h>*/

#ifdef SUPERPROTECT
#define get_name(var) #var

#define check_out(stk)                     \
    if (is_stack_corrupted (stk))          \
    {                                      \
        stack_dump (stk);                  \
        assert (!is_stack_corrupted (stk));\
    }
#else
#define get_name
#define check_out;
#endif


//typedef double Elem_t;

//const double POISON = NAN;

//typedef char* Elem_t;

//char* const POISON = nullptr;

typedef bool Elem_t;

bool const POISON = 0;

const short coef = 2;

const short min_capacity = 1;

#ifdef SUPERPROTECT
enum Error
{
    No_Error,
    STK_EQUALS_NULLPTR,
    NO_ACCESS_STK,
    DEAD_CANARY_IN_STACK,
    INCORRECT_HASH_IN_STACK,
    DATA_EQUALS_NULLPTR,
    INCORRECT_HASH_IN_ELEM,
    DEAD_CANARY_IN_ELEM,
    NEGATIVE_SIZE,
    NOT_POSITIVE_CAPACITY,
    SIZE_BIGGER_THAN_CAPACITY,
    TOO_BIG_SIZE,
    TOO_SMALL_SIZE
};
#endif

struct Stack_t
{

    #ifdef SUPERPROTECT

    Stack_t* canary1 = nullptr;

    #endif

    int      size = 0;
    int      capacity = 0;
    Elem_t*  data = nullptr;

    #ifdef SUPERPROTECT

    char*    name_of_stack = nullptr;
    char     hash = 0;
    Stack_t* canary2 = nullptr;

    #endif
};

#ifdef SUPERPROTECT
const size_t safetyness = 2*sizeof(Stack_t*) + sizeof(char);
#else
const size_t safetyness = 0;
#endif

/*!
Construct for stack
\param[in] stk               Pointer on struct Stack_t
\param[in] start_capacity    Start capacity for stack, by default it is min_capacity
*/

void Construct (Stack_t* stk, int start_capacity = min_capacity);

#ifdef SUPERPROTECT

/*!
Returns true if you can use the pointer, else returns false
\param[in] ptr     Pointer on something
*/

bool can_ptr_be_used (const void* ptr);

/*!
Returns pointer on left canary in array of elements in stack
\param[in] stk     Pointer on struct Stack_t
*/

Stack_t** get_ptr_on_left_canary_in_elem (Stack_t* stk);

/*!
Returns pointer on right canary in array of elements in stack
\param[in] stk     Pointer on struct Stack_t
*/

Stack_t** get_ptr_on_right_canary_in_elem (Stack_t* stk);

/*!
Function, which makes blanks elements in array equals NAN (POISON!)
\param[in] stk                 Pointer on struct Stack_t
\param[in] start_position      Position in array where from spills poison
*/

void spill_poison (Stack_t* stk, size_t start_position);

/*!
Updates value of hash in stack after changing
\param[in] stk     Pointer on struct Stack_t
*/

void replace_hash_in_Stack_t (Stack_t* stk);

/*!
Counts value of hash in stack, returns it
\param[in] stk     Pointer on struct Stack_t
*/

char count_hash_in_Stack_t (Stack_t* stk);

/*!
Updates value of hash in array after changing
\param[in] stk     Pointer on struct Stack_t
*/

void replace_hash_in_elements (Stack_t* stk);

/*!
Returns pointer on hash in array of elements in stack
\param[in] stk     Pointer on struct Stack_t
*/

char* get_ptr_on_hash_in_elem (Stack_t* stk);

/*!
Counts value of hash in array, returns it
\param[in] stk     Pointer on struct Stack_t
*/

char count_hash_in_elements (Stack_t* stk);

#endif

/*!
Function to add new value into stack
\param[in] stk       Pointer on struct Stack_t
\param[in] value     Value needed to be added
*/

void Push (Stack_t* stk, Elem_t value);

/*!
Extends capacity of array if it's necessarily
\param[in] stk     Pointer on struct Stack_t
*/

void extend_stack (Stack_t* stk);

/*!
Returns last pushed value into stack
\param[in] stk     Pointer on struct Stack_t
*/

Elem_t Top (Stack_t* stk);

/*!
Deletes last pushed value into stack
\param[in] stk     Pointer on struct Stack_t
*/

Elem_t Pop (Stack_t* stk);

/*!
Function to reduce capacity of array, returns last pushed value into stack
\param[in] stk     Pointer on struct Stack_t
*/

void shrink_to_fit (Stack_t* stk);

/*!
Destroyer for stack
\param[in] stk     Pointer on struct Stack_t
*/

void Destroy (Stack_t* stk);

#ifdef SUPERPROTECT

//-----------------------------------------------------------------------------

//Cheñking functions

//-----------------------------------------------------------------------------

/*!
Prints all about stack in LogFile.txt
\param[in] stk     Pointer on struct Stack_t
*/

void stack_dump (Stack_t* stk);

/*!
Analyzes stack, finds errors, returns number of error
\param[in] stk     Pointer on struct Stack_t
*/

Error is_stack_corrupted (Stack_t* stk);

/*!
By number of error prints description about it
\param[in] number      Number of an error
\param[in] log_file    Pointer on LogFile.txt
*/

void error_descriptor (Error status, FILE* log_file);

#endif

