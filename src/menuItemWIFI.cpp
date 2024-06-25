#include "MenuItemWIFI.hpp"
#include <esp_log.h>

#include "WifiManager.h"

static const char *TAG = "menuItemBT";

#include <pgmspace.h>
#include "../ressources/WIFI_sync_1.xbm"
#include "../ressources/WIFI_sync_2.xbm"
#include "../ressources/WIFI_sync_3.xbm"
#include "../ressources/WIFI.xbm"

#include "defines.h"

static MenuItemWIFI *wifi_instance; // so sad, but used for callbacks

/*
void avrc_metadata_callback(uint8_t id, const uint8_t *text)
{
    switch (id)
    {
    case ESP_AVRC_MD_ATTR_TITLE:
        bluetooth_instance->setInfo2(text);
        break;

    case ESP_AVRC_MD_ATTR_ARTIST:
        bluetooth_instance->setInfo3(text);
        break;
    }
}
*/

MenuItemWIFI::MenuItemWIFI(U8G2 *display, I2SStream &out, WiFiManager *wifi) : MenuItem("WIFI", display)
{
    ESP_LOGV(TAG, "MenuItem %s constructor", this->name);

    this->icon_vector.push_back((const unsigned char *)WIFI_sync_1);
    this->icon_vector.push_back((const unsigned char *)WIFI_sync_2);
    this->icon_vector.push_back((const unsigned char *)WIFI_sync_3);
    this->icon_vector.push_back((const unsigned char *)WIFI_bits);
    this->current_icon = this->icon_vector[0];
    this->wifi = wifi;

    wifi_instance = this; // for callbacks
    wifi->setWiFiAutoReconnect(true);
    wifi->setConnectRetries(10);
}

void MenuItemWIFI::start(void)
{
    previousStatus = WL_NO_SHIELD; // wrong so it updates 1st time

    ESP_LOGI(TAG, "Wifi state before start %d", WiFi.status());

    if (!wifi->getWiFiIsSaved())
    {
        ESP_LOGW(TAG, "No credential available - blocked");

        info1 = "No WIFI";
        info2 = "Registered";
        info3 = "please enter AP mode";
    }
    else
    {
        ESP_LOGI(TAG, "Autoconnect start");
        wifi->autoConnect();
    }
    ESP_LOGI(TAG, "Wifi state after start %d", WiFi.status());
}

void MenuItemWIFI::stop(void)
{
}

void MenuItemWIFI::update()
{
    ESP_LOGV(TAG, "MenuItem %s update", this->name);


}
void MenuItemWIFI::updateDisplay(uint32_t frame_count)
{
    ESP_LOGV(TAG, "MenuItem %s updateDisplay", this->name);

    wl_status_t currentStatus = WiFi.status();
    if (previousStatus != currentStatus)
    {

        ESP_LOGI(TAG, "Change of wifi status %d->%d", previousStatus, currentStatus);

        switch (currentStatus)
        {
        case WL_CONNECTED:
            info1 = wifi->getWiFiSSID();
            info2 = "Artist";
            info3 = "Song";
            break;

        case WL_CONNECTION_LOST:
            info1 = wifi->getWiFiSSID();
            info2 = "Connection Lost";
            info3 = "";
            break;
            
        default:
            info1 = "OUPS";
            info2 = "unexpected";
            info3 = String(currentStatus);
            break;
        }

        previousStatus = currentStatus;
    }

    uint8_t nb_dots = 0;
    if (WiFi.status() == WL_CONNECTED)
    {
        current_icon = icon_vector[3];
    }
    else
    {
        current_icon = icon_vector[frame_count % 3];
        nb_dots = (frame_count % 3) + 1;
    }
    ESP_LOGV(TAG, "MenuItem %s updateDisplay (iter %d)", this->name, frame_count % 3);

    u8g2->firstPage();
    do
    {
        u8g2->drawXBM(-5, -3, 50, 50, current_icon);
        u8g2->setFont(u8g2_font_courR12_tr);

        char tempstring[80];

        u8g2->setFont(u8g2_font_courR14_tr);
        info1.toCharArray(tempstring, sizeof(tempstring), 0);
        u8g2->drawStr(45, 5, tempstring);

        u8g2->setFont(u8g2_font_courR08_tr);
        info2.toCharArray(tempstring, sizeof(tempstring), 0);
        u8g2->drawStr(45, CENTER_Y(0) + 2, tempstring);

        info3.toCharArray(tempstring, sizeof(tempstring), 0);
        u8g2->drawStr(0, CENTER_Y(0) + 2 + 16, tempstring);

    } while (u8g2->nextPage());
}
