/*

This file contains system dependencies. Is it bad design? Yea. Do I care? No. 

*/


#ifndef __VIRT_DEPEND_H
#define __VIRT_DEPEND_H

typedef unsigned char u8;

typedef unsigned short u16;

typedef unsigned int u32;

typedef unsigned long u64;


#include <linux/init.h>
#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/types.h>
#include <asm/io.h>
#include <linux/mm_types.h>
#include <linux/percpu-defs.h>
#include <asm/page.h>

#endif
