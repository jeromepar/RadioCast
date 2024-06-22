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

/*
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#define SCREEN_WIDTH 128 // OLED display width, in pixels
#define SCREEN_HEIGHT 64 // OLED display height, in pixels
#define OLED_RESET     -1 // Reset pin # (or -1 if sharing Arduino reset pin)
#define SCREEN_ADDRESS 0x3C ///< See datasheet for Address; 0x3D for 128x64, 0x3C for 128x32
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);
*/

#include "U8g2lib.h" 

U8G2_SH1106_128X64_NONAME_1_SW_I2C u8g2(U8G2_R0, /* clock=*/ 22, /* data=*/ 21);
const unsigned char epd_bitmap_Bitmap [] PROGMEM = {
	// 'vol, 26x26px
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xF0, 0x1F, 0x00, 0x00, 
  0xFC, 0x7F, 0x00, 0x00, 0xFE, 0xFF, 0x00, 0x00, 0xFE, 0xFF, 0x00, 0x00, 
  0xFE, 0xFC, 0x00, 0x00, 0x7E, 0xFC, 0x30, 0x00, 0xFE, 0xFC, 0x70, 0x00, 
  0xFE, 0xFF, 0xF6, 0x00, 0xFE, 0xFF, 0xFE, 0x01, 0xFE, 0xFF, 0xFC, 0x01, 
  0xFE, 0xFF, 0x78, 0x00, 0x3E, 0xFB, 0x78, 0x00, 0xFE, 0xF7, 0xFC, 0x01, 
  0xDE, 0xF7, 0xFE, 0x01, 0xDE, 0xFF, 0xF6, 0x00, 0xDE, 0xF7, 0x70, 0x00, 
  0xBE, 0xFF, 0x30, 0x00, 0x7E, 0xFC, 0x00, 0x00, 0xFE, 0xFF, 0x00, 0x00, 
  0xFC, 0xFF, 0x00, 0x00, 0xFC, 0x7F, 0x00, 0x00, 0xF0, 0x1F, 0x00, 0x00, 
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, };

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


/*

  // SSD1306_SWITCHCAPVCC = generate display voltage from 3.3V internally
  if(!display.begin(SSD1306_SWITCHCAPVCC, SCREEN_ADDRESS)) {

      ESP_LOGE(TAG, "ERROR LCD");

    Serial.println(F("SSD1306 allocation failed"));
    for(;;); // Don't proceed, loop forever
  }
        ESP_LOGI(TAG, "LCD OK");


  // Show initial display buffer contents on the screen --
  // the library initializes this with an Adafruit splash screen.
display.clearDisplay();
display.drawCircle(20,20,10,SSD1306_WHITE);
display.drawCircle(40,40,10,SSD1306_BLACK);

display.display();
*/
  u8g2.begin();
    u8g2.firstPage();

u8g2.setDrawColor(1);
  u8g2.setFontPosTop();
  u8g2.setFontDirection(0);   
    u8g2.setFont(u8g2_font_ncenB14_tr);

  u8g2.firstPage();
  do {
    u8g2.setFont(u8g2_font_ncenB14_tr);
    u8g2.drawStr(0,0,"Hello World!");
    u8g2.setDrawColor(1);
    u8g2.drawXBM(10, 10, 26, 26, epd_bitmap_Bitmap);

  } while ( u8g2.nextPage() );


  ESP_LOGD(TAG, "End INIT");
}

void loop()
{
  // put your main code here, to run repeatedly:
  /*
  u8g2.firstPage();
  do {
    u8g2.setFont(u8g2_font_ncenB14_tr);
    u8g2.drawStr(0,24,"Hello World!");
  } while ( u8g2.nextPage() );
  */
  
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

  ESP_LOGD(TAG, "LOOP");
  delay(2000);

}
