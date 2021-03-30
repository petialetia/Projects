#include "TXLib.h"

const int SCREEN_LENGTH = 1200;
const int SCREEN_WIDTH  = 1000;

const float CENTER_PIXEL_X = 800.f;
const float CENTER_PIXEL_Y = 500.f;

const float ZOOM_PIXEL_X = 99.f;
const float ZOOM_PIXEL_Y = 500.f;

const float START_SCALE = 500;

const int SCALE_INCREASING = 2;

const float MAX_SCALE = 10000000;

const float MAX_R = 1000;

const int MAX_COUNTER = 510;

int main ()
{
    txCreateWindow (SCREEN_LENGTH, SCREEN_WIDTH);

    typedef RGBQUAD (&scr_t) [SCREEN_WIDTH][SCREEN_LENGTH];
    scr_t screen = (scr_t) *txVideoMemory ();

    float zoom_coordinate_x = (ZOOM_PIXEL_X - CENTER_PIXEL_X) / START_SCALE;
    float zoom_coordinate_y = (ZOOM_PIXEL_Y - CENTER_PIXEL_Y) / START_SCALE;

    for (float current_scale = START_SCALE; current_scale < MAX_SCALE; current_scale *= SCALE_INCREASING)
    {
        if (GetAsyncKeyState (VK_ESCAPE)) break;

        float step_x = 1/current_scale;
        float step_y = 1/current_scale;

        float start_y = zoom_coordinate_y - ZOOM_PIXEL_Y / current_scale;

        for (int i_y = 0; i_y < SCREEN_WIDTH; i_y++, start_y += step_y)
        {
            float start_x = zoom_coordinate_x - ZOOM_PIXEL_X / current_scale;

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

                screen[i_y][i_x] = { (counter%255) * counter / 255, counter%255, 0};
                //screen[i_y][i_x] = {i, i, i};
            }
        }

    printf ("\t\r%.0lf", txGetFPS ());
    }
}
