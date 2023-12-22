#ifndef LCD1602_H
#define LCD1602_H
#include<stdint.h>

class LCD1602 
{
public:
    //
    // Opens a file system handle to the I2C device
    // Turns on the LCD, sets the 4-bit mode and clears the memory
    // Returns 0 for success, 1 for failure
    //
    LCD1602(int iChannel, int iAddr);
    
    //Apaga
    ~LCD1602();

    //
    // Set the cursor position on the LCD
    //
    int setCursor(int x, int y);

    //
    // Control the backlight, cursor, and blink
    //
    int control(int bBacklight, int bCursor, int bBlink);

    //
    // Print a zero-terminated character string
    // Only 1 line at a time can be accessed
    //
    int writeString(char *szText);

    //
    // Clear the characters from the LCD
    //
    int clear(void);

    //
    // Turn off the LCD and backlight
    // close the I2C file handle
    //
    void shutdown(void);

    //
    // Crea un caracter personalizado
    // Se almacena en la localizacion de la CGRAM 1..7
    //
    void createChar(int location, char *charmap);

private:
    //
    //Envia un comando al LCD
    //
    void writeCommand(unsigned char ucCMD);

    int file_i2c;
    int iBackLight;
};
#endif