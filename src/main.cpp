#include <Arduino.h>
#include <esp_log.h>

#include "MenuItem.hpp"

MenuItemBT *menuBluetooth = new MenuItemBT();

// #include<button.h>
// QueueHandle_t button_events = button_init(PIN_BIT(0) | PIN_BIT(1));

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
void setup()
{
  Serial.begin(115200);

  sleep(1);
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
  ESP_LOGD(TAG, "End INIT");
}

void loop()
{
  // put your main code here, to run repeatedly:
  sleep(1);
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
}
