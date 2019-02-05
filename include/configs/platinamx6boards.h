/*
 * Copyright 2014 Freescale Semiconductor, Inc.
 *
 * Configuration settings for the Freescale i.MX6SX Sabresd board.
 *
 * SPDX-License-Identifier:	GPL-2.0+
 */


#ifndef __CONFIG_H
#define __CONFIG_H

#include "mx6_common.h"

#ifdef CONFIG_SPL
#define CONFIG_SPL_LIBCOMMON_SUPPORT
#define CONFIG_SPL_MMC_SUPPORT
#include "imx6_spl.h"
#endif

/* Size of malloc() pool */
#define CONFIG_SYS_MALLOC_LEN		(3 * SZ_1M)

#define CONFIG_BOARD_EARLY_INIT_F

#define CONFIG_MXC_UART
#define CONFIG_MXC_UART_BASE		UART1_BASE

#define CONFIG_EXTRA_ENV_SETTINGS \
  "baudrate=115200\0" \
  "bootargs=console=ttymxc0,115200n8 mem=1024m init=/init start ip=dhcp\0" \
  "bootcmd=run readmac;sf probe 0;sf read ${fdt_addr} 0x00080000 ${sz_fdt};sf read ${loadaddr} 0x00100000 ${sz_kernel};sf read ${initrd_addr} 0x00300000 ${sz_initrd};bootz ${loadaddr} ${initrd_addr} ${fdt_addr}\0" \
  "bootdelay=10\0" \
  "dlbmc=mw.b 80800000 00 00600000;run dw_hdr;run dw_uboot;run dw_fdt;run dw_kernel;run dw_initrd\0" \
  "dw_fdt=tftpboot 80880000 ${serverip}:platina-mk1-bmc.dtb;setenv sz_fdt ${filesize}\0" \
  "dw_hdr=tftpboot 80800400 ${serverip}:qspi-header-sckl00;setenv sz_hdr ${filesize}\0" \
  "dw_initrd=tftpboot 80B00000 ${serverip}:initrd.img.xz;setenv sz_initrd ${filesize}\0" \
  "dw_kernel=tftpboot 80900000 ${serverip}:zImage;setenv sz_kernel ${filesize}\0" \
  "dw_uboot=tftpboot 80801000 ${serverip}:u-boot.imx;setenv sz_uboot ${filesize}\0" \
  "ethact=FEC\0" \
  "ethprime=FEC\0" \
  "fdt_addr=0x88000000\0" \
  "fdt_high=0xffffffff\0" \
  "fileaddr=80b00000\0" \
  "filesize=1fa758\0" \
  "gatewayip=192.168.101.1\0" \
  "initrd_addr=0x89000000\0" \
  "initrd_high=0xffffffff\0" \
  "ipaddr=192.168.101.100\0" \
  "loadaddr=0x82000000\0" \
  "mask=255.255.255.0\0" \
  "netmask=255.255.255.0\0" \
  "qspi=sf probe;sf erase 0 00600000;sf erase fc0000 40000;sf write 80800000 0 00600000;saveenv\0" \
  "qspi0=mw 020e01b8 00000005; mw 20a8004 c7000000; mw 020a8000 4300ca05\0" \
  "qspi1=mw 020e01b8 00000005; mw 20a8004 c7000000; mw 020a8000 c300ca05\0" \
  "readmac=i2c read 55 0.2 200 80800000; setmac 80800000 24; saveenv\0" \
  "serverip=192.168.101.1\0" \
  "stderr=serial\0" \
  "stdin=serial\0" \
  "stdout=serial\0" \
  "sz_env=4000\0" \
  "sz_fdt=f000\0" \
  "sz_hdr=200\0" \
  "sz_initrd=300000\0" \
  "sz_kernel=200000\0" \
  "sz_uboot=5b718\0" \
  "wd=mw 020e01a0 00000005;mw 020e01a4 00000005;mw 020e01a8 00000005;mw 020e01b8 00000005;mw 020e01bc 00000005;mw 020a8000 0300ca05;mw 020a8004 07000000\0" \


#define CONFIG_BOOTCOMMAND

/* Miscellaneous configurable options */
#define CONFIG_CMD_MEMTEST
#define CONFIG_SYS_ALT_MEMTEST
#define CONFIG_SYS_MEMTEST_START	0x80000000
#define CONFIG_SYS_MEMTEST_END		(CONFIG_SYS_MEMTEST_START + 0x10000)

/* Physical Memory Map */
#define CONFIG_NR_DRAM_BANKS		1
#define PHYS_SDRAM			MMDC0_ARB_BASE_ADDR
#define PHYS_SDRAM_SIZE			SZ_1G

#define CONFIG_SYS_SDRAM_BASE		PHYS_SDRAM
#define CONFIG_SYS_INIT_RAM_ADDR	IRAM_BASE_ADDR
#define CONFIG_SYS_INIT_RAM_SIZE	IRAM_SIZE

#define CONFIG_SYS_INIT_SP_OFFSET \
	(CONFIG_SYS_INIT_RAM_SIZE - GENERATED_GBL_DATA_SIZE)
#define CONFIG_SYS_INIT_SP_ADDR \
	(CONFIG_SYS_INIT_RAM_ADDR + CONFIG_SYS_INIT_SP_OFFSET)

/* MMC Configuration */
#define CONFIG_SYS_FSL_ESDHC_ADDR	USDHC4_BASE_ADDR

/* I2C Configs */
#define CONFIG_CMD_I2C
#define CONFIG_SYS_I2C
#define CONFIG_SYS_I2C_MXC
#define CONFIG_SYS_I2C_MXC_I2C1		/* enable I2C bus 1 */
#define CONFIG_SYS_I2C_MXC_I2C2		/* enable I2C bus 2 */
#define CONFIG_SYS_I2C_MXC_I2C3		/* enable I2C bus 3 */
#define CONFIG_SYS_I2C_SPEED		  100000

/* PMIC */
#define CONFIG_POWER
#define CONFIG_POWER_I2C
#define CONFIG_POWER_PFUZE100
#define CONFIG_POWER_PFUZE100_I2C_ADDR	0x08

/* Network */
#define CONFIG_CMD_PING
#define CONFIG_CMD_DHCP
#define CONFIG_CMD_MII
#define CONFIG_FEC_MXC
#define CONFIG_MII

#define IMX_FEC_BASE			ENET_BASE_ADDR
#define CONFIG_FEC_MXC_PHYADDR          0x1

#define CONFIG_FEC_XCV_TYPE             RGMII
#define CONFIG_ETHPRIME                 "FEC"

/* USB Configs */
#ifdef CONFIG_CMD_USB
#define CONFIG_USB_EHCI
#define CONFIG_USB_EHCI_MX6
#define CONFIG_USB_STORAGE
#define CONFIG_EHCI_HCD_INIT_AFTER_RESET
#define CONFIG_USB_HOST_ETHER
#define CONFIG_USB_ETHER_ASIX
#define CONFIG_MXC_USB_PORTSC  (PORT_PTS_UTMI | PORT_PTS_PTW)
#define CONFIG_MXC_USB_FLAGS   0
#define CONFIG_USB_MAX_CONTROLLER_COUNT 2
#endif

#define CONFIG_CMD_PCI
#ifdef CONFIG_CMD_PCI
#define CONFIG_PCI
#define CONFIG_PCI_PNP
#define CONFIG_PCI_SCAN_SHOW
#define CONFIG_PCIE_IMX
/* #define CONFIG_PCIE_IMX_PERST_GPIO	IMX_GPIO_NR(2, 0) */
/* #define CONFIG_PCIE_IMX_POWER_GPIO	IMX_GPIO_NR(2, 1) */
#endif

#define CONFIG_IMX_THERMAL

#define CONFIG_CMD_TIME



#define CONFIG_ENV_SIZE                 SZ_8K


/* QSPI or MMC */
#ifdef CONFIG_SYS_BOOT_QSPI
#define CONFIG_ENV_IS_IN_SPI_FLASH
#else
#define CONFIG_ENV_IS_IN_MMC
#endif


/* adding SF commands */
#define CONFIG_FSL_QSPI


#ifdef CONFIG_FSL_QSPI
#define CONFIG_CMD_SF
#define CONFIG_SPI_FLASH_SPANSION
#define CONFIG_SPI_FLASH_STMICRO
#define CONFIG_SPI_FLASH_MACRONIX
#define CONFIG_SF_DEFAULT_BUS           0
#define CONFIG_SF_DEFAULT_CS            0
#define CONFIG_SF_DEFAULT_SPEED         40000000
#define CONFIG_SF_DEFAULT_MODE          SPI_MODE_0
#define CONFIG_SYS_FSL_QSPI_LE
#define CONFIG_SYS_FSL_QSPI_AHB
#define FSL_QSPI_FLASH_SIZE		SZ_64M
#define FSL_QSPI_FLASH_NUM		1
#endif


#ifdef CONFIG_ENV_IS_IN_MMC
#define CONFIG_ENV_OFFSET		(8 * SZ_64K)
#elif defined(CONFIG_ENV_IS_IN_SPI_FLASH)
#define CONFIG_ENV_OFFSET               (768 * 1024)
#define CONFIG_ENV_SECT_SIZE            SZ_256K
#define CONFIG_ENV_SPI_BUS              CONFIG_SF_DEFAULT_BUS
#define CONFIG_ENV_SPI_CS               CONFIG_SF_DEFAULT_CS
#define CONFIG_ENV_SPI_MODE             CONFIG_SF_DEFAULT_MODE
#define CONFIG_ENV_SPI_MAX_HZ           CONFIG_SF_DEFAULT_SPEED
#endif


#define CONFIG_SYS_FSL_USDHC_NUM	1
#ifdef CONFIG_ENV_IS_IN_MMC
#define CONFIG_SYS_MMC_ENV_DEV		0  /*USDHC4*/
#endif


#endif			/* __CONFIG_H */
