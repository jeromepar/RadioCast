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

MenuItemBT::MenuItemBT(U8G2 *display) : MenuItem("BT", display)
{
    this->bt_state = e_bt_not_connected;

    this->icon_vector.push_back(BT_c_1_bits);
    this->icon_vector.push_back(BT_c_2_bits);
    this->icon_vector.push_back(BT_c_3_bits);
    this->icon_vector.push_back(BT_note_bits);
    this->current_icon = this->icon_vector[0];
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
        u8g2->setFont(u8g2_font_courR14_tr);

        if (bt_state == e_bt_not_connected)
        {

            u8g2->drawStr(45, CENTER_Y(14) - 14, "Waiting");
            switch (nb_dots)
            {
            case 2:
                u8g2->drawStr(45, CENTER_Y(14) + 14, "input..");
                break;
            case 3:
                u8g2->drawStr(45, CENTER_Y(14) + 14, "input...");
                break;
            default:
                u8g2->drawStr(45, CENTER_Y(14) + 14, "input.");
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
            u8g2->drawStr(45, CENTER_Y(0)+10, tempstring);
            info2.toCharArray(tempstring, sizeof(tempstring), 0);
            u8g2->drawStr(0, CENTER_Y(0) + 10+ 16, tempstring);
        }

    } while (u8g2->nextPage());
}

void MenuItemBT::setConnectionStatus(e_bt_state state)
{
    ESP_LOGI(TAG, "setConnectionStatus to %d (0=>not connected/1=>connected)", state);
    this->bt_state = state;
}
