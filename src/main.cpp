#include <Arduino.h>
#include <esp_log.h>

#include "defines.h"

#include <AceButton.h>
using namespace ace_button;
AceButton button1(PIN_BUTTON_1);
AceButton button2(PIN_BUTTON_2);
void handleButtonEvent(AceButton *button, uint8_t eventType,
                       uint8_t state);

// put function declarations here:
static const char *TAG = "main";
/*
#include <WiFiManager.h>
WiFiManager wifiManager;

void configModeCallback(WiFiManager *myWiFiManager)
{
  ESP_LOGW(TAG, "WifiManager Entered config mode on %s/%s",
           WiFi.softAPIP(),
           WiFi.softAPIP().toString());
}
void saveConfigCallback()
{
  ESP_LOGI(TAG, "WifiManager data saved");
}
*/

#include "U8g2lib.h"
U8G2_SH1106_128X64_NONAME_1_SW_I2C u8g2(U8G2_R0, PIN_I2C_CLK, PIN_I2C_SDA);

#include "MenuItem.hpp"
#define IDX_MENU_BLUETOOTH 0
#define IDX_MENU_RADIO    1
#define IDX_MENU_AP       2
#define IDX_MENU_DEFAULT IDX_MENU_BLUETOOTH
MenuItemBT menuBT(&u8g2);

uint8_t currentMenuIndex = IDX_MENU_BLUETOOTH;
std::vector<MenuItem *> menus;

void init_buttons()
{
  pinMode(PIN_BUTTON_1, INPUT_PULLUP);
  pinMode(PIN_BUTTON_2, INPUT_PULLUP);
  button1.setEventHandler(handleButtonEvent);
  button1.setEventHandler(handleButtonEvent);
  currentMenuIndex=IDX_MENU_DEFAULT;
}

void init_u8g2()
{
  ESP_LOGD(TAG, "init_u8g2");

  u8g2.begin();
  u8g2.firstPage();

  u8g2.setDrawColor(1);
  u8g2.setFontPosTop();
  u8g2.setFontDirection(0);
  u8g2.setFont(u8g2_font_courR08_tr);

  u8g2.firstPage();
  do
  {
    u8g2.drawStr(0, 0, "Hello World!");
  } while (u8g2.nextPage());
}

void handleButtonEvent(AceButton *, uint8_t, uint8_t);

void setup()
{
  Serial.begin(115200);

  sleep(1);
  ESP_LOGD(TAG, "Start INIT");

  init_buttons();

  // wifiManager.erase(); //suppression des credentials memorises
  // wifiManager.getWiFiIsSaved(); test de WIFI memorise

  /*
    ESP_LOGE(TAG, "wifiManager attempt to AC");

  wifiManager.setAPCallback(configModeCallback);
  wifiManager.setSaveConfigCallback(saveConfigCallback);
  wifiManager.autoConnect("AP-ESP32-RADIOCAST");

  ESP_LOGI(TAG, "Connected to %s at %s",
           wifiManager.getWiFiSSID(),
           wifiManager.getWiFiHostname());
           */

  init_u8g2();

  /*
  This creates a new Bluetooth device with the name BTsync_ESP32 and the output will be sent to the following default I2S pins which need to be connected to an external DAC:
  */
  /*
   auto cfg = i2s.defaultConfig();
   cfg.pin_bck = 14;  // bck_io_num
   cfg.pin_ws = 15;   // ws_io_num
   cfg.pin_data = 22; // data_out_num
   i2s.begin(cfg);
 */

  menus.push_back(&menuBT);
  for (auto &m : menus)
  {
    m->init();
  }

  ESP_LOGI(TAG, "End INIT");
}

void loop()
{
  /* LOOP start */
  static uint32_t frame_count = 0; // will overflow, no big deal
  static uint64_t time_last_frame = millis();

  ESP_LOGV(TAG, "LOOP");


  /* testing area*/
  static bool test_done = false;
  if (0&&(test_done == false) && (frame_count == 30))
  {
    test_done = true;
    ESP_LOGW(TAG, "TEST pressed long 2");
    handleButtonEvent(&button2, AceButton::kEventLongPressed, 0);
  }

  /* end of loop managment -> DISPLAY */
  uint64_t time_end_loop = millis();
  if ((time_end_loop - time_last_frame) > (uint64_t)(FRAME_DURATION_MS))
  {
    frame_count++;
    time_last_frame = time_end_loop;

    ESP_LOGI(TAG, "Display updated (frame %d at %f))", frame_count, (double)time_end_loop / 1000.0);
    ESP_LOGI(TAG, "Menu activated: %d at %x",currentMenuIndex, menus[currentMenuIndex]);
    (menus[currentMenuIndex])->updateDisplay(frame_count);

  }
}

void handleButtonEvent(AceButton *button, uint8_t eventType,
                       uint8_t state)
{

  if ((button == &button1) && (AceButton::kEventPressed))
  {
    ESP_LOGI(TAG, "Button1 pressed");
  }
  else if ((button == &button2) && (AceButton::kEventPressed))
  {
    ESP_LOGI(TAG, "Button2 pressed");
    menus[currentMenuIndex]->actionB2_shortPress();
  }
  else if ((button == &button1) && (AceButton::kEventLongPressed))
  {
    ESP_LOGI(TAG, "Button1 LONG pressed");
  }
  else if ((button == &button2) && (AceButton::kEventLongPressed))
  {
    ESP_LOGI(TAG, "Button2 LONG pressed");
    menus[currentMenuIndex]->actionB2_longPress();
  }
}