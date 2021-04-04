#include <time.h>
#include <SDL2/SDL.h>

const int SCREEN_LENGTH = 1200;
const int SCREEN_WIDTH  = 800;

//const float ZERO_POINT_X = 800.f;
//const float ZERO_POINT_Y = 500.f;

const float ZERO_POINT_X = 800.f;
const float ZERO_POINT_Y = 400.f;

const float START_SCALE = 400;

const int SCALE_INCREASING = 2;

const float MAX_R = 1000;

const int MAX_COUNTER = 512;

const int ESC_SCAN_CODE = 41;

const int UP_SCAN_CODE    = 26;
const int DOWN_SCAN_CODE  = 22;
const int LEFT_SCAN_CODE  = 4;
const int RIGHT_SCAN_CODE = 7;
const int PLUS_SCAN_CODE  = 8;
const int MINUS_SCAN_CODE = 20;

const int BYTE_SIZE = 8;

const int TITLE_SIZE = 10;

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

    screen_info scr_info = {(SCREEN_LENGTH/2 - ZERO_POINT_X)/START_SCALE, (SCREEN_WIDTH/2 - ZERO_POINT_Y)/START_SCALE, START_SCALE};

    printf ("%f %f\n", scr_info.center_pixel_x, scr_info.center_pixel_y); 

    printf ("%f %f\n", scr_info.center_pixel_y + SCREEN_WIDTH / (2 * scr_info.scale), scr_info.center_pixel_x - SCREEN_LENGTH / (2 * scr_info.scale));

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
    float step_x = 1/scr_info->scale;
    float step_y = 1/scr_info->scale;

    float start_y = scr_info->center_pixel_y + SCREEN_WIDTH / (2 * scr_info->scale);

    for (int i_y = 0; i_y < SCREEN_WIDTH; i_y++, start_y -= step_y)
    {
        float start_x = scr_info->center_pixel_x - SCREEN_LENGTH / (2 * scr_info->scale); 

        for (int i_x = 0; i_x < SCREEN_LENGTH; i_x++, start_x += step_x)
        {
            float current_x = start_x;
            float current_y = start_y;
            int counter = 0;

            for (; counter < MAX_COUNTER; counter++)
            {
                float x_squared = current_x*current_x,
                      y_squared = current_y*current_y,
                      xy = current_x*current_y;

                float r_squared = x_squared + y_squared;

                if (r_squared >= MAX_R) break;

                current_x = x_squared - y_squared + start_x;
                current_y = xy + xy + start_y;
            }

            //SetPixel (win_info, i_x, i_y, Color ((counter%256) * counter / 256, counter%256, 0));

            SetPixel (win_info, i_x, i_y, Color (counter, counter, counter));
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
            scr_info->center_pixel_y += 1/scr_info->scale;
            break;

        case DOWN_SCAN_CODE:
            scr_info->center_pixel_y -= 1/scr_info->scale;
            break;

        case LEFT_SCAN_CODE:
            scr_info->center_pixel_x -= 1/scr_info->scale;
            break;

        case RIGHT_SCAN_CODE:
            scr_info->center_pixel_x += 1/scr_info->scale;
            break;

        case PLUS_SCAN_CODE:
            scr_info->scale *= SCALE_INCREASING;
            break;

        case MINUS_SCAN_CODE:
            scr_info->scale /= SCALE_INCREASING;
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