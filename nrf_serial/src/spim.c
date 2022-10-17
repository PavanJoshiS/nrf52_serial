#include "spim.h"

#if SPIM_PROT_EN == 1

static int gpiote_setup(unsigned int pin_hsync, unsigned int pin_hsync_inv)
{
	/* This event happens when the hsync pin goes from Low to High */
	NRF_GPIOTE->CONFIG[2] =
		(GPIOTE_CONFIG_MODE_Event << 0) | (pin_hsync << 8) | (GPIOTE_CONFIG_POLARITY_LoToHi << 16);

	/* This event happens when the hsync pin goes from High to Low */
	NRF_GPIOTE->CONFIG[3] =
		(GPIOTE_CONFIG_MODE_Event << 0) | (pin_hsync << 8) | (GPIOTE_CONFIG_POLARITY_HiToLo << 16);

	/* This task inverts the hsync_inv pin */
	NRF_GPIOTE->CONFIG[4] = (GPIOTE_CONFIG_MODE_Task << 0) | (pin_hsync_inv << 8) |
				(GPIOTE_CONFIG_POLARITY_None << 16) | (GPIOTE_CONFIG_OUTINIT_Low << 20);

	/* Connect the two together */
	NRF_PPI->CH[3].EEP = (uint32_t)&NRF_GPIOTE->EVENTS_IN[2];
	NRF_PPI->CH[3].TEP = (uint32_t)&NRF_GPIOTE->TASKS_CLR[4];

	NRF_PPI->CH[4].EEP = (uint32_t)&NRF_GPIOTE->EVENTS_IN[3];
	NRF_PPI->CH[4].TEP = (uint32_t)&NRF_GPIOTE->TASKS_SET[4];
	NRF_PPI->CHENSET = PPI_CHEN_CH3_Msk | PPI_CHEN_CH4_Msk;
	return 0;
}

int spi_init(unsigned int pin_hsync, unsigned int pin_hsync_inv, unsigned int pin_cs, unsigned int pin_clk,
	     unsigned int pin_mosi)
{
	gpiote_setup(pin_hsync, pin_hsync_inv);

	nrf_gpio_cfg_input(pin_cs, NRF_GPIO_PIN_NOPULL);
	nrf_gpio_cfg_input(pin_clk, NRF_GPIO_PIN_NOPULL);
	nrf_gpio_cfg_input(pin_mosi, NRF_GPIO_PIN_NOPULL);

	NRF_SPIS1->PSEL.CSN = pin_cs;
	NRF_SPIS1->PSEL.SCK = pin_clk;
	NRF_SPIS1->PSEL.MOSI = pin_mosi;

	/* Change the SPI mode here */
	NRF_SPIS1->CONFIG = (SPIS_CONFIG_CPHA_Trailing << SPIS_CONFIG_CPHA_Pos) |
			    (SPIS_CONFIG_CPOL_ActiveHigh << SPIS_CONFIG_CPOL_Pos) |
			    (SPIS_CONFIG_ORDER_MsbFirst << SPIS_CONFIG_ORDER_Pos);

	return 0;
}

int spi_receive(uint8_t *rx_buf, unsigned int n_rx_max)
{
	NRF_SPIS1->ENABLE = (SPIS_ENABLE_ENABLE_Enabled << SPIS_ENABLE_ENABLE_Pos);

	/* The SPI transaction will last for max(n_tx, n_rx) bytes */
	NRF_SPIS1->TXD.MAXCNT = 0;
	NRF_SPIS1->RXD.MAXCNT = n_rx_max;

	NRF_SPIS1->RXD.PTR = (uint32_t)rx_buf;

	NRF_SPIS1->TASKS_RELEASE = 1;
	NRF_SPIS1->EVENTS_END = 0;

	/* Busy wait until it has finished */
	while (NRF_SPIS1->EVENTS_END == 0) {
	};
	NRF_SPIS1->EVENTS_END = 0;
	NRF_SPIS1->ENABLE = (SPIS_ENABLE_ENABLE_Disabled << SPIS_ENABLE_ENABLE_Pos);

	return 0;
}

#endif