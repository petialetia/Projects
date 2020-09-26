#include "Declarations and documentation.hpp"

//-----------------------------------------------------------------------------

//  MAIN

//-----------------------------------------------------------------------------

int main (int argC, char* argV[])
    {
    FILE* input  = nullptr;
    FILE* output = nullptr;

    if (argC > 1)
        {

        for (int temp = 1; temp < argC; temp++)
            {
            if (temp == argC - 1)
                {
                input = in_fopen();
                }
            if (strcmp(argV[temp], "in") == 0)
                {
                input = in_fopen (argV[temp+1]);
                break;
                }
            }
        for (int temp = 1; temp < argC; temp++)
            {
            if (temp == argC - 1)
                {
                output = out_fopen();
                }
            if (strcmp(argV[temp], "out") == 0)
                {
                output = out_fopen (argV[temp+1]);
                break;
                }
            }
        }
    else
        {
        input  = in_fopen();
        output = out_fopen();
        }
    assert (input  != nullptr);
    assert (output != nullptr);

    size_t amount = measure_file (input);
    char* pointer = (char*) calloc (amount+1, sizeof(char));
    assert (pointer != nullptr);
    copy_file (input, pointer, &amount);

    fclose (input);

    size_t lines = count_lines (pointer, amount);
    size_t x = replace_enter (pointer, amount);
    assert (x == lines);



    str* struct_array = (str*) calloc (lines, sizeof(str));
    assert (struct_array != nullptr);

    fill_struct_str_array (struct_array, pointer, amount, &lines);

    str* reverse_array = (str*) calloc (lines, sizeof (str));
    assert (reverse_array != nullptr);
    copy_struct_array (lines, struct_array, reverse_array);



    quick_sort (struct_array,  lines, front_compare,   swaper);
    quick_sort (reverse_array, lines, reverse_compare, swaper);



    write_result (pointer, amount, struct_array, reverse_array, lines, output);


    free (struct_array);
    free (reverse_array);
    free (pointer);

//-----------------------------------------------------------------------------

//  Tests

//-----------------------------------------------------------------------------

    if (argC > 1)
        {
        for (int temp = 0; temp < argC; temp ++)
            {
            if (strcmp(argV[temp], "test") == 0)
                {

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
                size_t a = measure_file (fite);
                ftest (a == 69);

                char* p = (char*) calloc (a+1, sizeof(char));
                assert (p != nullptr);
                copy_file (fite, p, &a);
                char chars[68] = "What good is love and peace on Earth?\nWhen it's exclusive\nGreen Day";
                ftest ((strcmp (chars, p) == 0) && (a == 68));
                fclose (fite);

                size_t l = count_lines (p, a);
                ftest (l == 3);

                size_t y = replace_enter (p, a);
                assert (y == l);
                size_t g = 0,
                       am = a;
                for (char* po = p; am>0; po++, am--)
                    {
                    if (*po == '\n')
                        {
                        g++;
                        }
                    }
                ftest (g == 0);

                str* straus = (str*) calloc (l, sizeof(str));
                assert (straus != nullptr);
                fill_struct_str_array (straus, p, a, &l);
                char char1[38] = "What good is love and peace on Earth?";
                char char2[20] = "When it's exclusive";
                char char3[10] = "Green Day";
                ftest ((l == 3) && (strcmp (char1, straus[0].beginning) == 0) && (strcmp (char2, straus[1].beginning) == 0) && (strcmp (char3, straus[2].beginning) == 0));

                str* revaus = (str*) calloc (l, sizeof (str));
                assert (revaus != nullptr);
                copy_struct_array (l, straus, revaus);
                ftest ((strcmp (revaus[0].beginning, straus[0].beginning) == 0) && (strcmp (revaus[1].beginning, straus[1].beginning) == 0) && (strcmp (revaus[2].beginning, straus[2].beginning) == 0) && (revaus[0].length == straus[0].length) && (revaus[1].length == straus[1].length) && (revaus[2].length == straus[2].length));

                quick_sort (straus, l, front_compare, swaper);
                ftest ((strcmp (char3, straus[0].beginning) == 0) && (strcmp (char1, straus[1].beginning) == 0) && (strcmp (char2, straus[2].beginning) == 0));

                quick_sort (revaus, l, reverse_compare, swaper);
                ftest ((strcmp (char2, revaus[0].beginning) == 0) && (strcmp (char1, revaus[1].beginning) == 0) && (strcmp (char3, revaus[2].beginning) == 0));

                free (p);
                free (straus);
                free (revaus);
                break;
                }
            }
        }
    }

//-----------------------------------------------------------------------------

//  END OF TESTS

//-----------------------------------------------------------------------------



//-----------------------------------------------------------------------------

//  FUNCTIONS

//-----------------------------------------------------------------------------


FILE* in_fopen (char* filename)
    {
    FILE* input = fopen(filename, "r");
    return input;
    }


//-----------------------------------------------------------------------------


FILE* out_fopen (char* filename)
    {
    FILE* output = fopen(filename, "w");
    return output;
    }


//-----------------------------------------------------------------------------


size_t measure_file (FILE* input)
    {
    fseek (input, 0, SEEK_END);
    int temp = ftell(input);
    assert (temp != 0);

    fseek (input, 0, SEEK_SET);
    return temp;
    }


//-----------------------------------------------------------------------------


void copy_file (FILE* input, char* pointer, size_t* amount)
    {
    size_t size = fread(pointer, sizeof(char), *amount, input);
    assert (size != 0);

    *amount = size+1;
    }


//-----------------------------------------------------------------------------


size_t count_lines (char* pointer, size_t amount)
    {
    size_t i = 0;

    for (;amount > 0; amount--, pointer++)
        {
        if (*pointer == 10)
            {
            i++;
            }
        }

    assert ((i + 1) != 0);
    return i + 1;
    }


//-----------------------------------------------------------------------------

size_t replace_enter (char* pointer, size_t amount)
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


void fill_struct_str_array (str* struct_array, char* pointer, size_t amount, size_t* lines)
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


void copy_struct_array (size_t lines, str* struct_array, str* reverse_array)
    {
    for (size_t i = 0; i < lines; i++)
        {
        reverse_array[i].beginning = struct_array[i].beginning;
        reverse_array[i].length = struct_array[i].length;
        }
    }


//-----------------------------------------------------------------------------


void quick_sort (str* s, int last, int (*cmp)(str str1, str str2), void (*swaper)(str* s1, str* s2))
    {
    assert (last>0);
    assert (s != nullptr);


    int i = 0;
    int j = last-1;
    str mid = s[last/2];


    do
        {
        while (cmp(s[i], mid) < 0)
            {
            i++;
            }
        while (cmp(s[j], mid) > 0)
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
        quick_sort (s + i, last-i, cmp, swaper);
        }
    if (0 < j)
        {
        quick_sort (s, j+1, cmp, swaper);
        }
    }


//-----------------------------------------------------------------------------


int front_compare (str str1, str str2)
    {
    str temp1 = str1;
    str temp2 = str2;

    char* left  = nullptr;
    char* right = nullptr;
    char* x = nullptr;
    char* y = nullptr;

    left  = temp1.beginning;
    right = temp2.beginning;
    x = temp1.beginning + temp1.length - 1;
    y = temp2.beginning + temp2.length - 1;

    for (;;)
        {
            while ((!isalpha(*left))  && (left != x))
                {
                left++;
                }
            while ((!isalpha(*right)) && (right != y))
                {
                right++;
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


        left++;
        right++;
        }
    }


//-----------------------------------------------------------------------------


int reverse_compare (str str1, str str2)
    {
    str temp1 = str1;
    str temp2 = str2;

    char* left  = nullptr;
    char* right = nullptr;
    char* x = nullptr;
    char* y = nullptr;

    left  = temp1.beginning + (temp1.length - 1);
    right = temp2.beginning + (temp2.length - 1);
    x = temp1.beginning;
    y = temp2.beginning;

    for (;;)
        {
            while ((!isalpha(*left))  && (left != x))
                {
                left --;
                }
            while ((!isalpha(*right)) && (right != y))
                {
                right --;
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

        left  --;
        right --;
        }
    }


//-----------------------------------------------------------------------------


void swaper (str* s1, str* s2)
    {
    str* left  =  s1;
    str* right =  s2;
    str* temp = (str*) calloc (1, sizeof (str));


    (*temp). beginning = (*left). beginning;
    (*temp). length    = (*left). length;

    (*left). beginning = (*right).beginning;
    (*left). length    = (*right).length;

    (*right).beginning = (*temp). beginning;
    (*right).length    = (*temp). length;


    free (temp);
    }


//-----------------------------------------------------------------------------


void write_result (char* pointer, size_t amount, str* struct_array, str* reverse_array, size_t lines, FILE* output)
    {
    fprintf (output,"\n\n\nFrontsorted Origin\n\n\n");
    for (size_t temp = 0;temp <lines;temp++)
        {
        fprintf (output, "%s\n", struct_array[temp].beginning);
        }


    fprintf (output,"\n\n\nReversesorted Origin\n\n\n");
    for (size_t temp = 0;temp <lines;temp++)
        {
        fprintf (output, "%s\n", reverse_array[temp].beginning);
        }


    fprintf (output,"\n\n\nOrigin Itself\n\n\n");
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
    }

//-----------------------------------------------------------------------------


void ftest (bool test)
    {
    static size_t numero = 0;
    if (test) {testok} else {pressf};
    #undef testok
    #undef pressf
    }


//-----------------------------------------------------------------------------


//  THE END

//-----------------------------------------------------------------------------
