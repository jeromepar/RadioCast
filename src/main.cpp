#include <Arduino.h>
#include <esp_log.h>



// put function declarations here:
static const char *TAG = "main";

#include <WiFiManager.h>
WiFiManager wifiManager;

void configModeCallback(WiFiManager *myWiFiManager)
{
  ESP_LOGW(TAG, "WifiManager Entered config mode on %s/%s",
           WiFi.softAPIP(),
           WiFi.softAPIP().toString());
}
void saveConfigCallback () {
  ESP_LOGI(TAG, "WifiManager data saved");
}

void setup()
{
  Serial.begin(115200);
  esp_log_level_set("*", ESP_LOG_DEBUG);

  sleep(2);

  ESP_LOGI(TAG, "wifiManager attempt to AC");
  wifiManager.setAPCallback(configModeCallback);
  wifiManager.setSaveConfigCallback(saveConfigCallback);
  wifiManager.autoConnect("AP-ESP32-RADIOCAST");

  ESP_LOGI(TAG, "Connected to %s at %s", 
  wifiManager.getWiFiSSID(),
  wifiManager.getWiFiHostname());
}

void loop()
{
  // put your main code here, to run repeatedly:
}
