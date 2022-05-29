#include "vmcs.h"
#include "util.h"


struct qogchamp_vmcs* alloc_vmcs(void){
    void* page = (void*)get_zeroed_page(GFP_KERNEL); //the vmcs needs to be zeroed
    if(!page)
        return NULL;
    struct qogchamp_vmcs* ret = kmalloc(sizeof(struct qogchamp_vmcs), GFP_KERNEL);
    ret -> addr = page_address(page);
    ret -> phys_addr = __pa(ret->addr);
    return ret;
}

void free_vmcs(struct qogchamp_vmcs* ptr){

    BUG_ON(!ptr); 
    
    free_page((unsigned long)ptr -> addr);
    kfree(ptr);
    
}



