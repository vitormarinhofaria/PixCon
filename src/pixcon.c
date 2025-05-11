#include "pixcon.h"
#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>

#ifdef __linux__
#include <unistd.h>
#include <locale.h>
#include <sys/ioctl.h>
#endif

#ifdef _WIN32
#include <Windows.h>
#include <fcntl.h>
#include <io.h>
#endif

WindowData InitWindow(int w, int h)
{
#ifdef _WIN32
    _setmode(_fileno(stdout), _O_U16TEXT);

    HANDLE console = GetStdHandle(STD_OUTPUT_HANDLE);

    int columns = w;
    int rows = h;
    if (w == 0 || h == 0)
    {
        CONSOLE_SCREEN_BUFFER_INFO csbi;
        GetConsoleScreenBufferInfo(console, &csbi);
        columns = csbi.srWindow.Right - 2;
        rows = csbi.srWindow.Bottom - 5;
    }

    system("cls");

    CharColor *charColors = (CharColor *)malloc(sizeof(CharColor) * columns * rows);
    memset(charColors, 0, sizeof(CharColor) * columns * rows);

    DWORD mode = 0;
    GetConsoleMode(console, &mode);
    mode |= ENABLE_VIRTUAL_TERMINAL_PROCESSING;
    SetConsoleMode(console, mode);

    WindowData windowData;
    windowData.charColors = charColors;
    windowData.height = rows;
    windowData.width = columns;
    windowData.bufferSize = sizeof(wchar_t) * windowData.width * windowData.height * 30;
    windowData.buffer = malloc(windowData.bufferSize);
    return windowData;
#endif
#ifdef __linux__
    WindowData windowData = {0};
    system("clear");

    setlocale(LC_ALL, "");
    struct winsize winsize = {0};
    int res = ioctl(STDOUT_FILENO, TIOCGWINSZ, &winsize);
    if (res != 0)
    {
        perror("ioctl");
        return windowData;
    }

    windowData.height = winsize.ws_row - 5;
    windowData.width = winsize.ws_col - 2;
    windowData.charColors = (CharColor *)malloc(sizeof(CharColor) * winsize.ws_col * winsize.ws_row);
    memset(windowData.charColors, 0, sizeof(CharColor) * winsize.ws_col * winsize.ws_row);

    windowData.bufferSize = sizeof(char) * windowData.width * windowData.height * 30;
    windowData.buffer = malloc(windowData.bufferSize);

    return windowData;
#endif
}

Image CreateImage(int width, int height)
{
    Pixel *px = (Pixel *)malloc(sizeof(Pixel) * width * height);
    memset(px, 0, sizeof(Pixel) * width * height);
    Image img = {.data = px, .height = height, .width = width};
    return img;
}

void PutPixel(Image *image, int x, int y, Pixel pixel)
{
    image->data[(y * image->width) + x] = pixel;
}

Pixel Getpixel(Image *image, int x, int y)
{
    return image->data[(y * image->width) + x];
}

void DrawImage(WindowData *windowData, Image *image, int offX, int offY)
{
    for (int y = 0; y < image->height; y++)
    {
        for (int x = 0; x < image->width; x++)
        {
            int absY = y + offY;
            int wx = x + offX;
            int wy = absY / 2;

            if (wy >= windowData->height || wx >= windowData->width)
                continue;

            CharColor *cell = &windowData->charColors[wy * windowData->width + wx];

            if ((y + offY) % 2 == 0)
                cell->upper = image->data[y * image->width + x];
            else
                cell->lower = image->data[y * image->width + x];
        }
    }
}

void Present(WindowData *windowData)
{
#ifdef _WIN32
    wchar_t *buffer = windowData->buffer;
    wchar_t *ptr = windowData->buffer;
    ptr += swprintf(ptr, windowData->bufferSize, L"\033[H\033[?25l");

    for (int y = 0; y < windowData->height; y++)
    {
        for (int x = 0; x < windowData->width; x++)
        {
            CharColor ch = windowData->charColors[y * windowData->width + x];
            ptr += swprintf(ptr, windowData->bufferSize - (ptr - buffer),
                            L"\033[38;2;%d;%d;%dm\033[48;2;%d;%d;%dm▀",
                            ch.upper.r, ch.upper.g, ch.upper.b,
                            ch.lower.r, ch.lower.g, ch.lower.b);
        }
        ptr += swprintf(ptr, windowData->bufferSize - (ptr - buffer), L"\033[0m\n");
    }
    DWORD written;
    HANDLE hOut = GetStdHandle(STD_OUTPUT_HANDLE);
    WriteConsoleW(hOut, windowData->buffer, (ptr - buffer), &written, NULL);
#endif
#ifdef __linux__
    char *buffer = windowData->buffer;
    char *ptr = windowData->buffer;
    ptr += snprintf(ptr, windowData->bufferSize, "\033[H\033[?25l");

    for (int y = 0; y < windowData->height; y++)
    {
        for (int x = 0; x < windowData->width; x++)
        {
            CharColor ch = windowData->charColors[y * windowData->width + x];
            ptr += snprintf(ptr, windowData->bufferSize - (ptr - buffer),
                            "\033[38;2;%d;%d;%dm\033[48;2;%d;%d;%dm▀",
                            ch.upper.r, ch.upper.g, ch.upper.b,
                            ch.lower.r, ch.lower.g, ch.lower.b);
        }
        ptr += snprintf(ptr, windowData->bufferSize - (ptr - buffer), "\033[0m\n");
    }
    write(STDOUT_FILENO, buffer, (ptr - buffer));
#endif
}

void Clear(WindowData *windowData)
{
    wprintf(L"\033[0m\n");
    memset(windowData->charColors, 0, sizeof(CharColor) * windowData->width * windowData->height);
}
