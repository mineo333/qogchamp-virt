#include <asm/asm.h>
#include <linux/linkage.h>
#include <uapi/asm/processor-flags.h>
.intel_syntax noprefix



#this entire file is intended to contain assembly utility functions for everything else. 


#TODO: introduce better error checking. If we are in VMX root operation, the CPU will return an error number which we can use to debug. 

.section .text, "ax"

#RDI contains physical pointer

SYM_FUNC_START(__vmx_support)
    push rbx
    push rcx
    push rdx
    mov rax, 1
    cpuid
    mov rax, rcx 
    pop rdx
    pop rcx
    pop rbx
    ret
SYM_FUNC_END(__vmx_support)

SYM_FUNC_START(__vmx_on)
    
    push rdi    
    vmxon [rsp]
    setbe al
    add rsp, 8
    ret

SYM_FUNC_END(__vmx_on)

SYM_FUNC_START(__vmx_off)
    vmxoff
    ret

SYM_FUNC_END(__vmx_off)



#vm_write semantics:
#   RDI contains field to write to. This value should originate from VMCS_ENCODING 
#   RSI contains value to write to field referenced by RDI.
#   on fail return 1
SYM_FUNC_START(__vm_write)
    vmwrite rdi, rsi
    setbe al
    ret
SYM_FUNC_END(__vm_write)

#vm_clear semantics:
#   RDI contains 

SYM_FUNC_START(__vm_clear)
    push rdi
    vmclear [rsp]
    setbe al
    add rsp, 8
    ret
SYM_FUNC_END(__vm_clear)



SYM_FUNC_START(__vm_read)


SYM_FUNC_END(__vm_read)




SYM_FUNC_START(get_cs_sel)
    mov ax, cs
    ret
SYM_FUNC_END(get_cs_sel)