#include "pixcon.h"
#include <stdio.h>
#include <unistd.h>
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"
#define STB_IMAGE_RESIZE_IMPLEMENTATION
#include "stb_image_resize2.h"

struct
{
    unsigned char *data;
    int w;
    int h;
    int c;
} typedef StbImage;

void writeImage(Image dst, StbImage src);

int main()
{
    WindowData window = InitWindow(0, 0);

    StbImage original = {0};
    original.data = stbi_load("nyan.png", &original.w, &original.h, &original.c, 3);

    // TODO: segmentation fault if image size is a bit larger
    int factor = 4;
    StbImage resized = {0};
    resized.h = original.h / factor;
    resized.w = original.w / factor;
    resized.c = original.c;
    resized.data = malloc(resized.w * resized.h * 3);

    stbir_resize_uint8_linear(original.data, original.w, original.h, 0, resized.data, resized.w, resized.h, 0, STBIR_RGB);

    Image image = CreateImage(resized.w, resized.h);
    writeImage(image, resized);

    int posX = 0;
    int posY = 0;

    while (1)
    {
        Clear(&window);
        DrawImage(&window, &image, posX, posY);

        posX++;
        if (posX > window.width)
        {
            posX = 0;
        }
        posY++;
        if (posY > window.height * 2)
        {
            posY = 0;
        }

        Present(&window);
    }
}

Pixel pixelFromArray(StbImage src, int x, int y)
{
    int off = ((y * src.w) + x) * src.c;
    unsigned char r = *(src.data + off);
    unsigned char g = *(src.data + off + 1);
    unsigned char b = *(src.data + off + 2);
    Pixel p = {.r = r, .g = g, .b = b};
    return p;
}

void writeImage(Image dst, StbImage src)
{
    for (int y = 0; y < dst.height; y++)
    {
        for (int x = 0; x < dst.width; x++)
        {
            Pixel p = pixelFromArray(src, x, y);
            PutPixel(&dst, x, y, p);
        }
    }
}