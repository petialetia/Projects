#include <immintrin.h>
#include <time.h>
#include <SDL2/SDL.h>

const int SCREEN_LENGTH = 1200;
const int SCREEN_WIDTH  = 800;

const float ZERO_POINT_X = 800.f;
const float ZERO_POINT_Y = 400.f;

const float START_SCALE = 400;

//const int SCALE_INCREASING = 1.5;

const int SCALE_INCREASING = 2;

const float MAX_R = 1000;

const __m256 MAX_R_VECTOR = _mm256_set1_ps (1000.f);

const int MAX_COUNTER = 512;

const int ESC_SCAN_CODE = 41;

const int UP_SCAN_CODE    = 26;
const int DOWN_SCAN_CODE  = 22;
const int LEFT_SCAN_CODE  = 4;
const int RIGHT_SCAN_CODE = 7;
const int PLUS_SCAN_CODE  = 8;
const int MINUS_SCAN_CODE = 20;

const int BYTE_SIZE = 8;

const int TITLE_SIZE = 100;

const int VECTOR_SIZE = 8;

struct sdl_window_info
{
    SDL_Window*   window   = nullptr;
    SDL_Renderer* renderer = nullptr;
    SDL_Surface*  surface  = nullptr;
};

struct screen_info
{
    float center_pixel_x = 0;
    float center_pixel_y = 0;
    float scale          = 0;
};

void CalculateMandelbrot (sdl_window_info* win_info, screen_info* scr_info);

void SetPixel (sdl_window_info* win_info, int x, int y, uint32_t color);
uint32_t Color (unsigned char red, unsigned char green, unsigned char blue, unsigned char a = 255);

void MoveSet (SDL_Event* event, screen_info* scr_info);

void WriteFPS (sdl_window_info* win_info, int fps);

int main ()
{
    sdl_window_info win_info {};

    win_info.window = SDL_CreateWindow ("Test", /*1920 + */(1920 - SCREEN_LENGTH)/2, (1080 - SCREEN_WIDTH)/2, SCREEN_LENGTH, SCREEN_WIDTH, SDL_WINDOW_MAXIMIZED);

    /*if (win_info.window == nullptr)
    {
	    std::cout << "SDL_CreateWindow Error: " << SDL_GetError() << std::endl;
	    return 1;
    }*/    

    win_info.renderer = SDL_CreateRenderer (win_info.window, -1, SDL_RENDERER_ACCELERATED);

    win_info.surface = SDL_GetWindowSurface (win_info.window); 

    SDL_Event event = {};

    /*for (int x = 0; x < SCREEN_LENGTH; x++)
    {
        for (int y = 0; y < SCREEN_WIDTH; y++)
        {
            SetPixel (&win_info, x, y, Color (0, 0, 0));
        }
    }*/

    //SetPixel (&win_info, 0, 0, Color (255, 255, 255));

    SDL_UpdateWindowSurface (win_info.window);

    screen_info scr_info = {(SCREEN_LENGTH/2 - ZERO_POINT_X)/START_SCALE, (SCREEN_WIDTH/2 - ZERO_POINT_Y)/START_SCALE, 1/START_SCALE};

    printf ("%f %f\n", scr_info.center_pixel_x, scr_info.center_pixel_y); 

    printf ("%f %f\n", scr_info.center_pixel_y + SCREEN_WIDTH * scr_info.scale / 2, scr_info.center_pixel_x - SCREEN_LENGTH * scr_info.scale / 2 );

    bool is_programm_ended = false;

    while (!is_programm_ended)
    {
        clock_t start_time = clock ();

        CalculateMandelbrot (&win_info, &scr_info);
        SDL_UpdateWindowSurface (win_info.window);

        clock_t end_time = clock ();

        WriteFPS (&win_info, CLOCKS_PER_SEC / (end_time - start_time));

        while (SDL_PollEvent (&event))
        {
            if (event.type == SDL_QUIT) 
            {
                SDL_DestroyWindow (win_info.window);
                SDL_Quit();
                is_programm_ended = true;
            }
            else 
            {
                if (event.type == SDL_KEYDOWN)
                {
                    if (event.key.keysym.scancode == ESC_SCAN_CODE) 
                    {
                        SDL_DestroyWindow (win_info.window);
                        SDL_Quit();
                        is_programm_ended = true;
                    }
                    else MoveSet (&event, &scr_info);
                }
            }
        }
    }

}

void CalculateMandelbrot (sdl_window_info* win_info, screen_info* scr_info)
{
    float step_x = scr_info->scale,
          step_y = step_x;

    float start_y = scr_info->center_pixel_y + SCREEN_WIDTH * scr_info->scale / 2;

    for (int i_y = 0; i_y < SCREEN_WIDTH; i_y++, start_y -= step_y)
    {
        float start_x = scr_info->center_pixel_x - SCREEN_LENGTH * scr_info->scale / 2; 

        for (int i_x = 0; i_x < SCREEN_LENGTH; i_x += VECTOR_SIZE, start_x += VECTOR_SIZE * step_x)
        {
            //float current_x = start_x;
            //float current_y = start_y;
            //int counter = 0;

            float started_x_array[VECTOR_SIZE] = {};
            float started_y_array[VECTOR_SIZE] = {};

            for (int i = 0; i < VECTOR_SIZE; i++)
            {
                started_x_array[i] = start_x + ((float) i)*step_x;
                started_y_array[i] = start_y;
            }

            __m256 started_x = _mm256_load_ps (started_x_array);
            __m256 started_y = _mm256_load_ps (started_y_array);

            __m256 current_x = _mm256_setzero_ps ();
            __m256 current_y = _mm256_setzero_ps ();


            /*for (; counter < MAX_COUNTER; counter++)
            {
                float x_squared = current_x*current_x,
                      y_squared = current_y*current_y,
                      xy = current_x*current_y;

                float r_squared = x_squared + y_squared;

                if (r_squared >= MAX_R) break;

                current_x = x_squared - y_squared + start_x;
                current_y = xy + xy + start_y;
            }*/

            int counters[VECTOR_SIZE] = {0};

            for (int main_counter = 0; main_counter < MAX_COUNTER; main_counter++)
            {
                __m256 x_squared = _mm256_mul_ps (current_x, current_x);
                __m256 y_squared = _mm256_mul_ps (current_y, current_y);

                __m256 r_squared = _mm256_add_ps (x_squared, y_squared);

                int mask = _mm256_movemask_ps (_mm256_cmp_ps (r_squared, MAX_R_VECTOR, _CMP_LT_OS));

                if (!mask) break;

                int bit = 1;

                for (int i = 0; i < VECTOR_SIZE; i++)
                {
                   if (mask & bit) counters[i]++;

                   bit = bit << 1;
                }

                __m256 xy = _mm256_mul_ps (current_x, current_y);

                current_x = _mm256_add_ps (_mm256_sub_ps (x_squared, y_squared), started_x);
                current_y = _mm256_add_ps (_mm256_add_ps (xy,        xy),        started_y);

            }

            //printf ("HEY\n");

            //SetPixel (win_info, i_x, i_y, Color ((counter%256) * counter / 256, counter%256, 0));
            for (int i = 0; i < VECTOR_SIZE; i++)
            {
                SetPixel (win_info, i_x + i, i_y, Color ((counters[i]%256) * counters[i] / 256, counters[i]%256, 0));
            }
        }
    }    
}

uint32_t Color (unsigned char red, unsigned char green, unsigned char blue, unsigned char a)
{
    uint32_t color = a;
    color <<= BYTE_SIZE;
    color |= red;
    color <<= BYTE_SIZE;
    color |= green;
    color <<= BYTE_SIZE;
    color |= blue;
    return color;
}

void SetPixel (sdl_window_info* win_info, int x, int y, uint32_t color)
{
    uint32_t* pixel = (uint32_t*)((uint8_t*)win_info->surface->pixels + y * win_info->surface->pitch +
                                                                        x * win_info->surface->format->BytesPerPixel);

                                                                        // (uint8_t*) потому что win_info->surface->pitch, win_info->surface->format->BytesPerPixel даются в байтах
    *pixel = color;
}

void MoveSet (SDL_Event* event, screen_info* scr_info)
{
    switch (event->key.keysym.scancode)
    {
        case UP_SCAN_CODE:
            scr_info->center_pixel_y += scr_info->scale;
            break;

        case DOWN_SCAN_CODE:
            scr_info->center_pixel_y -= scr_info->scale;
            break;

        case LEFT_SCAN_CODE:
            scr_info->center_pixel_x -= scr_info->scale;
            break;

        case RIGHT_SCAN_CODE:
            scr_info->center_pixel_x += scr_info->scale;
            break;

        case PLUS_SCAN_CODE:
            scr_info->scale /= SCALE_INCREASING;
            break;

        case MINUS_SCAN_CODE:
            scr_info->scale *= SCALE_INCREASING;
            break;
    }

    //printf ("%d\n", event->key.keysym.scancode);
}

void WriteFPS (sdl_window_info* win_info, int fps)
{
    static char title[TITLE_SIZE] = {};

    snprintf (title, TITLE_SIZE, "FPS %d", fps);
    SDL_SetWindowTitle (win_info->window, title);
}