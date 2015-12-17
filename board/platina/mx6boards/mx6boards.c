/*
 * Copyright (C) 2014 Freescale Semiconductor, Inc.
 *
 * Author: Fabio Estevam <fabio.estevam@freescale.com>
 *
 * SPDX-License-Identifier:	GPL-2.0+
 */

#include <asm/arch/clock.h>
#include <asm/arch/crm_regs.h>
#include <asm/arch/iomux.h>
#include <asm/arch/imx-regs.h>
#include <asm/arch/mx6-pins.h>
#include <asm/arch/sys_proto.h>
#include <asm/gpio.h>
#include <asm/imx-common/iomux-v3.h>
#include <asm/io.h>
#include <asm/imx-common/mxc_i2c.h>
#include <linux/sizes.h>
#include <common.h>
#include <fsl_esdhc.h>
#include <mmc.h>
#include <i2c.h>
#include <miiphy.h>
#include <netdev.h>
#include <power/pmic.h>
#include <power/pfuze100_pmic.h>
#include "../../freescale/common/pfuze.h"
#include <usb.h>
#include <usb/ehci-fsl.h>

DECLARE_GLOBAL_DATA_PTR;

#define UART_PAD_CTRL  (PAD_CTL_PKE | PAD_CTL_PUE |		\
	PAD_CTL_PUS_100K_UP | PAD_CTL_SPEED_MED |		\
	PAD_CTL_DSE_40ohm   | PAD_CTL_SRE_FAST  | PAD_CTL_HYS)

#define USDHC_PAD_CTRL (PAD_CTL_PKE | PAD_CTL_PUE |		\
	PAD_CTL_PUS_22K_UP  | PAD_CTL_SPEED_LOW |		\
	PAD_CTL_DSE_80ohm   | PAD_CTL_SRE_FAST  | PAD_CTL_HYS)

#define I2C_PAD_CTRL    (PAD_CTL_PKE | PAD_CTL_PUE |            \
	PAD_CTL_PUS_100K_UP | PAD_CTL_SPEED_MED |               \
	PAD_CTL_DSE_40ohm | PAD_CTL_HYS |			\
	PAD_CTL_ODE)

#define ENET_PAD_CTRL  (PAD_CTL_PUS_100K_UP | PAD_CTL_PUE |     \
	PAD_CTL_SPEED_HIGH   |                                   \
	PAD_CTL_DSE_48ohm   | PAD_CTL_SRE_FAST)

#define ENET_CLK_PAD_CTRL  (PAD_CTL_SPEED_MED | \
	PAD_CTL_DSE_120ohm   | PAD_CTL_SRE_FAST)

#define ENET_RX_PAD_CTRL  (PAD_CTL_PKE | PAD_CTL_PUE |          \
	PAD_CTL_SPEED_HIGH   | PAD_CTL_SRE_FAST)

#define I2C_PAD_CTRL    (PAD_CTL_PKE | PAD_CTL_PUE |            \
	PAD_CTL_PUS_100K_UP | PAD_CTL_SPEED_MED |               \
	PAD_CTL_DSE_40ohm | PAD_CTL_HYS |			\
	PAD_CTL_ODE)

/* The PMIC on our board is located on i2c bus 2 (0-based) */
#define I2C_PMIC 2

int dram_init(void)
{
	gd->ram_size = PHYS_SDRAM_SIZE;
	return 0;
}

static iomux_v3_cfg_t const uart1_pads[] = {
	MX6_PAD_GPIO1_IO04__UART1_TX | MUX_PAD_CTRL(UART_PAD_CTRL),
	MX6_PAD_GPIO1_IO05__UART1_RX | MUX_PAD_CTRL(UART_PAD_CTRL),
};

static iomux_v3_cfg_t const usdhc4_pads[] = {
	MX6_PAD_SD4_CLK__USDHC4_CLK | MUX_PAD_CTRL(USDHC_PAD_CTRL),
	MX6_PAD_SD4_CMD__USDHC4_CMD | MUX_PAD_CTRL(USDHC_PAD_CTRL),
	MX6_PAD_SD4_DATA0__USDHC4_DATA0 | MUX_PAD_CTRL(USDHC_PAD_CTRL),
	MX6_PAD_SD4_DATA1__USDHC4_DATA1 | MUX_PAD_CTRL(USDHC_PAD_CTRL),
	MX6_PAD_SD4_DATA2__USDHC4_DATA2 | MUX_PAD_CTRL(USDHC_PAD_CTRL),
	MX6_PAD_SD4_DATA3__USDHC4_DATA3 | MUX_PAD_CTRL(USDHC_PAD_CTRL),
	MX6_PAD_SD4_DATA7__GPIO6_IO_21 | MUX_PAD_CTRL(NO_PAD_CTRL),
};

static iomux_v3_cfg_t const fec1_pads[] = {
	MX6_PAD_ENET1_MDC__ENET1_MDC | MUX_PAD_CTRL(ENET_PAD_CTRL),
	MX6_PAD_ENET1_MDIO__ENET1_MDIO | MUX_PAD_CTRL(ENET_PAD_CTRL),
	MX6_PAD_RGMII1_RX_CTL__ENET1_RX_EN | MUX_PAD_CTRL(ENET_RX_PAD_CTRL),
	MX6_PAD_RGMII1_RD0__ENET1_RX_DATA_0 | MUX_PAD_CTRL(ENET_RX_PAD_CTRL),
	MX6_PAD_RGMII1_RD1__ENET1_RX_DATA_1 | MUX_PAD_CTRL(ENET_RX_PAD_CTRL),
	MX6_PAD_RGMII1_RD2__ENET1_RX_DATA_2 | MUX_PAD_CTRL(ENET_RX_PAD_CTRL),
	MX6_PAD_RGMII1_RD3__ENET1_RX_DATA_3 | MUX_PAD_CTRL(ENET_RX_PAD_CTRL),
	MX6_PAD_RGMII1_RXC__ENET1_RX_CLK | MUX_PAD_CTRL(ENET_RX_PAD_CTRL),
	MX6_PAD_RGMII1_TX_CTL__ENET1_TX_EN | MUX_PAD_CTRL(ENET_PAD_CTRL),
	MX6_PAD_RGMII1_TD0__ENET1_TX_DATA_0 | MUX_PAD_CTRL(ENET_PAD_CTRL),
	MX6_PAD_RGMII1_TD1__ENET1_TX_DATA_1 | MUX_PAD_CTRL(ENET_PAD_CTRL),
	MX6_PAD_RGMII1_TD2__ENET1_TX_DATA_2 | MUX_PAD_CTRL(ENET_PAD_CTRL),
	MX6_PAD_RGMII1_TD3__ENET1_TX_DATA_3 | MUX_PAD_CTRL(ENET_PAD_CTRL),
	MX6_PAD_RGMII1_TXC__ENET1_RGMII_TXC | MUX_PAD_CTRL(ENET_PAD_CTRL),
};

static void setup_iomux_uart(void)
{
	imx_iomux_v3_setup_multiple_pads(uart1_pads, ARRAY_SIZE(uart1_pads));
}

static int setup_fec(void)
{
	struct iomuxc *iomuxc_regs = (struct iomuxc *)IOMUXC_BASE_ADDR;
	struct anatop_regs *anatop = (struct anatop_regs *)ANATOP_BASE_ADDR;
	int reg;

	/* Use 125MHz anatop loopback REF_CLK1 for ENET1 */
	clrsetbits_le32(&iomuxc_regs->gpr[1], IOMUX_GPR1_FEC1_MASK, 0);

	reg = readl(&anatop->pll_enet);
	reg |= BM_ANADIG_PLL_ENET_REF_25M_ENABLE;
	writel(reg, &anatop->pll_enet);

	return enable_fec_anatop_clock(0, ENET_125MHZ);
}

int board_eth_init(bd_t *bis)
{
	imx_iomux_v3_setup_multiple_pads(fec1_pads, ARRAY_SIZE(fec1_pads));
	setup_fec();
	udelay(500);
	return cpu_eth_init(bis);
}

#define PC MUX_PAD_CTRL(I2C_PAD_CTRL)
/*
 * PMIC:
 * REV 1 - I2C1 (1-based)
 * REV 1A - I2C3 (1-based)
*/
static struct i2c_pads_info i2c_pad_info1 = {
	.scl = {
		.i2c_mode = MX6_PAD_KEY_COL4__I2C3_SCL | PC,
		.gpio_mode = MX6_PAD_KEY_COL4__GPIO2_IO_14 | PC,
		.gp = IMX_GPIO_NR(2, 14),
	},
	.sda = {
		.i2c_mode = MX6_PAD_KEY_ROW4__I2C3_SDA | PC,
		.gpio_mode = MX6_PAD_KEY_ROW4__GPIO2_IO_19 | PC,
		.gp = IMX_GPIO_NR(2, 19),
	},
};

int power_init_board(void)
{
	struct pmic *p;
	unsigned int reg;
	int ret;

	p = pfuze_common_init(I2C_PMIC);
	if (!p)
		return -ENODEV;

	ret = pfuze_mode_init(p, APS_PFM);
	if (ret < 0)
		return ret;

	/* Enable power of VGEN5 3V3, needed for SD3 */
	pmic_reg_read(p, PFUZE100_VGEN5VOL, &reg);
	reg &= ~LDO_VOL_MASK;
	reg |= (LDOB_3_30V | (1 << LDO_EN));
	pmic_reg_write(p, PFUZE100_VGEN5VOL, reg);

	return 0;
}

#ifdef CONFIG_USB_EHCI_MX6
#define USB_OTHERREGS_OFFSET	0x800
#define UCTRL_PWR_POL		(1 << 9)

static iomux_v3_cfg_t const usb_otg_pads[] = {
	/* OGT1 */
	MX6_PAD_GPIO1_IO09__USB_OTG1_PWR | MUX_PAD_CTRL(NO_PAD_CTRL),
	MX6_PAD_GPIO1_IO10__ANATOP_OTG1_ID | MUX_PAD_CTRL(NO_PAD_CTRL),
	/* OTG2 */
	MX6_PAD_GPIO1_IO12__USB_OTG2_PWR | MUX_PAD_CTRL(NO_PAD_CTRL)
};

static void setup_usb(void)
{
	imx_iomux_v3_setup_multiple_pads(usb_otg_pads,
					 ARRAY_SIZE(usb_otg_pads));
}

int board_usb_phy_mode(int port)
{
	if (port == 1)
		return USB_INIT_HOST;
	else
		return usb_phy_mode(port);
}

int board_ehci_hcd_init(int port)
{
	u32 *usbnc_usb_ctrl;

	if (port > 1)
		return -EINVAL;

	usbnc_usb_ctrl = (u32 *)(USB_BASE_ADDR + USB_OTHERREGS_OFFSET +
				 port * 4);

	/* Set Power polarity */
	setbits_le32(usbnc_usb_ctrl, UCTRL_PWR_POL);

	return 0;
}
#endif

int board_phy_config(struct phy_device *phydev)
{
	/*
	 * Enable 1.8V(SEL_1P5_1P8_POS_REG) on
	 * Phy control debug reg 0
	 */
	phy_write(phydev, MDIO_DEVAD_NONE, 0x1d, 0x1f);
	phy_write(phydev, MDIO_DEVAD_NONE, 0x1e, 0x8);

	/* rgmii tx clock delay enable */
	phy_write(phydev, MDIO_DEVAD_NONE, 0x1d, 0x05);
	phy_write(phydev, MDIO_DEVAD_NONE, 0x1e, 0x100);

	if (phydev->drv->config)
		phydev->drv->config(phydev);

	return 0;
}

int board_early_init_f(void)
{
	setup_iomux_uart();

#ifdef CONFIG_USB_EHCI_MX6
	setup_usb();
#endif

	return 0;
}

static struct fsl_esdhc_cfg usdhc_cfg[1] = {
	{USDHC4_BASE_ADDR},
};

int board_mmc_getcd(struct mmc *mmc)
{
	struct fsl_esdhc_cfg *cfg = (struct fsl_esdhc_cfg *)mmc->priv;
	int ret = 0;

	switch (cfg->esdhc_base) {
	case USDHC4_BASE_ADDR:
		ret = 1; // Assume always present.
		break;
	}

	return ret;
}

int board_mmc_init(bd_t *bis)
{
	int i, ret;

	for (i = 0; i < CONFIG_SYS_FSL_USDHC_NUM; i++) {
		switch (i) {
		case 0:
			imx_iomux_v3_setup_multiple_pads(
				usdhc4_pads, ARRAY_SIZE(usdhc4_pads));
			usdhc_cfg[0].sdhc_clk = mxc_get_clock(MXC_ESDHC4_CLK);
			break;
		default:
			printf("Warning: you configured more USDHC controllers"
				"(%d) than supported by the board\n", i + 1);
			return -EINVAL;
		}

		ret = fsl_esdhc_initialize(bis, &usdhc_cfg[i]);
		if (ret) {
			printf("Warning: failed to initialize mmc dev %d\n", i);
			return ret;
		}
	}
	return 0;
}

#ifdef CONFIG_FSL_QSPI

#define QSPI_PAD_CTRL1				 \
	(PAD_CTL_SRE_FAST | PAD_CTL_SPEED_HIGH | \
	 PAD_CTL_PKE | PAD_CTL_PUE | PAD_CTL_PUS_47K_UP | PAD_CTL_DSE_40ohm)

static iomux_v3_cfg_t const quadspi_pads[] = {
	MX6_PAD_QSPI1A_DQS__ECSPI5_MOSI       | MUX_PAD_CTRL(QSPI_PAD_CTRL1),
	MX6_PAD_QSPI1A_SS1_B__ECSPI5_MISO     | MUX_PAD_CTRL(QSPI_PAD_CTRL1),
	MX6_PAD_QSPI1B_DQS__ECSPI5_SS0        | MUX_PAD_CTRL(QSPI_PAD_CTRL1),
	MX6_PAD_QSPI1B_SS1_B__ECSPI5_SCLK     | MUX_PAD_CTRL(QSPI_PAD_CTRL1),
};

int board_qspi_init(void)
{
	/* Set the iomux */
	imx_iomux_v3_setup_multiple_pads(quadspi_pads, ARRAY_SIZE(quadspi_pads));

	/* Set the clock */
	enable_qspi_clk(0);

	return 0;
}
#endif

int board_init(void)
{
	/* Address of boot parameters */
	gd->bd->bi_boot_params = PHYS_SDRAM + 0x100;

#ifdef CONFIG_SYS_I2C_MXC
	setup_i2c(0, CONFIG_SYS_I2C_SPEED, 0x7f, &i2c_pad_info1);
#endif

	// Turn on LC_POWER_EN.  Eventually remove this.
	gpio_direction_output(IMX_GPIO_NR(4,0), 1);
	udelay(1000000);
	gpio_direction_output(IMX_GPIO_NR(7,4), 1); /* unreset switch */
	gpio_direction_output(IMX_GPIO_NR(7,5), 1); /* unreset phy */
	gpio_direction_output(IMX_GPIO_NR(1,10), 0); /* place x86 in reset */

#ifdef CONFIG_FSL_QSPI
	board_qspi_init();
#endif

	return 0;
}

int checkboard(void)
{
	puts("Board: Platina Systems BMC\n");

	return 0;
}
