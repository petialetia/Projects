#include "MathBeauty.hpp"

int main ()
{
    sdl_window_info win_info {};
    OpenWindowWithSurface (&win_info);

    screen_info scr_info = {(SCREEN_LENGTH/2 - ZERO_POINT_X)/START_SCALE, (SCREEN_WIDTH/2 - ZERO_POINT_Y)/START_SCALE, 1/START_SCALE};
    SDL_Event event = {};

    bool is_programm_ended = false;

    while (!is_programm_ended)
    {
        DrawFrame (&win_info, &scr_info);
        CheckEvent (&win_info, &scr_info, &event, &is_programm_ended);
    }

}

void OpenWindowWithSurface (sdl_window_info* win_info)
{
    win_info->window = SDL_CreateWindow ("Mandelbrot", (1920 - SCREEN_LENGTH)/2, (1080 - SCREEN_WIDTH)/2, SCREEN_LENGTH, SCREEN_WIDTH, SDL_WINDOW_MAXIMIZED);

    win_info->surface = SDL_GetWindowSurface (win_info->window); 
}

void DrawFrame (sdl_window_info* win_info, screen_info* scr_info)
{
    clock_t start_time = clock ();

    CalculateMandelbrot (win_info, scr_info);
    SDL_UpdateWindowSurface (win_info->window);

    clock_t end_time = clock ();

    WriteFPS (win_info, CLOCKS_PER_SEC / (end_time - start_time));
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
            float started_x_array[VECTOR_SIZE] = {};
            float started_y_array[VECTOR_SIZE] = {};

            for (int i = 0; i < VECTOR_SIZE; i++)
            {
                started_x_array[i] = start_x + ((float) i)*step_x;
                started_y_array[i] = start_y;
            }

            __m256 started_x = _mm256_load_ps (started_x_array),
                   started_y = _mm256_load_ps (started_y_array);

            __m256 current_x = _mm256_setzero_ps (),
                   current_y = _mm256_setzero_ps ();

            int counters[VECTOR_SIZE] = {0};

            for (int main_counter = 0; main_counter < MAX_COUNTER; main_counter++)
            {
                __m256 x_squared = _mm256_mul_ps (current_x, current_x),
                       y_squared = _mm256_mul_ps (current_y, current_y);

                int mask = _mm256_movemask_ps (_mm256_cmp_ps (_mm256_add_ps (x_squared, y_squared), MAX_R_VECTOR, _CMP_LT_OS));

                if (!mask) break;

                for (int i = 0, bit = 1; i < VECTOR_SIZE; i++, bit <<= 1) if (mask & bit) counters[i]++;

                __m256 xy = _mm256_mul_ps (current_x, current_y);

                current_x = _mm256_add_ps (_mm256_sub_ps (x_squared, y_squared), started_x);
                current_y = _mm256_add_ps (_mm256_add_ps (xy,        xy),        started_y);
            }

            for (int i = 0; i < VECTOR_SIZE; i++) SetPixel (win_info, i_x + i, i_y, Color (counters[i], counters[i]*3, counters[i]) * 2);
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

void WriteFPS (sdl_window_info* win_info, int fps)
{
    static char title[TITLE_SIZE] = {};

    snprintf (title, TITLE_SIZE, "FPS %d", fps);
    SDL_SetWindowTitle (win_info->window, title);
}

void CheckEvent (sdl_window_info* win_info, screen_info* scr_info, SDL_Event* event, bool* is_programm_ended)
{
    while (SDL_PollEvent (event))
        {
            if (event->type == SDL_QUIT) 
            {
                ProcessEndEvent (win_info, is_programm_ended);
            }
            else 
            {
                if (event->type == SDL_KEYDOWN)
                {
                    if (event->key.keysym.scancode == SDL_SCANCODE_ESCAPE) ProcessEndEvent (win_info, is_programm_ended);
                    else MoveSet (event, scr_info);
                }
            }
        }
}

void ProcessEndEvent (sdl_window_info* win_info, bool* is_programm_ended)
{
    SDL_DestroyWindow (win_info->window);
    SDL_Quit();
    *is_programm_ended = true;
}

void MoveSet (SDL_Event* event, screen_info* scr_info)
{
    switch (event->key.keysym.scancode)
    {
        case SDL_SCANCODE_W:
            scr_info->center_pixel_y += HORIZONTAL_MOVEMENT_OFFSET * scr_info->scale;
            break;

        case SDL_SCANCODE_S:
            scr_info->center_pixel_y -= HORIZONTAL_MOVEMENT_OFFSET * scr_info->scale;
            break;

        case SDL_SCANCODE_A:
            scr_info->center_pixel_x -= VERTICAL_MOVEMENT_OFFSET * scr_info->scale;
            break;

        case SDL_SCANCODE_D:
            scr_info->center_pixel_x += VERTICAL_MOVEMENT_OFFSET * scr_info->scale;
            break;

        case SDL_SCANCODE_E:
            scr_info->scale /= SCALE_INCREASING;
            break;

        case SDL_SCANCODE_Q:
            scr_info->scale *= SCALE_INCREASING;
            break;

        default:
            break;
    }
}