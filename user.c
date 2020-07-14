/*
 * Copyright (C) 2019 TheFloW
 *
 * This software may be modified and distributed under the terms
 * of the MIT license.  See the LICENSE file for details.
 */


#include <psp2/kernel/modulemgr.h>

#include <stdio.h>
#include <string.h>

#include "minfwver.h"


int minfwver_get_factory_firmware(void) {
	return k_minfwver_get_factory_firmware();
}

void _start() __attribute__ ((weak, alias("module_start")));
int module_start(SceSize args, void *argp) {
	return SCE_KERNEL_START_SUCCESS;
}

int module_stop(SceSize args, void *argp) {
	return SCE_KERNEL_STOP_SUCCESS;
}