/*
 * include/configs/h3ulcb.h
 *     This file is VC2 board configuration.
 *     CPU r8a7795.
 *
 * Copyright (C) 2018 Renesas Electronics Corporation
 * Copyright (C) 2018 Cogent Embedded, Inc.
 *
 * SPDX-License-Identifier: GPL-2.0+
 */

#ifndef __H3VC2_H
#define __H3VC2_H

#undef DEBUG
#define CONFIG_RCAR_BOARD_STRING "H3VC2"
#define CONFIG_RCAR_TARGET_STRING "r8a7795"

#include "rcar-gen3-common.h"

/* SCIF */
#define CONFIG_SCIF_CONSOLE
#define CONFIG_CONS_SCIF2
#define CONFIG_SH_SCIF_CLK_FREQ	CONFIG_S3D4_CLK_FREQ

/* [A] Hyper Flash */
/* use to RPC(SPI Multi I/O Bus Controller) */
/* #define CONFIG_RPC_HYPERFLASH */

#define CONFIG_SYS_NO_FLASH



#define CONFIG_CMD_FLASH

/* Disable Ethernet RAVB */
#undef CONFIG_RAVB
#undef CONFIG_CMD_NET
#undef CONFIG_CMD_MII
#undef CONFIG_CMD_PING
#undef CONFIG_CMD_DHCP
#undef CONFIG_CMD_NFS

/* Board Clock */
/* XTAL_CLK : 33.33MHz */
#define RCAR_XTAL_CLK		33333333u
#define CONFIG_SYS_CLK_FREQ	RCAR_XTAL_CLK
/* ch0to2 CPclk, ch3to11 S3D2_PEREclk, ch12to14 S3D2_RTclk */
/* CPclk 16.66MHz, S3D2 133.33MHz , S3D4 66.66MHz */
#define CONFIG_CP_CLK_FREQ	(CONFIG_SYS_CLK_FREQ / 2)
#define CONFIG_PLL1_CLK_FREQ	(CONFIG_SYS_CLK_FREQ * 192 / 2)
#define CONFIG_S3D2_CLK_FREQ	(266666666u/2)
#define CONFIG_S3D4_CLK_FREQ	(266666666u/4)

/* Generic Timer Definitions (use in assembler source) */
#define COUNTER_FREQUENCY	0xFE502A /* 16.66MHz from CPclk */

/* Generic Interrupt Controller Definitions */
#define GICD_BASE	(0xF1010000)
#define GICC_BASE	(0xF1020000)
#define CONFIG_GICV2

/* i2c */
#define CONFIG_SYS_I2C
#define CONFIG_SYS_I2C_SH
#define CONFIG_SYS_I2C_SLAVE		0x60
#define CONFIG_SYS_I2C_SH_NUM_CONTROLLERS	1
#define CONFIG_SYS_I2C_SH_SPEED0	400000
#define CONFIG_SH_I2C_DATA_HIGH		4
#define CONFIG_SH_I2C_DATA_LOW		5
#define CONFIG_SH_I2C_CLOCK		10000000

#define CONFIG_SYS_I2C_POWERIC_ADDR	0x30

/* USB */
#define CONFIG_USB_STORAGE
#define CONFIG_USB_EHCI
#define CONFIG_USB_EHCI_RCAR_GEN3
#define CONFIG_USB_MAX_CONTROLLER_COUNT	2

/* SDHI */
#define CONFIG_MMC
#define CONFIG_CMD_MMC
#define CONFIG_GENERIC_MMC
#define CONFIG_SH_SDHI_FREQ	200000000
#define CONFIG_SH_SDHI_MMC

/* Environment in eMMC, at the end of 2nd "boot sector" */
#define CONFIG_ENV_IS_IN_MMC
#define CONFIG_ENV_OFFSET	(-CONFIG_ENV_SIZE)
#define CONFIG_SYS_MMC_ENV_DEV	1
#define CONFIG_SYS_MMC_ENV_PART	2

/* Module clock supply/stop status bits */
/* MFIS */
#define CONFIG_SMSTP2_ENA	0x00002000
/* serial(SCIF2) */
#define CONFIG_SMSTP3_ENA	0x00000400
/* INTC-AP, INTC-EX */
#define CONFIG_SMSTP4_ENA	0x00000180

/* ENV setting */
#define CONFIG_ENV_OVERWRITE
#define CONFIG_ENV_SECT_SIZE	(128 * 1024)
#define CONFIG_ENV_SIZE		(CONFIG_ENV_SECT_SIZE)
#define CONFIG_ENV_SIZE_REDUND	(CONFIG_ENV_SIZE)

#define CONFIG_EXTRA_ENV_SETTINGS \
	"fdt_high=0xffffffffffffffff\0" \
	"initrd_high=0xffffffffffffffff\0"

#define CONFIG_BOOTARGS \
	""

#define CONFIG_BOOTCOMMAND \
	""

#endif /* __H3VC2_H */
