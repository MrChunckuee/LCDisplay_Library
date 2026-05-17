/*******************************************************************************
 *
 *                 LCD Display Library
 *
 *******************************************************************************
 * FileName:        LiquidCrystalDisplay.c
 * Complier:        XC8 v2.31
 * Author:          Pedro Sanchez Ramirez (@mrchunckuee_electronics)
 * Blog:            http://mrchunckuee.blogspot.com/
 * Email:           mrchunckuee.electronics@gmail.com
 * Description:     Library for control LCD16x2 or LCD20x4 display use in PIC
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
 ******************************************************************************/

#include <stdbool.h>
#include <xc.h>
#include "LiquidCrystalDisplay.h"

#define _XTAL_FREQ 16000000

LCD_t LCDisplay;

// Macros internos para manipular bits
#define SET_BIT(reg, bit)    (*(reg) |= (1 << (bit)))
#define CLR_BIT(reg, bit)    (*(reg) &= ~(1 << (bit)))

/*******************************************************************************
 * Function:        bool LCD_Init(LCD_t display)
 * Description:     Initializes the LCD with the specified pin configuration,
 *                  sets up 4-bit mode, cursor options, and clears the screen.
 * Precondition:    None
 * Parameters:      display - LCD_t structure containing ports and pin mappings.
 * Return Values:   true if configuration is successful, false otherwise.
 * Remarks:         None
 ******************************************************************************/
bool LCD_Initialize(LCD_t display){
    LCDisplay = display;
    if (LCDisplay.TRIS != NULL) {
        // Clear only the appropriate bits
        CLR_BIT(LCDisplay.TRIS, LCDisplay.RS);
        CLR_BIT(LCDisplay.TRIS, LCDisplay.EN);
        CLR_BIT(LCDisplay.TRIS, LCDisplay.D4);
        CLR_BIT(LCDisplay.TRIS, LCDisplay.D5);
        CLR_BIT(LCDisplay.TRIS, LCDisplay.D6);
        CLR_BIT(LCDisplay.TRIS, LCDisplay.D7);
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

    // Send reset signal to the LCD (RS = 0)
    CLR_BIT(LCDisplay.PORT, LCDisplay.RS); 
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

/*******************************************************************************
 * Function:        void LCD_Out(char c)
 * Description:     Sets the physical status of data pins (D4-D7) according to 
 *                  the 4 low bits of the passed character.
 * Precondition:    None
 * Parameters:      c - character containing the 4-bit nibble data.
 * Return Values:   None
 * Remarks:         None
 ******************************************************************************/
void LCD_Out(char c){
    // Clear bits D4-D7 
    CLR_BIT(LCDisplay.PORT, LCDisplay.D4);
    CLR_BIT(LCDisplay.PORT, LCDisplay.D5);
    CLR_BIT(LCDisplay.PORT, LCDisplay.D6);
    CLR_BIT(LCDisplay.PORT, LCDisplay.D7);
    
    // SET_BIT si el bit en 'c' está activo
    if(c & 1) SET_BIT(LCDisplay.PORT, LCDisplay.D4);
    if(c & 2) SET_BIT(LCDisplay.PORT, LCDisplay.D5);
    if(c & 4) SET_BIT(LCDisplay.PORT, LCDisplay.D6);
    if(c & 8) SET_BIT(LCDisplay.PORT, LCDisplay.D7);
}

/*******************************************************************************
 * Function:        void LCD_Write(uint8_t data)
 * Description:     Outputs data to the port and generates the Enable pulse 
 *                  (high-to-low latching signal).
 * Precondition:    None
 * Parameters:      data - 8-bit data value (only low nibble is effectively used).
 * Return Values:   None
 * Remarks:         None
 ******************************************************************************/
void LCD_Write(uint8_t data){
    LCD_Out(data);
    SET_BIT(LCDisplay.PORT, LCDisplay.EN);     // EN = 1
    __delay_ms(4);
    CLR_BIT(LCDisplay.PORT, LCDisplay.EN);     // EN = 0
}

/*******************************************************************************
 * Function:        void LCD_Command(uint8_t cmd)
 * Description:     Sends a command byte to the LCD splitting it into high 
 * and low nibbles with RS line cleared.
 * Precondition:    None
 * Parameters:      cmd - instruction command byte to send.
 * Return Values:   None
 * Remarks:         None
 ******************************************************************************/
void LCD_Command(uint8_t cmd){
    CLR_BIT(LCDisplay.PORT, LCDisplay.RS);     // RS = 0(command)
    LCD_Write((cmd & 0xF0) >> 4);   // High nibble 
    LCD_Write(cmd & 0x0F);          // Low nibble
}

/*******************************************************************************
 * Function:        void LCD_putc(char c)
 * Description:     Sends a character data byte to the LCD splitting it into 
 * high and low nibbles with RS line set.
 * Precondition:    None
 * Parameters:      c - character byte to display.
 * Return Values:   None
 * Remarks:         None
 ******************************************************************************/
void LCD_putc(char c) {
    SET_BIT(LCDisplay.PORT, LCDisplay.RS);     // RS = 1 (data)
    LCD_Write((c & 0xF0) >> 4);     // High nibble
    LCD_Write(c & 0x0F);            // Low nibble
}

/*******************************************************************************
 * Function:        void LCD_puts(char *a)
 * Description:     Prints a null-terminated string from RAM to the LCD.
 * Precondition:    None
 * Parameters:      a - pointer to the string array in RAM.
 * Return Values:   None
 * Remarks:         None
 ******************************************************************************/
void LCD_puts(char *a){
    for(int i = 0; a[i] != '\0'; ++i){
        LCD_putc(a[i]);
    }
}

/*******************************************************************************
 * Function:        void LCD_putrs(const char *a)
 * Description:     Prints a null-terminated string from Program Memory (ROM) 
 * to the LCD.
 * Precondition:    None
 * Parameters:      a - pointer to the constant string array in ROM.
 * Return Values:   None
 * Remarks:         None
 ******************************************************************************/
void LCD_putrs(const char *a){
    for(int i = 0; a[i] != '\0'; ++i){
        LCD_putc(a[i]);
    }
}

/*******************************************************************************
 * Function:        void LCD_createChar(uint8_t location, const uint8_t charmap[])
 * Description:     Stores a custom character bitmap into the LCD CGRAM at 
 *                  the specified location.
 * Precondition:    None
 * Parameters:      location - CGRAM address character slot (0 to 7).
 *                  charmap - array of 8 bytes defining the custom pixel pattern.
 * Return Values:   None
 * Remarks:         None
 ******************************************************************************/
void LCD_createChar(uint8_t location, const uint8_t charmap[]){
    // Direcction CGRAM, start 0x40
    LCD_Command (0x40 | ((location & 0x07) << 3));
    
    // Send 8 bytes, special caracter
    for(uint8_t i = 0; i < 8; i++){
        LCD_putc(charmap[i]);
    }
}
