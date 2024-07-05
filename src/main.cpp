#include <Arduino.h>
#include <esp_log.h>

#include "defines.h"

#include <InterruptButton.h>
InterruptButton button1(PIN_BUTTON_1, LOW); // Statically allocated button (safest)
InterruptButton button2(PIN_BUTTON_2, LOW); // Statically allocated button (safest)

void handleButtonEvent(uint8_t pin, events event);

// put function declarations here:
static const char *TAG = "main";

/*


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
U8G2_SH1106_128X64_WINSTAR_F_HW_I2C u8g2(U8G2_R0, PIN_I2C_CLK, PIN_I2C_SDA);
#include "../ressources/splash.xbm"

#include "MenuItemBT.hpp"
#include "MenuItemWIFI.hpp"
#include "MenuItemWIFIAP.hpp"
typedef enum
{
  IDX_MENU_BLUETOOTH,
  IDX_MENU_RADIO,
  IDX_END_NORMAL_MENU = IDX_MENU_RADIO,
  IDX_MENU_AP,
} e_menu_index;

#define IDX_MENU_DEFAULT IDX_MENU_BLUETOOTH

e_menu_index currentMenuIndex = IDX_MENU_DEFAULT;
std::vector<MenuItem *> menus;

void init_buttons()
{
  InterruptButton::setMode(Mode_Synchronous); // Defaults to Asynchronous (immediate like an ISR and not actioned in main loop)

  button1.bind(Event_KeyPress, []()
               { handleButtonEvent(PIN_BUTTON_1, Event_KeyPress); });
  button1.bind(Event_LongKeyPress, []()
               { handleButtonEvent(PIN_BUTTON_1, Event_LongKeyPress); });
  button1.bind(Event_DoubleClick, []()
               { handleButtonEvent(PIN_BUTTON_1, Event_DoubleClick); });

  button2.bind(Event_KeyPress, []()
               { handleButtonEvent(PIN_BUTTON_2, Event_KeyPress); });
  button2.bind(Event_LongKeyPress, []()
               { handleButtonEvent(PIN_BUTTON_2, Event_LongKeyPress); });
  button2.bind(Event_DoubleClick, []()
               { handleButtonEvent(PIN_BUTTON_2, Event_DoubleClick); });

  // button2.bind(Event_KeyDown, [](){ESP_LOGI(TAG, "press2");});
  // button2.bind(Event_KeyUp, [](){ESP_LOGI(TAG, "release2");});
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

  ESP_LOGI(TAG, "Previous bus clock : %dms", u8g2.getBusClock());

  u8g2.setBusClock(800000);

  uint64_t time_before = micros();

  u8g2.firstPage();
  do
  {
    u8g2.drawXBM(0, 0, 128, 64, (const uint8_t *)splash);
  } while (u8g2.nextPage());

  ESP_LOGI(TAG, "time to draw logo : %.3fms", (micros() - time_before) / 1000.0);
}

void setup()
{
  Serial.begin(115200);
  init_u8g2();

  sleep(1);
  ESP_LOGD(TAG, "Start INIT");

  init_buttons();

  currentMenuIndex = IDX_MENU_DEFAULT;

  // wifiManager.erase(); //suppression des credentials memorises

  // init menu items (respect e_menu_index order)
  menus.push_back(new MenuItemBT(&u8g2));
  //menus.push_back(new MenuItem("placeholderWifi", &u8g2));
  menus.push_back(new MenuItemWIFI(&u8g2));
  menus.push_back(new MenuItemWIFIAP(&u8g2));

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

  /* first process inputs */
  button1.processSyncEvents();
  button2.processSyncEvents();

  /* test New Mode and housekeeping */
  if (old_currentMenuIndex != currentMenuIndex)
  {
    ESP_LOGI(TAG, "Quitting menu index (%d)", old_currentMenuIndex);
    menus[old_currentMenuIndex]->stop();
    ESP_LOGI(TAG, "Init menu index (%d)", currentMenuIndex);
    menus[currentMenuIndex]->start();
  }

  (menus[currentMenuIndex])->update();

  /* testing area*/
  static bool test_done = true;
  if ((test_done == false) && (frame_count == 10))
  {
    test_done = true;
    ESP_LOGW(TAG, "TEST pressed 2 (%d)", Event_KeyPress);
    handleButtonEvent(PIN_BUTTON_2, Event_KeyPress);
  }

  /* end of loop managment -> DISPLAY */
  uint64_t time_end_loop = millis();
  if ((time_end_loop - time_last_frame) > (uint64_t)(FRAME_DURATION_MS))
  {

    frame_count++;
    time_last_frame = time_end_loop;

    ESP_LOGD(TAG, "Display updated (frame %d at %f))", frame_count, (double)time_end_loop / 1000.0);
    ESP_LOGD(TAG, "Menu activated: %d at %x", currentMenuIndex, menus[currentMenuIndex]);

    uint64_t time_before = micros();
    (menus[currentMenuIndex])->updateDisplay(frame_count);
    ESP_LOGD(TAG, "time to render : %.3fms", (micros() - time_before) / 1000.0);
  }

  old_currentMenuIndex = currentMenuIndex;
  delay(1);
}

void handleButtonEvent(uint8_t pin, events eventType)
{

  switch (pin)
  {
  case PIN_BUTTON_1:
    switch (eventType)
    {
    case Event_KeyPress:
      ESP_LOGI(TAG, "----------- B1 clicked");
      currentMenuIndex = (e_menu_index)((currentMenuIndex + 1) % (IDX_END_NORMAL_MENU + 1));
      ESP_LOGI(TAG, "new menu %d", currentMenuIndex);

      break;
    case Event_DoubleClick:
      ESP_LOGI(TAG, "----------- B1 Doubleclicked");

      break;
    case Event_LongKeyPress:
      ESP_LOGI(TAG, "----------- B1 long click");
      currentMenuIndex = IDX_MENU_AP;
      break;
    default:
      /* do nothing */
      break;
    }
    break;
  case PIN_BUTTON_2:
    switch (eventType)
    {
    case Event_KeyPress:
      ESP_LOGI(TAG, "----------- B2 clicked");
      menus[currentMenuIndex]->actionB2_shortPress();

      break;
    case Event_DoubleClick:
      ESP_LOGI(TAG, "----------- B2 Doubleclicked");
      menus[currentMenuIndex]->actionB2_doublePress();

      break;
    case Event_LongKeyPress:
      ESP_LOGI(TAG, "----------- B2 long click");
      menus[currentMenuIndex]->actionB2_longPress();

      break;
    default:
      /* do nothing */
      break;
    }
    break;
  }
}