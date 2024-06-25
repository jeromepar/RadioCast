#pragma once

#include <cstdint>
#include <vector>
#include <WiFiManager.h>

#include "U8g2lib.h"
#include "MenuItem.hpp"

#include "AudioTools.h"
#include "BluetoothA2DPSink.h"


class MenuItemWIFI : public MenuItem
{
public:
    MenuItemWIFI(U8G2 *u8g2, I2SStream  &out, WiFiManager *wifi);

    void start(void);
    void stop(void);

    void update(void);
    void updateDisplay(uint32_t frame_count);    
    
private:
    BluetoothA2DPSink *a2dp_sink;
    WiFiManager *wifi;
    wl_status_t previousStatus;
};
