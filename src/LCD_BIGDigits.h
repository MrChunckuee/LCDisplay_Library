/* 
 * File:   LCD_BIGDigits.h
 * Author: MrChunckuee Electronics
 *
 * Created on 20 de septiembre de 2025, 06:28 PM
 */

#ifndef LCD_BIGDIGITS_H
#define	LCD_BIGDIGITS_H

#ifdef	__cplusplus
extern "C" {
#endif

    // Define 'enum' for fonts
    typedef enum {
        FONT_TRON_BOLD      = 0,
        FONT_SQUARE_FOUR,
        FONT_BLOCKS
    }FontType_t;
    
    void LCD_SetFont(FontType_t FONT);
    void LCD_DrawBigDigit(uint8_t digit, uint8_t xpos, uint8_t ypos);


#ifdef	__cplusplus
}
#endif

#endif	/* LCD_BIGDIGITS_H */

