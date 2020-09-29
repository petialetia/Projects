#include "Declarations and documentation.hpp"

//-----------------------------------------------------------------------------

//  MAIN

//-----------------------------------------------------------------------------

int main (int argC, char* argV[])
    {

    assert (argC != 0);
    assert (argV != nullptr);

    if (find_string(argC, argV, "--help") == 1)
        {
        help_printf();
        }
    else
        {
        FILE* input  = nullptr;
        FILE* output = nullptr;
        if (argC > 1)
            {
            char in [3] = "in" ;
            char out[4] = "out";


            char standart_name[18] = "Hamlet.txt";
            input  = read_name_of_file (argC, argV, in_fopen,  in,  standart_name);


            strcpy (standart_name,"Sorted Hamlet.txt");
            output = read_name_of_file (argC, argV, out_fopen, out, standart_name);
            }
        else
            {
            input  = in_fopen();
            output = out_fopen();
            }
        assert (input  != nullptr);
        assert (output != nullptr);

        size_t file_length = find_length_of_file (input);
        assert (file_length != 0);

        char* pointer_on_buffer = (char*) calloc (file_length+1, sizeof(char));
        assert (pointer_on_buffer != nullptr);

        copy_file (input, pointer_on_buffer, &file_length);

        fclose (input);

        size_t lines = count_lines (pointer_on_buffer, file_length);
        assert (lines != 0);

        size_t deleted_enters = replace_enter (pointer_on_buffer, file_length);
        assert (deleted_enters != 0);
        assert (deleted_enters == lines);



        str* struct_array = (str*) calloc (lines, sizeof(str));
        assert (struct_array != nullptr);

        fill_struct_str_array (struct_array, pointer_on_buffer, file_length, &lines);


        if (find_string(argC, argV, "reverse") == 0)
            {
            quick_sort (struct_array, lines, front_compare,   swaper);

            fprintf (output,"\n\n\nFront Sorted Origin\n\n\n");
            write_result_sorted (struct_array, lines, output);
            }

        if (find_string(argC, argV, "front") == 0)
            {
            quick_sort (struct_array, lines, reverse_compare, swaper);

            fprintf (output,"\n\n\nReverse Sorted Origin\n\n\n");
            write_result_sorted (struct_array, lines, output);
            }


        if (find_string(argC, argV, "no_origin") == 0)
            {
            fprintf (output,"\n\n\nOrigin Itself\n\n\n");
            write_origin (pointer_on_buffer, file_length, output);
            }



        free (struct_array);
        free (pointer_on_buffer);



//-----------------------------------------------------------------------------

//  Tests

//-----------------------------------------------------------------------------


        if (find_string(argC, argV, "test") == 1)
            {
            FILE* tefi = fopen("C:\\Users\\petialetia\\Desktop\\hamlet\\TEST.txt", "w");
            assert (tefi != nullptr);
            fprintf (tefi, "What good is love and peace on Earth?\nWhen it's exclusive\nGreen Day");
            fclose (tefi);


            FILE* fite = fopen("C:\\Users\\petialetia\\Desktop\\hamlet\\TEST.txt", "r");
            assert (fite != nullptr);


            size_t a = find_length_of_file (fite);
            assert (a != 0);
            ftest (a == 69);


            char* p = (char*) calloc (a+1, sizeof(char));
            assert (p != nullptr);

            copy_file (fite, p, &a);
            char chars[68] = "What good is love and peace on Earth?\nWhen it's exclusive\nGreen Day";
            ftest ((strcmp (chars, p) == 0) && (a == 68));
            fclose (fite);


            size_t l = count_lines (p, a);
            assert (l != 0);
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

            quick_sort (straus, l, front_compare, swaper);
            ftest ((strcmp (char3, straus[0].beginning) == 0) && (strcmp (char1, straus[1].beginning) == 0) && (strcmp (char2, straus[2].beginning) == 0));


            quick_sort (straus, l, reverse_compare, swaper);
            ftest ((strcmp (char2, straus[0].beginning) == 0) && (strcmp (char1, straus[1].beginning) == 0) && (strcmp (char3, straus[2].beginning) == 0));


            free (p);
            free (straus);
            }
        }
    }


//-----------------------------------------------------------------------------

//  END OF TESTS

//-----------------------------------------------------------------------------



//-----------------------------------------------------------------------------

//  FUNCTIONS

//-----------------------------------------------------------------------------


void help_printf()
    {
    printf ("Program was created for sorting files with text\n");
    printf ("There are 2 ways of sorting: front and reverse\n");
    printf ("You can specify the following parameters\n");
    printf ("1) Write \"in <name of file>\" to tell program name of file it needs to read\n");
    printf ("\tBy default it's Hamlet.txt\n");
    printf ("2) Write \"out <name of file>\" to tell program name of file it needs to write in results of sorting\n");
    printf ("\tBy default it's Sorted Hamlet.txt\n");
    printf ("3) Write \"front\" if you want ONLY front sorted origin\n");
    printf ("4) Write \"reverse\" if you want ONLY reverse sorted origin\n");
    printf ("5) Write \"no_origin\" if you want the program not to copy origin\n");
    printf ("6) Write \"test\" for unittests to work\n");
    }


//-----------------------------------------------------------------------------


FILE* read_name_of_file (int argC, char* argV[], FILE* (*u_fopen)(char* filename), char* str, char* standart_name)
    {
    assert (argC != 0);
    assert (argV != nullptr);
    assert (str != nullptr);
    assert (standart_name != nullptr);

    FILE* input = nullptr;
    for (int temp = 1; temp < argC; temp++)
        {
        if (temp == argC - 1)
            {
            input = u_fopen(standart_name);
            assert (input != nullptr);
            return input;
            }
        if (strcmp(argV[temp], str) == 0)
            {
            input = u_fopen (argV[temp+1]);
            assert (input != nullptr);
            return input;
            }
        }
    return nullptr;
    }


//-----------------------------------------------------------------------------


FILE* in_fopen (char* filename)
    {
    assert (filename != nullptr);

    FILE* input = fopen(filename, "r");
    assert (input != nullptr);
    return input;
    }


//-----------------------------------------------------------------------------


FILE* out_fopen (char* filename)
    {
    assert (filename != nullptr);

    FILE* output = fopen(filename, "w");
    assert (output != nullptr);
    return output;
    }


//-----------------------------------------------------------------------------


size_t find_length_of_file (FILE* input)
    {
    assert (input != nullptr);

    fseek (input, 0, SEEK_END);
    int temp = ftell(input);
    assert (temp != 0);

    fseek (input, 0, SEEK_SET);
    return temp;
    }


//-----------------------------------------------------------------------------


void copy_file (FILE* input, char* pointer_on_buffer, size_t* file_length)
    {
    assert (input != nullptr);
    assert (pointer_on_buffer != nullptr);
    assert (file_length != 0);

    size_t size = fread(pointer_on_buffer, sizeof(char), *file_length, input);
    assert (size != 0);

    *file_length = size+1;
    }


//-----------------------------------------------------------------------------


size_t count_lines (char* pointer, size_t amount)
    {
    assert (pointer != nullptr);
    assert (amount != 0);

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
    assert (pointer != nullptr);
    assert (amount != 0);

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


void fill_struct_str_array (str* struct_array, char* pointer_on_buffer, size_t file_length, size_t* lines)
    {
    assert (struct_array != nullptr);
    assert (pointer_on_buffer != nullptr);
    assert (file_length != 0);
    assert (lines != 0);

    size_t cnt = 0;

    for (size_t temp = 0; file_length > 0; file_length--, pointer_on_buffer++)
        {
        if (*pointer_on_buffer != '\0')
            {
            struct_array[cnt].beginning = pointer_on_buffer;
            }
        for (; *pointer_on_buffer != '\0'; pointer_on_buffer++, temp++, file_length--)
            {
            }
        if (struct_array[cnt].beginning != nullptr)
            {
            struct_array[cnt].length = temp;
            temp = 0;
            cnt++;
            }
        }

    assert (cnt != 0);

    *lines = cnt;
    }


//-----------------------------------------------------------------------------


bool find_string (int argC, char* argV[], char* str)
    {
    assert (argC != 0);
    assert (str != nullptr);

    if (argC == 1)
        {
        return false;
        }
    for (int temp = 1; temp < argC; temp ++)
        {
        if (strcmp(argV[temp], str) == 0)
            {
            return true;
            }
        }
    return false;
    }

//-----------------------------------------------------------------------------


void quick_sort (str* s_array, int length, int (*cmp)(str str1, str str2), void (*swaper)(str* s1, str* s2))
    {
    assert (s_array != nullptr);
    assert (length>0);


    int i = 0;
    int j = length-1;
    str mid = s_array[length/2];


    do
        {
        while (cmp(s_array[i], mid) < 0)
            {
            i++;
            }
        while (cmp(s_array[j], mid) > 0)
            {
            j--;
            }
        if (i <= j)
            {
            swaper (&s_array[i], &s_array[j]);
            i++;
            j--;
            }
        }
    while (i <= j);


    if (i < length)
        {
        quick_sort (s_array + i, length-i, cmp, swaper);
        }
    if (0 < j)
        {
        quick_sort (s_array, j+1, cmp, swaper);
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
    assert (s1 != nullptr);
    assert (s2 != nullptr);

    str* left  =  s1;
    str* right =  s2;
    str* temp = (str*) calloc (1, sizeof (str));


    temp->beginning = left->beginning;
    temp->length    = left->length;

    left->beginning = right->beginning;
    left->length    = right->length;

    right->beginning = temp->beginning;
    right->length    = temp->length;


    free (temp);
    }


//-----------------------------------------------------------------------------


void write_result_sorted (str* str, size_t lines, FILE* output)
    {
    for (size_t temp = 0;temp <lines;temp++)
        {
        fprintf (output, "%s\n", str[temp].beginning);
        }
    }


//-----------------------------------------------------------------------------


void write_origin (char* pointer_on_buffer, size_t file_length, FILE* output)
    {
    for (size_t temp = 0; temp < file_length; temp++)
        {
        if (temp == 0)
            {
            fprintf (output, "%s\n", pointer_on_buffer);
            }
        else
            {
            while (pointer_on_buffer[temp] != '\0')
                {
                temp++;
                }
            fprintf (output, "%s\n", pointer_on_buffer+temp+1);
            }
        }
    }


//-----------------------------------------------------------------------------


void ftest (bool test)
    {
    static size_t numero = 0;
    if (test)
        {
        numero++;
        printf ("TEST #%u is ok\n", numero);
        }
    else
        {
        numero++;
        printf ("TEST #%u is failed\n", numero);
        }
    }


//-----------------------------------------------------------------------------


//  THE END

//-----------------------------------------------------------------------------
