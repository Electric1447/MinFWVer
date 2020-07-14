/* main.c -- launcher
 *
 * Copyright (C) 2019 TheFloW
 *
 * This software may be modified and distributed under the terms
 * of the MIT license.  See the LICENSE file for details.
 */
#pragma GCC diagnostic ignored "-Wimplicit-function-declaration"


#include <psp2/ctrl.h>
#include <psp2/vshbridge.h>
#include <psp2/kernel/modulemgr.h>
#include <taihen.h>

#include <stdio.h>
#include <string.h>
#include <malloc.h>

#include "pspdebug.h"

#include "minfwver.h"


#define printf psvDebugScreenPrintf

#define APP_PATH "ux0:app/MINFW0000/"

#define WHITE  0x00FFFFFF
#define YELLOW 0x0000FFFF


static SceUID minfwver_kernel_id = -1, minfwver_user_id = -1;

void firmware_string(char string[8], unsigned int version) {
	char a = (version >> 24) & 0xf;
	char b = (version >> 20) & 0xf;
	char c = (version >> 16) & 0xf;
	char d = (version >> 12) & 0xf;

	memset(string, 0, 8);
	string[0] = '0' + a;
	string[1] = '.';
	string[2] = '0' + b;
	string[3] = '0' + c;
	string[4] = '\0';

	if (d) {
		string[4] = '0' + d;
		string[5] = '\0';
	}
}

int main(int argc, char *argv[]) {
	psvDebugScreenInit();
	sceKernelPowerLock(0);

	printf("-- Min Firmware Version Checker v1.0\n");
	printf("   MinFWVer by Electric1447, factory firmware code by TheFloW\n\n");

	if (sceIoDevctl("ux0:", 0x3001, NULL, 0, NULL, 0) == 0x80010030) {
		printf("Enable unsafe homebrew first before using this software.\n");
		
		sceKernelDelayThread(5*1000*1000);

		sceKernelPowerUnlock(0);
		sceKernelExitProcess(0);
	}

	// Load modules
	int search_unk[2];
	
	SceUID search_modid;
	search_modid  = _vshKernelSearchModuleByName("minfwver_kernel", search_unk);
	if (search_modid < 0) {
		minfwver_kernel_id = taiLoadKernelModule(APP_PATH "minfwver_kernel.skprx", 0, NULL);
		if (minfwver_kernel_id >= 0) {
			int res = taiStartKernelModule(minfwver_kernel_id, 0, NULL, 0, NULL, NULL);
			if (res < 0)
				taiStopUnloadKernelModule(minfwver_kernel_id, 0, NULL, 0, NULL, NULL);
		}
	}
	
	minfwver_user_id = sceKernelLoadStartModule(APP_PATH "minfwver_user.suprx", 0, NULL, 0, NULL, NULL);


	SceKernelFwInfo fwinfo;
	fwinfo.size = sizeof(SceKernelFwInfo);
	_vshSblGetSystemSwVersion(&fwinfo);

	unsigned int current_version = (unsigned int)fwinfo.version;
	unsigned int factory_version = minfwver_get_factory_firmware();

	char current_fw[8], factory_fw[8];
	firmware_string(current_fw, current_version);
	firmware_string(factory_fw, factory_version);

	printf("Firmware information:\n");
	printf(" - Current firmware: ");
	psvDebugScreenSetTextColor(YELLOW);
	printf("%s", current_fw);
	psvDebugScreenSetTextColor(WHITE);
	printf("\n");
	printf(" - Factory firmware: ");
	psvDebugScreenSetTextColor(YELLOW);
	printf("%s", factory_fw);
	psvDebugScreenSetTextColor(WHITE);
	printf("\n\n");

	
	SceCtrlData pad;
	printf("Press X to exit.\n\n");

	while (1) {
		sceCtrlPeekBufferPositive(0, &pad, 1);

		if (pad.buttons & SCE_CTRL_CROSS)
			break;
	}

	sceKernelPowerUnlock(0);
	sceKernelExitProcess(0);
	return 0;
}