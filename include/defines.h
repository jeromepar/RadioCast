#pragma once


#define PIN_I2C_CLK 22
#define PIN_I2C_SDA 21

#define PIN_BUTTON_1 GPIO_NUM_36
#define PIN_BUTTON_2 GPIO_NUM_39

#define SCREEN_HEIGHT 64
#define CENTER_Y(a) (SCREEN_HEIGHT/2-(a)/2)

#define FPS 1.0
#define FRAME_DURATION_MS (1000.0/FPS)