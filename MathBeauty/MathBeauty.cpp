#include "TXLib.h"

const int SCREEN_LENGTH = 1501;
const int SCREEN_WIDTH  = 1001;

const float CENTER_PIXEL_X = 1001.f;
const float CENTER_PIXEL_Y = 501.f;

const float ZOOM_PIXEL_X = 300.f;
const float ZOOM_PIXEL_Y = 501.f;

const float START_SCALE = 500;

const int SCALE_INCREASING = 1;

int main ()
{
    txCreateWindow (SCREEN_LENGTH, SCREEN_WIDTH);
    //txBegin ();

    typedef RGBQUAD (&scr_t) [SCREEN_WIDTH][SCREEN_LENGTH];
    scr_t screen = (scr_t) *txVideoMemory ();

    float zoom_coordinate_x = (ZOOM_PIXEL_X - CENTER_PIXEL_X) / START_SCALE;
    float zoom_coordinate_y = (ZOOM_PIXEL_Y - CENTER_PIXEL_Y) / START_SCALE;

    for (float current_scale = START_SCALE; current_scale < 10000000; current_scale *= SCALE_INCREASING)
    {
        if (GetAsyncKeyState (VK_ESCAPE)) break;


        float dx = 1/current_scale;
        float dy = 1/current_scale;

        float y0 = zoom_coordinate_y - ZOOM_PIXEL_Y / current_scale;

        for (int i_y = 0; i_y < SCREEN_WIDTH; i_y++, y0 += dy)
        {
            float x0 = zoom_coordinate_x - ZOOM_PIXEL_X / current_scale;

            for (int i_x = 0; i_x < SCREEN_LENGTH; i_x++, x0 += dx)
            {
                float x = x0;
                float y = y0;

                int i = 0;

                for (; i < 510; i++)
                {
                    float x2 = x*x,
                          y2 = y*y,
                          xy = x*y;

                    float r2 = x2 + y2;

                    if (r2 >= 1000) break;

                    x = x2 - y2 + x0;
                    y = xy + xy + y0;
                }

                screen[i_y][i_x] = {(i%255) * i / 255, i%255, 0};
            }
        }

    printf ("\t\r%.0lf", txGetFPS ());
    }
}
