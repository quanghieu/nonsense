/*
 *  linux/include/linux/clk.h
 *
 *  Copyright (C) 2004 ARM Limited.
 *  Written by Deep Blue Solutions Limited.
 *  Copyright (C) 2011-2012 Linaro Ltd <mturquette@linaro.org>
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 as
 * published by the Free Software Foundation.
 */
#ifndef __LINUX_NV_H
#define __LINUX_NV_H

extern void __attribute__((section(".mct"))) (*NvOps) (uint64_t);
extern unsigned long __attribute__((section(".mct"))) addr_tpmTime;

#endif
