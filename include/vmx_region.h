#ifndef __VIRT_VMCS_H
#define __VIRT_VMCS_H

#include "depend.h"


/*
The intel VMCS as well as the VMX strcture are technically opaque data structures as their structure may change between . The former should be accessed via the vmwrite and vmread instructions. 
*/




struct vmx_on_region{

};

struct vmcs{

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
    struct vmx_region* vmcs; 
    struct vmx_region* vmx_on;
};





DECLARE_PER_CPU(struct qogchamp_vmcs*, percpu_vmcs);

struct vmx_region* alloc_vmx_region(void);
void free_vmx_region(struct vmx_region* ptr);

#endif

