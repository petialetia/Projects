//#define SUPERPROTECT

#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <math.h>
#include <windows.h>

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

//Che�king functions

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

//-----------------------------------------------------------------------------

//Functions

//-----------------------------------------------------------------------------

#ifdef SUPERPROTECT

void Construct (Stack_t* stk, int start_capacity)
{
    if (stk == nullptr)
    {
        printf ("Your pointer equals nullptr\n");
    }
    else
    {
        if (!can_ptr_be_used(stk))
        {
            printf ("Your pointer can not be read\n");
        }
        else
        {
            if ((stk->canary1 != nullptr) || (stk->canary2 != nullptr))
            {
                printf ("Your pointer is invalid or you didn't destroy stack before construct new\n");
            }
            else
            {
                if (start_capacity > 1024)
                {
                    printf ("You're trying to construct stack with too big start capacity\n");
                }
                else
                {
                    if (start_capacity <= 0)
                    {
                        printf ("You're trying to construct stack with capacity, which is not positive\n");
                    }
                    else
                    {
                        stk->canary1 = stk;
                        stk->canary2 = stk;

                        stk->capacity = start_capacity;
                        stk->name_of_stack = get_name(stk);

                        stk->data = (Elem_t*) ((char*) calloc (1, (start_capacity * sizeof(Elem_t)) + safetyness) + sizeof(Stack_t*));
                        *get_ptr_on_left_canary_in_elem (stk) = stk;
                        *get_ptr_on_right_canary_in_elem (stk) = stk;

                        spill_poison (stk, stk->size);
                        replace_hash_in_Stack_t (stk);
                        replace_hash_in_elements (stk);
                        check_out (stk);
                    }
                }
            }
        }
    }
}

#else

void Construct (Stack_t* stk, int start_capacity)
{
    if (stk == nullptr)
    {
        printf ("Your pointer equals nullptr\n");
    }
    else
    {
        if ((stk->size != 0) || (stk->capacity != 0) || (stk->data != nullptr))
        {
            printf ("You're trying to construct stack, but there are some data you didn't delete\n");
        }
        else
        {
            if (start_capacity <= 0)
            {
                printf ("You're trying to construct stack with capacity, which is not positive\n");
            }
            else
            {
                stk->capacity = start_capacity;

                stk->data = (Elem_t*) calloc (1, (start_capacity * sizeof(Elem_t)) + safetyness);
            }
        }
    }
}

#endif

#ifdef SUPERPROTECT

bool can_ptr_be_used (const void* ptr)
{
    MEMORY_BASIC_INFORMATION mbi;

    if (!VirtualQuery (ptr, &mbi, sizeof (mbi)))
    {
        return false;
    }

    if (mbi.Protect & (PAGE_GUARD | PAGE_NOACCESS))
    {
        return false;
    }

    DWORD read_rights = PAGE_READONLY | PAGE_READWRITE | PAGE_WRITECOPY | PAGE_EXECUTE_READ | PAGE_EXECUTE_READWRITE | PAGE_EXECUTE_WRITECOPY;

    return (mbi.Protect & read_rights);
}

Stack_t** get_ptr_on_left_canary_in_elem (Stack_t* stk)
{
    return (Stack_t**)((char*)stk->data - sizeof(Stack_t*));
}

Stack_t** get_ptr_on_right_canary_in_elem (Stack_t* stk)
{
    return (Stack_t**) ((char*) (stk->data + stk->capacity) + sizeof(char));
}

void spill_poison (Stack_t* stk, size_t start_position)
{
    for (; start_position < stk->capacity; start_position++)
    {
        stk->data[start_position] = NAN;
    }
}

void replace_hash_in_Stack_t (Stack_t* stk)
{
    stk->hash = count_hash_in_Stack_t (stk);
}

char count_hash_in_Stack_t (Stack_t* stk)
{
    char old_hash = stk->hash;
    stk->hash = 0;
    char* array = (char*) stk;
    char new_hash = 0;

    for (size_t i = 0; i < sizeof (Stack_t); i++)
    {
        new_hash = array[i] ^ new_hash;
    }

    stk->hash = old_hash;
    return new_hash;
}

void replace_hash_in_elements (Stack_t* stk)
{
    *get_ptr_on_hash_in_elem (stk) = count_hash_in_elements (stk);
}

char* get_ptr_on_hash_in_elem (Stack_t* stk)
{
    return (char*)(stk->data + stk->capacity);
}

char count_hash_in_elements (Stack_t* stk)
{
    char old_hash = *get_ptr_on_hash_in_elem (stk);
    *get_ptr_on_hash_in_elem (stk) = 0;
    char* array = ((char*)stk->data - sizeof(Stack_t*));
    char new_hash = 0;

    for (size_t i = 0; i < (stk->capacity * sizeof(Elem_t)) + safetyness; i++)
    {
        new_hash = ((array[i] ^ new_hash) + i) % 126;
    }

    *get_ptr_on_hash_in_elem (stk) = old_hash;
    return new_hash;
}

#endif

void Push (Stack_t* stk, Elem_t value)
{
    check_out (stk);
    if (stk->size == stk->capacity)
    {
    extend_stack (stk);
    }

    stk->data[stk->size] = value;
    stk->size++;

    #ifdef SUPERPROTECT

    replace_hash_in_Stack_t (stk);
    replace_hash_in_elements (stk);
    check_out (stk);

    #endif
}

#ifdef SUPERPROTECT

void extend_stack (Stack_t* stk)
{
    size_t old_capacity = stk->capacity;

    Stack_t* canary_right = *get_ptr_on_right_canary_in_elem (stk);

    stk->capacity = stk->capacity*coef;

    stk->data = (Elem_t*) ((char*) realloc (get_ptr_on_left_canary_in_elem (stk), stk->capacity*sizeof(Elem_t) + safetyness) + sizeof(Stack_t*));

    spill_poison (stk, old_capacity);

    *get_ptr_on_right_canary_in_elem (stk) = canary_right;
}

#else

void extend_stack (Stack_t* stk)
{
    size_t old_capacity = stk->capacity;

    stk->capacity = stk->capacity*coef;

    stk->data = (Elem_t*) realloc (stk->data, stk->capacity*sizeof(Elem_t) + safetyness);
}

#endif

Elem_t Top (Stack_t* stk)
{
    check_out (stk);
    if (stk->size == 0)
    {
        printf ("No top elements in this stack, it's size equals 0");
    }
    else
    {
        return (stk->data[stk->size - 1]);
    }
    return POISON;
}

Elem_t Pop (Stack_t* stk)
{
    check_out (stk);

    Elem_t top = POISON;

    if (stk->size == 0)
    {
        printf ("You're poping stack with no elements\n");
    }
    else
    {
        stk->size--;
        top = stk->data[stk->size];

        if ((stk->size == stk->capacity/(2*coef)) && (stk->capacity/coef >= min_capacity))
        {
            shrink_to_fit (stk);
        }
        else
        {
            stk->data[stk->size] = POISON;
        }
    }

    #ifdef SUPERPROTECT

    replace_hash_in_Stack_t (stk);
    replace_hash_in_elements (stk);

    check_out (stk);

    #endif

    return top;
}

#ifdef SUPERPROTECT

void shrink_to_fit (Stack_t* stk)
{
    Stack_t* canary_right = *get_ptr_on_right_canary_in_elem (stk);

    stk->capacity = stk->capacity/coef;

    stk->data = (Elem_t*) ((char*) realloc (get_ptr_on_left_canary_in_elem (stk), stk->capacity*sizeof(Elem_t) + safetyness) + sizeof (Stack_t*));

    spill_poison (stk, stk->capacity/2);

    *get_ptr_on_right_canary_in_elem (stk) = canary_right;
}

#else

void shrink_to_fit (Stack_t* stk)
{
    stk->capacity = stk->capacity/coef;

    stk->data = (Elem_t*) realloc (stk->data, stk->capacity*sizeof(Elem_t) + safetyness);

}

#endif

#ifdef SUPERPROTECT

void Destroy (Stack_t* stk)
{
    check_out (stk);

    free ((char*)stk->data - sizeof(Stack_t*));

    stk->data = nullptr;
    stk->size = 0;
    stk->capacity = 0;
    stk->name_of_stack = nullptr;
    stk->canary1 = nullptr;
    stk->canary2 = nullptr;
    stk->hash = 0;
}

#else

void Destroy (Stack_t* stk)
{
    check_out (stk);

    free (stk->data);

    stk->data = nullptr;
    stk->size = 0;
    stk->capacity = 0;
}

#endif

//-----------------------------------------------------------------------------

#ifdef SUPERPROTECT

void stack_dump (Stack_t* stk)
{
    static size_t num_of_call = 0;
    num_of_call++;

    static FILE* log_file = nullptr;

    if (num_of_call == 1)
    {
        log_file = fopen ("LogFile.txt", "w");
    }

    Error status = is_stack_corrupted (stk);

    fprintf (log_file, "Stack_t ");

    fprintf (log_file, "(");

    error_descriptor (status, log_file);

    fprintf (log_file, ")");

    fprintf (log_file, " [%p]", stk);

    if (status == STK_EQUALS_NULLPTR)
    {
        fprintf (log_file, "\nPointer on stack equals nullptr, variables are lost\n");
    }
    else
    {
        if ((status == DEAD_CANARY_IN_STACK) || (status == NO_ACCESS_STK))
        {
            fprintf (log_file, "\nPointer on stack might be invalid, variables are lost\n");
        }
        else
        {

            fprintf (log_file, " \"%s\"\n", stk->name_of_stack);

            fprintf (log_file, "{\n");

            fprintf (log_file, "canary1[%p]\n", stk->canary1);

            fprintf (log_file, "size = %d\n", stk->size);

            fprintf (log_file, "capacity = %d\n", stk->capacity);

            fprintf (log_file, "data[%p]\n", stk->data);

            fprintf (log_file, "\t{\n");

            if ((status != DATA_EQUALS_NULLPTR) && (status != INCORRECT_HASH_IN_STACK))
            {
                fprintf (log_file, "\tcanary1[%p]\n", *get_ptr_on_left_canary_in_elem (stk));

                for (size_t i = 0; i < stk->capacity; i++)
                {
                    fprintf (log_file, "\t");

                    if (isnan(stk->data[i]))
                    {
                        fprintf (log_file, " ");
                        fprintf (log_file, "[%u] = ", i);
                        fprintf (log_file, "NAN (Poison!)");
                    }

                    else
                    {
                        fprintf (log_file, "*");
                        fprintf (log_file, "[%u] = ", i);
                        fprintf (log_file, "%lg", stk->data[i]);
                    }

                    fprintf (log_file, "\n");
                }
                fprintf (log_file, "\thash = %d\n", *get_ptr_on_hash_in_elem (stk));

                if (status == INCORRECT_HASH_IN_ELEM)
                {
                    fprintf (log_file, "\t*True hash = %d*\n", count_hash_in_elements (stk));
                }

                fprintf (log_file, "\tcanary2[%p]\n", *get_ptr_on_right_canary_in_elem (stk));
            }
            else
            {
                fprintf (log_file, "\tCapacity or data might be invalid, elements are lost\n");
            }

            fprintf (log_file, "\t}\n");

            fprintf (log_file, "hash = %d\n", stk->hash);

            if (status == INCORRECT_HASH_IN_STACK)
            {
                fprintf (log_file, "*True hash = %d*\n", count_hash_in_Stack_t (stk));
            }

            fprintf (log_file, "canary2[%p]\n", stk->canary2);

            fprintf (log_file, "}\n");
        }
    }

}

Error is_stack_corrupted (Stack_t* stk)
{
    if (stk == nullptr)
    {
        return STK_EQUALS_NULLPTR;
    }

    if (!can_ptr_be_used(stk))
    {
        return NO_ACCESS_STK;
    }

    if ((stk->canary1 != stk) || (stk->canary2 != stk))
    {
        return DEAD_CANARY_IN_STACK;
    }

    if (stk->hash != count_hash_in_Stack_t (stk))
    {
        return INCORRECT_HASH_IN_STACK;
    }

    if (stk->data == nullptr)
    {
        return DATA_EQUALS_NULLPTR;
    }

    if (*get_ptr_on_hash_in_elem (stk) != count_hash_in_elements (stk))
    {
        return INCORRECT_HASH_IN_ELEM;
    }

    if ((*get_ptr_on_right_canary_in_elem (stk) != stk) || (*get_ptr_on_left_canary_in_elem (stk) != stk))
    {
        return DEAD_CANARY_IN_ELEM;
    }

    if (stk->size < 0)
    {
        return NEGATIVE_SIZE;
    }

    if (stk->capacity <= 0)
    {
        return NOT_POSITIVE_CAPACITY;
    }

    if (stk->size > stk->capacity)
    {
        return SIZE_BIGGER_THAN_CAPACITY;
    }

    if ((isnan(stk->data[stk->size-1])) && (stk->size != 0))
    {
        return TOO_BIG_SIZE;
    }

    if ((!(isnan(stk->data[stk->size]))) && (stk->size != stk->capacity))
    {
        return TOO_SMALL_SIZE;
    }

    return No_Error;
}

void error_descriptor (Error status, FILE* log_file)
{
    switch (status)
    {
        case STK_EQUALS_NULLPTR:         fprintf (log_file, "ERROR 1: Pointer on struct equals nullptr");
                                         break;
        case NO_ACCESS_STK:              fprintf (log_file, "ERROR 2: No access to variables on pointer");
                                         break;
        case DEAD_CANARY_IN_STACK:       fprintf (log_file, "ERROR 3: Canary protection worked, your stack is corrupted or not constructed");
                                         break;
        case INCORRECT_HASH_IN_STACK:    fprintf (log_file, "Error 4: Hash counted incorrectly, stack is corrupted");
                                         break;
        case DATA_EQUALS_NULLPTR:        fprintf (log_file, "ERROR 5: Pointer on array equals nullptr");
                                         break;
        case INCORRECT_HASH_IN_ELEM:     fprintf (log_file, "Error 6: Hash counted incorrectly, elements in stack are corrupted");
                                         break;
        case DEAD_CANARY_IN_ELEM:        fprintf (log_file, "Error 7: Canary protection worked, elements in stack are corrupted");
                                         break;
        case NEGATIVE_SIZE:              fprintf (log_file, "ERROR 8: Size of stack is negative");
                                         break;
        case NOT_POSITIVE_CAPACITY:      fprintf (log_file, "ERROR 9: Capacity of stack isn't positive");
                                         break;
        case SIZE_BIGGER_THAN_CAPACITY:  fprintf (log_file, "ERROR 10: Size of stack is bigger than capacity of stack");
                                         break;
        case TOO_BIG_SIZE:               fprintf (log_file, "ERROR 11: Number of elements in stack is lesser than size of stack");
                                         break;
        case TOO_SMALL_SIZE:             fprintf (log_file, "ERROR 12: Number of elements in stack is bigger than size of stack");
                                         break;


        case No_Error: fprintf (log_file, "OK");
                       break;

        default: fprintf (log_file, "Something went wrong");
    }
}

#endif

#undef get_name(var)
#undef check_out(stk)
