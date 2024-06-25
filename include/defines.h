#pragma once


#define PIN_I2C_CLK 22
#define PIN_I2C_SDA 21

#define PIN_BUTTON_1        GPIO_NUM_36
#define PIN_BUTTON_2        GPIO_NUM_39
#define PIN_I2S_B_CK         GPIO_NUM_8
#define PIN_I2S_W_S          GPIO_NUM_9
#define PIN_I2S_D_OUT    GPIO_NUM_10

#define MAX(a,b) ((a)>(b)?(a):(b))

#define SCREEN_HEIGHT 64
#define SCREEN_WIDTH 128
#define CENTER_Y(a) (SCREEN_HEIGHT/2-(a)/2)
#define POSITION_Y(idx,nb_lines) (((float)(SCREEN_HEIGHT))/(float)(nb_lines)*(float)(idx))
#define NB_CHAR_DISPLAYED(offset, font_sz) (int(float(SCREEN_WIDTH-offset)/(font_sz))+1)

#define FPS 1.0
#define FRAME_DURATION_MS (1000.0/FPS)

#define BT_NAME "BT_ESP32"
#define WIFI_NAME "WLAN_ESP32"