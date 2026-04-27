/*******************************************************************************
 *
 *                       LCDisplay Library
 *
 *******************************************************************************
 *                  MIT License
 * 
 * Copyright (c) 2016 Pedro Sánchez Ramírez
 * 
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 * 
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 * 
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 ********************************************************************************/

#include <stdbool.h>
#include <xc.h>
#include "LiquidCrystalDisplay.h"

#define _XTAL_FREQ 16000000

LCD_t LCDisplay;

bool LCD_Init(LCD_t display){
    LCDisplay = display;
    
    if (LCDisplay.TRIS != NULL) {
        // Clear only the appropriate bits
        *(LCDisplay.TRIS) &= ~((1 << LCDisplay.RS) | (1 << LCDisplay.EN) | (1 << LCDisplay.D4) | (1 << LCDisplay.D5) | (1 << LCDisplay.D6) | (1 << LCDisplay.D7));
    } else {
        // The function should clear only the appropriate bits, not the whole PORT
        // "#if defined" needs to support more microcontrollers that have PORTD and PORTE
        if (LCDisplay.PORT == &PORTA) TRISA = 0x00;
        else if (LCDisplay.PORT == &PORTB) TRISB = 0x00;
        else if (LCDisplay.PORT == &PORTC) TRISC = 0x00;
        #if defined(_16F877) || defined(_16F877A) || defined(_18F4620)
        else if (LCDisplay.PORT == &PORTD) TRISD = 0x00;
        else if (LCDisplay.PORT == &PORTE) TRISE = 0x00;
        #endif
        else return false;
    }

    // Give some time to the LCD to start function properly
    __delay_ms(20);

    // Send reset signal to the LCD
    *(LCDisplay.PORT) &= ~(1 << LCDisplay.RS); // => RS = 0
    LCD_Write(0x03); 
    __delay_ms(5);
    LCD_Write(0x03); 
    __delay_ms(16);
    LCD_Write(0x03); 

    // Specify the data lenght to 4 bits
    LCD_Write(0x02); 

    // Set interface data length to 8 bits, number of display lines to 2 and font to 5x8 dots
    LCD_Command (0x28); 

    // Set cursor to move from left to right
    LCD_Command (0x06); 

    LCD_Display(true, false, false); // Turn on display and set cursor off

    LCD_Clear();
    
    return true;
}

void LCD_Out(char c){
    // Clear bits D4-D7 
    *(LCDisplay.PORT) &= ~((1 << LCDisplay.D4) | (1 << LCDisplay.D5) | (1 << LCDisplay.D6) | (1 << LCDisplay.D7));
    
    if(c & 1) *(LCDisplay.PORT) |= 1 << LCDisplay.D4;
    if(c & 2) *(LCDisplay.PORT) |= 1 << LCDisplay.D5;
    if(c & 4) *(LCDisplay.PORT) |= 1 << LCDisplay.D6;
    if(c & 8) *(LCDisplay.PORT) |= 1 << LCDisplay.D7;
}

void LCD_Write(uint8_t data){
    LCD_Out(data);
    *(LCDisplay.PORT) |= 1 << LCDisplay.EN;     // EN = 1
    __delay_ms(4);
    *(LCDisplay.PORT) &= ~(1 << LCDisplay.EN);  // EN = 0
}

void LCD_Command(uint8_t cmd){
    *(LCDisplay.PORT) &= ~(1 << LCDisplay.RS);  // RS = 0(command)
    LCD_Write((cmd & 0xF0) >> 4);   // High nibble 
    LCD_Write(cmd & 0x0F);          // Low nibble
}

void LCD_putc(char c) {
    *(LCDisplay.PORT) |= 1 << LCDisplay.RS;     // RS = 1 (data)
    LCD_Write((c & 0xF0) >> 4);     // High nibble
    LCD_Write(c & 0x0F);            // Low nibble

}

void LCD_puts(char *a){
    for(int i = 0; a[i] != '\0'; ++i){
        LCD_putc(a[i]);
    }
}

void LCD_putrs(const char *a){
    for(int i = 0; a[i] != '\0'; ++i){
        LCD_putc(a[i]);
    }
}

void LCD_createChar(uint8_t location, const uint8_t charmap[]){
    // Direcction CGRAM, start 0x40
    LCD_Command (0x40 | ((location & 0x07) << 3));
    
    // Send 8 bytes, special caracter
    for(uint8_t i = 0; i < 8; i++){
        LCD_putc(charmap[i]);
    }
}
