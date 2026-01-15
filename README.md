# ClockX-V1

This is a clock which can display time, Date etc, alarms can be set ,has games and has 4 inbuilt buttons to control them.
I decided to make this because i wake up from sleep, I cannot se the time clearly because the clock is far away from my
bed and im feeling lazy to move near the clock and see the time , So i designed ClockX which has my most favourite and usefull things at one place.

It is powered by a wemos d1 mini for proccessing all data and has 0.96 inch i2c oled display for displaying all the datas.
It has a menu with  4 options which are NTP clock, Alarm, Setting, and game. It has 1000 MaH which would power them  for some hours.
It derives time using the NTP protocol and inbuilt WiFi of wemos d1 mini. It has a chrome dino game in the game menu.

# Circuit Diagram
![Circuit Diagram](https://github.com/sasukeop108/ClockX-V1/blob/main/design/circuitDiagram.png)

# BOM
|Item           |Description                     |Qty|Unit Price (INR)|Total (INR)|Unit Price (USD)|Total (USD)|Link                                                                                                                                                                                                                                            |
|---------------|--------------------------------|---|----------------|-----------|----------------|-----------|------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------|
|Microcontroller|Wemos D1 Mini                   |1  |599.00          |599.00     |6.63            |6.63       |https://makerbazar.in/products/wemos-d1-mini-iot-esp8266-based-development-board?variant=48251175600368                                                                                                                                         |
|DISPLAY        |128x64 I2C OLED Display         |1  |199.00          |199.00     |2.20            |2.20       |https://makerbazar.in/products/0-96-inch-128x64-oled-display-module                                                                                                                                                                             |
|Boost Module   |Step-up Converter               |1  |99.00           |99.00      |1.10            |1.10       |https://makerbazar.in/products/boost-converterdc-dc?variant=19361177763936                                                                                                                                                                      |
|Buzzer         |Hdp1206 Active Buzzer           |1  |25.00           |25.00      |0.28            |0.28       |https://makerbazar.in/products/b12-hdp1206-mini-active-buzzer                                                                                                                                                                                   |
|Button         |6x6mm Push Button               |1  |25.00           |25.00      |0.28            |0.28       |https://makerbazar.in/products/small-push-button-switch?variant=44840000880880                                                                                                                                                                  |
|Battery        |10000mAh LiPo Battery           |1  |369.00          |369.00     |4.09            |4.09       |https://makerbazar.in/products/single-cell-3-7v-10000mah-lipo-battery?variant=48251044659440                                                                                                                                                    |
|Charger        |TP4056 Charger Module           |1  |49.00           |49.00      |0.54            |0.54       |https://makerbazar.in/products/tp4056-1a-li-ion-lithium-battery-charging-module?_pos=9&_sid=fec3b1e5e&_ss=r&variant=45459125567728                                                                                                              |
|Resistor       |10kΩ THT Resistor (Pack of 10)  |1  |29.00           |29.00      |0.32            |0.32       |https://makerbazar.in/products/carbon-film-resistor-dip-1-4-watt-resistance-through-hole-1k-to-99kohm?variant=47248529621232                                                                                                                    |
|Power Switch   |Slide Switch (SPDT)             |2  |13.00           |26.00      |0.14            |0.29       |https://makerbazar.in/products/right-angle-3-pin-2-position-spdt-1p2t-pcb-panel-mini-slide-switch-l-shape-bent?pr_prod_strat=e5_desc&pr_rec_id=337371f17&pr_rec_pid=7019382800535&pr_ref_pid=8573508452592&pr_seq=uniform&variant=48251177631984|
|Capacitor      |100µF 16V Capacitor (Pack of 10)|1  |13.00           |13.00      |0.14            |0.14       |https://makerbazar.in/products/electrolytic-capacitor-through-hole?variant=48721342562544                                                                                                                                                       |
|Zero PCB       |Green PCB Prototype Board       |1  |119.00          |119.00     |1.32            |1.32       |https://makerbazar.in/products/double-sided-universal-pcb-prototype-board-2-54mm-hole-pitch?variant=46000183771376                                                                                                                              |
|Header         |Female Berg Strip (40-pin)      |1  |29.00           |29.00      |0.32            |0.32       |https://makerbazar.in/products/female-berg-strip-2-54mm-header-pin                                                                                                                                                                              |
|Nut & Bolt     |Phillips Set (Pack of 10)       |1  |29.00           |29.00      |0.32            |0.32       |https://makerbazar.in/products/philips-head-nuts-bolts-set-silver-plated-pack-of-10?variant=43604541571312                                                                                                                                      |
|Heatshrink     |Black Heatshrink (1m)           |1  |13.00           |13.00      |0.14            |0.14       |https://makerbazar.in/products/heat-shrink?variant=44960740475120                                                                                                                                                                               |
|TOTAL          |                                |14 |                |1550.00    |                |17.17      |

# Cart photos
![CART1](https://github.com/sasukeop108/ClockX-V1/blob/main/BOM/Screenshot%202026-01-15%20044426.png) ![CART2](https://github.com/sasukeop108/ClockX-V1/blob/main/BOM/Screenshot%202026-01-15%20044454.png)

# Key Features
* Game high scores are saved in EEPROM

* Alarms will still continue even when the game is being used

* All the alarm's are saved in EEPROM

* Live and accurate time with no mistake

* The use of a specific button can be changed by coding

  # Libraries Required

  * Wire.h
  * u8g2lib.h
  * ESP8266WiFi.h
  * EEPROM.h
  * NTPClient.h
  * WiFiUDP.h
 
    
**Project url**- https://blueprint.hackclub.com/projects/9123
