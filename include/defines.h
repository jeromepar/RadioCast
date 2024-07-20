#pragma once


#define PIN_I2C_CLK 22
#define PIN_I2C_SDA 21

#define PIN_BUTTON_1        GPIO_NUM_12
#define PIN_BUTTON_2        GPIO_NUM_14

#define PIN_I2S_B_CK         GPIO_NUM_17
#define PIN_I2S_W_S          GPIO_NUM_4
#define PIN_I2S_D_OUT       GPIO_NUM_16




#define MAX(a,b) ((a)>(b)?(a):(b))

#define SCREEN_HEIGHT 64
#define SCREEN_WIDTH 128
#define CENTER_Y(a) (SCREEN_HEIGHT/2-(a)/2)
#define POSITION_Y(idx,nb_lines) (((float)(SCREEN_HEIGHT))/(float)(nb_lines)*(float)(idx))
#define NB_CHAR_DISPLAYED(offset, font_sz) (int(float(SCREEN_WIDTH-offset)/(font_sz))+1)
#define SCROLLING_SPEED       1 //char number


#define COOLDOWN_CMD_MS     1

#define FPS 3.0
#define FRAME_DURATION_MS (1000.0/FPS)
#define TIMEOUT_BUFFERING_MS   3000 //3s
#define DEFAULT_BT_NAME "BT_ESP32"
#define WIFI_NAME "AP_ESP32"

#define DEFAULT_STATIONS ("Radio Paradise,http://stream.radioparadise.com/aac-320,"\
"France Culture,http://direct.franceculture.fr/live/franceculture-hifi.aac,"\
"FIP,http://direct.fipradio.fr/live/fip-hifi.aac,"\
"Rire&Chanson nouvelle generation,http://185.52.127.162/fr/30405/mp3_128.mp3?origine=fluxradios&adws_out_1=&access_token=38fb0edda3954ad48a099f50110212cd,"\
"Radio Paradise Mellow,http://stream.radioparadise.com/mellow-320,"\
"Radio Paradise Global,http://stream.radioparadise.com/global-320")
#define MAX_LENGTH_STATION_LIST 2000
#define MAX_LENGTH_BT_NAME  40

#define EEPROM_ADDR_CURRENT_IDX 0
#define EEPROM_ADDR_ESPNAME_VALID 1
#define EEPROM_MAGIC_VALID 0xa5
#define EEPROM_ADDR_ESPNAME (EEPROM_ADDR_ESPNAME_VALID+1)
#define EEPROM_ADDR_STATIONS_VALID (EEPROM_ADDR_ESPNAME+MAX_LENGTH_BT_NAME)
#define EEPROM_ADDR_STATIONS_LENGTH (EEPROM_ADDR_STATIONS_VALID+1)
#define EEPROM_ADDR_STATIONS (EEPROM_ADDR_STATIONS_LENGTH+2)
