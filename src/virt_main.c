#include "depend.h"
#include "util.h"
#include "vmx.h"





int virt_init(void){
    pr_info("Initializing hypervisor\n");
    if(vmx_support())
        pr_info("VMX Supported: true\n");
    else{
        pr_info("VMX not supported!\n");
        return -ENODEV;  
    }
    //if VT-x is supported we may continue
    on_each_cpu(vmx_setup, NULL, 1); 
   //vmx_setup(NULL);
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
  