#include "vmx.h"
#include "vmx_region.h"
#include "percpu_ops.h"

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

static unsigned int vmcs_revision_id(void){
	return rdmsr(MSR_IA32_VMX_BASIC) & REVISION_ID_MASK; 
}

static void setup_vmx_on(struct vmx_cpu* vmx_cpu){
	struct vmx_on_region* vmx_on_region = vmx_cpu->vmx_on->vmx_on;
	vmx_on_region -> vmcs_revision_id = vmcs_revision_id();
}

static void setup_vmcs(struct vmx_cpu* vmx_cpu){
	struct vmcs* vmcs = vmx_cpu -> vmcs_region -> vmcs;
	phys_addr_t vmcs_addr = vmx_cpu -> vmcs_region -> phys_addr;
	
	
	
	if(vm_clear(vmcs_addr)){
		pr_err("vmclear failed\n");
		return;
	}

	vmcs -> vmcs_hdr.revision_id = vmcs_revision_id(); //setup revision id

	if(vm_ptrld(vmcs_addr)){
		pr_err("vmptrld failed\n");
		return;
	}

	pr_info("VMCS loaded!\n");


	
}

static void clear_vmcs(struct vmx_cpu* vmx_cpu){
	phys_addr_t vmcs_addr = vmx_cpu -> vmcs_region -> phys_addr;
	if(vm_clear(vmcs_addr)){
		pr_err("vmclear failed\n");
	}
	
}






static void __vmx_setup(void){ //this function **might** run in interrupt context because it is called via an IPI
    int cpu = get_cpu(); //mainly for disabling preemption
 


    struct vmx_cpu* vmx_cpu = alloc_vmx_cpu(cpu);

    if(!vmx_cpu){
        pr_info("Couldn't initalize CPU %d\n", cpu);
        goto out;
    }

    
	vtx_feature_check();

	setup_vmx_on(vmx_cpu);

	this_cpu_write(vmx_cpu_pcpu, vmx_cpu);


    

    pr_info("CPU %d region: 0x%llx\n", cpu, vmx_cpu -> vmx_on -> phys_addr);
    
    if(vmx_on(vmx_cpu -> vmx_on -> phys_addr)){
        pr_err("CPU %d failed to enter VMX root operation\n", cpu);
        goto out;
    }

	this_cpu_write(vmx_on_pcpu, 1);


    pr_info("CPU %d is now in VMX Root Operation!\n", cpu);

	setup_vmcs(vmx_cpu);

	clear_vmcs(vmx_cpu);

    vmx_off(); //if this faults it will be invalid opcode. so we'll die anyway :)

	this_cpu_write(vmx_on_pcpu, 0);

	pr_info("CPU %d has now exited VMX Root Operation!\n", cpu);

	

    
out:  
    if(vmx_cpu){
		this_cpu_write(vmx_cpu_pcpu, NULL);
		free_vmx_cpu(vmx_cpu);
		
	}
	
    put_cpu();
}

/*
This function should only be called from on_each_cpu
*/

void vmx_setup(void* info){ 
    __vmx_setup(); //info is always NULL
}