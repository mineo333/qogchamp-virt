#ifndef __VIRT_ARCH_H
#define __VIRT_ARCH_H
//various architecture definitions :)

#include "depend.h"

struct gdt_ptr{
    u16 limit;
    u64 phys_ptr;
};




#endif