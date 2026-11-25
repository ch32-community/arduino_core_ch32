## CH32V203F8P6 ##


### PINOUT ###

CH32V203F8P6 TSSOP20 PINOUT
```
                          +-----v-----+
SWDIO=PA13/UDM/SCL/D17~ 1-+PB6    PA10+-20 ~D10/RX
SWCLK=PA14/UDP/SDA/D18~ 2-+PB7     PA9+-19 ~D9/TX
                   GND  3-+VSS     PA8+-18 ~D8/MCO
                   VCC  4-+VDD    PB15+-17 ~D21/OP1P0
                /RESET  5-+RST    PB14+-16 ~D20/OP2P0
            WKUP/A0/D0~ 6-+PA0    PB13+-15 ~D19
                 A1/D1~ 7-+PA1     PB0+-14 ~D16/A8/OP1P1 
       OP2O0/TX2/A2/D2~ 8-+PA2     PA7+-13 ~D7/A7/MOSI 
       OP1O0/RX2/A3/D3~ 9-+PA3     PA5+-12  D5/A5/SCK 
     OP1N1/MISO/A6/D6~ 10-+PA6     PA4+-11  D4/A4/SS 
                          +-----------+    
```
BOOT0=GND (TIM1_BKIN)
PB6/D17/UDM/SCL=PA13/D13/SWDIO
PB7/D18/UDP/SDA=PA14/D14/SWCLK

### MAIN FEATURES/LIMITATIONS ###

 - Max 17 I/O pins, PWM/ADC/I2C/UART/SPI
 - RST can not be used as GPIO pin. It only functions as reset pin (active low)
 - SWDIO/SWCLK are on PA13/PA14. These are shared with PB6/UDM/SCL and PB7/UDP/SDA. Beware of bricking! 
   When used as GPIO pin, shared pins can not both be output and the programming function can be blocked. 
   To unbrick use WCH Link Utility option "Clear All Code by Power Off".
