#pragma once

#include <cstdint>
#include <vector>
#include <WiFiManager.h>

#include <U8g2lib.h>
#include "MenuItem.hpp"

class MenuItemWIFIAP : public MenuItem
{
public:
    MenuItemWIFIAP(U8G2 *u8g2);

    void start(void);
    void stop(void);

    void update(void);
    void updateDisplay(uint32_t frame_count);

private:
    WiFiManager *wifi;

};

