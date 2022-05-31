#include "vmx.h"
#include "vmx_region.h"
int vmx_support(void){
    return __vmx_support() & VMX_BIT;
}

static void vtx_feature_check(void){
	uint64_t feature_control;
	uint64_t required;
	unsigned long cr0;
	unsigned long cr4;
		/*
	 * Ensure bits in CR0 and CR4 are valid in VMX operation:
	 * - Bit X is 1 in _FIXED0: bit X is fixed to 1 in CRx.
	 * - Bit X is 0 in _FIXED1: bit X is fixed to 0 in CRx.
	 */
	__asm__ __volatile__("mov %%cr0, %0" : "=r"(cr0) : : "memory");
	cr0 &= rdmsr(MSR_IA32_VMX_CR0_FIXED1);
	cr0 |= rdmsr(MSR_IA32_VMX_CR0_FIXED0);
	__asm__ __volatile__("mov %0, %%cr0" : : "r"(cr0) : "memory");

	__asm__ __volatile__("mov %%cr4, %0" : "=r"(cr4) : : "memory");
	cr4 &= rdmsr(MSR_IA32_VMX_CR4_FIXED1);
	cr4 |= rdmsr(MSR_IA32_VMX_CR4_FIXED0);
	/* Enable VMX operation */
	cr4 |= X86_CR4_VMXE;
	__asm__ __volatile__("mov %0, %%cr4" : : "r"(cr4) : "memory");

	/*
	 * Configure IA32_FEATURE_CONTROL MSR to allow VMXON:
	 *  Bit 0: Lock bit. If clear, VMXON causes a #GP.
	 *  Bit 2: Enables VMXON outside of SMX operation. If clear, VMXON
	 *    outside of SMX causes a #GP.
	 */

	required = FEAT_CTL_VMX_ENABLED_OUTSIDE_SMX;
	required |= FEAT_CTL_LOCKED;
	feature_control = rdmsr(MSR_IA32_FEAT_CTL);
	if ((feature_control & required) != required)
		wrmsr(MSR_IA32_FEAT_CTL, feature_control | required);
}

static unsigned int vmcs_revision_id(){

}

static void setup_vmx_cpu(struct vmx_cpu* vmx_cpu){
	
}



static void __vmx_setup(void){ //this function **might** run in interrupt context because it is called via an IPI
    int cpu = get_cpu(); //mainly for disabling preemption
 


    struct vmx_region* region = alloc_vmx_region();

    if(!region){
        pr_info("Couldn't initalize CPU %d\n", cpu);
        goto out;
    }

    
	vtx_feature_check();



    

    pr_info("CPU %d region: 0x%llx\n", cpu, region -> phys_addr);
    
    if(vmx_on(region -> phys_addr)){
        pr_err("CPU %d failed to enter VMX root operation\n", cpu);
        return;
    }

    pr_info("CPU %d is now in VMX Root Operation!\n", cpu);

    vmx_off();

    
out:  
    if(region)
        free_vmx_region(region);

    put_cpu();
    
    
}

/*
This function should only be called from on_each_cpu
*/

void vmx_setup(void* info){ 
    __vmx_setup(); //info is always NULL
}