#include "vmcs.h"
#include "util.h"
#include "depend.h"


struct vmcs* alloc_vmcs(void){
    struct page* page = alloc_page(GFP_KERNEL);

    return (struct vmcs*)page_address(page);
}

void free_vmcs(struct vmcs* ptr){
    struct page* page = virt_to_page(ptr); //this works because page_address just does page -> virtual. 
    free_page(page);
}



