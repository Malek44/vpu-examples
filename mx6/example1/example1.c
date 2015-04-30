/*
 * Copyright 2004-2015 Freescale Semiconductor, Inc.
 *
 * Copyright (c) 2006, Chips & Media.  All rights reserved.
 */

/*
 * The code contained herein is licensed under the GNU General Public
 * License. You may obtain a copy of the GNU General Public License
 * Version 2 or later at the following locations:
 *
 * http://www.opensource.org/licenses/gpl-license.html
 * http://www.gnu.org/copyleft/gpl.html
 */

#include <sys/stat.h>
#include <sys/time.h>
#include <sys/ioctl.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <signal.h>
#include <getopt.h>

#include <linux/videodev2.h>
#include <pthread.h>
#include <errno.h>
#include <stdint.h>
#include <semaphore.h>

#include "mxc_ipu_hl_lib.h"
#include "vpu_lib.h"
#include "vpu_io.h"
#include "../vpu_test.h"

void main(void) {

	vpu_versioninfo ver;
	int err, ret;
	struct decode *dec;

	vpu_mem_desc mem_desc = {0};
	vpu_mem_desc ps_mem_desc = {0};
	vpu_mem_desc slice_mem_desc = {0};
	vpu_mem_desc vp8_mbparam_mem_desc = {0};

	err = vpu_Init(NULL);
	if (err) {
		printf("VPU Init Failure.\n");
		return;
	}

	err = vpu_GetVersionInfo(&ver);
	if (err) {
		printf("Cannot get version info, err:%d\n", err);
		vpu_UnInit();
		return;
	}

	printf("VPU firmware version: %d.%d.%d_r%d\n", ver.fw_major, ver.fw_minor,
						ver.fw_release, ver.fw_code);
	printf("VPU library version: %d.%d.%d\n", ver.lib_major, ver.lib_minor,
						ver.lib_release);

// from dec file
	dec = (struct decode *)calloc(1, sizeof(struct decode));
	if (dec == NULL) {
		printf("Failed to allocate decode structure\n");
		ret = -1;
	}

	mem_desc.size = STREAM_BUF_SIZE;
	ret = IOGetPhyMem(&mem_desc);
	if (ret) {
		printf("Unable to obtain physical mem\n");
		goto err;
	}

	if (IOGetVirtMem(&mem_desc) <= 0) {
		printf("Unable to obtain virtual mem\n");
		ret = -1;
		goto err;
	}

	dec->phy_bsbuf_addr = mem_desc.phy_addr;
	dec->virt_bsbuf_addr = mem_desc.virt_uaddr;

	dec->reorderEnable = 1;
	dec->tiled2LinearEnable = 0;

	dec->userData.enable = 0;
	dec->mbInfo.enable = 0;
	dec->mvInfo.enable = 0;
	dec->frameBufStat.enable = 0;
	dec->mjpgLineBufferMode = 0;

	//dec->cmdl = cmdl;

err:
	vpu_UnInit();

}
