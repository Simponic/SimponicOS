#include "gdt.h"
#include "types.h"
#include "print.h"

#define FOREGROUND 0x0 
#define BACKGROUND 0xF

void PrintWithScreenFill(char* string, TextOutput* output_stream) {
    // Print a string and fill the screen
    print(string, BACKGROUND, FOREGROUND, output_stream);
    int row = output_stream->terminal_row;
    while (output_stream->terminal_row < output_stream->max_row) {
        putChar('\n', BACKGROUND, FOREGROUND, output_stream);
    }
    output_stream->terminal_row = row;
}

void kernel_main(void) {
    TextOutput output_stream = createOutput(25,80,(uint16_t*)0xB8000);
    PrintWithScreenFill("Hello, Logan World!\n", &output_stream);
}

