#include "depend.h"
#include "util.h"
#include "vmx.h"
#include "vmx_region.h" //for vmx_cpu type


int virt_setup(){
    vmx_cpu_pcpu = alloc_per_cpu(struct vmx_cpu*);
    if(!vmx_cpu_pcpu){
        return -ENOMEM;
    }

    vmx_on_pcpu = alloc_per_cpu(int);

    if(!vmx_on_pcpu){
        return -ENOMEM;
    }

}

int virt_init(void){

    if(virt_setup()){
        return -ENOMEM;
    }
    
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

    free_per_cpu(vmx_cpu_pcpu);
    free_per_cpu(vmx_on_pcpu);

    pr_info("Cleaning up\n");

}


MODULE_AUTHOR("mineo333");
MODULE_DESCRIPTION("Playing around with Intel VT-x");

module_init(virt_init);
module_exit(virt_cleanup);
MODULE_LICENSE("GPL");
  