#ifndef __I2CMI_H_
#define __I2CMI_H_

#define I2C_PROT_EN  1

#include<stdint.h>
#include "nrf52840.h"
#include "nrf52840_bitfields.h"
//#include "nrf_delay.h"
#include "nrf_gpio.h"
#include "printf.h"
#include "uart.h"
#include "hm0360.h"

int i2c_init(void);
void i2c_write(uint16_t addr, uint8_t data);
uint8_t i2c_read(uint16_t addr);

#endif /* __I2CMI_H_ */