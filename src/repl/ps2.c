/*
 * This file is part of Refera, the interpreted scripting language.
 *
 * Refera is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License as published
 * by the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * Refera is distributed in the hope that it will be useful, but WITHOUT
 * ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
 * FITNESS FOR A PARTICULAR PURPOSE.  See the GNU Lesser General Public
 * License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public License
 * along with Refera.  If not, see <https://www.gnu.org/licenses/>.
 *
 * Copyright (C) 2024  Ahmet Hakan Candar
 * Copyright (C) 2024  Berk Tahir Kılıç
 * Copyright (C) 2024  Barış Getiren
 * Copyright (C) 2024  Egemen Aybir
 */

#include "ps2.h"
#include "ps2_drivers.h"
#include <stdlib.h>

/* PS2SDK */
#include <loadfile.h>
#include <libpad.h>
#include <libkbd.h>
#include <delaythread.h>
#include <iopcontrol.h>
#include <libmc.h>
#include <sbv_patches.h>
#define NEWLIB_PORT_AWARE
#include <fileXio_rpc.h>
#include <fileio.h>
/* !PS2SDK */

#define PS2_PRINTF_BUF_LEN 256
#define PS2_INPUT_BUF_LEN 256

unsigned int ps2_line_idx = 1;
unsigned int ps2_col_idx = 1;

void ps2_load_modules()
{
	SifInitRpc(0);
	while (!SifIopReset("", 0)) { }
	while (!SifIopSync()) { }
	SifLoadFileInit();

	sbv_patch_enable_lmb();
	sbv_patch_disable_prefix_check();

	int ret; // TODO: check for errors
	SifExecModuleBuffer(iomanx, size_iomanx, 0, NULL, &ret);
	SifExecModuleBuffer(filexio, size_filexio, 0, NULL, &ret);
	SifLoadModule("rom0:SIO2MAN", 0, NULL);
	SifExecModuleBuffer(mcman, size_mcman, 0, NULL, &ret);
	SifExecModuleBuffer(mcserv, size_mcserv, 0, NULL, &ret);

	fileXioInit();
	fileXioSetRWBufferSize(128 * 1024);
	mcInit(MC_TYPE_MC);

	int dummy; // TODO: what was this?
	ret = SifExecModuleBuffer(usbd, size_usbd, 0, NULL, &dummy);
	ret = SifExecModuleBuffer(ps2kbd, size_ps2kbd, 0, NULL, &dummy);
	ret = PS2KbdInit();
}

void ps2_printf(const char* format, ...)
{
	char buffer[PS2_PRINTF_BUF_LEN];
	va_list args;
	va_start(args, format);
	vsnprintf(buffer, PS2_PRINTF_BUF_LEN, format, args);
	va_end(args);
	buffer[PS2_PRINTF_BUF_LEN - 1] = '\0'; // Ensure termination

	printf("print call\n");

	for (const char* chrp = buffer; *chrp != '\0'; chrp++)
	{
		if (*chrp == '\n')
		{
			ps2_line_idx++;
			ps2_col_idx = 1; // Carriage return
		}
		else
		{
			scr_putchar(ps2_col_idx * 8, ps2_line_idx * 8, 0xFFFFFF, *chrp);
			ps2_col_idx++;
		}
	}
}

char* ps2_fgets(char* str, int n, FILE* stream)
{
	int idx = 0;
	while (idx < n - 1)
	{
		char key;
		if (PS2KbdRead(&key) > 0)
		{
			ps2_printf("%c", key);
			str[idx] = key;
			idx++;

			if (key == '\n')
				break;
		}
	}

	str[idx] = '\0';

	return str;
}
