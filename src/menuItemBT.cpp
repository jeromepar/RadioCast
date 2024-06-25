#include "MenuItemBT.hpp"
#include <esp_log.h>
#include "utils.hpp"
static const char *TAG = "menuItemBT";

#include <pgmspace.h>
#include "../ressources/BT_c_1.xbm"
#include "../ressources/BT_c_2.xbm"
#include "../ressources/BT_c_3.xbm"
#include "../ressources/BT_note.xbm"

#include "defines.h"

static MenuItemBT *bluetooth_instance; // so sad, but used for callbacks

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
        bluetooth_instance->setInfo1((const uint8_t *)bluetooth_instance->get_a2dp_sink()->get_connected_source_name());
    }
}

void avrc_metadata_callback(uint8_t id, const uint8_t *text)
{
    switch (id)
    {
    case ESP_AVRC_MD_ATTR_TITLE:
        bluetooth_instance->setInfo3(text);
        break;

    case ESP_AVRC_MD_ATTR_ARTIST:
        bluetooth_instance->setInfo2(text);
        break;
    }
}

MenuItemBT::MenuItemBT(U8G2 *display, I2SStream &out) : MenuItem("BT", display)
{
    ESP_LOGV(TAG, "MenuItem %s constructor", this->name);

    this->icon_vector.push_back((const unsigned char *)BT_c_1);
    this->icon_vector.push_back((const unsigned char *)BT_c_2);
    this->icon_vector.push_back((const unsigned char *)BT_c_3);
    this->icon_vector.push_back((const unsigned char *)BT_note);
    this->current_icon = this->icon_vector[0];

    bluetooth_instance = this; // for callbacks

    this->a2dp_sink = new BluetoothA2DPSink(out);

    this->a2dp_sink->set_avrc_metadata_callback(avrc_metadata_callback);
    this->a2dp_sink->set_avrc_connection_state_callback(a2dp_connection_state_changed);
}

void MenuItemBT::start(void)
{
    this->current_icon = this->icon_vector[0];
    this->bt_state = e_bt_not_connected;
    this->a2dp_sink->start(BT_NAME);
}

void MenuItemBT::stop(void)
{
    this->a2dp_sink->stop();
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

    char tempstringInfo1[80];
    char tempstringInfo2[80];
    char tempstringInfo3[80];
    info1.toCharArray(tempstringInfo1, sizeof(tempstringInfo1), 0);
    string2char(info2, tempstringInfo2, 3, frame_count, NB_CHAR_DISPLAYED(45, 6 /*px wide*/));
    string2char(info3, tempstringInfo3, 3, frame_count, NB_CHAR_DISPLAYED(0, 6 /*px wide*/));

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
            u8g2->drawStr(45, 5, tempstringInfo1);

            u8g2->setFont(u8g2_font_courR08_tr);
            u8g2->drawStr(45, CENTER_Y(0) + 2, tempstringInfo2);
            u8g2->drawStr(0, CENTER_Y(0) + 2 + 16, tempstringInfo3);
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
    a2dp_sink->disconnect();
}

BluetoothA2DPSink *MenuItemBT::get_a2dp_sink(void)
{
    return this->a2dp_sink;
}