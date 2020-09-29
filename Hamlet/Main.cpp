#include "Declarations and documentation.hpp"

//-----------------------------------------------------------------------------

//  MAIN

//-----------------------------------------------------------------------------

int main (int argC, char* argV[])
    {
    assert (argC != 0);
    assert (argV != nullptr);

    for_text* hamlet = (for_text*) calloc (1, sizeof(for_text));

    if (find_string (argC, argV, "--help") > 0)
        {
        help_printf ();
        }
    else
        {
        if (argC > 1)
            {


            char standart_name[18] = "Hamlet.txt";
            hamlet->input = read_name_of_file (argC, argV, in_fopen, "in", standart_name);


            strcpy (standart_name, "Sorted Hamlet.txt");
            hamlet->output = read_name_of_file (argC, argV, out_fopen, "out", standart_name);
            }
        else
            {
            hamlet->input  = in_fopen ();
            hamlet->output = out_fopen ();
            }
        assert (hamlet->input  != nullptr);
        assert (hamlet->output != nullptr);

        hamlet->file_length = find_length_of_file (hamlet->input);
        assert (hamlet->file_length != 0);

        hamlet->pointer_on_buffer = (char*) calloc (hamlet->file_length + 1, sizeof(char));
        assert (hamlet->pointer_on_buffer != nullptr);

        copy_file (hamlet->input, hamlet->pointer_on_buffer, &(hamlet->file_length));

        fclose (hamlet->input);

        hamlet->lines = count_lines (hamlet->pointer_on_buffer, hamlet->file_length);
        assert (hamlet->lines != 0);

        replace_char_from_buffer (hamlet->pointer_on_buffer, hamlet->file_length, '\n', '\0', hamlet->lines);



        hamlet->struct_array = (str*) calloc (hamlet->lines, sizeof(str));
        assert (hamlet->struct_array != nullptr);

        fill_str_array (hamlet->struct_array, hamlet->pointer_on_buffer, hamlet->file_length, &(hamlet->lines));


        if (find_string (argC, argV, "reverse") == 0)
            {
            quick_sort (hamlet->struct_array, hamlet->lines, front_compare);

            fprintf (hamlet->output,"\n\n\nFront Sorted Origin\n\n\n");
            write_result_sorted (hamlet->struct_array, hamlet->lines, hamlet->output);
            }

        if (find_string (argC, argV, "front") == 0)
            {
            qsort (hamlet->struct_array, hamlet->lines, sizeof(str), reverse_compare);

            fprintf (hamlet->output,"\n\n\nReverse Sorted Origin\n\n\n");
            write_result_sorted (hamlet->struct_array, hamlet->lines, hamlet->output);
            }


        if (find_string (argC, argV, "no_origin") == 0)
            {
            fprintf (hamlet->output,"\n\n\nOrigin Itself\n\n\n");
            write_origin (hamlet->pointer_on_buffer, hamlet->file_length, hamlet->output);
            }



        free (hamlet->struct_array);
        free (hamlet->pointer_on_buffer);
        free (hamlet);

        if (find_string (argC, argV, "test") > 0)
            {
            test_me ();
            }
        }
    }




//-----------------------------------------------------------------------------

//  FUNCTIONS

//-----------------------------------------------------------------------------


size_t find_string (int argC, char* argV[], char* str)
    {
    assert (argC != 0);
    assert (str != nullptr);

    if (argC == 1)
        {
        return 0;
        }
    for (size_t cnt = 1; cnt < argC; cnt++)
        {
        if (strcmp (argV[cnt], str) == 0)
            {
            return cnt;
            }
        }
    return 0;
    }


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

    size_t num = find_string (argC, argV, str);
    if (num == 0)
        {
        input = u_fopen (standart_name);
        }
    else
        {
        input = u_fopen (argV[num+1]);
        }


    assert (input != nullptr);
    return input;
    }


//-----------------------------------------------------------------------------


FILE* in_fopen (char* filename)
    {
    assert (filename != nullptr);

    FILE* input = fopen (filename, "r");
    assert (input != nullptr);
    return input;
    }


//-----------------------------------------------------------------------------


FILE* out_fopen (char* filename)
    {
    assert (filename != nullptr);

    FILE* output = fopen (filename, "w");
    assert (output != nullptr);
    return output;
    }


//-----------------------------------------------------------------------------


size_t find_length_of_file (FILE* input)
    {
    assert (input != nullptr);

    fseek (input, 0, SEEK_END);
    int temp = ftell (input);
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

    size_t size = fread (pointer_on_buffer, sizeof(char), *file_length, input);
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


void replace_char_from_buffer (char* pointer, size_t amount, char c_old, char c_new, size_t lines)
    {
    assert (pointer != nullptr);
    assert (amount != 0);

    size_t i = 0;
    for (;amount > 0; amount--, pointer++)
        {
        if (*pointer == c_old)
            {
            *pointer = c_new;
            }
        if (*pointer == c_new)
            {
            i++;
            }
        }
    assert (i != 0);

    if (lines != 0)
        {
        assert (lines == i);
        }
    }


//-----------------------------------------------------------------------------


void fill_str_array (str* struct_array, char* pointer_on_buffer, size_t file_length, size_t* lines)
    {
    assert (struct_array != nullptr);
    assert (pointer_on_buffer != nullptr);
    assert (file_length != 0);
    assert (lines != 0);

    size_t cnt = 0;

    for (size_t size_of_line = 0; file_length > 0; file_length--, pointer_on_buffer++)
        {
        if (*pointer_on_buffer != '\0')
            {
            struct_array[cnt].beginning = pointer_on_buffer;
            }
        for (; *pointer_on_buffer != '\0'; pointer_on_buffer++, size_of_line++, file_length--)
            {
            }
        if (struct_array[cnt].beginning != nullptr)
            {
            struct_array[cnt].length = size_of_line;
            size_of_line = 0;
            cnt++;
            }
        }

    assert (cnt != 0);

    *lines = cnt;
    }


//-----------------------------------------------------------------------------


void quick_sort (str* s_array, int length, int (*cmp)(const void* str1, const void* str2)/*, void (*swaper)(str* s1, str* s2)*/)
    {
    assert (s_array != nullptr);
    assert (length>0);


    int i = 0;
    int j = length-1;
    str mid = s_array[length/2];


    do
        {
        while (cmp(&s_array[i], &mid) < 0)
            {
            i++;
            }
        while (cmp(&s_array[j], &mid) > 0)
            {
            j--;
            }
        if (i <= j)
            {
            swaper (&s_array[i], &s_array[j], sizeof(str));
            i++;
            j--;
            }
        }
    while (i <= j);


    if (i < length)
        {
        quick_sort (s_array + i, length-i, cmp);
        }
    if (0 < j)
        {
        quick_sort (s_array, j+1, cmp);
        }
    }


//-----------------------------------------------------------------------------


int front_compare (const void* str1, const void* str2)
    {
    str* temp1 = (str*) str1;
    str* temp2 = (str*) str2;

    char* left  = nullptr;
    char* right = nullptr;
    char* x = nullptr;
    char* y = nullptr;

    left  = temp1->beginning;
    right = temp2->beginning;
    x = temp1->beginning + temp1->length - 1;
    y = temp2->beginning + temp2->length - 1;

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


int reverse_compare (const void* str1, const void* str2)
    {
    str* temp1 = (str*) str1;
    str* temp2 = (str*) str2;

    char* left  = nullptr;
    char* right = nullptr;
    char* x = nullptr;
    char* y = nullptr;

    left  = temp1->beginning + (temp1->length - 1);
    right = temp2->beginning + (temp2->length - 1);
    x = temp1->beginning;
    y = temp2->beginning;

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


void swaper (void* a1, void* a2, size_t size)
    {
    assert (a1 != nullptr);
    assert (a2 != nullptr);

    void* temp = calloc(1, size);
    memcpy (temp, a1, size);
    memcpy (a1, a2, size);
    memcpy (a2, temp, size);
    free(temp);
    }


//-----------------------------------------------------------------------------


void write_result_sorted (str* str, size_t lines, FILE* output)
    {
    for (size_t temp = 0; temp < lines; temp++)
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

//  Testing functions

//-----------------------------------------------------------------------------


void test_me ()
    {

    for_text* test = (for_text*) calloc (1, sizeof(for_text));

    test->output = fopen ("C:\\Users\\petialetia\\Desktop\\hamlet\\TEST.txt", "w");
    assert (test->output != nullptr);
    fprintf (test->output, "What good is love and peace on Earth?\nWhen it's exclusive\nGreen Day");
    fclose (test->output);


    test->input = fopen ("C:\\Users\\petialetia\\Desktop\\hamlet\\TEST.txt", "r");
    assert (test->input != nullptr);


    test->file_length = find_length_of_file (test->input);
    assert (test->file_length != 0);
    ftest (test->file_length == 69);


    test->pointer_on_buffer = (char*) calloc (test->file_length+1, sizeof(char));
    assert (test->pointer_on_buffer != nullptr);

    copy_file (test->input, test->pointer_on_buffer, &(test->file_length));
    char chars[68] = "What good is love and peace on Earth?\nWhen it's exclusive\nGreen Day";
    ftest ((strcmp (chars, test->pointer_on_buffer) == 0) && (test->file_length == 68));
    fclose (test->input);


    test->lines = count_lines (test->pointer_on_buffer, test->file_length);
    assert (test->lines != 0);
    ftest (test->lines == 3);


    replace_char_from_buffer (test->pointer_on_buffer, test->file_length, '\n', '\0', test->lines);

    size_t temp = 0,
           length = test->file_length;
    for (char* pointer = test->pointer_on_buffer;length > 0; pointer++, length--)
        {
        if (*pointer == '\n')
            {
            temp++;
            }
        }
    ftest (temp == 0);


    test->struct_array = (str*) calloc (test->lines, sizeof(str));
    assert (test->struct_array != nullptr);

    fill_str_array (test->struct_array, test->pointer_on_buffer, test->file_length, &(test->lines));
    char char1[38] = "What good is love and peace on Earth?";
    char char2[20] = "When it's exclusive";
    char char3[10] = "Green Day";
    ftest ((test->lines == 3) && (strcmp (char1, test->struct_array[0].beginning) == 0) && (strcmp (char2, test->struct_array[1].beginning) == 0) && (strcmp (char3, test->struct_array[2].beginning) == 0));

    quick_sort (test->struct_array, test->lines, front_compare);
    ftest ((strcmp (char3, test->struct_array[0].beginning) == 0) && (strcmp (char1, test->struct_array[1].beginning) == 0) && (strcmp (char2, test->struct_array[2].beginning) == 0));


    qsort (test->struct_array, test->lines, sizeof(str), reverse_compare);
    ftest ((strcmp (char2, test->struct_array[0].beginning) == 0) && (strcmp (char1, test->struct_array[1].beginning) == 0) && (strcmp (char3, test->struct_array[2].beginning) == 0));


    free (test->pointer_on_buffer);
    free (test->struct_array);
    free (test);
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
