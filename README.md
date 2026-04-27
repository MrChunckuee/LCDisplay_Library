## LCDisplay Library
A C library for driving standard character LCDs (HD44780 compliant) with Microchip PIC microcontrollers. Designed for ease of use, it supports both 4-bit and 8-bit modes and is compatible with any PIC16 or PIC18 device using the XC8 compiler. Also have option for show big digits segments.

## Features
* **Flexible Configuration:** Uses a structure-based initialization for easy integration.
* **LCD Sizes:** Supports 16x1, 16x2, 20x2, and 20x4 modules.
* **4-bit Mode Support:** Save I/O pins by using only 4 data lines.
* **Smart Functions:** Includes commands for clear screen, home, cursor positioning, and custom character creation.
* **Special feature:** Includes files for displaying characters in large segments (`LCD_BIGDigits.c` and `LCD_BIGDigits.h`).

## Hardware Connection
Typical connection for 4-bit mode operation:

| LCD Pin | Function | PIC Connection (Example) |
| :--- | :--- | :--- |
| **VSS** | Ground | GND |
| **VDD** | Power | +5V |
| **VO** | Contrast | Potentiometer Center |
| **RS** | Register Select | Digital Pin (e.g., RD2) |
| **RW** | Read/Write | GND (Always Write) |
| **E** | Enable | Digital Pin (e.g., RD3) |
| **D4-D7** | Data Bits | Digital Pins (e.g., RD4-RD7) |
| **A / K** | Backlight | +5V / GND (via resistor) |

##  Usage Example
This example shows how to initialize the display and print formatted text.

```c
#include <xc.h>
#include "lcd_display.h"

void main(void) {
    // Initialize system ports
    SYSTEM_Initialize();
    
    // Initialize LCD (4-bit mode default)
    LCD_Init();
    
    // Clear screen and say hello
    LCD_Clear();
    LCD_SetCursor(1, 1); // Row 1, Column 1
    LCD_Print("MrChunckuee");
    
    LCD_SetCursor(2, 1); // Row 2, Column 1
    LCD_Print("PIC18F25K22");
    
    while(1) {
        // Your application logic
    }
}
```

## Project Structure
* `src/`: Core library files (`LiquidCrystalDisplay.c`, `LiquidCrystalDisplay.h`, `LCD_BIGDigits.c` and `LCD_BIGDigits.h`).
* `examples/`: Project examples for MPLAB X.
* `LICENSE`: MIT License.
* `CHANGELOG`: History of updates and bug fixes.

## Documentation & Tutorial
For a detailed implementation explanation and step-by-step guide, you can review the following examples:
* [PIC18F25K22 & XC8: Librería para pantalla LCD 16x2 - Test basico](https://mrchunckuee.blogspot.com/2017/02/mplabx-y-xc8-005.html)
* [PIC18F25K22 & XC8: Libreria para LCD - Creacion de caracteres personalizados en la CGRAM](https://mrchunckuee.blogspot.com/2025/10/pic18f25k22-xc8-libreria-para-lcd.html)
* [PIC18F25K22 & XC8: Libreria para LCD - Implementacion de funciones para mostrar numeros grandes](https://mrchunckuee.blogspot.com/2025/10/pic18f25k22-xc8-libreria-para-lcd_01385353221.html)
* [PIC18F25K83 & XC8: ADC modo basico- Leer voltaje y mostrarlo en una LCD16x2 con números grandes](https://mrchunckuee.blogspot.com/2025/12/pic18f25k83-xc8-adc-modo-basico-leer-de.html)
* [PIC18F25K22 & XC8: Uso del sensor de temperatura y humedad relativa DHT22/AM2302 - Mostrar datos en una LCD16x2](https://mrchunckuee.blogspot.com/2026/03/pic18f25k22-xc8-uso-del-sensor-de.html)
