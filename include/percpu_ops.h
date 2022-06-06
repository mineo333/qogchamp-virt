#ifndef __VIRT_PERCPU_H
#define __VIRT_PERCPU_H

#undef this_cpu_write
#undef this_cpu_read

/*
Fuck per-cpu ops, so I redid the macros. 
*/

#define this_cpu_write(pcp, nval)                    \
({                                                   \
    typeof(pcp) __this_cpu_ptr = this_cpu_ptr(pcp);  \
    *__this_cpu_ptr =  nval;                         \
})



#define this_cpu_read(pcp)                    \
({                                                  \
    *this_cpu_ptr(pcp);                             \
})  
#endif