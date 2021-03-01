#include "isr.h"
#include "print.h"

void isr_handler(struct registers regs) {
    printToMonitor("recieved interrupt: \n");
    printIntToMonitor(regs.int_no, 10);
    printToMonitor("\n");
}
