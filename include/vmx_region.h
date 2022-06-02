#ifndef __VIRT_VMCS_H
#define __VIRT_VMCS_H

#include "depend.h"


/*
The intel VMCS as well as the VMX strcture are technically opaque data structures as their structure may change between . The former should be accessed via the vmwrite and vmread instructions. 
*/


struct vmcs_hdr{
    u32 revision_id:31;
    u32 shadow_vmcs:1;
};

struct vmx_on_region{
    u32 vmcs_revision_id;
    char data[]; //data should not be accessed
};

struct vmcs{
    struct vmcs_hdr vmcs_hdr;
    u32 abort;
    char data[];
};

struct vmx_region{ //this is used for both the VMXON region as well as the VMCS
    union{
        struct vmx_on_region* vmx_on; //both of these fields in the union are casted from a get_page
        struct vmcs* vmcs;
    };
    phys_addr_t phys_addr;
};

struct vmx_cpu{
    unsigned int cpu;
    struct vmx_region* vmcs; //this is probably questionable design and can probably be improved. 
    struct vmx_region* vmx_on;
};





DECLARE_PER_CPU(struct vmx_cpu*, percpu_vmx_cpu);

struct vmx_cpu* alloc_vmx_cpu(int cpu);
void free_vmx_cpu(struct vmx_cpu* vmx_cpu);

#endif

