#include "hm0360.h"

// array to hold a complete frame
uint8_t  frameBuffer[frameBufferSize];

// array to receive data from spi master
static uint8_t rx_buf[64] = { 0 };

// initialise the pins of sensor
void hm0360_init()
{
    // GPIO I/P
    nrf_gpio_cfg_input(PIN_VSYNC, NRF_GPIO_PIN_PULLUP);
    //nrf_gpio_cfg_input(PIN_HSYNC, NRF_GPIO_PIN_PULLUP);
    //nrf_gpio_cfg_input(PIN_PCLK, NRF_GPIO_PIN_PULLUP);

    // GPIO O/P
    //nrf_gpio_cfg_input(PIN_D0,NRF_GPIO_PIN_PULLUP);
    //nrf_gpio_cfg_input(PIN_D1,NRF_GPIO_PIN_PULLUP);
    //nrf_gpio_cfg_input(PIN_D2,NRF_GPIO_PIN_PULLUP);
    //nrf_gpio_cfg_input(PIN_D3,NRF_GPIO_PIN_PULLUP);

}

void hm01b0_read_frame()
{
    uint8_t i = 0;
    static uint32_t    ui32Idx         = 0x00;
  
  // while the pclko is high
  while(nrf_gpio_pin_read(PIN_HSYNC) == 1){
    // spi read from sensor
    spi_receive(rx_buf, 64);
 
     // copy the data to frame buffer
    while(i < 64){
    *(frameBuffer + ui32Idx++) = rx_buf[i];
      i++;
    }
    // re-initialise the rx_buf
    memset(rx_buf,0,sizeof(rx_buf));
  }
 }

// function to reset the sensor
void sw_reset()
{
    i2c_write(0x103, 0x0);
}

// function to wake up the sensor
void sw_set()
{
    i2c_write(0x103, 0x1);
}