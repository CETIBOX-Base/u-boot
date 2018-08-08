/*
 * board/renesas/h3vc2/h3vc2.c
 *     This file is H3VC2 board support.
 *
 * Copyright (C) 2018 Renesas Electronics Corporation
 * Copyright (C) 2018 Cogent Embedded, Inc.
 *
 * SPDX-License-Identifier: GPL-2.0+
 */

#include <common.h>
#include <malloc.h>
#include <netdev.h>
#include <dm.h>
#include <dm/platform_data/serial_sh.h>
#include <asm/processor.h>
#include <asm/mach-types.h>
#include <asm/io.h>
#include <asm/errno.h>
#include <asm/arch/sys_proto.h>
#include <asm/gpio.h>
#include <asm/arch/prr_depend.h>
#include <asm/arch/gpio.h>
#include <asm/arch/rcar_gen3.h>
#include <asm/arch/rcar-mstp.h>
#include <asm/arch/sh_sdhi.h>
#include <i2c.h>
#include <mmc.h>

DECLARE_GLOBAL_DATA_PTR;

#define SCIF2_MSTP310	(1 << 10)
#define DVFS_MSTP926	(1 << 26)
#define RPC_MSTP917	(1 << 17)
#define SD0_MSTP314	(1 << 14)
#define SD1_MSTP313	(1 << 13)
#define SD2_MSTP312	(1 << 12)
#define SD3_MSTP311	(1 << 11)

#define SD0CKCR		0xE6150074
#define SD1CKCR		0xE6150078
#define SD2CKCR		0xE6150268
#define SD3CKCR		0xE615026C

int board_early_init_f(void)
{
	int freq;

	rcar_prr_init();

	/* SCIF2 */
	mstp_clrbits_le32(SMSTPCR3, SMSTPCR3, SCIF2_MSTP310);
	mstp_wait_clrbits_le32(MSTPSR3, SCIF2_MSTP310);
	/* RPC */
	mstp_clrbits_le32(SMSTPCR9, SMSTPCR9, RPC_MSTP917);
	mstp_wait_clrbits_le32(MSTPSR9, RPC_MSTP917);
	/* eMMC */
	mstp_clrbits_le32(SMSTPCR3, SMSTPCR3, SD1_MSTP313 | SD2_MSTP312);
	mstp_wait_clrbits_le32(MSTPSR3, SD1_MSTP313 | SD2_MSTP312);
	/* SDHI0 */
	mstp_clrbits_le32(SMSTPCR3, SMSTPCR3, SD0_MSTP314);
	mstp_wait_clrbits_le32(MSTPSR3, SD0_MSTP314);

	freq = rcar_get_sdhi_config_clk();
	writel(freq, SD0CKCR);
	writel(freq, SD1CKCR);
	writel(freq, SD2CKCR);
	writel(freq, SD3CKCR);

#if defined(CONFIG_SYS_I2C) && defined(CONFIG_SYS_I2C_SH)
	/* DVFS for reset */
	mstp_clrbits_le32(SMSTPCR9, SMSTPCR9, DVFS_MSTP926);
	mstp_wait_clrbits_le32(MSTPSR9, DVFS_MSTP926);
#endif
	return 0;
}

DECLARE_GLOBAL_DATA_PTR;

int board_init(void)
{
	/* adress of boot parameters */
	gd->bd->bi_boot_params = CONFIG_SYS_TEXT_BASE + 0x50000;

	/* Init PFC controller */
	pinmux_init();

	return 0;
}

int board_mmc_init(bd_t *bis)
{
	int ret = -ENODEV;

#ifdef CONFIG_SH_SDHI
	/* SDHI0 */
	gpio_request(GPIO_GFN_SD0_DAT0, NULL);
	gpio_request(GPIO_GFN_SD0_DAT1, NULL);
	gpio_request(GPIO_GFN_SD0_DAT2, NULL);
	gpio_request(GPIO_GFN_SD0_DAT3, NULL);
	gpio_request(GPIO_GFN_SD0_CLK, NULL);
	gpio_request(GPIO_GFN_SD0_CMD, NULL);
	gpio_request(GPIO_GFN_SD0_CD, NULL);
	gpio_request(GPIO_GFN_SD0_WP, NULL);

	gpio_request(GPIO_GP_5_2, NULL);
	gpio_request(GPIO_GP_5_1, NULL);
	gpio_direction_output(GPIO_GP_5_2, 1); /* power on */
	gpio_direction_output(GPIO_GP_5_1, 1); /* 1: 3.3V, 0: 1.8V */

	ret = sh_sdhi_init(CONFIG_SYS_SH_SDHI0_BASE, 0,
			   SH_SDHI_QUIRK_64BIT_BUF);
	if (ret)
		return ret;

	/* SDHI1/SDHI2, eMMC */
	gpio_request(GPIO_GFN_SD1_DAT0, NULL);
	gpio_request(GPIO_GFN_SD1_DAT1, NULL);
	gpio_request(GPIO_GFN_SD1_DAT2, NULL);
	gpio_request(GPIO_GFN_SD1_DAT3, NULL);
	gpio_request(GPIO_GFN_SD2_DAT0, NULL);
	gpio_request(GPIO_GFN_SD2_DAT1, NULL);
	gpio_request(GPIO_GFN_SD2_DAT2, NULL);
	gpio_request(GPIO_GFN_SD2_DAT3, NULL);
	gpio_request(GPIO_GFN_SD2_CLK, NULL);
	gpio_request(GPIO_GFN_SD2_CMD, NULL);

	gpio_request(GPIO_GP_5_3, NULL);
	gpio_request(GPIO_GP_5_9, NULL);
	gpio_direction_output(GPIO_GP_5_3, 0); /* 1: 3.3V, 0: 1.8V */
	gpio_direction_output(GPIO_GP_5_9, 0); /* 1: 3.3V, 0: 1.8V */

	ret = sh_sdhi_init(CONFIG_SYS_SH_SDHI2_BASE, 1,
			   SH_SDHI_QUIRK_64BIT_BUF);
#endif
	return ret;
}


int dram_init(void)
{
	gd->ram_size = PHYS_SDRAM_1_SIZE;
#if (CONFIG_NR_DRAM_BANKS >= 2)
	gd->ram_size += PHYS_SDRAM_2_SIZE;
#endif
#if (CONFIG_NR_DRAM_BANKS >= 3)
	gd->ram_size += PHYS_SDRAM_3_SIZE;
#endif
#if (CONFIG_NR_DRAM_BANKS >= 4)
	gd->ram_size += PHYS_SDRAM_4_SIZE;
#endif

	return 0;
}

void dram_init_banksize(void)
{
	gd->bd->bi_dram[0].start = PHYS_SDRAM_1;
	gd->bd->bi_dram[0].size = PHYS_SDRAM_1_SIZE;
#if (CONFIG_NR_DRAM_BANKS >= 2)
	gd->bd->bi_dram[1].start = PHYS_SDRAM_2;
	gd->bd->bi_dram[1].size = PHYS_SDRAM_2_SIZE;
#endif
#if (CONFIG_NR_DRAM_BANKS >= 3)
	gd->bd->bi_dram[2].start = PHYS_SDRAM_3;
	gd->bd->bi_dram[2].size = PHYS_SDRAM_3_SIZE;
#endif
#if (CONFIG_NR_DRAM_BANKS >= 4)
	gd->bd->bi_dram[3].start = PHYS_SDRAM_4;
	gd->bd->bi_dram[3].size = PHYS_SDRAM_4_SIZE;
#endif
}

const struct rcar_sysinfo sysinfo = {
	CONFIG_RCAR_BOARD_STRING
};

#if defined(CONFIG_DISPLAY_BOARDINFO)
int checkboard(void)
{
	printf("Board: %s\n", sysinfo.board_string);
	return 0;
}

void reset_cpu(ulong addr)
{
}
#endif
