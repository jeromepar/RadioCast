#pragma once

#include <cstdint>
#include <vector>
#include <WiFiManager.h>

#include <U8g2lib.h>
#include "MenuItem.hpp"

#include <Audio.h>
#include <AudioTools.h>

typedef struct
{
    const char *name;
    const char *url;
} station;

class MenuItemWIFI : public MenuItem
{
public:
    MenuItemWIFI(U8G2 *u8g2);

    void start(void);
    void stop(void);

    void update(void);
    void updateDisplay(uint32_t frame_count);

    void actionB2_shortPress();
    void actionB2_doublePress();

    Audio *get_outStream();

private:
    WiFiManager *wifi;
    Audio *outStream;

    wl_status_t previousStatus;
    // Handle to the RTOS audio task
    TaskHandle_t pAudioTask;

};

