# RadioCast
bluetooth sink / Internet Radio receiver

inspired by : ( https://www.hackster.io/esikora/esp32-audio-project-part-i-internet-radio-with-i-s-dac-a5515c)

based on multicore esp32 (needed by audio lib)
32 bit DAC for output controlled through I2S
12x64 I2C Oled screen for display
2 buttons for control, click & doubleClick & long press

optional case? (cnc)

# Hardware

## microcrontroler: ESP32S3 WROOM (extended ram)
pre-requisite: DualCore for performances, BT, Wifi

| <!-- -->              | <!-- -->      |
| -----------           | -----------   |
| Microcontroller       | ESP32         |
| Frequency             | 240MHz        |
| Flash                 | 4MB           |
| RAM                   | 320KB         |
| Vendor                | Espressif     |

![pinout](./ressources/ESP-WROOM-32-Dev-Module-pinout-2117679404.jpg)

## Screen: SH1106 128X64 OLED
Communication Lib:
-  [olikraus/u8g2](https://github.com/olikraus/u8g2), potentially compatible with SSD1306
        * strange behaviour from compiler: not finding csrc et cppsrc in u8g2 => Include directive added to platformio.ini


| <!-- -->              | <!-- -->              |
| -----------           | -----------           |
| Resolution            | 128x64                |
| Type                  | OLED                  |
| Voltage               | 3.3V-5V DC            |
| Driver IC             | SH1106                |
| Protocol              | IIC(GND,VCC,SCL,SDA)  |
| Module Size           | 27mmx 27mm x 4mm      |

<img align="center" src="ressources/I2C-OLED-Display-Module-Pinout.jpg" width="300">

from AE: [link](https://fr.aliexpress.com/item/1005004355547926.html)

*Image generation workflow:*
- scale down to YYxZZ
- saved as XMB
- append PROGMEM to declaration

## DAC: PCM5102A

| <!-- -->              | <!-- -->              |
| -----------           | -----------           |
| Resolution            | 32bits                |
| Dynamic range         | 212dB                 |
| Voltage               | 3.3V                  |
| Protocol              | I2S(GND,VCC,DIN,BCK,LCK)  **SCK tied to GND** |

<img align="center" src="ressources/DAC.jpg" width="300">

from AE: [link](https://fr.aliexpress.com/item/1005005393398013.html)



# Implementation choices

## Updating screen

Time management in main loop:
> IF time elapsed since last display update > Frame period  
> THEN  
>     * memorize current time  
>     * increment frame count  
>     * call updateDisplay  


# UI flow

2 functions: 
- Radio from internet
- A2DP bluetooth sink

use *Button 1* short press to pass from one to another.  
Bluetooth / wifi only activated in their respected mode  
  
1 auxiliary function: *Button 1* long press enter WIFI AP configuration mode. Allows to:
- input credential for the Wifi
- update firmware

## Radio

press *button 2* to cycle between Radio stations. Those are hardcoded

## Bluetooth sink

press *button 2* to force disconnect

# TODO List
* memorize & load through AP mode :
        - urls list
        - ESP name