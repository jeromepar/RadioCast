#include <Arduino.h>
#include <esp_log.h>

#include "defines.h"

#include <button.h>
QueueHandle_t button_events = button_init(PIN_BIT(PIN_BUTTON_1) | PIN_BIT(PIN_BUTTON_2));

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

#include "AudioTools.h"
#include "BluetoothA2DPSink.h"

// I2SStream i2s;
BluetoothA2DPSink a2dp_sink = BluetoothA2DPSink();

// Forward declaration of the connection state change callback in bluetooth sink mode
void a2dp_connection_state_changed(bool state)
{
  if (state == false)
  {
    menuBluetooth->setConnectionStatus(e_bt_not_connected);
    menuBluetooth->setInfo1((const uint8_t *)"");
  }
  else
  {
    menuBluetooth->setConnectionStatus(e_bt_connected);
    menuBluetooth->setInfo1((const uint8_t *)a2dp_sink.get_connected_source_name());
  }
}

void avrc_metadata_callback(uint8_t id, const uint8_t *text)
{
  switch (id)
  {
  case ESP_AVRC_MD_ATTR_TITLE:
    menuBluetooth->setInfo2(text);
    break;

  case ESP_AVRC_MD_ATTR_ARTIST:
    menuBluetooth->setInfo3(text);
    break;
  }
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
  a2dp_sink.set_avrc_metadata_callback(avrc_metadata_callback);
  a2dp_sink.set_avrc_connection_state_callback(a2dp_connection_state_changed);
  a2dp_sink.start("BTsync_ESP32");

  ESP_LOGI(TAG, "End INIT");
}

void loop()
{

  static uint32_t frame_count = 0; // will overflow, no big deal
  static uint64_t time_last_frame = millis();

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

  // ESP_LOGD(TAG, "LOOP");
  // delay(10);

  // end of loop managment
  uint64_t time_end_loop = millis();
  if ((time_end_loop - time_last_frame) > (uint64_t)(FRAME_DURATION_MS))
  {
    frame_count++;
    time_last_frame = time_end_loop;

    ESP_LOGV(TAG, "Display updated (frame %d at %f))", frame_count, (double)time_end_loop / 1000.0);
    menuBluetooth->updateDisplay(frame_count);

    ESP_LOGD(TAG, "Log level for main %d, for BT_AV %d", esp_log_level_get("main"), esp_log_level_get("BT_AV"));
  }
}
