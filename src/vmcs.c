#include "vmcs.h"
#include "util.h"


struct qogchamp_vmcs* alloc_vmcs(void){
    void* zero_page = (void*)get_zeroed_page(GFP_KERNEL); //the vmcs needs to be zeroed
    if(!zero_page)
        return NULL;
    struct qogchamp_vmcs* ret = kmalloc(sizeof(struct qogchamp_vmcs), GFP_KERNEL);
    ret -> addr = zero_page;
    ret -> phys_addr = __pa(zero_page); //get the physical address associated with the page
    return ret;
}

void free_vmcs(struct qogchamp_vmcs* ptr){

    BUG_ON(!ptr); 
    
    free_page((unsigned long)ptr -> addr);
    kfree(ptr);
    
}



