# RadioCast
bluetooth sink / Internet Radio receiver

based on multicore esp32 (needed by audio lib)
use 32 bit DAC for output
basic screen for display
3-4 buttons for control

optional case? (cnc)

# Hardware
## Screen
Screen Display Size: 0.91″ 

Display Resolution: 128X32 pixel 
Working Voltage: DC 3.3-5V 
Driver IC:SSD1306 
Communication Interface:IIC(GND,VCC,SCL,SDA) 
from AE: (https://fr.aliexpress.com/item/1005004355547926.html)

converting pbg to byte arrays: (https://mischianti.org/images-to-byte-array-online-converter-cpp-arduino/)

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