#include "ForCrack\\TXLib.h"

#include <windows.h>
#include <mmsystem.h>

#include "ForCrack\\TextFunctions.hpp"

const char* const STANDART_INPUT_FILE = "CrackMe.com";

const int RIGHT_HASH = 12163;

const double TEXT_X = 600;

const double TEXT_Y = 160;

HDC PrepareScreen ();
void PrintMessageWithPause (const char* message, double* text_y);
int CountHash (char* buffer, size_t num_of_elems);
int ror (int num);
void IncorrectFileMessage (double* text_y);
void CrackFile (char* buffer, size_t length, double* text_y);

int main (int argC, char* argV[])
{
    HDC background = PrepareScreen ();

    double text_y = TEXT_Y;

    PrintMessageWithPause ("Step 1: Reading your file", &text_y);

    text source = {};

    ProcessInput (argC, argV, &source, STANDART_INPUT_FILE);

    PrintMessageWithPause ("Step 2: Counting hash", &text_y);

    if (CountHash (source.pointer_on_buffer, source.file_length) != RIGHT_HASH)
    {
        IncorrectFileMessage (&text_y);
        free (source.pointer_on_buffer);
        txDeleteDC (background);
        return 0;
    }

    CrackFile (source.pointer_on_buffer, source.file_length, &text_y);
    free (source.pointer_on_buffer);

    txTextOut (TEXT_X, text_y, "Step 5: Profit");

    txDeleteDC (background);
}

HDC PrepareScreen ()
{
    sndPlaySound ("ForCrack\\Running-in-the-90_s.wav", SND_ASYNC + SND_LOOP);

    txCreateWindow (1200, 675);

    HDC  background = txLoadImage ("ForCrack\\Image.bmp");

    assert (background);

    txBitBlt (0, 0, background);

    txSetColor (TX_LIGHTGREEN);

    txSetTextAlign (TA_CENTER);

    return background;
}

void PrintMessageWithPause (const char* message, double* text_y)
{
    txTextOut (TEXT_X, *text_y, message);
    *text_y += 20;

    Sleep (1000);
}

int CountHash (char* buffer, size_t num_of_elems)
{
    int hash = 0;

    for (size_t i = 0; i < num_of_elems; i++)
    {
        hash += buffer[i];
        ror (hash);
    }

    return hash;
}

int ror (int num)
{
    if (num < 0)
    {
        num = num << 1;
        num++;
    }
    else num = num << 1;

    return num;
}

void IncorrectFileMessage (double* text_y)
{
    txTextOut (TEXT_X, *text_y, "Sorry, I can't crack this file");
    *text_y += 20;

    txTextOut (TEXT_X, *text_y, "Please, give me right file");
    *text_y += 20;
}

void CrackFile (char* buffer, size_t length, double* text_y)
{
    PrintMessageWithPause ("Step 3: Cracking", text_y);

    #pragma GCC diagnostic push
    #pragma GCC diagnostic ignored "-Wconversion"

    //disabling warning for meaning of my replacement to be more clear

    buffer[0] = 0xEB; //jmp
    buffer[1] = 0x1E; //offset for jmp

    #pragma GCC diagnostic pop

    PrintMessageWithPause ("Step 4: ...    ", text_y);

    FILE* cracked = fopen ("Cracked.com", "wb");

    for (size_t i = 0; i < length; i++)
    {
        fprintf (cracked, "%c", buffer[i]);
    }
}
