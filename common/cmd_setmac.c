/*
 * Copyright 2017
 * Platina Systems, Inc.
 *
 */

#include <common.h>
#include <command.h>

static int do_setmac(cmd_tbl_t *cmdtp, int flag, int argc, char * const argv[])
{
	int i;
	char *memaddr;
	uint onie;
	char b0, b1, b2, b3, b4, b5;
	char ethaddr[20];

	memaddr = (char *)simple_strtol(argv[1], NULL, 16);  
	onie = (uint)simple_strtol(argv[2], NULL, 16);  
	for (i = 0; i < 0x200; i++) {
		if (*memaddr == onie) {
			memaddr++;
			memaddr++;
			b0 = *memaddr++;
			b1 = *memaddr++;
			b2 = *memaddr++;
			b3 = *memaddr++;
			b4 = *memaddr++;
			b5 = *memaddr++;
			sprintf(ethaddr, "%02x:%02x:%02x:%02x:%02x:%02x", b0, b1, b2, b3, b4, b5);
			break;
		}
		memaddr++;
	}
	setenv("ethaddr", ethaddr);
	return 0;
}

U_BOOT_CMD(
	setmac,	CONFIG_SYS_MAXARGS,	1,	do_setmac,
	"setenv of 'ethaddr' based on address found in memory",
	"\n"
	"setmac MEM_ADDR ONIE_ID\n"
	"	- reads through memory looking for onie identifier\n"
	"          then sets 'ethaddr' to value found in memory\n"
);
