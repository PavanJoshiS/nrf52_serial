#ifndef __HM_H_
#define __HM_H_

#include<stdint.h>
#include "nrf52840.h"
#include "nrf52840_bitfields.h"
#include "nrf_gpio.h"
#include "printf.h"
#include "uart.h"
#include "i2c.h"
#include "spim.h"
#include "string.h"

#define HM_PROT_EN 0

//#define DEVICE_ADDRESS (0x24)
//#define PIN_SCL        (3)
//#define PIN_SDA        (4)
#define PIN_VSYNC      (28)          
#define PIN_HSYNC      (29)          
#define PIN_PCLK       (30)
#define PIN_D0         (31)         
//#define PIN_D1         (29)          
//#define PIN_D2         (30)          
//#define PIN_D3         (31)         
                  
#define HM01B0_PIXEL_X_NUM                              (324)
#define HM01B0_PIXEL_Y_NUM                              (244)

#define HM01B0_REG_AE_TARGET_MEAN                       (0x2101)
#define HM01B0_REG_AE_MIN_MEAN                          (0x2102)
#define HM01B0_REG_CONVERGE_IN_TH                       (0x2103)
#define HM01B0_REG_CONVERGE_OUT_TH                      (0x2104)

#define frameBufferSize 79056
//static const uint32_t  frameBufferSize = ((uint32_t)HM01B0_PIXEL_X_NUM * (uint32_t)HM01B0_PIXEL_Y_NUM);
extern uint8_t   frameBuffer[frameBufferSize];

#define MODEL_ID_H 0x0
#define MODEL_ID_L 0x1
#define MODE_SELECT 0x100
#define COMMAND_UPDATE 0x104
#define PLL1CFG 0x300
#define OPFM_CTRL 0x1014
#define ANA_REGISTER_04 0x310F
#define WIN_MODE 0x3030
#define PMU_CFG_7 0x3028
#define PMU_CFG_8 0x3029
#define PMU_CFG_9 0x302A

void hm0360_init();
void hm01b0_read_frame();

#endif /* __HM_H_ */