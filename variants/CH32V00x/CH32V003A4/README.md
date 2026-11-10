## CH32V003A4M6 ##


### PINOUT ###

CH32V003A4M6 SOP16 PINOUT
```
              +-----v-----+
RX'/SDA D3  1-+PC1     PC0+-16 D2~/TX'
    SCL D4  2-+PC2     VDD+-15 VCC
        D5~ 3-+PC3     VSS+-14 GND
     A2/D6~ 4-+PC4     PA2+-13 D1/A0
        D7  5-+PC6     PA1+-12 D0~/A1
        D8  6-+PC7     PD7+-11 RST
      SWIO  7-+PD1     PD6+-10 RX/A6
    A7/D10  8-+PD4     PD5+-9  TX/A5
              +-----------+
```

### MAIN FEATURES/LIMITATIONS ###

 - Max 14 I/O pins, PWM/ADC/I2C/UART. CH32V003A4M6 has no SPI!
 - Pinout is similar to CH32V002A4M6, but some Arduino pin numbers are different.
 - Hardware SPI is missing SCK pin and is unavailable.
   If a sketch needs SPI, use a software SPI implementation.
 - PD7/RST can be used as GPIO pin by disabling RST using WCH Link Utility. By default RST is enabled.
 - TX'/RX' - PC0/PC1 can be used as alternative pins, but when I2C is enabled, RX' conflicts with SDA.
