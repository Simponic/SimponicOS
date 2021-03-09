#ifndef IDT_H
#define IDT_H

#include "types.h"
#include "isr.h"

struct IDT {
    uint32_t base;
    uint16_t selector;
    uint8_t  flags;
} __attribute__((packed));

struct IDT_ptr {
    uint16_t limit;
    uint32_t base;
} __attribute__((packed));

uint8_t idt_entries[8 * 256]; // 8 bytes per entry * 256 entries

struct IDT createIDT(uint32_t base, uint16_t selector, uint8_t flags); 
void encodeIDT(uint8_t* idtEntry, struct IDT source);
void initializeIDT();

#endif // IDT_H
