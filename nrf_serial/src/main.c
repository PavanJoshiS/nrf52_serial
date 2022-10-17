#include "nrf52840.h"
#include "nrf52840_bitfields.h"
#include "nrf_delay.h"
#include "nrf_gpio.h"
#include "printf.h"
#include "uart.h"
#include "spim.h"
#include "i2c.h"
#include "hm0360.h"

#define PIN_LED 15
#define PIN_UART_TX 6

int main(void) {
	volatile uint8_t data;

	uart_init(PIN_UART_TX);

	// i2c initialisation
	int err = i2c_init();

	//hm0360 initialisation
	hm0360_init();

	// spis initialisation
	err = spi_init(29,33,34,30,31);

	// configure button 1 as input 
	nrf_gpio_cfg_input(11, NRF_GPIO_PIN_PULLUP);

	// read the sensor part number
	data = i2c_read(MODEL_ID_H);
	data = i2c_read(MODEL_ID_L);
	// sensor mode selection (0x0: Sleep ,0x1: continuous streaming, 0x10: wákeup sleep cycles)
	i2c_write(MODE_SELECT, 0x1);
	data = i2c_read(MODE_SELECT);
	//command update
	i2c_write(COMMAND_UPDATE, 0x1);

	//Clock control register [3:2]pclk divider
	i2c_write(PLL1CFG,0xC);
	data = i2c_read(PLL1CFG);

	//hsync,vsync,pclk gating enable
	i2c_write(OPFM_CTRL, 0xB);
	data = i2c_read(OPFM_CTRL);
	
	//Operation mode registers, output frame count
	//i2c_write(PMU_CFG_7, 0x2); 
	//sleep
	//i2c_write(PMU_CFG_9, 0x0);
	//i2c_write(PMU_CFG_8, 0xF);

	//IO and clock control register, srl_enable_1bit
	i2c_write(ANA_REGISTER_04, 0x80);
	data = i2c_read(ANA_REGISTER_04);

	// win mode
	i2c_write(WIN_MODE, 0x1);

	while (1) {
		while (nrf_gpio_pin_read(11) == 1) {
		};
		
		// read a frame from sensor
		hm01b0_read_frame();

		nrf_delay_ms(500);
	}
}




