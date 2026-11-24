## CH32V203F8U6 ##


### PINOUT ###

CH32V203F8U6 QFN20->DIP22 DEV-BOARD PINOUT
```
                      +-----v-----+
       /WKUP/A0/D0~ 1-+PA0     PA6+-20 ~D6/A6/MISO
             A1/D1~ 2-+PA1     VDD+-19  VCC
         TX2/A2/D2~ 3-+PA2    PA10+-18 ~D10/RX
         RX2/A3/D3~ 4-+PA3    PA14+-17  D14/SWCLK = ~PA11/D11/UDM/CRX
          SS/A4/D4  5-+PA4 /\ PA13+-16  D13/SWDIO =  PA12/D12/UDP/CTX
         SCK/A5/D5  6-+PA5 \/  PA9+-15 ~D9/TX
        MOSI/A7/D7~ 7-+PA7     PA8+-14 ~D8/MCO
           TX4/D16~ 8-+PB0    PB15+-13 ~D21
           RX4/D17~ 9-+PB1    PB14+-12 ~D20
      TX3/SCL2/D18 10-+PB10   PB11+-11  D19/SDA2/RX3
               GND   -+VSS     VSS+-    GND 
                      +-----------+    
```
BOOT0=GND
PA12/UDP=PA13/SWDIO
PA11/UDM=PA13/SWCLK

### MAIN FEATURES/LIMITATIONS ###

 - Max 19 I/O pins, PWM/ADC/I2C2/4xUART/SPI
 - I2C is only available via I2C2. This variant selects Wire to use that peripheral instance. 
 - Missing pins: SPI2 misses SCK pin. No pin for external reset.
 - SWDIO+SWCLK are needed for programming. These are shared with UDM/UDP, preventing simultaneous use of these functions.
 - SWDIO is on PA13, SWCLK is on PA14. Beware of bricking! When used as GPIO pin, the SWIO function can be blocked.  
   To unbrick use WCH Link Utility option "Clear All Code by Power Off".
