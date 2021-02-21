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

#endif //GDT_H
