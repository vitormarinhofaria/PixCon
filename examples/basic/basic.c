#include "pixcon.h"
#include <stdio.h>
#include <unistd.h>
void addColor(Image image, int r, int g, int b)
{
    for (int y = 0; y < image.height; y++)
    {
        for (int x = 0; x < image.width; x++)
        {
            Pixel p = Getpixel(&image, x, y);
            p.r += r;
            p.g += g;
            p.b += b;
            if (p.r > 255)
            {
                p.r = 0;
            }
            if (p.g > 255)
            {
                p.g = 0;
            }
            if (p.b > 255)
            {
                p.b = 0;
            }
            PutPixel(&image, x, y, p);
        }
    }
}

int main()
{
    WindowData window = InitWindow(0, 0);
    int s = 9;

    Image image = CreateImage(s, s);
    Image image2 = CreateImage(s, s);
    Image image3 = CreateImage(s, s);
    Image image4 = CreateImage(s, s);

    int pos = 0;
    int pos2 = 0;

    while (1)
    {
        Clear(&window);

        addColor(image, 5, 0, 1);
        addColor(image2, 3, 2, 0);
        addColor(image3, 1, 0, 7);
        addColor(image4, 0, 2, 1);

        DrawImage(&window, &image, 0, 0);
        DrawImage(&window, &image2, s, s);
        DrawImage(&window, &image3, 0, s);
        DrawImage(&window, &image4, pos2, pos);

        pos++;
        pos2++;
        if (pos > window.height * 2)
        {
            pos = 0;
        }
        if (pos2 > window.width)
        {
            pos2 = 0;
        }

        Present(&window);
    }
}