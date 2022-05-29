#ifndef __VIRT_VMCS_H
#define __VIRT_VMCS_H

#include "depend.h"

struct vmcs_hdr{
    u32 revision_id:31;
    u32 shadow_vmcs:1;
};

struct vmcs{
    struct vmcs_hdr hdr;
    u32 abort;
    char data[]    
};   

DECLARE_PERCPU(struct vmcs*, percpu_vmcs);

struct vmcs* alloc_vmcs(void);
void free_vmcs(struct vmcs* ptr);

#endif

