#include <stdio.h>
#include <assert.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>

//My Heroes:
//Эвелина
//Вася
//Никита
//Серёжа

struct str
{
    char*  begi = nullptr;
    size_t leng = 0;
};

size_t measureorigin (FILE* input);
char* copypoem (FILE* input, size_t* amount);
size_t countlines (char* pointer, size_t amount);
void fillstray (str* stray, char* pointer, size_t amount, size_t* lines);
str* copystray (size_t lines, str* stray);
void quicksort (str* s, int last, int flag, int (*cmp)(str str1, str str2, int flag), void (*swaper)(str* s1, str* s2));
int unicomp (str str1, str str2, int flag);
void swaper (str* s1, str* s2);
void writeresult (char* ponter, size_t amount, str* stray, str* revay, size_t lines, FILE* output);

//-----------------------------------------------------------------------------

//  MAIN

//-----------------------------------------------------------------------------

int main ()
    {
    FILE* input  = fopen("C:\\Users\\petialetia\\Desktop\\hamlet\\hamlet.txt",        "r");
    FILE* output = fopen("C:\\Users\\petialetia\\Desktop\\hamlet\\Sorted Hamlet.txt", "w");
    assert (input  != 0);
    assert (output != 0);

    size_t amount = measureorigin (input);
    char* pointer = copypoem (input, &amount);

    //pointer[amount-1] = 0;

    fclose (input);

    size_t lines = countlines (pointer, amount);



    str* stray = (str*) calloc (lines, sizeof(str));
    assert (stray != 0);

    fillstray (stray, pointer, amount, &lines);

    str* revay = copystray (lines, stray);

    quicksort (stray, lines, 1 , unicomp, swaper);
    quicksort (revay, lines, -1, unicomp, swaper);

    writeresult (pointer, amount, stray, revay, lines, output);

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
    size_t a = measureorigin (fite);
    if (a == 69)
        {
        testok
        }
    else
        {
        pressf
        }
    char* p = copypoem (fite, &a);
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
    size_t l = countlines (p, a);
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

    fillstray (straus, p, a, &l);
    if (l == 3)
        {
        testok
        }
    else
        {
        pressf
        }
    char char1[38] = "What good is love and peace on Earth?";
    if (strcmp (char1, straus[0].begi) == 0)
        {
        testok
        }
    else
        {
        pressf
        }
    char char2[20] = "When it's exclusive";
    if (strcmp (char2, straus[1].begi) == 0)
        {
        testok
        }
    else
        {
        pressf
        }
    char char3[10] = "Green Day";
    if (strcmp (char3, straus[2].begi) == 0)
        {
        testok
        }
    else
        {
        pressf
        }
    str* revaus = copystray (l, straus);
    for (size_t i = 0; i < l; i++)
        {
        if (strcmp (revaus[i].begi, straus[i].begi) == 0)
            {
            testok
            }
        else
            {
            pressf
            }
        }
    quicksort (straus, l, 1 , unicomp, swaper);
    if (strcmp (char3, straus[0].begi) == 0)
        {
        testok
        }
    else
        {
        pressf
        }
    if (strcmp (char1, straus[1].begi) == 0)
        {
        testok
        }
    else
        {
        pressf
        }
    if (strcmp (char2, straus[2].begi) == 0)
        {
        testok
        }
    else
        {
        pressf
        }
    quicksort (revaus, l, -1 , unicomp, swaper);
    if (strcmp (char2, revaus[0].begi) == 0)
        {
        testok
        }
    else
        {
        pressf
        }
    if (strcmp (char1, revaus[1].begi) == 0)
        {
        testok
        }
    else
        {
        pressf
        }
    if (strcmp (char3, revaus[2].begi) == 0)
        {
        testok
        }
    else
        {
        pressf
        }

//-----------------------------------------------------------------------------

//  END OF TESTS

//-----------------------------------------------------------------------------

    }

//-----------------------------------------------------------------------------

//  FUNCTIONS

//-----------------------------------------------------------------------------

size_t measureorigin (FILE* input)
    {
    fseek (input, 0, SEEK_END);
    int temp = ftell(input);
    assert (temp != 0);
    fseek (input, 0, SEEK_SET);
    return temp;
    }

char* copypoem (FILE* input, size_t* amount)
    {
    char* pointer = (char*) calloc (*amount+1, sizeof(char));
    assert (pointer != 0);
    size_t size = fread(pointer, sizeof(char), *amount, input);
    assert (size != 0);
    *amount = size+1;
    return pointer;
    }

size_t countlines (char* pointer, size_t amount)
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

void fillstray (str* stray, char* pointer, size_t amount, size_t* lines)
    {
    size_t cnt = 0;
    size_t temp = 0;
    for (bool flag = 1; amount > 0; amount--, pointer++)
        {
        if ((flag == 1) && (*pointer != 0))
            {
            flag = 0;
            temp = 0;
            stray[cnt].begi = pointer;
            }
        else if ((*(pointer) == '\0') && (flag == 0))
                {
                flag = 1;
                stray[cnt].leng = temp;
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

str* copystray (size_t lines, str* stray)
    {
    str* s = (str*) calloc (lines, sizeof (str));
    assert (s != 0);
    for (size_t i = 0; i < lines; i++)
        {
        s[i].begi = stray[i].begi;
        s[i].leng = stray[i].leng;
        }
    return s;
    }

void quicksort (str* s, int last, int flag, int (*cmp)(str str1, str str2, int flag), void (*swaper)(str* s1, str* s2))
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
        quicksort (s + i, last-i, flag, cmp, swaper);
        }
    if (0 < j)
        {
        quicksort (s, j+1, flag, cmp, swaper);
        }
    }

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
        left  = temp1.begi + (temp1.leng - 1);
        right = temp2.begi + (temp2.leng - 1);
        x = temp1.begi;
        y = temp2.begi;
        }
    else if (flag == 1)
            {
            left  = temp1.begi;
            right = temp2.begi;
            x = temp1.begi + temp1.leng - 1;
            y = temp2.begi + temp2.leng - 1;
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

void swaper (str* s1, str* s2)
    {
    str* left  =  s1;
    str* right =  s2;
    str* temp = (str*) calloc (1, sizeof (str));
    (*temp). begi = (*left). begi;
    (*temp). leng = (*left). leng;
    (*left). begi = (*right).begi;
    (*left). leng = (*right).leng;
    (*right).begi = (*temp). begi;
    (*right).leng = (*temp). leng;
    free (temp);
    }

void writeresult (char* pointer, size_t amount, str* stray, str* revay, size_t lines, FILE* output)
    {
    fprintf (output,"\n\n\nFrontsorted Hamlet\n\n\n");
    for (size_t temp = 0;temp <lines;temp++)
        {
        fprintf (output, "%s\n", stray[temp].begi);
        }
    free (stray);
    fprintf (output,"\n\n\nReversesorted Hamlet\n\n\n");
    for (size_t temp = 0;temp <lines;temp++)
        {
        fprintf (output, "%s\n", revay[temp].begi);
        }
    free (revay);
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
