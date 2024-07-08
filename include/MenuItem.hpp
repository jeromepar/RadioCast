#pragma once

#include <cstdint>
#include <vector>
#include <U8g2lib.h>

class MenuItem
{
public:
    MenuItem(const char *name, U8G2 *u8g2);

    virtual void start(void) { /*do nothing*/ };
    virtual void stop(void) { /*do nothing*/ };

    virtual void update(void) { /*do nothing*/ };
    virtual void updateDisplay(uint32_t frame_count) { /*do nothing*/ };

    virtual void actionB1_shortPress(void) { /*do nothing*/ };
    virtual void actionB1_longPress(void) { /*do nothing*/ };
    virtual void actionB2_shortPress(void) { /*do nothing*/ };
    virtual void actionB2_longPress(void) { /*do nothing*/ };
    virtual void actionB2_doublePress(void) { /*do nothing*/ };

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