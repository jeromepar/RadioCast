#include "MenuItem.hpp"
#include <esp_log.h>

static const char *TAG = "menuItem";

#include <pgmspace.h>
#include "../ressources/glyphs_50x50"
#include <esp32-hal.h>
#include "defines.h"

MenuItem::MenuItem(const char *name, U8G2 *u8g2)
{
    ESP_LOGI(TAG, "Init MenuItem %s", name);
    this->name = const_cast<char *>(name);
    this->icon_vector.clear();
    this->current_icon = NULL;

    this->u8g2 = u8g2;
}

void MenuItem::setInfo1(const uint8_t *s)
{
    this->info1 = (const char *)s;
    ESP_LOGI(TAG, "Info1 set to %s", this->info1);
}
void MenuItem::setInfo2(const uint8_t *s)
{
    this->info2 = (const char *)s;
    ESP_LOGI(TAG, "Info2 set to %s", this->info2);
}
void MenuItem::setInfo3(const uint8_t *s)
{
    this->info3 = (const char *)s;
    ESP_LOGI(TAG, "Info3 set to %s", this->info3);
}

/* BLUETOOTH */
#include "AudioTools.h"
#include "BluetoothA2DPSink.h"
static MenuItemBT *bluetooth_instance; // so sad, but used for callbacks

// I2SStream i2s;
BluetoothA2DPSink a2dp_sink = BluetoothA2DPSink();

// Forward declaration of the connection state change callback in bluetooth sink mode
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
        bluetooth_instance->setInfo1((const uint8_t *)a2dp_sink.get_connected_source_name());
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

MenuItemBT::MenuItemBT(U8G2 *display) : MenuItem("BT", display)
{
    this->bt_state = e_bt_not_connected;

    this->icon_vector.push_back(BT_c_1_bits);
    this->icon_vector.push_back(BT_c_2_bits);
    this->icon_vector.push_back(BT_c_3_bits);
    this->icon_vector.push_back(BT_note_bits);
    this->current_icon = this->icon_vector[0];
}

void MenuItemBT::init(void)
{
    ESP_LOGV(TAG, "MenuItem %s init", this->name);

    bluetooth_instance = this; // for callbacks
    a2dp_sink.set_avrc_metadata_callback(avrc_metadata_callback);
    a2dp_sink.set_avrc_connection_state_callback(a2dp_connection_state_changed);
    a2dp_sink.start(BT_NAME);
}

void MenuItemBT::update()
{
    ESP_LOGV(TAG, "MenuItem %s update", this->name);
}
void MenuItemBT::updateDisplay(uint32_t frame_count)
{
    ESP_LOGV(TAG, "MenuItem %s updateDisplay", this->name);

    uint8_t nb_dots = 0;
    if (bt_state == e_bt_connected)
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

        if (bt_state == e_bt_not_connected)
        {

            u8g2->drawStr(50, POSITION_Y(0, 3), BT_NAME);
            u8g2->drawStr(50, POSITION_Y(1, 3), "Ready ");

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

void MenuItemBT::setConnectionStatus(e_bt_state state)
{
    ESP_LOGI(TAG, "setConnectionStatus to %d (0=>not connected/1=>connected)", state);
    this->bt_state = state;
}

void MenuItemBT::actionB2_longPress()
{
    a2dp_sink.disconnect();
}

