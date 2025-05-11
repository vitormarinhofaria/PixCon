#pragma once

#include <stdint.h>
#include <wchar.h>

#ifdef __cplusplus
#define PUBLIC extern "C"
#else
#define PUBLIC
#endif

struct
{
    uint8_t r;
    uint8_t g;
    uint8_t b;
} typedef Pixel;

struct
{
    Pixel upper;
    Pixel lower;
} typedef CharColor;

struct
{
    void *buffer;
    size_t bufferSize;
    CharColor *charColors;
    int width;
    int height;
} typedef WindowData;

struct
{
    Pixel *data;
    int width;
    int height;
} typedef Image;

PUBLIC WindowData InitWindow(int w, int h);
PUBLIC void FinishWindow(WindowData *windowData);
PUBLIC Image CreateImage(int width, int height);

PUBLIC void PutPixel(Image *image, int x, int y, Pixel pixel);
PUBLIC Pixel Getpixel(Image *image, int x, int y);
PUBLIC void DrawImage(WindowData *windowData, Image *image, int offX, int offY);
PUBLIC void Present(WindowData *windowData);
PUBLIC void Clear(WindowData *windowData);