#include "vmx.h"

int vmx_support(void){
    return __vmx_support() & VMX_BIT;
}