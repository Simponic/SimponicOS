#include "gdt.h"
#include "idt.h"
#include "isr.h"
#include "types.h"
#include "print.h"
#include "port.h"

// inline function to swap two numbers

#define FOREGROUND 0x0 
#define BACKGROUND 0xF

void kernel_main(void) {
    initializeGDT();
    initializeIDT();
    printToMonitor("Hello\n");
    printIntToMonitor(10, 10);
    asm ("int $0x3");
    asm ("int $0x4");
}

