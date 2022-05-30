#include <asm/asm.h>
#include <linux/linkage.h>
.intel_syntax noprefix


.section .text, "ax"

#heavy lifting for vmx

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

vmxon rdi
ret

SYM_FUNC_END(__vmx_on)