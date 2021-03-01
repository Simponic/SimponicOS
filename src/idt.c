#include "idt.h"

struct IDT createIDT(uint32_t base, uint16_t selector, uint8_t flags) {
    struct IDT result;
    result.base     = base;
    result.selector = selector;
    result.flags    = flags;
    return result;
}

void encodeIDT(uint8_t* idtEntry, struct IDT source) {
    // Low base bytes
    idtEntry[0] = source.base & 0xFF;
    idtEntry[1] = (source.base >> 8) & 0xFF;
    // Selector bytes
    idtEntry[2] = source.selector & 0xFF;
    idtEntry[3] = (source.selector >> 8) & 0xFF;
    idtEntry[4] = 0; // Null byte
    idtEntry[5] = source.flags; // Flag bytes
    // High base bytes
    idtEntry[6] = (source.base >> 16) & 0xFF;
    idtEntry[7] = (source.base >> 24) & 0xFF;
}

void initializeIDT() {
    struct IDT_ptr idt_ptr;
    idt_ptr.limit = 8*256 - 1;
    idt_ptr.base  = (uint32_t)&idt_entries;
    
    for (int i = 0; i < 256*8; i++)
        idt_entries[i] = 0; // Just to be safe, set IDT to all 0

    encodeIDT(&idt_entries[0]       , createIDT((uint32_t)isr0 , 0x08, 0x8E));
    encodeIDT(&idt_entries[1*8 - 1] , createIDT((uint32_t)isr1 , 0x08, 0x8E));
    encodeIDT(&idt_entries[2*8 - 1] , createIDT((uint32_t)isr2 , 0x08, 0x8E));
    encodeIDT(&idt_entries[3*8 - 1] , createIDT((uint32_t)isr3 , 0x08, 0x8E));
    encodeIDT(&idt_entries[4*8 - 1] , createIDT((uint32_t)isr4 , 0x08, 0x8E));
    encodeIDT(&idt_entries[5*8 - 1] , createIDT((uint32_t)isr5 , 0x08, 0x8E));
    encodeIDT(&idt_entries[6*8 - 1] , createIDT((uint32_t)isr6 , 0x08, 0x8E));
    encodeIDT(&idt_entries[7*8 - 1] , createIDT((uint32_t)isr7 , 0x08, 0x8E));
    encodeIDT(&idt_entries[8*8 - 1] , createIDT((uint32_t)isr8 , 0x08, 0x8E));
    encodeIDT(&idt_entries[9*8 - 1] , createIDT((uint32_t)isr9 , 0x08, 0x8E));
    encodeIDT(&idt_entries[10*8 - 1], createIDT((uint32_t)isr10, 0x08, 0x8E));
    encodeIDT(&idt_entries[11*8 - 1], createIDT((uint32_t)isr11, 0x08, 0x8E));
    encodeIDT(&idt_entries[12*8 - 1], createIDT((uint32_t)isr12, 0x08, 0x8E));
    encodeIDT(&idt_entries[13*8 - 1], createIDT((uint32_t)isr13, 0x08, 0x8E));
    encodeIDT(&idt_entries[14*8 - 1], createIDT((uint32_t)isr14, 0x08, 0x8E));
    encodeIDT(&idt_entries[15*8 - 1], createIDT((uint32_t)isr15, 0x08, 0x8E));
    encodeIDT(&idt_entries[16*8 - 1], createIDT((uint32_t)isr16, 0x08, 0x8E));
    encodeIDT(&idt_entries[17*8 - 1], createIDT((uint32_t)isr17, 0x08, 0x8E));
    encodeIDT(&idt_entries[18*8 - 1], createIDT((uint32_t)isr18, 0x08, 0x8E));
    encodeIDT(&idt_entries[19*8 - 1], createIDT((uint32_t)isr19, 0x08, 0x8E));
    encodeIDT(&idt_entries[20*8 - 1], createIDT((uint32_t)isr20, 0x08, 0x8E));
    encodeIDT(&idt_entries[21*8 - 1], createIDT((uint32_t)isr21, 0x08, 0x8E));
    encodeIDT(&idt_entries[22*8 - 1], createIDT((uint32_t)isr22, 0x08, 0x8E));
    encodeIDT(&idt_entries[23*8 - 1], createIDT((uint32_t)isr23, 0x08, 0x8E));
    encodeIDT(&idt_entries[24*8 - 1], createIDT((uint32_t)isr24, 0x08, 0x8E));
    encodeIDT(&idt_entries[25*8 - 1], createIDT((uint32_t)isr25, 0x08, 0x8E));
    encodeIDT(&idt_entries[26*8 - 1], createIDT((uint32_t)isr26, 0x08, 0x8E));
    encodeIDT(&idt_entries[27*8 - 1], createIDT((uint32_t)isr27, 0x08, 0x8E));
    encodeIDT(&idt_entries[28*8 - 1], createIDT((uint32_t)isr28, 0x08, 0x8E));
    encodeIDT(&idt_entries[29*8 - 1], createIDT((uint32_t)isr29, 0x08, 0x8E));
    encodeIDT(&idt_entries[30*8 - 1], createIDT((uint32_t)isr30, 0x08, 0x8E));
    encodeIDT(&idt_entries[31*8 - 1], createIDT((uint32_t)isr31, 0x08, 0x8E));

    asm("lidt (%0)" : :"r" ((uint8_t*)(&idt_ptr)));
}
