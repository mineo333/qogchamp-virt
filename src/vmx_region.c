#include "vmx_region.h"
#include "util.h"



static struct vmx_region* alloc_vmx_region(void){ //this function can be called in interrupt context. Read comment in __vmx_setup
    struct page* zero_page = alloc_page(GFP_KERNEL | __GFP_ZERO | GFP_ATOMIC); //the vmcs needs to be zeroed
    if(!zero_page)
        return NULL;
    struct vmx_region* ret = kmalloc(sizeof(struct vmx_region), GFP_KERNEL |GFP_ATOMIC);
    if(!ret)
        return NULL;
    ret -> vmx_on = page_address(zero_page); //this is union so it doesn't matter
    ret -> phys_addr = page_to_phys(zero_page); //get the physical address associated with the page
    return ret;
}

static void free_vmx_region(struct vmx_region* vmx_region){

    BUG_ON(!vmx_region); 
    BUG_ON(!vmx_region -> vmx_on); 

    free_page((unsigned long)vmx_region -> vmx_on);
    kfree(vmx_region);
    
}

struct vmx_cpu* alloc_vmx_cpu(int cpu){
    struct vmx_cpu* vmx_cpu = kmalloc(sizeof(vmx_cpu), GFP_KERNEL | GFP_ATOMIC);
    if(!vmx_cpu)
        return NULL;
    vmx_cpu -> cpu = cpu;

    vmx_cpu -> vmcs = alloc_vmx_region(); 

    if(!vmx_cpu -> vmcs){
        kfree(vmx_cpu);
    }
    vmx_cpu -> vmx_on = alloc_vmx_region();

    if(!vmx_cpu -> vmx_on){
        free_vmx_region(vmx_cpu -> vmcs);
        kfree(vmx_cpu);
    }

    


    return vmx_cpu;
    
}

void free_vmx_cpu(struct vmx_cpu* vmx_cpu){

    BUG_ON(vmx_cpu -> cpu != __smp_processor_id()); //the semantics of this are still up for question. Should this only be freeable on the same CPU? 

    BUG_ON(!vmx_cpu -> vmcs);
    BUG_ON(!vmx_cpu -> vmx_on);
    
    free_vmx_region(vmx_cpu -> vmcs);
    free_vmx_region(vmx_cpu -> vmx_on);
    kfree(vmx_cpu);
}





