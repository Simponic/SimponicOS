#ifndef GDT_H
#define GDT_H

#include "types.h"

struct GDT {
    uint32_t limit;
    uint32_t base;
    uint8_t type;
} __attribute((packed));

struct GDT_ptr {
    uint16_t limit;
    uint32_t base;
} __attribute((packed));

uint8_t gdt_entries[8 * 3]; // (8 bytes per entry) * (num entries) 

void encodeGDT(uint8_t* gdtEntry, struct GDT source);
void initializeGDT();
#endif //GDT_H
