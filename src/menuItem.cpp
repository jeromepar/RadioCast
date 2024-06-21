#include "MenuItem.hpp"
#include <esp_log.h>

static const char *TAG = "menuItem";

#include <pgmspace.h>
#include "../ressources/gliphs_26x26"

MenuItemBT::MenuItemBT(void) : MenuItem("BT")
{
    this->bt_state = e_bt_unknown;

    this->icon_vector.push_back(glyph_bt_connect);
    this->icon_vector.push_back(glyph_bt_speaker);
    this->current_icon = this->icon_vector[0];
}

void MenuItemBT::update(void)
{
    ESP_LOGV(TAG, "MenuItem %s update", this->name);
}
void MenuItemBT::updateDisplay(void)
{
    ESP_LOGV(TAG, "MenuItem %s updateDisplay", this->name);
}

MenuItem::MenuItem(const char *name)
{
    ESP_LOGD(TAG, "Init MenuItem %s", name);
    this->name = const_cast<char *>(name);
    this->icon_vector.clear();
    this->current_icon = NULL;
}