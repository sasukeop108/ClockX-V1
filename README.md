# ClockX-V1

This is a clock which can display time, Date etc, alarms can be set ,has games and has 4 inbuilt buttons to control them.
I decided to make this because i wake up from sleep, I cannot se the time clearly because the clock is far away from my
bed and im feeling lazy to move near the clock and see the time , So i designed ClockX which has my most favourite and usefull things at one place.

It is powered by a wemos d1 mini for proccessing all data and has 0.96 inch i2c oled display for displaying all the datas.
It has a menu with  4 options which are NTP clock, Alarm, Setting, and game. It has 1000 MaH which would power them  for some hours.
It derives time using the NTP protocol and inbuilt WiFi of wemos d1 mini. It has a chrome dino game in the game menu.

# Circuit Diagram
[Circuit Diagram](https://github.com/sasukeop108/ClockX-V1/blob/main/Photos/circuitDiagram.png)
# BOM
# Bill of Materials

| Item | Description | Qty | Unit Price (INR) | Total (INR) | Unit Price (USD)* | Total (USD)* | Link |
| :--- | :--- | :--- | :--- | :--- | :--- | :--- | :--- |
| Microcontroller | Wemos D1 Mini | 1 | ₹699.00 | ₹699.00 | $8.42 | $8.42 | [makerbazar.in](https://makerbazar.in/products/wemos-d1-mini-iot-esp8266-based-development-board?variant=48251175600368) |
| DISPLAY | 128x64 I2C OLED Display | 1 | ₹189.00 | ₹189.00 | $2.28 | $2.28 | [makerbazar.in](https://makerbazar.in/products/0-96-inch-128x64-oled-display-module) |
| Boost Module | Step-up Converter | 1 | ~~₹65.00~~ | **Self Sourced** | ~~$0.78~~ | **Self Sourced** | **Self Sourced** |
| Buzzer | Hdp1206 Active Buzzer | 1 | ₹19.00 | ₹19.00 | $0.23 | $0.23 | [makerbazar.in](https://makerbazar.in/products/b12-hdp1206-mini-active-buzzer) |
| Button | 6x6mm Push Button | 1 | ₹20.00 | ₹20.00 | $0.24 | $0.24 | [makerbazar.in](https://makerbazar.in/products/small-push-button-switch?variant=44840000880880) |
| Battery | 800mAh LiPo Battery | 1 | ₹150.00 | ₹150.00 | $1.81 | $1.81 | [quartzcomponents.com](https://quartzcomponents.com/products/3-7v-800mah-li-po-rechargeable-battery) |
| Charger | TP4056 Charger Module | 1 | ₹16.00 | ₹16.00 | $0.19 | $0.19 | [makerbazar.in](https://makerbazar.in/products/tp4056-1a-li-ion-lithium-battery-charging-module?_pos=9&_sid=fec3b1e5e&_ss=r&variant=45459125567728) |
| Resistor | 10kΩ THT Resistor (Pack of 10) | 1 | ₹5.00 | ₹5.00 | $0.06 | $0.06 | [makerbazar.in](https://makerbazar.in/products/carbon-film-resistor-dip-1-4-watt-resistance-through-hole-1k-to-99kohm?variant=47248529621232) |
| Power Switch | Slide Switch (SPDT) | 2 | ₹8.00 | ₹16.00 | $0.10 | $0.19 | [makerbazar.in](https://makerbazar.in/products/right-angle-3-pin-2-position-spdt-1p2t-pcb-panel-mini-slide-switch-l-shape-bent?pr_prod_strat=e5_desc&pr_rec_id=337371f17&pr_rec_pid=7019382800535&pr_ref_pid=8573508452592&pr_seq=uniform&variant=48251177631984) |
| Capacitor | 100µF 16V Capacitor (Pack of 10) | 1 | ₹12.00 | ₹12.00 | $0.14 | $0.14 | [makerbazar.in](https://makerbazar.in/products/electrolytic-capacitor-through-hole?variant=48721342562544) |
| Header | Female Berg Strip (40-pin) | 1 | ~~₹8.50~~ | **Self Sourced** | ~~$0.10~~ | **Self Sourced** | **Self Sourced** |
| Nut | M3*12mm nut (Pack of 10) | 1 | ₹42.00 | ₹42.00 | $0.51 | $0.51 | [quartzcomponents.com](https://quartzcomponents.com/products/m3-x-10mm-hex-allen-socket-head-ss202-screwpack-of-10) |
| Heat Insert | M3x12mm Brass Heat Set Insert Nut (10 Pcs) | 2 | ₹42.00 | ₹84.00 | $0.51 | $1.02 | [quartzcomponents.com](https://quartzcomponents.com/products/m3x12mm-brass-heat-set-threaded-round-insert-nut-10-pcs) |
| Heatshrink | Black Heatshrink (1m) | 1 | ₹7.00 | ₹7.00 | $0.08 | $0.08 | [makerbazar.in](https://makerbazar.in/products/heat-shrink?variant=44960740475120) |
| **Subtotal (Purchased Components)** | | | | **₹1259.00** | | **$15.17** | |
| **Maker Bazaar Delivery** | | | | **+₹60.00** | | **+$0.72** | |
| **Quartz Components Delivery** | | | | **+₹50.00** | | **+$0.60** | |
| **Quartz Components Tax** | | | | **+₹58.68** | | **+$0.71** | |
| **PCB Grant** | PCB Manufacturing Grant | | | **+₹1169.47** | | **+$14.09** | |
| **3D Print Grant** |Shipping Grant | | | **+₹498.00** | | **+$6.00** | |
| **GRAND TOTAL** | | | | **₹3095.15** | | **$37.29** | |

*\*Note: USD conversions are approximate and based on an exchange rate of 1 USD = 83 INR. Actual rates may vary at the time when you see. It is approximately 34 dollar according to today's exchange rate.*

## Summary

- **Maker Bazar Purchased Items:** 9 items totaling **₹983.00** (Wemos + Display + Buzzer + Button + Charger + Resistor + Switch + Capacitor + Heatshrink)
- **Quartz Components Items:** Battery + Nut + 2×Heat Insert = **₹276.00**
- **Self Sourced Items:** Boost Module (₹65) + Header (₹8.50) = **₹73.50**
- **Delivery & Tax:** Maker Bazaar Delivery (₹60) + Quartz Delivery (₹50) + Quartz Tax (₹58.68) = **₹168.68**
- **Grants:** PCB Grant (₹1,169.47) + 3D Print Grant (₹498.00) = **₹1,667.47**
- **Grand Total:** **₹3,095.15 INR** (approximately **$37.29 USD**)

## Self Sourced Items (Not Purchased)
- Boost Module (Step-up Converter) - ₹65.00
- Header (Female Berg Strip 40-pin) - ₹8.50

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
