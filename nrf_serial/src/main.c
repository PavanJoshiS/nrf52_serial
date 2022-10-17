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

<<<<<<< HEAD
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
=======
int blink_init(unsigned int pin_led)
{
	/* Start LFCLK in low power mode driven by LFXO */
	NRF_CLOCK->LFRCMODE = CLOCK_LFRCMODE_MODE_ULP;
	NRF_CLOCK->LFCLKSRC = CLOCK_LFCLKSRC_SRC_Xtal;
	NRF_CLOCK->TASKS_LFCLKSTART = 1;

	NRF_RTC0->CC[0] = 32768;
	NRF_RTC0->CC[1] = 328;

	NRF_RTC0->EVTENSET = RTC_EVTENSET_COMPARE0_Msk | RTC_EVTENSET_COMPARE1_Msk;

	NRF_PPI->CH[0].EEP = (uint32_t)&NRF_RTC0->EVENTS_COMPARE[0];
	NRF_PPI->CH[0].TEP = (uint32_t)&NRF_RTC0->TASKS_CLEAR;

	NRF_GPIOTE->CONFIG[1] = (GPIOTE_CONFIG_MODE_Task << 0) | (pin_led << 8) |
	(GPIOTE_CONFIG_POLARITY_Toggle << 16) | (GPIOTE_CONFIG_OUTINIT_Low << 20);

	NRF_PPI->CH[1].EEP = (uint32_t)&NRF_RTC0->EVENTS_COMPARE[0];
	NRF_PPI->CH[1].TEP = (uint32_t)&NRF_GPIOTE->TASKS_OUT[1];

	NRF_PPI->CH[2].EEP = (uint32_t)&NRF_RTC0->EVENTS_COMPARE[1];
	NRF_PPI->CH[2].TEP = (uint32_t)&NRF_GPIOTE->TASKS_OUT[1];

	NRF_PPI->CHENSET = PPI_CHEN_CH0_Msk | PPI_CHEN_CH1_Msk | PPI_CHEN_CH2_Msk;

	NRF_RTC0->TASKS_CLEAR = 1;
	NRF_RTC0->TASKS_START = 1;
	return 0;
}

int main(void)
{
 
  volatile uint8_t data;

  uart_init(PIN_UART_TX);

  hm0360_init();

  int err = i2c_init();

  nrf_gpio_cfg_input(11, NRF_GPIO_PIN_PULLUP);

  while (1) {
    while (nrf_gpio_pin_read(11) == 1) {
    };
    data = i2c_read(0x0);
    data = i2c_read(0x1);
    i2c_write(0x100, 0x2);
    i2c_write(0x104, 0x1);
    data = i2c_read(0x100);

	  //hsync vsync
	  i2c_write(0x1014, 0x3);
	  data = i2c_read(0x1014);
    
	  //1 bit
	  i2c_write(0x310F, 0x80);
	  data = i2c_read(0x310F);

	  hm01b0_read_frame();

    nrf_delay_ms(500);
  }
  
>>>>>>> 42bc5fa01fd0fe7ca872bf87c2bb0b37985b5bb2
}


