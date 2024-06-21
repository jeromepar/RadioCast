#pragma once


#include <cstdint>
#include <vector>

class MenuItem
{
public:
    MenuItem(const char *name);

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
};

class MenuItemBT : public MenuItem
{
public:
    MenuItemBT(void);
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

