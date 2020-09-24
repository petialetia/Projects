#include <stdio.h>
#include <assert.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>

//My Heroes:
//Elya
//Vasya
//Nikita
//Sergey

struct str
{
    char*  beginning = nullptr;
    size_t length = 0;
};

/*!
Measures the size of the origin, returns it
\param[in] input    Pointer to origin
*/

size_t measure_origin (FILE* input);

/*!
Copies the origin into buffer, returns pointer to the beginning of buffer
\param[in] input     Pointer to origin
\param[in] amount    Size of origin
*/

char* copy_poem (FILE* input, size_t* amount);

/*!
Counts number of lines, replaces '\n' to '\0', returns number of lines
\param[in]     pointer    Pointer to buffer with origin
\param[in,out] amount     Size of origin
*/

size_t count_lines (char* pointer, size_t amount);

/*!
Fills buffer of structs with pointers to beginnings of the lines and sizes of lines
\param[in]     struct_array    Pointer to beginning of buffer of structs
\param[in]     pointer         Pointer to buffer with origin
\param[in]     amount          Size of origin
\param[in,out] lines           Number of lines
*/

void fill_struct_array (str* struct_array, char* pointer, size_t amount, size_t* lines);

/*!
Creates same buffer with structs for revers sorting, returns pointer to it
\param[in] lines           Number of lines
\param[in] struct_array    Pointer to beginning of buffer of structs
*/

str* copy_struct_array (size_t lines, str* struct_array);

/*!
Quick sorts array of structs
\param[in] s         Pointer to the beginning of buffer with structs
\param[in] last      Number of elements in the buffer
\param[in] step      Step for the comparator
\param[in] cmp       Comparator
\param[in] swaper    Function to swap structs
*/

void quick_sort (str* s, int last, int step, int (*cmp)(str str1, str str2, int flag), void (*swaper)(str* s1, str* s2));

/*!
Universal comparator for structs
\param[in] str1    Pointer to first struct
\param[in] str2    Pointer to second struct
\param[in] step    Step of coming throw the symbols, 1 - front coming, -1 - reverse coming
*/

int unicomp (str str1, str str2, int step);

/*!
Function to swap structs
\param[in] s1    Pointer to first struct
\param[in] s2    Pointer to second struct
*/

void swaper (str* s1, str* s2);

/*!
Writes results of sorting, frees memory from the buffers
\param[in] pointer          Pointer to buffer with origin
\param[in] amount           Size of origin
\param[in] struct_array     Pointer to the beginning of buffer with structs
\param[in] reverse_array    Pointer to the buffer with structs for revers sorting
\param[in] lines            Number of lines
\param[in] output           Pointer to output file
*/

void write_result (char* ponter, size_t amount, str* struct_array, str* reverse_array, size_t lines, FILE* output);

//-----------------------------------------------------------------------------

//  MAIN

//-----------------------------------------------------------------------------

int main ()
    {
    FILE* input  = fopen("C:\\Users\\petialetia\\Desktop\\hamlet\\hamlet.txt",        "r");
    FILE* output = fopen("C:\\Users\\petialetia\\Desktop\\hamlet\\Sorted Hamlet.txt", "w");
    assert (input  != 0);
    assert (output != 0);

    size_t amount = measure_origin (input);
    char* pointer = copy_poem (input, &amount);

    fclose (input);

    size_t lines = count_lines (pointer, amount);



    str* struct_array = (str*) calloc (lines, sizeof(str));
    assert (struct_array != 0);

    fill_struct_array (struct_array, pointer, amount, &lines);

    str* reverse_array = copy_struct_array (lines, struct_array);

    quick_sort (struct_array, lines, 1 , unicomp, swaper);
    quick_sort (reverse_array, lines, -1, unicomp, swaper);

    write_result (pointer, amount, struct_array, reverse_array, lines, output);

//-----------------------------------------------------------------------------

//  Tests

//-----------------------------------------------------------------------------

    size_t numero = 0;
    #define testok                         \
        numero++;                          \
        printf ("TEST #%u is ok\n", numero);

    #define pressf                             \
        numero++;                              \
        printf ("TEST #%u is failed\n", numero);


    FILE* tefi = fopen("C:\\Users\\petialetia\\Desktop\\hamlet\\TEST.txt", "w");
    fprintf (tefi, "What good is love and peace on Earth?\nWhen it's exclusive\nGreen Day");
    fclose (tefi);
    FILE* fite = fopen("C:\\Users\\petialetia\\Desktop\\hamlet\\TEST.txt", "r");
    size_t a = measure_origin (fite);
    if (a == 69)
        {
        testok
        }
    else
        {
        pressf
        }
    char* p = copy_poem (fite, &a);
    if (a == 68)
        {
        testok
        }
    else
        {
        pressf
        }
    char chars[68] = "What good is love and peace on Earth?\nWhen it's exclusive\nGreen Day";
    if (strcmp (chars, p) == 0)
        {
        testok
        }
    else
        {
        pressf
        }
    fclose (fite);
    size_t l = count_lines (p, a);
    if (l == 3)
        {
        testok
        }
    else
        {
        pressf
        }
    str* straus = (str*) calloc (l, sizeof(str));
    assert (straus != 0);

    fill_struct_array (straus, p, a, &l);
    if (l == 3)
        {
        testok
        }
    else
        {
        pressf
        }
    char char1[38] = "What good is love and peace on Earth?";
    if (strcmp (char1, straus[0].beginning) == 0)
        {
        testok
        }
    else
        {
        pressf
        }
    char char2[20] = "When it's exclusive";
    if (strcmp (char2, straus[1].beginning) == 0)
        {
        testok
        }
    else
        {
        pressf
        }
    char char3[10] = "Green Day";
    if (strcmp (char3, straus[2].beginning) == 0)
        {
        testok
        }
    else
        {
        pressf
        }
    str* revaus = copy_struct_array (l, straus);
    for (size_t i = 0; i < l; i++)
        {
        if (strcmp (revaus[i].beginning, straus[i].beginning) == 0)
            {
            testok
            }
        else
            {
            pressf
            }
        }
    quick_sort (straus, l, 1 , unicomp, swaper);
    if (strcmp (char3, straus[0].beginning) == 0)
        {
        testok
        }
    else
        {
        pressf
        }
    if (strcmp (char1, straus[1].beginning) == 0)
        {
        testok
        }
    else
        {
        pressf
        }
    if (strcmp (char2, straus[2].beginning) == 0)
        {
        testok
        }
    else
        {
        pressf
        }
    quick_sort (revaus, l, -1 , unicomp, swaper);
    if (strcmp (char2, revaus[0].beginning) == 0)
        {
        testok
        }
    else
        {
        pressf
        }
    if (strcmp (char1, revaus[1].beginning) == 0)
        {
        testok
        }
    else
        {
        pressf
        }
    if (strcmp (char3, revaus[2].beginning) == 0)
        {
        testok
        }
    else
        {
        pressf
        }

    free (p);
    free (straus);
    free (revaus);

//-----------------------------------------------------------------------------

//  END OF TESTS

//-----------------------------------------------------------------------------

    }

//-----------------------------------------------------------------------------

//  FUNCTIONS

//-----------------------------------------------------------------------------

size_t measure_origin (FILE* input)
    {
    fseek (input, 0, SEEK_END);
    int temp = ftell(input);
    assert (temp != 0);
    fseek (input, 0, SEEK_SET);
    return temp;
    }


//-----------------------------------------------------------------------------


char* copy_poem (FILE* input, size_t* amount)
    {
    char* pointer = (char*) calloc (*amount+1, sizeof(char));
    assert (pointer != 0);
    size_t size = fread(pointer, sizeof(char), *amount, input);
    assert (size != 0);
    *amount = size+1;
    return pointer;
    }


//-----------------------------------------------------------------------------


size_t count_lines (char* pointer, size_t amount)
    {
    size_t i = 0;
    for (;amount > 0; amount--, pointer++)
        {
        if (*pointer == 10)
            {
            *pointer = '\0';
            }
        if (*pointer == '\0')
            {
            i++;
            }
        }
    assert (i != 0);
    return i;
    }


//-----------------------------------------------------------------------------


void fill_struct_array (str* struct_array, char* pointer, size_t amount, size_t* lines)
    {
    size_t cnt = 0;
    size_t temp = 0;
    for (bool flag = 1; amount > 0; amount--, pointer++)
        {
        if ((flag == 1) && (*pointer != 0))
            {
            flag = 0;
            temp = 0;
            struct_array[cnt].beginning = pointer;
            }
        else if ((*(pointer) == '\0') && (flag == 0))
                {
                flag = 1;
                struct_array[cnt].length = temp;
                cnt++;
                assert (cnt <= *lines);
                }
        if (flag == 0)
            {
            temp ++;
            }
        }
    *lines = cnt;
    }


//-----------------------------------------------------------------------------


str* copy_struct_array (size_t lines, str* struct_array)
    {
    str* s = (str*) calloc (lines, sizeof (str));
    assert (s != 0);
    for (size_t i = 0; i < lines; i++)
        {
        s[i].beginning = struct_array[i].beginning;
        s[i].length = struct_array[i].length;
        }
    return s;
    }


//-----------------------------------------------------------------------------


void quick_sort (str* s, int last, int flag, int (*cmp)(str str1, str str2, int flag), void (*swaper)(str* s1, str* s2))
    {
    assert (last>0);
    assert (s);
    int i = 0;
    int j = last-1;
    str mid = s[last/2];


    do
        {
        while (cmp(s[i], mid, flag) < 0)
            {
            i++;
            }
        while (cmp(s[j], mid, flag) > 0)
            {
            j--;
            }
        if (i <= j)
            {
            swaper (&s[i], &s[j]);
            i++;
            j--;
            }
        }
    while (i <= j);


    if (i < last)
        {
        quick_sort (s + i, last-i, flag, cmp, swaper);
        }
    if (0 < j)
        {
        quick_sort (s, j+1, flag, cmp, swaper);
        }
    }


//-----------------------------------------------------------------------------


int unicomp (str str1, str str2, int flag)
    {
    str temp1 = str1;
    str temp2 = str2;
    char* left  = nullptr;
    char* right = nullptr;
    char* x = nullptr;
    char* y = nullptr;
    if (flag == -1)
        {
        left  = temp1.beginning + (temp1.length - 1);
        right = temp2.beginning + (temp2.length - 1);
        x = temp1.beginning;
        y = temp2.beginning;
        }
    else if (flag == 1)
            {
            left  = temp1.beginning;
            right = temp2.beginning;
            x = temp1.beginning + temp1.length - 1;
            y = temp2.beginning + temp2.length - 1;
            }
    for (;;)
        {
            while ((!isalpha(*left)) && (left != x))
                {
                left += flag;
                }
            while ((!isalpha(*right)) && (right != y))
                {
                right += flag;
                }
        if (((left == x) && (right == y)) || (tolower(left[0]) != tolower(right[0])))
            {
            return tolower(left[0]) - tolower(right[0]);
            }
        else if (left == x)
                {
                return -1;
                }
             else if (right == y)
                     {
                     return 1;
                     }
        left  += flag;
        right += flag;
        }
    }


//-----------------------------------------------------------------------------


void swaper (str* s1, str* s2)
    {
    str* left  =  s1;
    str* right =  s2;
    str* temp = (str*) calloc (1, sizeof (str));
    (*temp). beginning = (*left). beginning;
    (*temp). length   = (*left). length;
    (*left). beginning = (*right).beginning;
    (*left). length   = (*right).length;
    (*right).beginning = (*temp). beginning;
    (*right).length   = (*temp). length;
    free (temp);
    }


//-----------------------------------------------------------------------------


void write_result (char* pointer, size_t amount, str* struct_array, str* reverse_array, size_t lines, FILE* output)
    {
    fprintf (output,"\n\n\nFrontsorted Hamlet\n\n\n");
    for (size_t temp = 0;temp <lines;temp++)
        {
        fprintf (output, "%s\n", struct_array[temp].beginning);
        }
    free (struct_array);
    fprintf (output,"\n\n\nReversesorted Hamlet\n\n\n");
    for (size_t temp = 0;temp <lines;temp++)
        {
        fprintf (output, "%s\n", reverse_array[temp].beginning);
        }
    free (reverse_array);
    fprintf (output,"\n\n\nOriginal Hamlet, literaturavedi uspakoites\n\n\n");
    for (size_t i = 0; i < amount-1; i++)
        {
        if (pointer[i] == 0)
            {
            fprintf (output, "\n");
            }
        else
            {
            fprintf (output, "%c", pointer[i]);
            }
        }
    free (pointer);
    }

//-----------------------------------------------------------------------------

//  THE END

//-----------------------------------------------------------------------------
