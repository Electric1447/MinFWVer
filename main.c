/* 
 * Copyright (C) 2021 Electric1447
 *
 * This software may be modified and distributed under the terms
 * of the MIT license.  See the LICENSE file for details.
 */
#pragma GCC diagnostic ignored "-Wimplicit-function-declaration"


#include <psp2/ctrl.h>
#include <psp2/vshbridge.h>
#include <psp2/kernel/modulemgr.h>
#include <psp2/shellutil.h>

#include <stdio.h>
#include <string.h>
#include <malloc.h>

#include "pspdebug.h"


#define printf psvDebugScreenPrintf

#define WHITE  0x00FFFFFF
#define YELLOW 0x0000FFFF


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
	sceShellUtilInitEvents(0);
	sceShellUtilLock(SCE_SHELL_UTIL_LOCK_TYPE_PS_BTN);

	printf("-- Min Firmware Version Checker v2.1\n");
	printf("   MinFWVer by Electric1447\n\n");

	SceKernelFwInfo fwinfo, fwinfo2;
	int fwinfo3 = 0;
	
	fwinfo.size = sizeof(SceKernelFwInfo);
	fwinfo2.size = sizeof(SceKernelFwInfo);
	
	sceKernelGetSystemSwVersion(&fwinfo);
	_vshSblGetSystemSwVersion(&fwinfo2);
	_vshSblAimgrGetSMI(&fwinfo3);

	char factory_fw[8];
	firmware_string(factory_fw, (unsigned int)fwinfo3);

	printf("Firmware information:\n");
	printf(" - Spoofed firmware: ");
	psvDebugScreenSetTextColor(YELLOW);
	printf("%s", fwinfo.versionString);
	psvDebugScreenSetTextColor(WHITE);
	printf("\n");
	printf(" - Current firmware: ");
	psvDebugScreenSetTextColor(YELLOW);
	printf("%s", fwinfo2.versionString);
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

	sceShellUtilUnlock(SCE_SHELL_UTIL_LOCK_TYPE_PS_BTN);
	sceKernelPowerUnlock(0);
	sceKernelExitProcess(0);
	return 0;
}