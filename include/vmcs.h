#ifndef __VIRT_VMCS_H
#define __VIRT_VMCS_H

#include "depend.h"

/*
The intel VMCS as well as the VMX strcture are technically opaque data structures as their structure may change between . The former should be accessed via the vmwrite and vmread instructions. 


*/

struct qogchamp_vmcs{
    void* addr;
    phys_addr_t phys_addr;

};

DECLARE_PER_CPU(struct qogchamp_vmcs*, percpu_vmcs);

struct qogchamp_vmcs* alloc_vmcs(void);
void free_vmcs(struct qogchamp_vmcs* ptr);

#endif

