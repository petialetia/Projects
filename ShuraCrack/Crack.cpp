#include "Crack.hpp"

int main (int argC, char* argV[])
{
    HDC background = PrepareScreen ();

    PlaySound (MAIN_SOUND, NULL, SND_ASYNC + SND_LOOP);

    double text_y = START_TEXT_Y;
    PrintMessageWithPause ("Step 1: Reading your file", &text_y);

    text source = {};
    ProcessInput (argC, argV, &source, STANDART_INPUT_FILE);

    PrintMessageWithPause ("Step 2: Counting hash", &text_y);

    if (CountCheckSum (source.pointer_on_buffer, source.file_length) != RIGHT_CHECK_SUM)
    {
        IncorrectFileMessage (&text_y);
        ClearMemory (source.pointer_on_buffer, background);
	PlaySound (NULL, NULL, 0); // Stop playing main sound
	PlaySound (FAILURE_SOUND, NULL, SND_ASYNC);            
        return 0;
    }

    CrackFile (argC, argV, source.pointer_on_buffer, source.file_length, &text_y);
   
    txTextOut (TEXT_X, text_y, "Step 5: Profit");
    ClearMemory (source.pointer_on_buffer, background);
    PlaySound (NULL, NULL, 0); // Stop playing main sound
    PlaySound (SUCCESS_SOUND, NULL, SND_ASYNC);
}

HDC PrepareScreen ()
{
    txCreateWindow (LENGTH_OF_WINDOW, WIDTH_OF_WINDOW);

    HDC  background = txLoadImage (BACKGROUND_IMAGE);

    assert (background);

    txBitBlt (0, 0, background);

    txSetColor (TX_LIGHTGREEN);

    txSetTextAlign (TA_CENTER);

    return background;
}

void PrintMessageWithPause (const char* message, double* text_y)
{
    PrintMessage (message, text_y);

    Sleep (1000);
}

void PrintMessage (const char* message, double* text_y)
{
    txTextOut (TEXT_X, *text_y, message);
    *text_y += NEXT_LINE_OFFSET;
}

int CountCheckSum (char* buffer, size_t num_of_elems)  
{
    int check_sum = 0;

    for (size_t i = 0; i < num_of_elems; i++)
    {
        check_sum += buffer[i];
        ror (check_sum);
    }

    return check_sum;
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
    PrintMessage ("I can't crack this file", text_y);
    PrintMessage ("Give me right file",     text_y);
}

void ClearMemory (char* pointer_on_buffer, HDC background)
{
    free (pointer_on_buffer);
    txDeleteDC (background);
}

void CrackFile (int argC, char* argV[], char* buffer, size_t length, double* text_y)
{
    PrintMessageWithPause ("Step 3: Cracking", text_y);

    buffer[0] = CODE_JMP; 
    buffer[1] = CODE_OFFSET; 

    PrintMessageWithPause ("Step 4: ...    ", text_y);

    size_t output_key = GetArgVal (argC, argV, "out");

    FILE* cracked = (output_key != 0) ? (fopen (argV[output_key], "wb")) : fopen (STANDART_OUTPUT_FILE, "wb");
    assert (cracked != nullptr);
    fwrite (buffer, sizeof (char), length, cracked);
}