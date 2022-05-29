cmd_/home/mineo333/virt/src/vmx_asm.o := gcc -Wp,-MMD,/home/mineo333/virt/src/.vmx_asm.o.d -nostdinc -isystem /usr/lib/gcc/x86_64-linux-gnu/11/include -I./arch/x86/include -I./arch/x86/include/generated  -I./include -I./arch/x86/include/uapi -I./arch/x86/include/generated/uapi -I./include/uapi -I./include/generated/uapi -include ./include/linux/compiler-version.h -include ./include/linux/kconfig.h -I./ubuntu/include -D__KERNEL__ -fmacro-prefix-map=./= -D__ASSEMBLY__ -fno-PIE -m64 -DCONFIG_X86_X32_ABI -Wa,-gdwarf-2 -DCC_USING_FENTRY  -DMODULE  -c -o /home/mineo333/virt/src/vmx_asm.o /home/mineo333/virt/src/vmx_asm.S

source_/home/mineo333/virt/src/vmx_asm.o := /home/mineo333/virt/src/vmx_asm.S

deps_/home/mineo333/virt/src/vmx_asm.o := \
  include/linux/compiler-version.h \
    $(wildcard include/config/CC_VERSION_TEXT) \
  include/linux/kconfig.h \
    $(wildcard include/config/CPU_BIG_ENDIAN) \
    $(wildcard include/config/BOOGER) \
    $(wildcard include/config/FOO) \

/home/mineo333/virt/src/vmx_asm.o: $(deps_/home/mineo333/virt/src/vmx_asm.o)

$(deps_/home/mineo333/virt/src/vmx_asm.o):
