# RadioCast
bluetooth sink / Internet Radio receiver

based on multicore esp32 (needed by audio lib)
use 32 bit DAC for output
basic screen for display
2 buttons for control, short & long press

optional case? (cnc)

# Hardware

## ucrontroler
pre-requisite: DualCore for performances, BT, Wifi

| <!-- -->              | <!-- -->      |
| -----------           | -----------   |
| Microcontroller       | ESP32         |
| Frequency             | 240MHz        |
| Flash                 | 4MB           |
| RAM                   | 320KB         |
| Vendor                | Espressif     |

![pinout](ressources\ESP-WROOM-32-Dev-Module-pinout-2117679404.jpg)

## Screen

Communication Lib:
- for option 1 adafruit/Adafruit_SSD1306 but unsuitable for SH1106
- for SH1106 olikraus/u8g2, potentially compatible with SSD1306 (untested(yet?))
        * strange behaviour from compiler: not finding csrc et cppsrc in u8g2 => Include directive added to platformio.ini

### option 1 :

| <!-- -->              | <!-- -->              |
| -----------           | -----------           |
| Screen Display Size   | 0.91″                 |
| Resolution            | 128X32                |
| Type                  | OLED                  |
| Voltage               | 3.3V-5V DC            |
| Driver IC             | SSD1306               |
| Protocol              | IIC(GND,VCC,SCL,SDA)  |

from AE: (https://fr.aliexpress.com/item/1005004355547926.html)
converting pbg to byte arrays: (https://mischianti.org/images-to-byte-array-online-converter-cpp-arduino/)


### option 2 **USED** : 

| <!-- -->              | <!-- -->              |
| -----------           | -----------           |
| Resolution            | 128x64                |
| Type                  | OLED                  |
| Voltage               | 3.3V-5V DC            |
| Driver IC             | SH1106                |
| Protocol              | IIC(GND,VCC,SCL,SDA)  |
| Module Size           | 27mmx 27mm x 4mm      |

![pinout](ressources\I2C-OLED-Display-Module-Pinout.jpg)
from AE: (https://fr.aliexpress.com/item/1005004355547926.html)

*Image generation workflow:*
- scale down to YYxZZ
- saved as XMB
- append PROGMEM to declaration

Glyphs saved in [glyph file](ressources\glyphs_50x50)

# Implementation choices

## Updating screen

Time management in main loop:
> IF time elapsed since last display update > Frame period
> THEN
>     * memorize current time
>     * increment frame count
>     * call updateDisplay

drawbacks:
- no missing frame catch-up


# basic flow
Démarrage mode radio

-          Lancement Wifi avec connexion non bloquante

-          Choix 1ere radio

Icone Wifi représentative de l’etat

B1 Appui long : portail captif / re_appui long : reboot

B1 court : next mode

B2 Appui court : next station

 

Si passage mode BT

                Wifi off

                BT on

 

                B1 Appui long : deconnection forcée

B1 court : next mode

 

        Icone BT representative de l’etat + string appareil connecté

        B2 Appui court : mute ?