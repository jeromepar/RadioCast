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
#include "../ressources/splash.xbm"

#include "MenuItemBT.hpp"
#include "MenuItemWIFI.hpp"
typedef enum
{
  IDX_MENU_BLUETOOTH,
  IDX_MENU_RADIO,
  IDX_END_NORMAL_MENU = IDX_MENU_RADIO,
  IDX_MENU_AP,
} e_menu_index;

#define IDX_MENU_DEFAULT IDX_MENU_RADIO

e_menu_index currentMenuIndex = IDX_MENU_DEFAULT;
std::vector<MenuItem *> menus;

void init_buttons()
{
  pinMode(PIN_BUTTON_1, INPUT_PULLUP);
  pinMode(PIN_BUTTON_2, INPUT_PULLUP);
  button1.setEventHandler(handleButtonEvent);
  button1.setEventHandler(handleButtonEvent);
  currentMenuIndex = IDX_MENU_DEFAULT;
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
    u8g2.drawXBM(0, 0, 128, 64, (const uint8_t *)splash);
  } while (u8g2.nextPage());
}

void handleButtonEvent(AceButton *, uint8_t, uint8_t);

void setup()
{
  Serial.begin(115200);
  init_u8g2();

  sleep(1);
  ESP_LOGD(TAG, "Start INIT");

  init_buttons();

  // init I2S
  I2SStream outStream;



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

  // init menu items (respect e_menu_index order)
  menus.push_back(new MenuItemBT(&u8g2,outStream));
  menus.push_back(new MenuItemWIFI(&u8g2,outStream));

  // start current one
  menus[currentMenuIndex]->start();

  ESP_LOGI(TAG, "End INIT");
}

void loop()
{
  static e_menu_index old_currentMenuIndex = currentMenuIndex;

  /* LOOP start */
  static uint32_t frame_count = 0; // will overflow, no big deal
  static uint64_t time_last_frame = millis();

  ESP_LOGV(TAG, "LOOP");

  /* test New Mode and housekeeping */
  if (old_currentMenuIndex != currentMenuIndex)
  {
    menus[old_currentMenuIndex]->stop();
    menus[currentMenuIndex]->start();
  }

  /* testing area*/
  static bool test_done = false;
  if (0 && (test_done == false) && (frame_count == 30))
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
    ESP_LOGI(TAG, "Menu activated: %d at %x", currentMenuIndex, menus[currentMenuIndex]);
    (menus[currentMenuIndex])->updateDisplay(frame_count);
  }

  old_currentMenuIndex = currentMenuIndex;
}

void handleButtonEvent(AceButton *button, uint8_t eventType,
                       uint8_t state)
{

  if ((button == &button1) && (AceButton::kEventPressed))
  {
    ESP_LOGI(TAG, "Button1 pressed");
    currentMenuIndex=(e_menu_index)((currentMenuIndex+1)%(IDX_END_NORMAL_MENU+1));
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