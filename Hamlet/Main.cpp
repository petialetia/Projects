#include "Declarations and documentation.hpp"

//-----------------------------------------------------------------------------

//  MAIN

//-----------------------------------------------------------------------------

int main (int argC, char* argV[])
    {

    assert (argC != 0);
    assert (argV != nullptr);


    for_text* hamlet = construct_for_text ();
    assert (hamlet != nullptr);

    keys* hamlet_keys = (keys*) calloc (1, sizeof(keys));
    assert (hamlet_keys != nullptr);

    hamlet_keys->help = get_arg_val (argC, argV, "-help");

    if ((hamlet_keys->help != 0) && (strcmp (argV[hamlet_keys->help], "1") == 0))
        {
        help_printf ();
        }
    else
        {
        read_name_of_files (argC, argV, hamlet, hamlet_keys);

        process_files (argC, argV, hamlet, hamlet_keys);

        sort_and_write (argC, argV, hamlet, hamlet_keys);

        free_memory (hamlet);

        destructor_for_text (hamlet);

        hamlet_keys->test = get_arg_val (argC, argV, "-test");

        if (strcmp (argV[hamlet_keys->test], "1") == 0)
            {
            test_me ();
            }
        }

    free (hamlet_keys);
    }


//-----------------------------------------------------------------------------

//  FUNCTIONS

//-----------------------------------------------------------------------------


for_text* construct_for_text ()
    {
    for_text* pointer = (for_text*) calloc (1, sizeof(for_text));
    assert (pointer != nullptr);
    return pointer;
    }


//-----------------------------------------------------------------------------


size_t get_arg_val (int argC, char* argV[], char* key)
    {
    assert (argC != 0);
    assert (key != nullptr);


    for (size_t cnt = 1; cnt < argC; cnt++)
        {
        if (strcmp (argV[cnt], key) == 0)
            {
            return cnt + 1;
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
    printf ("1) Write \"-in <name of file>\" to tell program name of file it needs to read\n");
    printf ("\tBy default it's Hamlet.txt\n");
    printf ("2) Write \"-out <name of file>\" to tell program name of file it needs to write in results of sorting\n");
    printf ("\tBy default it's Sorted Hamlet.txt\n");
    printf ("3) Write \"-front 0\" if you don't want front sorted origin\n");
    printf ("4) Write \"-reverse 0\" if you don't want reverse sorted origin\n");
    printf ("5) Write \"-origin 0\" if you don't want the program to copy origin\n");
    printf ("6) Write \"-test 1\" for unittests to work\n");
    }


//-----------------------------------------------------------------------------


void read_name_of_files (int argC, char* argV[], for_text* hamlet, keys* hamlet_keys)
    {
    hamlet_keys->in = get_arg_val (argC, argV, "-in");
    char standart_name_in[] = "Hamlet.txt";
    hamlet->input = (hamlet_keys->in != 0) ? (fopen (argV[hamlet_keys->in], "r")) : (fopen (standart_name_in, "r"));

    hamlet_keys->out = get_arg_val (argC, argV, "-out");
    char standart_name_out[] = "Sorted Hamlet.txt";

    hamlet->output = (hamlet_keys->out != 0) ? (fopen (argV[hamlet_keys->out], "w")) : (fopen (standart_name_out, "w"));

    assert (hamlet->input  != nullptr);
    assert (hamlet->output != nullptr);
    }


//-----------------------------------------------------------------------------


void process_files (int argC, char* argV[], for_text* hamlet, keys* hamlet_keys)
    {
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


void sort_and_write (int argC, char* argV[], for_text* hamlet, keys* hamlet_keys)
    {
    hamlet_keys->front = get_arg_val (argC, argV, "-front");
    if ((hamlet_keys->front == 0) || (strcmp (argV[hamlet_keys->front], "1") == 0))
        {
        quick_sort (hamlet->struct_array, hamlet->lines, front_compare);

        fprintf (hamlet->output,"\n\n\nFront Sorted Origin\n\n\n");
        write_result_sorted (hamlet->struct_array, hamlet->lines, hamlet->output);
        }


    hamlet_keys->reverse = get_arg_val (argC, argV, "-reverse");
    if ((hamlet_keys->reverse == 0) || (strcmp (argV[hamlet_keys->reverse], "1") == 0))
        {
        qsort (hamlet->struct_array, hamlet->lines, sizeof(str), reverse_compare);

        fprintf (hamlet->output,"\n\n\nReverse Sorted Origin\n\n\n");
        write_result_sorted (hamlet->struct_array, hamlet->lines, hamlet->output);
        }


    hamlet_keys->origin = get_arg_val (argC, argV, "-origin");
    if ((hamlet_keys->origin == 0) || (strcmp (argV[hamlet_keys->origin], "1") == 0))
        {
        fprintf (hamlet->output,"\n\n\nOrigin Itself\n\n\n");
        write_origin (hamlet->pointer_on_buffer, hamlet->file_length, hamlet->output);
        }
    }


//-----------------------------------------------------------------------------


void quick_sort (str* s_array, int length, int (*cmp)(const void* str1, const void* str2))
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


void free_memory (for_text* hamlet)
    {
    free (hamlet->struct_array);
    free (hamlet->pointer_on_buffer);
    }


//-----------------------------------------------------------------------------


void destructor_for_text (for_text* hamlet)
    {
    assert (hamlet != nullptr);
    free (hamlet);
    }


//-----------------------------------------------------------------------------

//  Testing functions

//-----------------------------------------------------------------------------


void test_me ()
    {

    for_text* test = construct_for_text ();
    ftest (test != nullptr);

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


    free_memory (test);
    destructor_for_text (test);
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
