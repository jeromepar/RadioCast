#pragma once

#include <cstdint>
#include <vector>
#include "U8g2lib.h"
#include "MenuItem.hpp"

#include "AudioTools.h"
#include "BluetoothA2DPSink.h"

typedef enum
{
    e_bt_not_connected,
    e_bt_connected,
} e_bt_state;
class MenuItemBT : public MenuItem
{
public:
    MenuItemBT(U8G2 *u8g2);

    void start(void);
    void stop(void);

    void update(void);
    void updateDisplay(uint32_t frame_count);    

    void setConnectionStatus(e_bt_state state);
    void actionB2_longPress();

    BluetoothA2DPSink* get_a2dp_sink(void);

private:
    e_bt_state bt_state;
    BluetoothA2DPSink *a2dp_sink;
};
