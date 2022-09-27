#include "i2c.h"

#if I2C_PROT_EN == 1

#define DEVICE_ADDRESS (0x24)
#define PIN_SCL        (29)
#define PIN_SDA        (28)

int i2c_init(void)
{

	NRF_TWIM0->PSEL.SCL = PIN_SCL;
	NRF_TWIM0->PSEL.SDA = PIN_SDA;

	NRF_P0->PIN_CNF[PIN_SCL] =  (GPIO_PIN_CNF_PULL_Pullup << GPIO_PIN_CNF_PULL_Pos);
	NRF_P0->PIN_CNF[PIN_SDA] =  (GPIO_PIN_CNF_PULL_Pullup << GPIO_PIN_CNF_PULL_Pos);

	NRF_TWIM0->ADDRESS = DEVICE_ADDRESS;
	NRF_TWIM0->FREQUENCY = TWIM_FREQUENCY_FREQUENCY_K400 << TWIM_FREQUENCY_FREQUENCY_Pos;
	NRF_TWIM0->SHORTS = 0;

	NRF_TWIM0->ENABLE = TWIM_ENABLE_ENABLE_Enabled << TWIM_ENABLE_ENABLE_Pos;

	return 0;

}

void i2c_write(uint16_t addr, uint8_t data)
{
	uint8_t tx_buf[3];
	/* High address byte */
	tx_buf[0] = (addr >> 8) & 0xFF;
	/* Low address byte */
	tx_buf[1] = addr & 0xFF;
	// data
	tx_buf[2] = data;
	NRF_TWIM0->SHORTS = TWIM_SHORTS_LASTTX_STOP_Msk;

	NRF_TWIM0->TXD.MAXCNT = sizeof(tx_buf);
	NRF_TWIM0->TXD.PTR = (uint32_t)tx_buf;

	NRF_TWIM0->EVENTS_STOPPED = 0;
	NRF_TWIM0->TASKS_STARTTX = 1;
	while (NRF_TWIM0->EVENTS_STOPPED == 0);
}

uint8_t i2c_read(uint16_t addr) {

	uint8_t tx_buf[2];
	uint8_t rx_buf;

	/* High address byte */
	tx_buf[0] = (addr >> 8) & 0xFF;
	/* Low address byte */
	tx_buf[1] = addr & 0xFF;

	NRF_TWIM0->SHORTS = TWIM_SHORTS_LASTTX_STOP_Msk;

	NRF_TWIM0->TXD.MAXCNT = 2;
	NRF_TWIM0->TXD.PTR = (uint32_t)tx_buf;

	NRF_TWIM0->RXD.MAXCNT = 0;

	NRF_TWIM0->EVENTS_STOPPED = 0;
	NRF_TWIM0->TASKS_STARTTX = 1;
	while (NRF_TWIM0->EVENTS_STOPPED == 0);

	NRF_TWIM0->SHORTS = TWIM_SHORTS_LASTRX_STOP_Msk;

	NRF_TWIM0->RXD.MAXCNT = 1;
	NRF_TWIM0->RXD.PTR = (uint32_t)&rx_buf;

	NRF_TWIM0->EVENTS_STOPPED = 0;
	NRF_TWIM0->TASKS_STARTRX = 1;
	while (NRF_TWIM0->EVENTS_STOPPED == 0);

	return rx_buf;

}

#endif /* I2C_PROT_EN == 1 */