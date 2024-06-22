#pragma once


#include <cstdint>
#include <vector>
#include "U8g2lib.h"

class MenuItem
{
public:
    MenuItem(const char *name, U8G2 *u8g2);

    void actionB1_shortPress(void){ /*do nothing*/};
    void actionB1_longPress(void){ /*do nothing*/};
    void actionB2_shortPress(void){ /*do nothing*/};
    void actionB2_longPress(void){ /*do nothing*/};
    void update(void){ /*do nothing*/};

protected:
    std::vector<const unsigned char*> icon_vector;
    char *name;
    const unsigned char*current_icon;
    void updateDisplay(void){ /*do nothing*/};
    U8G2 *u8g2;
};

class MenuItemBT : public MenuItem
{
public:
    MenuItemBT(U8G2 *u8g2);
    void update(void);
private:    
    typedef enum {
        e_bt_unknown,
        e_bt_ready,
        e_bt_connected,
    }e_bt_state;

    e_bt_state bt_state;
    void updateDisplay(void);

};

