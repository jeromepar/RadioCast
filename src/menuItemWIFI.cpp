#include "MenuItemWIFI.hpp"
#include <esp_log.h>

static const char *TAG = "menuItemBT";

#include <pgmspace.h>
#include "../ressources/WIFI_sync_1.xbm"
#include "../ressources/WIFI_sync_2.xbm"
#include "../ressources/WIFI_sync_3.xbm"
#include "../ressources/WIFI.xbm"

#include "defines.h"

static MenuItemWIFI *wifi_instance; // so sad, but used for callbacks

// Forward declaration of the connection state change callback in bluetooth sink mode
/*
void a2dp_connection_state_changed(bool state)
{
    if (state == false)
    {
        bluetooth_instance->setConnectionStatus(e_bt_not_connected);
        bluetooth_instance->setInfo1((const uint8_t *)"");
    }
    else
    {
        bluetooth_instance->setConnectionStatus(e_bt_connected);
        bluetooth_instance->setInfo1((const uint8_t *)bluetooth_instance->get_a2dp_sink()->get_connected_source_name());
    }
}


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

MenuItemWIFI::MenuItemWIFI(U8G2 *display, I2SStream  &out) : MenuItem("WIFI", display)
{
    ESP_LOGV(TAG, "MenuItem %s constructor", this->name);

    this->icon_vector.push_back((const unsigned char *)WIFI_sync_1);
    this->icon_vector.push_back((const unsigned char *)WIFI_sync_2);
    this->icon_vector.push_back((const unsigned char *)WIFI_sync_3);
    this->icon_vector.push_back((const unsigned char *)WIFI_bits);
    this->current_icon = this->icon_vector[0];


    wifi_instance = this; // for callbacks

/*
    this->a2dp_sink = new BluetoothA2DPSink(out);

    this->a2dp_sink->set_avrc_metadata_callback(avrc_metadata_callback);
    this->a2dp_sink->set_avrc_connection_state_callback(a2dp_connection_state_changed);*/
}

void MenuItemWIFI::start(void)
{
    this->current_icon = this->icon_vector[0];
    this->wifi_state = e_wifi_not_connected;
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

    uint8_t nb_dots = 0;
    if (wifi_state == e_wifi_connected)
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

        if (wifi_state == e_wifi_not_connected)
        {

            u8g2->drawStr(50, POSITION_Y(0, 3), "???");
            u8g2->drawStr(50, POSITION_Y(1, 3), "Connecting ");

            switch (nb_dots)
            {
            case 2:
                u8g2->drawStr(45, POSITION_Y(2, 3), "   ..");
                break;
            case 3:
                u8g2->drawStr(45, POSITION_Y(2, 3), "   ...");
                break;
            default:
                u8g2->drawStr(45, POSITION_Y(2, 3), "   .");
                break;
            }
        }
        else
        {
            char tempstring[80];

            u8g2->setFont(u8g2_font_courR14_tr);
            info1.toCharArray(tempstring, sizeof(tempstring), 0);
            u8g2->drawStr(45, 5, tempstring);

            u8g2->setFont(u8g2_font_courR08_tr);
            info3.toCharArray(tempstring, sizeof(tempstring), 0);
            u8g2->drawStr(45, CENTER_Y(0) + 2, tempstring);

            info2.toCharArray(tempstring, sizeof(tempstring), 0);
            u8g2->drawStr(0, CENTER_Y(0) + 2 + 16, tempstring);
        }

    } while (u8g2->nextPage());
}
