#include <linux/linkage.h>
#include <asm/asm.h>
.intel_syntax noprefix


.section .text, "ax"

SYM_FUNC_START(read_rcx)

    mov rax, rcx
    ret

SYM_FUNC_END(read_rcx)