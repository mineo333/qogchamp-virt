#include "depend.h"
#include "util.h"
#include "vmx.h"
#include "vmx_region.h" //for vmx_cpu type


struct vmx_cpu __percpu **vmx_cpu_pcpu;

struct vmx_region __percpu **valid_vmcs_pcpu; 

int __percpu *vmx_on_pcpu; 

static int virt_setup(void){
    vmx_cpu_pcpu = alloc_percpu(struct vmx_cpu*);
    if(!vmx_cpu_pcpu){
        return -ENOMEM;
    }

    vmx_on_pcpu = alloc_percpu(int);

    if(!vmx_on_pcpu){
        return -ENOMEM;
    }

    valid_vmcs_pcpu = alloc_percpu(struct vmx_region*);

    if(!valid_vmcs_pcpu){
        return -ENOMEM;
    }

    return 0;

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

    free_percpu(vmx_cpu_pcpu);
    free_percpu(vmx_on_pcpu);

    pr_info("Cleaning up\n");

}


MODULE_AUTHOR("mineo333");
MODULE_DESCRIPTION("Playing around with Intel VT-x");

module_init(virt_init);
module_exit(virt_cleanup);
MODULE_LICENSE("GPL");
  