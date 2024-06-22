#include <Arduino.h>
#include <esp_log.h>

#include "defines.h"

#include <button.h>
//QueueHandle_t button_events = button_init(PIN_BIT(0) | PIN_BIT(1));

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

MenuItemBT *menuBluetooth = new MenuItemBT(&u8g2);

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

void setup()
{
  Serial.begin(115200);

  sleep(1);
  Serial.println("HEY");
  ESP_LOGD(TAG, "Start INIT");

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

  ESP_LOGD(TAG, "End INIT");
}

void loop()
{
  // put your main code here, to run repeatedly:

  /*
if (xQueueReceive(button_events, &ev, 1000/portTICK_PERIOD_MS)) {
      if ((ev.pin == BUTTON_1) && (ev.event == BUTTON_DOWN)) {
          // ...
      }
      if ((ev.pin == BUTTON_2) && (ev.event == BUTTON_DOWN)) {
          // ...
      }
  }
*/

  //ESP_LOGD(TAG, "LOOP");
  menuBluetooth->update();
  //delay(10);
}
