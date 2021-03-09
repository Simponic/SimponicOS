#include "gdt.h"
#include "print.h"

extern void reloadSegments();

void encodeGDT(uint8_t* gdtEntry, struct GDT source) {
    if ((source.limit > 65536) && ((source.limit & 0xFFF) == 0xFFF)) {
        // Set the GDT to use paging
        // To do this we need to make sure the limit is aligned to 4KiB
        source.limit = source.limit >> 12;
        // Granularity: 1 (use paging with 4KiB segments)
        // Size: 1 (32 bit protected mode)
        gdtEntry[6] = 0xC0;
    }
    else if (source.limit == 0) {
        // Don't set any granularity for null entry
        gdtEntry[6] = 0;
    }
    else {
        // Granularity: 0 (1 byte segments)
        // Size: 1
        gdtEntry[6] = 0x40;
    }
    
    // Here we are encoding the limit

    // Bits 0-15 encode the bottom 16 bits of the limit
    gdtEntry[0] = source.limit & 0xFF;
    gdtEntry[1] = (source.limit >> 8) & 0xFF;
    // Bits 48-51 encode the last 4 bits of the limit
    gdtEntry[6] |= (source.limit >> 16) & 0xF;

    // Bits 16-39 encode the bottom 24 bits of the base
    gdtEntry[2] = source.base & 0xFF;
    gdtEntry[3] = (source.base >> 8) & 0xFF;
    gdtEntry[4] = (source.base >> 16) & 0xFF;
    // Bits 56-64 encode the last byte of the base
    gdtEntry[7] = (source.base >> 24) & 0xFF;

    // Bits 40-47 set the access byte, which is documented at https://wiki.osdev.org/GDT,
    // where most of the ideas for this function are taken from shamelessly
    gdtEntry[5] = source.type;
}

void initializeGDT() {
    struct GDT nullEntry;
    nullEntry.limit = 0;
    nullEntry.base  = 0;
    nullEntry.type  = 0;

    struct GDT codeSection;
    codeSection.limit = 0xFFFFFFFF;
    codeSection.base  = 0;
    codeSection.type  = 0x9A;

    struct GDT dataSection;
    dataSection.limit = 0xFFFFFFFF;
    dataSection.base  = 0;
    dataSection.type  = 0x92;

    encodeGDT(gdt_entries     , nullEntry  );
    encodeGDT(gdt_entries + 8 , codeSection);
    encodeGDT(gdt_entries + 16, dataSection);

    struct GDT_ptr gdt_ptr;
    gdt_ptr.limit = 8*3;
    gdt_ptr.base  = (uint32_t)(&gdt_entries);

    asm("lgdt (%0)" : :"r" ((uint8_t*)(&gdt_ptr)));

    reloadSegments();
}
