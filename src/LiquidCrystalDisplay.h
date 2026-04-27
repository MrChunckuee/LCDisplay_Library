/* 
 * File:   LiquidCrystalDisplay.h
 * Author: MrChunckuee
 *
 * LCD library header file.
 */

#ifndef LIQUIDCRYSTALDISPLAY_H
#define	LIQUIDCRYSTALDISPLAY_H

#ifdef	__cplusplus
extern "C" {
#endif
    
    #include <xc.h>
    #include <stdbool.h>

    // Define struct LCD for 4bits
    typedef struct {
        volatile uint8_t* TRIS;  // (e.g., &TRISC)
        volatile uint8_t* PORT;  // (e.g., &PORTC)
        uint8_t RS;              // Pin RS
        uint8_t EN;              // Pin EN
        uint8_t D4;              // Pin data 4 - D4
        uint8_t D5;              // Pin data 5 - D5
        uint8_t D6;              // Pin data 6 - D6
        uint8_t D7;              // Pin data 7 - D7
    } LCD_t;

    extern LCD_t LCDisplay;

    // Macros that correspond to LCD commands
    #define LCD_Clear()                 LCD_Command (0x01); 
    #define LCD_DecrementCursor()       LCD_Command (0x04); 
    #define LCD_IncrementCursor()       LCD_Command (0x05); 
    #define LCD_ShiftDisplayRight()     LCD_Command (0x06); 
    #define LCD_ShiftDisplayLeft()      LCD_Command (0x07);
    #define LCD_ShiftRight()            LCD_Command (0x1C); 
    #define LCD_ShiftLeft()             LCD_Command (0x18); 

    // Sets the LCD cursor position
    #define LCD_SetCursor(x, y)              \
    do {                                     \
        if ( x == 0 ) {                      \
            LCD_Command(0x80 + y);           \
        }                                    \
        else if ( x == 1 ) {                 \
            LCD_Command(0xC0 + y);           \
        }                                    \
        else if ( x == 2 ) {                 \
            LCD_Command(0x94 + y);           \
        }                                    \
        else if ( x == 3 ) {                 \
            LCD_Command(0xD4 + y);           \
        }                                    \
    } while(false)

    // [Internal use only] Sets the display on/off, the cursor on/off and if it will blink or not
    #define LCD_Display(on, cursor, blink)   \
    do {                                     \
        unsigned char cmd = 0x08;            \
                                             \
        if ( on == true ) {                  \
            cmd |= 1 << 2;                   \
        }                                    \
                                             \
        if ( cursor == true ) {              \
            cmd |= 1 << 1;                   \
        }                                    \
                                             \
        if ( blink == true ) {               \
            cmd |= 1;                        \
        }                                    \
                                             \
        LCD_Command(cmd);                    \
    } while (false)

    // Initializes the LCD (See LCD struct)
    bool LCD_Init(LCD_t display);

    // Prints a character on the LCD
    void LCD_putc(char a);

    // Prints a string on the LCD that is allocated on the RAM
    void LCD_puts(char *a);

    // Prints a string on the LCD that is allocated on the program memory
    void LCD_putrs(const char *a);

    // Set character on CGRAM
    void LCD_createChar(uint8_t location, const uint8_t charmap[]);

    // [Internal use only] Write a command to the LCD
    void LCD_Command(uint8_t cmd);

    // [Internal use only] Write a byte to the LCD
    void LCD_Write(uint8_t data);

    // [Internal use only] Outputs the data to the LCD Port
    void LCD_Out(char a);
    

#ifdef	__cplusplus
}
#endif

#endif	/* LIQUIDCRYSTALDISPLAY_H */

