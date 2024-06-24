#pragma once

#include <cstdint>
#include <vector>
#include "U8g2lib.h"
#include "MenuItem.hpp"

#include "AudioTools.h"
#include "BluetoothA2DPSink.h"

typedef enum
{
    e_wifi_not_connected,
    e_wifi_connected,
} e_wifi_state;

class MenuItemWIFI : public MenuItem
{
public:
    MenuItemWIFI(U8G2 *u8g2, I2SStream  &out);

    void start(void);
    void stop(void);

    void update(void);
    void updateDisplay(uint32_t frame_count);    
    
private:
    e_wifi_state wifi_state;
    BluetoothA2DPSink *a2dp_sink;
};
