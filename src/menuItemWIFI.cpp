#include "MenuItemWIFI.hpp"
#include <esp_log.h>
#include <string.h>

#include "WifiManager.h"

static const char *TAG = "menuItemWIFI";

#include <pgmspace.h>
#include "../ressources/WIFI_sync_1.xbm"
#include "../ressources/WIFI_sync_2.xbm"
#include "../ressources/WIFI_sync_3.xbm"
#include "../ressources/WIFI.xbm"

#include "defines.h"
#include "utils.hpp"

static MenuItemWIFI *wifi_instance; // so sad, but used for callbacks

Audio outStream(false);

std::vector<station> stations;
static bool SHARED_cmd_next_station;
static bool SHARED_cmd_previous_station;

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

// called by Audio
void audio_showstation(const char *info)
{
    /*
        char *ptr = strstr(info, " (");
        if (ptr != NULL)
        {
            *ptr = 0; // we stop the string before additional metadata
        }
        wifi_instance->setInfo2((const uint8_t *)info);*/
    wifi_instance->setInfo3((const uint8_t *)"");
}
// called by Audio

void audio_showstreamtitle(const char *info)
{
    wifi_instance->setInfo3((const uint8_t *)info);
}

void audioProcessing(void *p)
{
    vTaskDelay((FRAME_DURATION_MS + 1) / portTICK_PERIOD_MS); // Let other tasks execute

    uint32_t audioBufferFilled_ = 0;
    uint32_t audioBufferSize_ = 0;
    bool streamError;
    bool streamPlaying = false;
    uint64_t timeConnect_; // Store time in order to detect stream errors after connecting

    int8_t currentStationIndex = 0;

    uint64_t time_last_cmd = millis();

    while (true)
    {
        if ((SHARED_cmd_next_station || SHARED_cmd_previous_station) && (millis() > (time_last_cmd + COOLDOWN_CMD_MS)))
        {
            time_last_cmd = millis();
            if (SHARED_cmd_next_station)
                currentStationIndex++;
            if (SHARED_cmd_previous_station)
                currentStationIndex--;

            if (currentStationIndex >= (int8_t)stations.size())
            {
                ESP_LOGI(TAG, "High limit");
                currentStationIndex = 0;
            }
            else if (currentStationIndex < 0)
            {
                ESP_LOGI(TAG, "Low limit");
                currentStationIndex = stations.size() - 1;
            }

            ESP_LOGI(TAG, "new currentStationIndex : %d (mod  %d)", currentStationIndex, stations.size());

            SHARED_cmd_next_station = false;
            SHARED_cmd_previous_station = false;

            ESP_LOGI(TAG, "Stopping previous song/station");
            outStream.stopSong();

            streamPlaying = false;
        }

        if (streamPlaying == false)
        {
            // new connection
            ESP_LOGI(TAG, "new connection to %s at %s", stations[currentStationIndex].name, stations[currentStationIndex].url);
            wifi_instance->setInfo2((const uint8_t *)stations[currentStationIndex].name);
            wifi_instance->setInfo3((const uint8_t *)"buffering");
            bool success = outStream.connecttohost(stations[currentStationIndex].url); // May fail due to wrong host address, socket error or timeout
            ESP_LOGI(TAG, "Exit code %d", success);

            timeConnect_ = millis(); // Store time in order to detect stream errors after connecting

            // Update buffer state variables
            audioBufferFilled_ = outStream.inBufferFilled(); // 0 after connecting
            audioBufferSize_ = outStream.inBufferFree() + audioBufferFilled_;

            streamPlaying = true; // for basic check post-connection
            outStream.setVolume(16);
        }

        // After the buffer has been filled up sufficiently enable audio output
        if (streamPlaying)
        {
            if (audioBufferFilled_ > 0.9f * audioBufferSize_)
            {
                // ESP_LOGW(TAG, "Unusual occupation of buffer space %d\%", (audioBufferFilled_ * 100) / audioBufferSize_);
            }
            else
            {
                // If the stream does not build up within a few seconds something is wrong with the connection
                if (millis() - timeConnect_ > TIMEOUT_BUFFERING_MS)
                {
                    if (!streamError)
                    {
                        ESP_LOGI(TAG, "Audio buffer low: %u of %u bytes.\n", audioBufferFilled_, audioBufferSize_);
                        streamError = true; // Raise connection error flag
                    }
                }
            }
        }

        // Let 'esp32-audioI2S' library process the web radio stream data
        outStream.loop();

        audioBufferFilled_ = outStream.inBufferFilled(); // Update used buffer capacity
        vTaskDelay(1 / portTICK_PERIOD_MS);              // Let other tasks execute
    }
}

MenuItemWIFI::MenuItemWIFI(U8G2 *display) : MenuItem("WIFI", display)
{
    ESP_LOGV(TAG, "MenuItem %s constructor", this->name);

    this->icon_vector.push_back((const unsigned char *)WIFI_sync_1);
    this->icon_vector.push_back((const unsigned char *)WIFI_sync_2);
    this->icon_vector.push_back((const unsigned char *)WIFI_sync_3);
    this->icon_vector.push_back((const unsigned char *)WIFI_bits);
    this->current_icon = this->icon_vector[0];
    this->wifi = NULL;

    wifi_instance = this; // for callbacks

    stations.push_back({"Radio Paradise", "http://stream.radioparadise.com/aac-320"});
    stations.push_back({"France Culture", "http://direct.franceculture.fr/live/franceculture-hifi.aac"});
    stations.push_back({"FIP", "http://direct.fipradio.fr/live/fip-hifi.aac"});
    stations.push_back({"Rire&Chanson nouvelle generation", "http://185.52.127.162/fr/30405/mp3_128.mp3?origine=fluxradios&adws_out_1=&access_token=38fb0edda3954ad48a099f50110212cd"});
    stations.push_back({"Radio Paradise Mellow", "http://stream.radioparadise.com/mellow-320"});
    stations.push_back({"Radio Paradise Global", "http://stream.radioparadise.com/global-320"});

    this->pAudioTask = NULL;

    /* ini I2S */
    ESP_LOGV(TAG, "Init I2S");
    outStream.setPinout(PIN_I2S_B_CK, PIN_I2S_W_S, PIN_I2S_D_OUT);
    ESP_LOGV(TAG, "Pin SET");
    outStream.setVolumeSteps(16);

    /* shared variable */
    SHARED_cmd_next_station = false;
    SHARED_cmd_previous_station = false;
}

void MenuItemWIFI::start(void)
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
    wifi->setHostname(WIFI_NAME);
    wifi->setConfigPortalBlocking(false);
    wifi->setWiFiAutoReconnect(true);
    wifi->setConnectRetries(10);

    WiFi.mode(WIFI_STA); // explicitly set mode, esp defaults to STA+AP

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

    // Start the audio processing task
    xTaskCreate(audioProcessing, "Audio processing task", 4096 /*stack*/, nullptr, configMAX_PRIORITIES - 4, &pAudioTask);
}

void MenuItemWIFI::stop(void)
{
    ESP_LOGI(TAG, "stopping song");
    outStream.stopSong();

    ESP_LOGI(TAG, "audio Task deletion");
    if (pAudioTask != NULL)
    {
        vTaskDelete(pAudioTask);
        pAudioTask = NULL;
    }
    ESP_LOGI(TAG, "WiFiManager deletion");

    wifi->~WiFiManager();
    wifi = NULL;

    WiFi.mode(WIFI_OFF);
}

void MenuItemWIFI::update()
{
    ESP_LOGV(TAG, "MenuItem %s update", this->name);

    wifi->process(); // usefull when non blocking

    wl_status_t currentStatus = WiFi.status();
    if (previousStatus != currentStatus)
    {

        ESP_LOGI(TAG, "Change of wifi status %d->%d", previousStatus, currentStatus);

        switch (currentStatus)
        {
        case WL_CONNECTED:
            info1 = wifi->getWiFiSSID();
            info2 = "";
            info3 = "";
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
}
void MenuItemWIFI::updateDisplay(uint32_t frame_count)
{
    ESP_LOGV(TAG, "MenuItem %s updateDisplay", this->name);

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

        u8g2->setFont(u8g2_font_courR14_tf);
        u8g2->drawStr(45, 5, tempstringInfo1);

        u8g2->setFont(u8g2_font_courR08_tf);
        u8g2->drawStr(45, CENTER_Y(0) + 2, tempstringInfo2);
        u8g2->drawStr(0, CENTER_Y(0) + 2 + 16, tempstringInfo3);

    } while (u8g2->nextPage());
}

void MenuItemWIFI::actionB2_shortPress()
{
    ESP_LOGI(TAG, "Next station");
    SHARED_cmd_next_station = true;
}

void MenuItemWIFI::actionB2_doublePress()
{
    ESP_LOGI(TAG, "Previous station");
    SHARED_cmd_previous_station = true;
}