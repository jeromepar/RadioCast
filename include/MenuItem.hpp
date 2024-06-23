#pragma once

#include <cstdint>
#include <vector>
#include "U8g2lib.h"

class MenuItem
{
public:
    MenuItem(const char *name, U8G2 *u8g2);

    void actionB1_shortPress(void) { /*do nothing*/ };
    void actionB1_longPress(void) { /*do nothing*/ };
    void actionB2_shortPress(void) { /*do nothing*/ };
    void actionB2_longPress(void) { /*do nothing*/ };
    void update(void) { /*do nothing*/ };
    void updateDisplay(uint32_t frame_count) { /*do nothing*/ };
    void setInfo1(const uint8_t *s);
    void setInfo2(const uint8_t *s);
    void setInfo3(const uint8_t *s);

protected:
    std::vector<const unsigned char *> icon_vector;
    char *name;
    const unsigned char *current_icon;
    U8G2 *u8g2;
    String info1;
    String info2;
    String info3;
};

typedef enum
{
    e_bt_not_connected,
    e_bt_connected,
} e_bt_state;
class MenuItemBT : public MenuItem
{
public:
    MenuItemBT(U8G2 *u8g2);
    void setConnectionStatus(e_bt_state state);
    void update(void);
    void updateDisplay(uint32_t frame_count);
private:
    e_bt_state bt_state;
};
