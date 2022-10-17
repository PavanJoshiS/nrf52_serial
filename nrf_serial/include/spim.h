
#ifndef __SPIMI_H_
#define __SPIMI_H_

#define SPIM_PROT_EN 1

#include<stdint.h>
#include "nrf52840.h"
#include "nrf52840_bitfields.h"
//#include "nrf_delay.h"
#include "nrf_gpio.h"
#include "printf.h"
#include "uart.h"

int spi_test(void);
int spi_init(unsigned int pin_hsync, unsigned int pin_hsync_inv, unsigned int pin_cs, unsigned int pin_clk,
	     unsigned int pin_mosi);
int spi_receive(uint8_t *rx_buf, unsigned int n_rx_max);

#endif /* __SPIMI_H_ */

