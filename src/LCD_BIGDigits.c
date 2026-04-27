/*******************************************************************************
 *
 *                  LCD BIG Digits for LCD Library
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
#include <xc.h>
#include <string.h>
#include "LiquidCrystalDisplay.h"
#include "LCD_BIGDigits.h"


// Global variable to hold the pointer to the current array of digits
const uint8_t (*current_digits)[4];

// Set eight special characters for the font: tron_bold 
const uint8_t tron_bold_chars[8][8] = {
    {0b11111, 0b11000, 0b00000, 0b00000, 0b00000, 0b00000, 0b00000, 0b00000}, // bits for special character 1/8
    {0b00000, 0b00000, 0b00000, 0b00000, 0b00000, 0b00000, 0b11000, 0b11111}, // bits for special character 2/8
    {0b11111, 0b00111, 0b00111, 0b00111, 0b00111, 0b00111, 0b00111, 0b00111}, // bits for special character 3/8
    {0b11111, 0b11100, 0b11100, 0b11100, 0b11100, 0b11100, 0b11100, 0b11111}, // bits for special character 4/8
    {0b11111, 0b11100, 0b11100, 0b11100, 0b11100, 0b11100, 0b11100, 0b11100}, // bits for special character 5/8
    {0b11100, 0b11100, 0b11100, 0b11100, 0b11100, 0b11100, 0b11100, 0b11100}, // bits for special character 6/8
    {0b11111, 0b00000, 0b00000, 0b00000, 0b00000, 0b00000, 0b00000, 0b11111}, // bits for special character 7/8
    {0b11100, 0b11100, 0b11100, 0b11100, 0b11100, 0b11100, 0b11100, 0b11111}  // bits for special character 8/8
};

// Special character used for the individual digits, each digit is made from 4 special characters. Character 254 is empty, character 255 is fully filled rectangle
uint8_t tron_bold_digits[10][4] = { {4,2,7,255}, { 254,5,254,5}, { 0,2,3,6}, { 0,2,1,255}, { 7,1,254,5}, { 3,6,1,255}, { 3,6,3,255}, { 0,2,254,5}, { 3,2,3,255}, { 3,2,254,2 } };

// Set eight special characters for the font: square_four
const uint8_t square_four_chars[8][8] = {
    {0b11111,0b11111,0b11100,0b11100,0b00000,0b00000,0b00000,0b00000},  // bits for special character 1/8
    {0b00000,0b00000,0b00000,0b00000,0b11100,0b11100,0b11111,0b11111},  // bits for special character 2/8
    {0b11111,0b11111,0b01111,0b01111,0b01111,0b01111,0b11111,0b11111},  // bits for special character 3/8
    {0b11111,0b11111,0b11110,0b11110,0b11110,0b11110,0b11111,0b11111},  // bits for special character 4/8
    {0b00001,0b00011,0b00111,0b01111,0b00000,0b00000,0b00000,0b00000},  // bits for special character 5/8
    {0b11110,0b11110,0b11110,0b11110,0b11110,0b11110,0b11110,0b11110},  // bits for special character 6/8
    {0b11111,0b11111,0b00000,0b00000,0b00000,0b00000,0b11111,0b11111},  // bits for special character 7/8
    {0b11110,0b11110,0b11110,0b11110,0b11110,0b11110,0b11111,0b11111}   // bits for special character 8/8
};

// Special character used for the individual digits, each digit is made from 4 special characters. Character 254 is empty, character 255 is fully filled rectangle
uint8_t square_four_digits[10][4] = { {255,255,3,2}, { 4,5,254,5}, { 6,2,3,6}, { 0,2,1,2}, { 7,1,254,5}, { 3,6,6,2}, { 3,6,3,2}, { 0,2,254,5}, { 3,2,3,2}, { 3,2,6,2 } };

// Set eight special characters for the font: blocks
const uint8_t blocks_chars[8][8] = {
    {0b11111,0b11111,0b11111,0b11111,0b00000,0b00000,0b00000,0b00000}, // bits for special character 1/8
    {0b00000,0b00000,0b00000,0b00000,0b11111,0b11111,0b11111,0b11111}, // bits for special character 2/8
    {0b11111,0b11111,0b11111,0b11111,0b11111,0b11111,0b11111,0b00000}, // bits for special character 3/8
    {0b00000,0b11111,0b11111,0b11111,0b11111,0b11111,0b11111,0b11111}, // bits for special character 4/8
    {0b00000,0b00000,0b00000,0b00000,0b00000,0b00000,0b00000,0b00000}, // bits for special character 5/8
    {0b00000,0b00000,0b00000,0b00000,0b00000,0b00000,0b00000,0b00000}, // bits for special character 6/8
    {0b00000,0b00000,0b00000,0b00000,0b00000,0b00000,0b00000,0b00000}, // bits for special character 7/8
    {0b00000,0b00000,0b00000,0b00000,0b00000,0b00000,0b00000,0b00000}, // bits for special character 8/8
};

// Special character used for the individual digits, each digit is made from 4 special characters. Character 254 is empty, character 255 is fully filled rectangle
uint8_t blocks_digits[10][4] = { {255,255,255,255}, { 254,255,254,255}, { 0,255,255,1}, { 0,255,1,255}, { 255,1,254,255}, { 255,0,1,255}, { 255,0,255,255}, { 0,255,254,255}, { 2,2,3,3}, { 255,255,1,255 } };

/*******************************************************************************
 * Function:        void LCD_SetFont(FontType_t FONT)
 * Description:     Select type font and load custom characters into CGRAM
 * Precondition:    None
 * Parameters:      FontType_t FONT, options on enum in LCD_BIGDigits.h
 * Return Values:   None
 * Remarks:         
 ******************************************************************************/
void LCD_SetFont(FontType_t FONT){
    uint8_t i;
    uint8_t buffer[8];
    const uint8_t (*char_data)[8];
    // Assigns the pointer to character data and digit data
    switch(FONT) {
        case FONT_TRON_BOLD:
            char_data = tron_bold_chars;
            current_digits = tron_bold_digits;
            break;
        case FONT_SQUARE_FOUR:
            char_data = square_four_chars;
            current_digits = square_four_digits;
            break;
        case FONT_BLOCKS:
            char_data = blocks_chars;
            current_digits = blocks_digits;
            break;
        default:
            return; 
    }
    
    // Load the 8 custom characters into the LCD
    for (i = 0; i < 8; i++) {
        memcpy(buffer, char_data[i], 8);
        LCD_createChar(i, buffer);
    }
}

/*******************************************************************************
 * Function:        void LCD_DrawBigDigit(uint8_t digit, uint8_t xpos, uint8_t ypos)
 * Description:     Draws a single digit
 * Precondition:    Before calling this function, you need call LCD_SetFont(FontType_t FONT)
 *                  selecting a valid font.
 * Parameters:      uint8_t digit = digit to be displayed
 *                  uint8_t xpos =  position in x (0 to 3)
 *                  uint8_t ypos =  position in y (0 to 19)
 * Return Values:   None
 * Remarks:         This function draws a single digit on the xpos, ypos, 
 *                  taking 2x2 cells (using 4 special characters).
 ******************************************************************************/
void LCD_DrawBigDigit(uint8_t digit, uint8_t xpos, uint8_t ypos){
    if (digit > 9 || current_digits == NULL) return;
    
    LCD_SetCursor(xpos, ypos);           
    LCD_putc(current_digits[digit][0]);
    LCD_putc(current_digits[digit][1]);
    
    LCD_SetCursor(xpos+1, ypos);           
    LCD_putc(current_digits[digit][2]);
    LCD_putc(current_digits[digit][3]);
}
