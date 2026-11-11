## CH32V003F4P6 ##


### PINOUT ###

CH32V003F4P6 TSSOP20 PINOUT
```
                +-----v-----+
      A7/D14~ 1-+PD4     PD3+-20 ~D13/A4
   TX/A5/D15  2-+PD5     PD2+-19 ~D12/A3
   RX/A6/D16  3-+PD6     PD1+-18 ~D11/SWIO
     RST/D17~ 4-+PD7     PC7+-17  D9/MISO
       A1/D0~ 5-+PA1     PC6+-16  D8/MOSI
       A0/D1~ 6-+PA2     PC5+-15  D7/SCK
         GND  7-+VSS     PC4+-14 ~D6/A2/SS (software CS)
         D10~ 8-+PD0     PC3+-13 ~D5
         VCC  9-+VDD     PC2+-12  D4/SCL
     TX'/D2~ 10-+PC0     PC1+-11  D3/RX'/SDA/NSS (hardware SSEL)
                +-----------+    
```

### MAIN FEATURES/LIMITATIONS ###

 - Max 18 I/O pins, PWM/ADC/I2C/UART/SPI
 - Pinout is shared with CH32V002F4P6 and CH32V006F8P6, but there are functional differences.
 - PD7/RST can be used as GPIO pin (with PWM) by disabling RST using WCH Link Utility. By default RST is enabled.
 - TX'/RX' - PC0/PC1 can be used as alternative pins, but when I2C is enabled, RX' conflicts with SDA.
 - PC4 is the default software chip-select pin (PIN_SPI_SS). Control this pin as a GPIO. 
   Hardware SPI NSS/SSEL uses PC1, which is also the default I2C SDA pin. SPI and I2C cannot use PC1 at the same time. 
   Do not pass PC4 as the hardware SSEL argument to SPIClass.
   Using alternative pin mappings now requires changing PeripheralPins.c, but may get core API support in a future release.
 - SWIO is on PD1. Beware of bricking! When used as output pin, the SWIO function will be blocked.  
   To unbrick use WCH Link Utility option "Clear All Code by Power Off".
