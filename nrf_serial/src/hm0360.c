#include "hm0360.h"

uint8_t  frameBuffer[frameBufferSize];

void hm0360_init()
{
    // GPIO I/P
    nrf_gpio_cfg_input(PIN_VSYNC, NRF_GPIO_PIN_PULLUP);
    nrf_gpio_cfg_input(PIN_HSYNC, NRF_GPIO_PIN_PULLUP);
    nrf_gpio_cfg_input(PIN_PCLK, NRF_GPIO_PIN_PULLUP);

    // GPIO O/P
    nrf_gpio_cfg_input(PIN_D0,NRF_GPIO_PIN_PULLUP);
    //nrf_gpio_cfg_input(PIN_D1,NRF_GPIO_PIN_PULLUP);
    //nrf_gpio_cfg_input(PIN_D2,NRF_GPIO_PIN_PULLUP);
    //nrf_gpio_cfg_input(PIN_D3,NRF_GPIO_PIN_PULLUP);

}

void hm01b0_read_frame()
{
    uint32_t    ui32Idx         = 0x00;
    uint32_t    ui32HsyncCnt    = 0x00;

    while((ui32HsyncCnt < HM01B0_PIXEL_Y_NUM))
    {
        while (0x00 == nrf_gpio_pin_read(PIN_HSYNC));

        // read one row
        while(nrf_gpio_pin_read(PIN_HSYNC))
        {
            while(0x00 == nrf_gpio_pin_read(PIN_PCLK));
            int data = nrf_gpio_pin_read(PIN_D0);
            *(frameBuffer + ui32Idx++) = nrf_gpio_pin_read(PIN_D0);

            if (ui32Idx == sizeof(frameBuffer)) {
               break;
            }

            while(nrf_gpio_pin_read(PIN_PCLK));
        }

        ui32HsyncCnt++;
    }
}
