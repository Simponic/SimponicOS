#include "print.h"

#define FOREGROUND 0x0 
#define BACKGROUND 0xF

TextOutput createOutput(const int max_row, const int max_column, uint16_t* vid_mem) {
    // Create a new TextOutput interface
    TextOutput output;
    output.max_row = max_row;
    output.max_column = max_column;
    output.vid_mem = vid_mem;
    output.terminal_row = 0;
    output.terminal_column = 0;
    return output;
}

void scrollText(TextOutput* textOutput) {
    // Move each character up one row
    for (int y = 0; y < textOutput->max_row; y++) {
        for (int x = 0; x < textOutput->max_column; x++) {
            *(textOutput->vid_mem + textOutput->max_column*y + x) = 
                    *(textOutput->vid_mem + textOutput->max_column*(y+1) + x);
        }
    }
    textOutput->terminal_row--;
}

void putChar(uint8_t character, uint8_t background, uint8_t foreground, 
                                                TextOutput* textOutput) {
    foreground = foreground & 0xF; background = background & 0xF;

    // Handle putting a character to the screen
    if (textOutput->terminal_row == textOutput->max_row) {
        scrollText(textOutput);
    }

    if (character == '\r') {
        // Delete the character before this \r
        if (textOutput->terminal_column == 0) {
            textOutput->terminal_row--;
            textOutput->terminal_column = 80;
        }
        textOutput->terminal_column--;
        *(textOutput->vid_mem + textOutput->terminal_row*textOutput->max_column
                + textOutput->terminal_column) = background << 12;
        return;
    }

    else if (character == '\n' || textOutput->terminal_column == textOutput->max_column) {
        // Make a new line
        for (int i = textOutput->terminal_column; i < textOutput->max_column; i++) {
            *(textOutput->vid_mem + textOutput->terminal_row*textOutput->max_column
                   + textOutput->terminal_column) = background << 12;
            textOutput->terminal_column++;
        }
   
        textOutput->terminal_row++;
        textOutput->terminal_column = 0;
    }

    if (character != '\n' && character != '\r') {
        // Write character to video memory
        uint16_t entry = (background << 12) | (foreground << 8) | character;
        *(textOutput->vid_mem + textOutput->terminal_row*textOutput->max_column
                + textOutput->terminal_column) = entry;
        textOutput->terminal_column++;
    }
}

void print(char* string, uint8_t background, uint8_t foreground, TextOutput* textOutput) {
    // Print a string
    while (*string) {
        putChar(*string, background, foreground, textOutput);
        string++;
    }
}

void PrintWithScreenFill(char* string, TextOutput* output_stream) {
    // Print a string and fill the screen
    print(string, BACKGROUND, FOREGROUND, output_stream);
    int row = output_stream->terminal_row;
    while (output_stream->terminal_row < output_stream->max_row) {
        putChar('\n', BACKGROUND, FOREGROUND, output_stream);
    }
    output_stream->terminal_row = row;
}


inline void swap(char *x, char *y) {
    char t = *x; *x = *y; *y = t;
}

int abs(int value) {
    return (value < 0 ? -value : value);
}
 
// function to reverse buffer[i..j]
char* reverse(char *buffer, int i, int j)
{
    while (i < j)
        swap(&buffer[i++], &buffer[j--]);
 
    return buffer;
}
 
// Iterative function to implement itoa() function in C
char* itoa(int value, char* buffer, int base)
{
    if (base < 2 || base > 32)
        return buffer;
    int n = abs(value);
    int i = 0;
    while (n)
    {
        int r = n % base;
        if (r >= 10) 
            buffer[i++] = 65 + (r - 10);
        else
            buffer[i++] = 48 + r;
        n = n / base;
    }
    if (i == 0)
        buffer[i++] = '0';
    if (value < 0 && base == 10)
        buffer[i++] = '-';
    buffer[i] = '\0';
    return reverse(buffer, 0, i - 1);
}

int haveFilledScreen = 0;

void printToMonitor(char* string) {
    if (!haveFilledScreen) {
        monitor = createOutput(25, 80, (uint16_t*)(0xB8000));
        PrintWithScreenFill(string, &monitor);
        haveFilledScreen = 1;
    }
    else
        print(string, BACKGROUND, FOREGROUND, &monitor);
}

void printIntToMonitor(int num, int base) {
    char* buffer;
    printToMonitor(itoa(num, buffer, base));
}
