#include "depend.h"

#include "util.h"







int virt_init(void){
    pr_info("Initializing hypervisor");
    
    return 0;
}

void virt_cleanup(void){
    pr_info("Cleaning up\n");

}


MODULE_AUTHOR("mineo333");
MODULE_DESCRIPTION("Playing around with Intel VT-x");

module_init(virt_init);
module_exit(virt_cleanup);
MODULE_LICENSE("GPL");
  