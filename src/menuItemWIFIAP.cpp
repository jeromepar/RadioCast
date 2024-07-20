#include "MenuItemWIFIAP.hpp"
#include <esp_log.h>
#include <EEPROM.h>

#include "WifiManager.h"
#include "utils.hpp"

static const char *TAG = "menuItemWIFI_AP";

#include "../ressources/AP_1.xbm"
#include "../ressources/AP_2.xbm"
#include "../ressources/AP_3.xbm"

#include "defines.h"

static bool SHARED_cmd_save_param = false;

// callback notifying us of the need to save config
void saveConfigCallback()
{
    ESP_LOGI(TAG, "Callback WIFI AP");
    SHARED_cmd_save_param = true;
}

MenuItemWIFIAP::MenuItemWIFIAP(U8G2 *display, char *radio_stations_memo) : MenuItem("WIFI_AP", display)
{
    ESP_LOGV(TAG, "MenuItem %s constructor", this->name);

    this->icon_vector.push_back((const unsigned char *)AP_1);
    this->icon_vector.push_back((const unsigned char *)AP_2);
    this->icon_vector.push_back((const unsigned char *)AP_3);
    this->current_icon = this->icon_vector[0];
    this->wifi = NULL;
    this->radio_stations_memo = radio_stations_memo;
}

void MenuItemWIFIAP::start(void)
{
    if (this->wifi == NULL)
    {
        ESP_LOGI(TAG, "WiFiManager creation");
        this->wifi = new WiFiManager();
    }
    else
    {
        ESP_LOGE(TAG, "WifiManager allready instancied");
    }

    ESP_LOGI(TAG, "WiFiManager init");

    // init WIFI
    wifi->setConfigPortalBlocking(false);
    wifi->setSaveParamsCallback(saveConfigCallback);

    this->BTnameParam = new WiFiManagerParameter("BTname", "Bluetooth Device name", DEFAULT_BT_NAME, MAX_LENGTH_BT_NAME);
    wifi->addParameter(BTnameParam);
    this->radioListParam = new WiFiManagerParameter("RadioList", "RadioList (name1,url1,name2,url2)", radio_stations_memo, MAX_LENGTH_STATION_LIST);
    wifi->addParameter(radioListParam);

    wifi->startConfigPortal(WIFI_NAME);
    info1 = WIFI_NAME;
    info2 = "waiting";
    info3 = "please connect";

    ESP_LOGI(TAG, "Wifi state after start %d", WiFi.status());
}

void MenuItemWIFIAP::stop(void)
{
    ESP_LOGI(TAG, "stopping captive portal");

    ESP_LOGI(TAG, "WiFiManager deletion");
    wifi->~WiFiManager();
    wifi = NULL;

    this->BTnameParam->~WiFiManagerParameter();
    this->radioListParam->~WiFiManagerParameter();

    WiFi.mode(WIFI_OFF);
}

void MenuItemWIFIAP::update()
{
    static uint8_t nb_clients_connected = 0;

    ESP_LOGV(TAG, "MenuItem %s update", this->name);

    uint8_t new_nb_clients_connected = WiFi.softAPgetStationNum();
    if (new_nb_clients_connected != nb_clients_connected)
    {
        ESP_LOGI(TAG, "Client connected number changed from %d to %d", nb_clients_connected, new_nb_clients_connected);

        nb_clients_connected = new_nb_clients_connected;

        if (nb_clients_connected == 0)
        {
            info2 = "waiting";
            info3 = "please connect";
        }
        else
        {
            info2 = "";
            info3 = String(nb_clients_connected) + " client connected";
        }
    }

    if (SHARED_cmd_save_param)
    {
        SHARED_cmd_save_param = false;

        ESP_LOGI(TAG, "Saving wifi AP parameters");
        ESP_LOGI(TAG, "new ESP name %s", BTnameParam->getValue());

        size_t ret_sz = EEPROM.writeByte(EEPROM_ADDR_ESPNAME_VALID, EEPROM_MAGIC_VALID);
        ESP_LOGD(TAG, "written %db", ret_sz);

        ESP_LOGD(TAG, "radio LEN %db",  strnlen(BTnameParam->getValue(), MAX_LENGTH_BT_NAME));
        ret_sz = EEPROM.writeBytes(EEPROM_ADDR_ESPNAME, BTnameParam->getValue(), strnlen(BTnameParam->getValue(), MAX_LENGTH_BT_NAME));
        ESP_LOGD(TAG, "written %db", ret_sz);

        ESP_LOGD(TAG, "new Radio List %s", radioListParam->getValue());
        ret_sz = EEPROM.writeByte(EEPROM_ADDR_STATIONS_VALID, EEPROM_MAGIC_VALID);
        ESP_LOGD(TAG, "written %db", ret_sz);
        int l = strnlen(radioListParam->getValue(), MAX_LENGTH_STATION_LIST);
        ret_sz = EEPROM.writeInt(EEPROM_ADDR_STATIONS_LENGTH, l);
        ESP_LOGD(TAG, "written %db", ret_sz);
        ret_sz = EEPROM.writeBytes(EEPROM_ADDR_STATIONS, radioListParam->getValue(), l);
        ESP_LOGD(TAG, "written %db", ret_sz);

        EEPROM.commit();
    }

    wifi->process();
}

void MenuItemWIFIAP::updateDisplay(uint32_t frame_count)
{
    ESP_LOGV(TAG, "MenuItem %s updateDisplay", this->name);

    current_icon = icon_vector[frame_count % 3];

    char tempstringInfo1[80];
    char tempstringInfo2[80];
    char tempstringInfo3[80];
    info1.toCharArray(tempstringInfo1, sizeof(tempstringInfo1), 0);
    string2char(info1, tempstringInfo1, SCROLLING_SPEED, frame_count, NB_CHAR_DISPLAYED(7, 9 /*px wide*/));
    string2char(info2, tempstringInfo2, SCROLLING_SPEED, frame_count, NB_CHAR_DISPLAYED(45, 6 /*px wide*/));
    string2char(info3, tempstringInfo3, SCROLLING_SPEED, frame_count, NB_CHAR_DISPLAYED(0, 6 /*px wide*/));

    u8g2->firstPage();
    do
    {
        u8g2->drawXBM(-5, -3, 50, 50, current_icon);

        u8g2->setFont(u8g2_font_courR14_tf);
        u8g2->drawUTF8(45, 5, tempstringInfo1);

        u8g2->setFont(u8g2_font_courR08_tf);
        u8g2->drawUTF8(45, CENTER_Y(0) + 2, tempstringInfo2);
        u8g2->drawUTF8(0, CENTER_Y(0) + 2 + 16, tempstringInfo3);

    } while (u8g2->nextPage());
}