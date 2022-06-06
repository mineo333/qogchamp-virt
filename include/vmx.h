#ifndef __VIRT_VMX_H
#define __VIRT_VMX_H

#include "depend.h"
#include "percpu_ops.h"

#undef rdmsr //undef so we can use our own definitions :)
#undef wrmsr

#define VMX_BIT 1 << 5

#define REVISION_ID_MASK ((1 << 31) - 1);

extern int __percpu *vmx_on_pcpu; //should be 1 for percpu var if we are in vmx root operation. Otherwise, 0

/*BEGIN ASSEMBLY HELPER PROTOTYPES*/

u8 __vmx_on(phys_addr_t page_addr);

void __vmx_off(void);

u8 __vm_write(u64 field, u64 value);

int __vmx_support(void);


/*END ASSEMBLY HELPER PROTOTYPES*/

static inline u8 vmx_on(phys_addr_t page_addr){
    
    if(this_cpu_read(vmx_on_pcpu)){
        pr_err("vmxon called in vmx root operation!");
        return 0;
    }  

    return __vmx_on(page_addr);    
}

static inline void vmx_off(void){
    if(!this_cpu_read(vmx_on_pcpu)){ //if vmx_on_pcpu is NULL, leave as we are not in vmx root operation. 
        pr_err("vmxoff called in non-vmx-root context!\n");
        return;
    }

    __vmx_off();

}


static inline uint64_t rdmsr(uint32_t msr)
{
	uint32_t a, d;

	__asm__ __volatile__("rdmsr" : "=a"(a), "=d"(d) : "c"(msr) : "memory");

	return a | ((uint64_t) d << 32);
}

static inline void wrmsr(uint32_t msr, uint64_t value)
{
	uint32_t a = value;
	uint32_t d = value >> 32;

	__asm__ __volatile__("wrmsr" :: "a"(a), "d"(d), "c"(msr) : "memory");
}

static inline u8 vm_write(u64 field, u64 value){
    return 0;
}   

static inline int vmx_support(void){
    return __vmx_support() & VMX_BIT;
}



void vmx_setup(void* info);


/*

Everything below is from:  https://github.com/LordNoteworthy/cpu-internals/master/headers/vmx.h

*/

// Updated to reflect Intel SDM of January 2019

// Appendix B Field Encoding in VMCS
enum VMCS_ENCODING
{
    VIRTUAL_PROCESSOR_ID = 0x00000000,  // 16-Bit Control Field
    POSTED_INTERRUPT_NOTIFICATION = 0x00000002,
    EPTP_INDEX = 0x00000004,
    GUEST_ES_SELECTOR = 0x00000800,  // 16-Bit Guest-State Fields
    GUEST_CS_SELECTOR = 0x00000802,
    GUEST_SS_SELECTOR = 0x00000804,
    GUEST_DS_SELECTOR = 0x00000806,
    GUEST_FS_SELECTOR = 0x00000808,
    GUEST_GS_SELECTOR = 0x0000080a,
    GUEST_LDTR_SELECTOR = 0x0000080c,
    GUEST_TR_SELECTOR = 0x0000080e,
    GUEST_INTERRUPT_STATUS = 0x00000810,
    HOST_ES_SELECTOR = 0x00000c00,  // 16-Bit Host-State Fields
    HOST_CS_SELECTOR = 0x00000c02,
    HOST_SS_SELECTOR = 0x00000c04,
    HOST_DS_SELECTOR = 0x00000c06,
    HOST_FS_SELECTOR = 0x00000c08,
    HOST_GS_SELECTOR = 0x00000c0a,
    HOST_TR_SELECTOR = 0x00000c0c,
    IO_BITMAP_A = 0x00002000,  // 64-Bit Control Fields
    IO_BITMAP_A_HIGH = 0x00002001,
    IO_BITMAP_B = 0x00002002,
    IO_BITMAP_B_HIGH = 0x00002003,
    MSR_BITMAP = 0x00002004,
    MSR_BITMAP_HIGH = 0x00002005,
    VM_EXIT_MSR_STORE_ADDR = 0x00002006,
    VM_EXIT_MSR_STORE_ADDR_HIGH = 0x00002007,
    VM_EXIT_MSR_LOAD_ADDR = 0x00002008,
    VM_EXIT_MSR_LOAD_ADDR_HIGH = 0x00002009,
    VM_ENTRY_MSR_LOAD_ADDR = 0x0000200a,
    VM_ENTRY_MSR_LOAD_ADDR_HIGH = 0x0000200b,
    EXECUTIVE_VMCS_POINTER = 0x0000200c,
    EXECUTIVE_VMCS_POINTER_HIGH = 0x0000200d,
    TSC_OFFSET = 0x00002010,
    TSC_OFFSET_HIGH = 0x00002011,
    VIRTUAL_APIC_PAGE_ADDR = 0x00002012,
    VIRTUAL_APIC_PAGE_ADDR_HIGH = 0x00002013,
    APIC_ACCESS_ADDR = 0x00002014,
    APIC_ACCESS_ADDR_HIGH = 0x00002015,
    EPT_POINTER = 0x0000201a,
    EPT_POINTER_HIGH = 0x0000201b,
    EOI_EXIT_BITMAP_0 = 0x0000201c,
    EOI_EXIT_BITMAP_0_HIGH = 0x0000201d,
    EOI_EXIT_BITMAP_1 = 0x0000201e,
    EOI_EXIT_BITMAP_1_HIGH = 0x0000201f,
    EOI_EXIT_BITMAP_2 = 0x00002020,
    EOI_EXIT_BITMAP_2_HIGH = 0x00002021,
    EOI_EXIT_BITMAP_3 = 0x00002022,
    EOI_EXIT_BITMAP_3_HIGH = 0x00002023,
    EPTP_LIST_ADDRESS = 0x00002024,
    EPTP_LIST_ADDRESS_HIGH = 0x00002025,
    VMREAD_BITMAP_ADDRESS = 0x00002026,
    VMREAD_BITMAP_ADDRESS_HIGH = 0x00002027,
    VMWRITE_BITMAP_ADDRESS = 0x00002028,
    VMWRITE_BITMAP_ADDRESS_HIGH = 0x00002029,
    VIRTUALIZATION_EXCEPTION_INFO_ADDDRESS = 0x0000202a,
    VIRTUALIZATION_EXCEPTION_INFO_ADDDRESS_HIGH = 0x0000202b,
    XSS_EXITING_BITMAP = 0x0000202c,
    XSS_EXITING_BITMAP_HIGH = 0x0000202d,
    GUEST_PHYSICAL_ADDRESS = 0x00002400,  // 64-Bit Read-Only Data Field
    GUEST_PHYSICAL_ADDRESS_HIGH = 0x00002401,
    VMCS_LINK_POINTER = 0x00002800,  // 64-Bit Guest-State Fields
    VMCS_LINK_POINTER_HIGH = 0x00002801,
    GUEST_IA32_DEBUGCTL = 0x00002802,
    GUEST_IA32_DEBUGCTL_HIGH = 0x00002803,
    GUEST_IA32_PAT = 0x00002804,
    GUEST_IA32_PAT_HIGH = 0x00002805,
    GUEST_IA32_EFER = 0x00002806,
    GUEST_IA32_EFER_HIGH = 0x00002807,
    GUEST_IA32_PERF_GLOBAL_CTRL = 0x00002808,
    GUEST_IA32_PERF_GLOBAL_CTRL_HIGH = 0x00002809,
    GUEST_PDPTR0 = 0x0000280a,
    GUEST_PDPTR0_HIGH = 0x0000280b,
    GUEST_PDPTR1 = 0x0000280c,
    GUEST_PDPTR1_HIGH = 0x0000280d,
    GUEST_PDPTR2 = 0x0000280e,
    GUEST_PDPTR2_HIGH = 0x0000280f,
    GUEST_PDPTR3 = 0x00002810,
    GUEST_PDPTR3_HIGH = 0x00002811,
    HOST_IA32_PAT = 0x00002c00,  // 64-Bit Host-State Fields
    HOST_IA32_PAT_HIGH = 0x00002c01,
    HOST_IA32_EFER = 0x00002c02,
    HOST_IA32_EFER_HIGH = 0x00002c03,
    HOST_IA32_PERF_GLOBAL_CTRL = 0x00002c04,
    HOST_IA32_PERF_GLOBAL_CTRL_HIGH = 0x00002c05,
    PIN_BASED_VM_EXEC_CONTROL = 0x00004000,  // 32-Bit Control Fields
    CPU_BASED_VM_EXEC_CONTROL = 0x00004002,
    EXCEPTION_BITMAP = 0x00004004,
    PAGE_FAULT_ERROR_CODE_MASK = 0x00004006,
    PAGE_FAULT_ERROR_CODE_MATCH = 0x00004008,
    CR3_TARGET_COUNT = 0x0000400a,
    VM_EXIT_CONTROLS = 0x0000400c,
    VM_EXIT_MSR_STORE_COUNT = 0x0000400e,
    VM_EXIT_MSR_LOAD_COUNT = 0x00004010,
    VM_ENTRY_CONTROLS = 0x00004012,
    VM_ENTRY_MSR_LOAD_COUNT = 0x00004014,
    VM_ENTRY_INTR_INFO_FIELD = 0x00004016,
    VM_ENTRY_EXCEPTION_ERROR_CODE = 0x00004018,
    VM_ENTRY_INSTRUCTION_LEN = 0x0000401a,
    TPR_THRESHOLD = 0x0000401c,
    SECONDARY_VM_EXEC_CONTROL = 0x0000401e,
    PLE_GAP = 0x00004020,
    PLE_WINDOW = 0x00004022,
    VM_INSTRUCTION_ERROR = 0x00004400,  // 32-Bit Read-Only Data Fields
    VM_EXIT_REASON = 0x00004402,
    VM_EXIT_INTR_INFO = 0x00004404,
    VM_EXIT_INTR_ERROR_CODE = 0x00004406,
    IDT_VECTORING_INFO_FIELD = 0x00004408,
    IDT_VECTORING_ERROR_CODE = 0x0000440a,
    VM_EXIT_INSTRUCTION_LEN = 0x0000440c,
    VMX_INSTRUCTION_INFO = 0x0000440e,
    GUEST_ES_LIMIT = 0x00004800,  // 32-Bit Guest-State Fields
    GUEST_CS_LIMIT = 0x00004802,
    GUEST_SS_LIMIT = 0x00004804,
    GUEST_DS_LIMIT = 0x00004806,
    GUEST_FS_LIMIT = 0x00004808,
    GUEST_GS_LIMIT = 0x0000480a,
    GUEST_LDTR_LIMIT = 0x0000480c,
    GUEST_TR_LIMIT = 0x0000480e,
    GUEST_GDTR_LIMIT = 0x00004810,
    GUEST_IDTR_LIMIT = 0x00004812,
    GUEST_ES_AR_BYTES = 0x00004814,
    GUEST_CS_AR_BYTES = 0x00004816,
    GUEST_SS_AR_BYTES = 0x00004818,
    GUEST_DS_AR_BYTES = 0x0000481a,
    GUEST_FS_AR_BYTES = 0x0000481c,
    GUEST_GS_AR_BYTES = 0x0000481e,
    GUEST_LDTR_AR_BYTES = 0x00004820,
    GUEST_TR_AR_BYTES = 0x00004822,
    GUEST_INTERRUPTIBILITY_INFO = 0x00004824,
    GUEST_ACTIVITY_STATE = 0x00004826,
    GUEST_SMBASE = 0x00004828,
    GUEST_SYSENTER_CS = 0x0000482a,
    VMX_PREEMPTION_TIMER_VALUE = 0x0000482e,
    HOST_IA32_SYSENTER_CS = 0x00004c00,  // 32-Bit Host-State Field
    CR0_GUEST_HOST_MASK = 0x00006000,    // Natural-Width Control Fields
    CR4_GUEST_HOST_MASK = 0x00006002,
    CR0_READ_SHADOW = 0x00006004,
    CR4_READ_SHADOW = 0x00006006,
    CR3_TARGET_VALUE0 = 0x00006008,
    CR3_TARGET_VALUE1 = 0x0000600a,
    CR3_TARGET_VALUE2 = 0x0000600c,
    CR3_TARGET_VALUE3 = 0x0000600e,
    EXIT_QUALIFICATION = 0x00006400,  // Natural-Width Read-Only Data Fields
    IO_RCX = 0x00006402,
    IO_RSI = 0x00006404,
    IO_RDI = 0x00006406,
    IO_RIP = 0x00006408,
    GUEST_LINEAR_ADDRESS = 0x0000640a,
    GUEST_CR0 = 0x00006800,  // Natural-Width Guest-State Fields
    GUEST_CR3 = 0x00006802,
    GUEST_CR4 = 0x00006804,
    GUEST_ES_BASE = 0x00006806,
    GUEST_CS_BASE = 0x00006808,
    GUEST_SS_BASE = 0x0000680a,
    GUEST_DS_BASE = 0x0000680c,
    GUEST_FS_BASE = 0x0000680e,
    GUEST_GS_BASE = 0x00006810,
    GUEST_LDTR_BASE = 0x00006812,
    GUEST_TR_BASE = 0x00006814,
    GUEST_GDTR_BASE = 0x00006816,
    GUEST_IDTR_BASE = 0x00006818,
    GUEST_DR7 = 0x0000681a,
    GUEST_RSP = 0x0000681c,
    GUEST_RIP = 0x0000681e,
    GUEST_RFLAGS = 0x00006820,
    GUEST_PENDING_DBG_EXCEPTIONS = 0x00006822,
    GUEST_SYSENTER_ESP = 0x00006824,
    GUEST_SYSENTER_EIP = 0x00006826,
    HOST_CR0 = 0x00006c00,  // Natural-Width Host-State Fields
    HOST_CR3 = 0x00006c02,
    HOST_CR4 = 0x00006c04,
    HOST_FS_BASE = 0x00006c06,
    HOST_GS_BASE = 0x00006c08,
    HOST_TR_BASE = 0x00006c0a,
    HOST_GDTR_BASE = 0x00006c0c,
    HOST_IDTR_BASE = 0x00006c0e,
    HOST_IA32_SYSENTER_ESP = 0x00006c10,
    HOST_IA32_SYSENTER_EIP = 0x00006c12,
    HOST_RSP = 0x00006c14,
    HOST_RIP = 0x00006c16
};

// Appendix C VMX Basic Exit Reasons
enum VM_EXIT_REASON
{
    EXIT_REASON_EXCEPTION_NMI = 0,        // Exception or non-maskable interrupt (NMI).
    EXIT_REASON_EXTERNAL_INTERRUPT = 1,   // External interrupt.
    EXIT_REASON_TRIPLE_FAULT = 2,         // Triple fault.
    EXIT_REASON_INIT = 3,                 // INIT signal.
    EXIT_REASON_SIPI = 4,                 // Start-up IPI (SIPI).
    EXIT_REASON_IO_SMI = 5,               // I/O system-management interrupt (SMI).
    EXIT_REASON_OTHER_SMI = 6,            // Other SMI.
    EXIT_REASON_PENDING_INTERRUPT = 7,    // Interrupt window exiting.
    EXIT_REASON_NMI_WINDOW = 8,           // NMI window exiting.
    EXIT_REASON_TASK_SWITCH = 9,          // Task switch.
    EXIT_REASON_CPUID = 10,               // Guest software attempted to execute CPUID.
    EXIT_REASON_GETSEC = 11,              // Guest software attempted to execute GETSEC.
    EXIT_REASON_HLT = 12,                 // Guest software attempted to execute HLT.
    EXIT_REASON_INVD = 13,                // Guest software attempted to execute INVD.
    EXIT_REASON_INVLPG = 14,              // Guest software attempted to execute INVLPG.
    EXIT_REASON_RDPMC = 15,               // Guest software attempted to execute RDPMC.
    EXIT_REASON_RDTSC = 16,               // Guest software attempted to execute RDTSC.
    EXIT_REASON_RSM = 17,                 // Guest software attempted to execute RSM in SMM.
    EXIT_REASON_VMCALL = 18,              // Guest software executed VMCALL.
    EXIT_REASON_VMCLEAR = 19,             // Guest software executed VMCLEAR.
    EXIT_REASON_VMLAUNCH = 20,            // Guest software executed VMLAUNCH.
    EXIT_REASON_VMPTRLD = 21,             // Guest software executed VMPTRLD.
    EXIT_REASON_VMPTRST = 22,             // Guest software executed VMPTRST.
    EXIT_REASON_VMREAD = 23,              // Guest software executed VMREAD.
    EXIT_REASON_VMRESUME = 24,            // Guest software executed VMRESUME.
    EXIT_REASON_VMWRITE = 25,             // Guest software executed VMWRITE.
    EXIT_REASON_VMXOFF = 26,              // Guest software executed VMXOFF.
    EXIT_REASON_VMXON = 27,               // Guest software executed VMXON.
    EXIT_REASON_CR_ACCESS = 28,           // Control-register accesses.
    EXIT_REASON_DR_ACCESS = 29,           // Debug-register accesses.
    EXIT_REASON_IO_INSTRUCTION = 30,      // I/O instruction.
    EXIT_REASON_MSR_READ = 31,            // RDMSR. Guest software attempted to execute RDMSR.
    EXIT_REASON_MSR_WRITE = 32,           // WRMSR. Guest software attempted to execute WRMSR.
    EXIT_REASON_INVALID_GUEST_STATE = 33, // VM-entry failure due to invalid guest state.
    EXIT_REASON_MSR_LOADING = 34,         // VM-entry failure due to MSR loading.
    EXIT_REASON_RESERVED_35 = 35,         // Reserved
    EXIT_REASON_MWAIT_INSTRUCTION = 36,   // Guest software executed MWAIT.
    EXIT_REASOM_MTF = 37,                 // VM-exit due to monitor trap flag.
    EXIT_REASON_RESERVED_38 = 38,         // Reserved
    EXIT_REASON_MONITOR_INSTRUCTION = 39, // Guest software attempted to execute MONITOR.
    EXIT_REASON_PAUSE_INSTRUCTION = 40,   // Guest software attempted to execute PAUSE.
    EXIT_REASON_MACHINE_CHECK = 41,       // VM-entry failure due to machine-check.
    EXIT_REASON_RESERVED_42 = 42,         // Reserved
    EXIT_REASON_TPR_BELOW_THRESHOLD = 43, // TPR below threshold. Guest software executed MOV to CR8.
    EXIT_REASON_APIC_ACCESS = 44,         // APIC access. Guest software attempted to access memory at a physical address on the APIC-access page.
    EXIT_REASON_VIRTUALIZED_EIO = 45,     // EOI virtualization was performed for a virtual interrupt whose vector indexed a bit set in the EOIexit bitmap
    EXIT_REASON_XDTR_ACCESS = 46,         // Guest software attempted to execute LGDT, LIDT, SGDT, or SIDT.
    EXIT_REASON_TR_ACCESS = 47,           // Guest software attempted to execute LLDT, LTR, SLDT, or STR.
    EXIT_REASON_EPT_VIOLATION = 48,       // An attempt to access memory with a guest-physical address was disallowed by the configuration of the EPT paging structures.
    EXIT_REASON_EPT_MISCONFIG = 49,       // An attempt to access memory with a guest-physical address encountered a misconfigured EPT paging-structure entry.
    EXIT_REASON_INVEPT = 50,              // Guest software attempted to execute INVEPT.
    EXIT_REASON_RDTSCP = 51,              // Guest software attempted to execute RDTSCP.
    EXIT_REASON_PREEMPT_TIMER = 52,       // VMX-preemption timer expired. The preemption timer counted down to zero.
    EXIT_REASON_INVVPID = 53,             // Guest software attempted to execute INVVPID.
    EXIT_REASON_WBINVD = 54,              // Guest software attempted to execute WBINVD
    EXIT_REASON_XSETBV = 55,              // Guest software attempted to execute XSETBV.
    EXIT_REASON_APIC_WRITE = 56,          // Guest completed write to virtual-APIC.
    EXIT_REASON_RDRAND = 57,              // Guest software attempted to execute RDRAND.
    EXIT_REASON_INVPCID = 58,             // Guest software attempted to execute INVPCID.
    EXIT_REASON_VMFUNC = 59,              // Guest software attempted to execute VMFUNC.
    EXIT_REASON_RESERVED_60 = 60,         // Reserved
    EXIT_REASON_RDSEED = 61,              // Guest software attempted to executed RDSEED and exiting was enabled.
    EXIT_REASON_RESERVED_62 = 62,         // Reserved
    EXIT_REASON_XSAVES = 63,              // Guest software attempted to executed XSAVES and exiting was enabled.
    EXIT_REASON_XRSTORS = 64,             // Guest software attempted to executed XRSTORS and exiting was enabled.
    EXIT_REASON_RESERVED_65 = 65,         // Reserved
    EXIT_REASON_SPP = 66,                 // The processor attempted to determine an access’s sub - page write permission and encountered
};

#endif
