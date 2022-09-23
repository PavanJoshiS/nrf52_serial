#include "spim.h"

#if SPIM_PROT_EN == 1

#define NRFX_SPIM_SCK_PIN  4
#define NRFX_SPIM_MOSI_PIN 28
#define NRFX_SPIM_MISO_PIN 29
#define NRFX_SPIM_SS_PIN   31

void assign_value_to_reg (uint32_t address,uint32_t value )
{
        volatile uint32_t *point = (uint32_t *)address;
        *point = value;
}

int spi_init()
{
	NRF_SPIM3->PSEL.CSN = NRFX_SPIM_SS_PIN;
    //(NRF_SPIM3_BASE + 0x514) = 
	NRF_SPIM3->PSEL.SCK = NRFX_SPIM_SCK_PIN;
	NRF_SPIM3->PSEL.MOSI = NRFX_SPIM_MOSI_PIN;
	NRF_SPIM3->PSEL.MISO = NRFX_SPIM_MISO_PIN;
    
    /* Full drive strength */
	NRF_P0->PIN_CNF[NRFX_SPIM_SCK_PIN] |= (0x3 << 8);
	NRF_P0->PIN_CNF[NRFX_SPIM_MOSI_PIN] |= (0x3 << 8);

	/* Change the SPI mode here */
	NRF_SPIM3->CONFIG = (SPI_CONFIG_CPHA_Trailing << SPI_CONFIG_CPHA_Pos) |
			    (SPI_CONFIG_CPOL_ActiveLow << SPI_CONFIG_CPOL_Pos) |
			    (SPI_CONFIG_ORDER_MsbFirst << SPI_CONFIG_ORDER_Pos);

	NRF_SPIM3->IFTIMING.CSNDUR = 0xFF;

	/* Change the frequency here */
	NRF_SPIM3->FREQUENCY = SPIM_FREQUENCY_FREQUENCY_M4;

	NRF_SPIM3->ENABLE = (SPIM_ENABLE_ENABLE_Enabled << 0);
    
	return 0;
}

int spi_transceive(uint8_t *tx_buf, uint8_t *rx_buf, unsigned int n_tx, unsigned n_rx)
{
	/* The SPI transaction will last for max(n_tx, n_rx) bytes */
	NRF_SPIM3->TXD.MAXCNT = n_tx;
	NRF_SPIM3->RXD.MAXCNT = n_rx;

	NRF_SPIM3->TXD.PTR = (uint32_t)tx_buf;
	NRF_SPIM3->RXD.PTR = (uint32_t)rx_buf;
	NRF_SPIM3->EVENTS_END = 0;
	/* Start the transaction */
	NRF_SPIM3->TASKS_START = 1;
	/* Busy wait until it has finished */
	while (NRF_SPIM3->EVENTS_END == 0) {
	};
	NRF_SPIM3->EVENTS_END = 0;
	return 0;
}

static uint8_t rx_buf[64] = { 0 };
static uint8_t tx_buf[64];

int spi_test(void)
{
	/* Fill the TX buf with some data */
	for (unsigned int i = 0; i < 64; i++)
		tx_buf[i] = 64 - i;

	spi_transceive(tx_buf, rx_buf, 64, 32);
    //printf("Done..\r\n");
	return 0;
}

#endif /* #ifdef SPIM_PROT_EN == 1 */