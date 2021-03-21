#include "ForCrack\\TXLib.h"

#include <windows.h>
#include <mmsystem.h>
#include <inttypes.h>

#include "ForCrack\\TextFunctions.hpp"

const char* const STANDART_INPUT_FILE  = "CrackMe.com";
const char* const STANDART_OUTPUT_FILE = "Cracked.com";

const int RIGHT_CHECK_SUM = 12163;

const double LENGTH_OF_WINDOW = 1200;
const double WIDTH_OF_WINDOW  = 675;

const double TEXT_X = 600;

const double START_TEXT_Y = 170;

const double NEXT_LINE_OFFSET = 20;

const uint8_t CODE_JMP    = 0xEB;
const uint8_t CODE_OFFSET = 0x1E;

const char* const MAIN_SOUND    = "ForCrack\\Running-in-the-90_s.wav";
const char* const FAILURE_SOUND = "ForCrack\\FailureSound.wav";
const char* const SUCCESS_SOUND = "ForCrack\\SuccessSound.wav";

HDC PrepareScreen ();
void PrintMessageWithPause (const char* message, double* text_y);
void PrintMessage (const char* message, double* text_y);
int CountCheckSum (char* buffer, size_t num_of_elems);
int ror (int num);
void IncorrectFileMessage (double* text_y);
void ClearMemory (char* pointer_on_buffer, HDC background);
void CrackFile (char* buffer, size_t length, double* text_y);
