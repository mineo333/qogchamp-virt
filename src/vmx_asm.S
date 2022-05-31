#include <asm/asm.h>
#include <linux/linkage.h>
#include <uapi/asm/processor-flags.h>
.intel_syntax noprefix



#this entire file is intended to contain assembly utility functions for everything else. 


.section .text, "ax"

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

#RDI contains physical pointer

SYM_FUNC_START(vmx_on)
    xor rax, rax
    push rdi    
    vmxon [rsp]
    setb al
    add rsp, 8
    ret

SYM_FUNC_END(vmx_on)

SYM_FUNC_START(vmx_off)
    vmxoff
    ret

SYM_FUNC_END(vmx_off)

SYM_FUNC_START(enable_vtx_cr4)

mov rax, cr4
or rax, X86_CR4_VMXE
mov cr4, rax
ret

SYM_FUNC_END(enable_vtx_cr4)