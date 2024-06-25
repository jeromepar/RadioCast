#include "MenuItem.hpp"
#include <esp_log.h>

static const char *TAG = "menuItem";


//#include <esp32-hal.h>

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
    ESP_LOGI(TAG, "Info1 set to %s", s);
}
void MenuItem::setInfo2(const uint8_t *s)
{
    this->info2 = (const char *)s;
    ESP_LOGI(TAG, "Info2 set to %s", s);
}
void MenuItem::setInfo3(const uint8_t *s)
{
    this->info3 = (const char *)s;
    ESP_LOGI(TAG, "Info3 set to %s", s);
}
