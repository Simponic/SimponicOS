#ifndef PRINT_H
#define PRINT_H

#include "types.h"

typedef struct TextOutput {
    int terminal_row;
    int terminal_column;
    int max_row;
    int max_column;
    uint16_t* vid_mem;
}__attribute__((packed)) TextOutput;

TextOutput monitor;

TextOutput createOutput(const int max_row, const int max_column, uint16_t* vid_mem);
void scrollText(TextOutput* textOutput);
void putChar(uint8_t character, uint8_t background, uint8_t foreground, TextOutput* textOutput);
void print(char* string, uint8_t background, uint8_t foreground, TextOutput* textOutput);
char* itoa(int value, char* buffer, int base);
void printToMonitor(char* string);
void printIntToMonitor(int num, int base);

#endif // PRINT_H
