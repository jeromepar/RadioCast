#include "MenuItem.hpp"
#include <esp_log.h>

static const char *TAG = "menuItem";

#include <pgmspace.h>
#include "../ressources/gliphs_50x50"
#include <esp32-hal.h>
#include "defines.h"

MenuItem::MenuItem(const char *name, U8G2 *u8g2)
{
    ESP_LOGD(TAG, "Init MenuItem %s", name);
    this->name = const_cast<char *>(name);
    this->icon_vector.clear();
    this->current_icon = NULL;

    this->u8g2 = u8g2;
}

MenuItemBT::MenuItemBT(U8G2 *display) : MenuItem("BT", display)
{
    this->bt_state = e_bt_unknown;

    this->icon_vector.push_back(BT_c_1_bits);
    this->icon_vector.push_back(BT_c_2_bits);
   this->icon_vector.push_back(BT_c_3_bits);
    this->icon_vector.push_back(BT_note_bits);
    this->current_icon = this->icon_vector[0];
}

void MenuItemBT::update(void)
{
    this->updateDisplay();
    ESP_LOGV(TAG, "MenuItem %s update", this->name);
}
void MenuItemBT::updateDisplay(void)
{
    ESP_LOGV(TAG, "MenuItem %s updateDisplay", this->name);

    if (bt_state == e_bt_connected)
    {
        current_icon = icon_vector[3];
    }
    else
    {
        current_icon = icon_vector[(millis() / 1000) % 3];
    }
    ESP_LOGV(TAG, "MenuItem %s updateDisplay %f, %d", this->name, millis() / 1000.0, (millis() / 1000) % 3);


    u8g2->firstPage();
    do
    {
        u8g2->drawXBM(-5, CENTER_Y(50), 50, 50, current_icon);
        //u8g2->drawStr(50, 50, "Hello World!");

    } while (u8g2->nextPage());
}
