#include "print.h"

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
    for (string; *string; string++) {
        putChar(*string, background, foreground, textOutput);
    }
}

