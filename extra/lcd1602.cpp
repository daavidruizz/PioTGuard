#include "lcd1602.h"
#include <unistd.h>
#include <stdio.h>
#include <string.h>
#include <stdint.h>
#include <fcntl.h>
#include <sys/ioctl.h>
#include <linux/types.h>
#include <linux/i2c-dev.h>

#define PULSE_PERIOD 500
#define CMD_PERIOD 4100
#define BACKLIGHT 8
#define DATA 1

    //========================================
    //========CUSTOM CHARACTERS===============
    //========================================
    char bell[8] = {
        0x00,
        0x04,
        0x0E,
        0x0E,
        0x0E,
        0x1F,
        0x04,
        0x00
    };

    char enabled[8] = {
        0x0E,
        0x11,
        0x11,
        0x1F,
        0x1B,
        0x1B,
        0x1F,
        0x00
    };

    char disabled[8] = {
        0x0E,
        0x10,
        0x10,
        0x1F,
        0x1B,
        0x1B,
        0x1F,
        0x00
    };

    char eyeL[8] = {
        0x0E,
        0x11,
        0x11,
        0x11,
        0x1D,
        0x1D,
        0x1D,
        0x0E
    };

    char eyeR[8] = {
        0x0E,
        0x11,
        0x11,
        0x11,
        0x17,
        0x17,
        0x17,
        0x0E
    };

//
// Open a file handle to the I2C device
// Set the controller into 4-bit mode and clear the display
// returns 0 for success, 1 for failure
//
LCD1602::LCD1602(int iChannel, int iAddr){
    char szFile[32];
    int rc;

	sprintf(szFile, "/dev/i2c-%d", iChannel);
	file_i2c = open(szFile, O_RDWR);
	if (file_i2c < 0)
	{
		fprintf(stderr, "Error opening i2c device; not running as sudo?\n");
	}
	rc = ioctl(file_i2c, I2C_SLAVE, iAddr);
	if (rc < 0)
	{
		close(file_i2c);
		fprintf(stderr, "Error setting I2C device address\n");
	}
	iBackLight = BACKLIGHT; // turn on backlight
	writeCommand(0x02); // Set 4-bit mode of the LCD controller
	writeCommand(0x28); // 2 lines, 5x8 dot matrix
	writeCommand(0x0c); // display on, cursor off
	writeCommand(0x06); // inc cursor to right when writing and don't scroll
	writeCommand(0x80); // set cursor to row 1, column 1
	clear();	    // clear the memory
	createChar(0x01, bell);
	createChar(0x02, enabled);
	createChar(0x03, disabled);
	createChar(0x04, eyeL);
	createChar(0x05, eyeR);
	clear();	    // clear the memory
}

LCD1602::~LCD1602() {
    shutdown();
}

void LCD1602::writeCommand(unsigned char ucCMD)
{
unsigned char uc;

	uc = (ucCMD & 0xf0) | iBackLight; // most significant nibble sent first
	write(file_i2c, &uc, 1);
	usleep(PULSE_PERIOD); // manually pulse the clock line
	uc |= 4; // enable pulse
	write(file_i2c, &uc, 1);
	usleep(PULSE_PERIOD);
	uc &= ~4; // toggle pulse
	write(file_i2c, &uc, 1);
	usleep(CMD_PERIOD);
	uc = iBackLight | (ucCMD << 4); // least significant nibble
	write(file_i2c, &uc, 1);
	usleep(PULSE_PERIOD);
        uc |= 4; // enable pulse
        write(file_i2c, &uc, 1);
        usleep(PULSE_PERIOD);
        uc &= ~4; // toggle pulse
        write(file_i2c, &uc, 1);
	usleep(CMD_PERIOD);

} /* writeCommand() */

/* lcd1602CreateChar()*/
void LCD1602::createChar(int location, char *charmap){
	if (file_i2c < 0 || charmap == NULL)
		return;

    if (location >= 0 && location < 8) { 
        writeCommand(0x40 | (location << 3)); // Address CGRAM
        for (int i = 0; i < 8; i++) {
            // Write each byte in two parts (4 bits each)
            unsigned char ucCMD = iBackLight | DATA | (charmap[i] & 0xF0);
            ucCMD |= 4; // Pulse E
            write(file_i2c, &ucCMD, 1);
            usleep(PULSE_PERIOD);
            ucCMD &= ~4; // Toggle E
            write(file_i2c, &ucCMD, 1);
            usleep(PULSE_PERIOD);

            ucCMD = iBackLight | DATA | ((charmap[i] << 4) & 0xF0);
            ucCMD |= 4; // Pulse E
            write(file_i2c, &ucCMD, 1);
            usleep(PULSE_PERIOD);
            ucCMD &= ~4; // Toggle E
            write(file_i2c, &ucCMD, 1);
            usleep(PULSE_PERIOD);
        }
    }
}/* lcd1602CreateChar()*/

//
// Control the backlight, cursor, and blink
// The cursor is an underline and is separate and distinct
// from the blinking block option
//
int LCD1602::control(int bBacklight, int bCursor, int bBlink)
{
unsigned char ucCMD = 0xc; // display control

	if (file_i2c < 0)
		return 1;
	iBackLight = (bBacklight) ? BACKLIGHT : 0;
	if (bCursor)
		ucCMD |= 2;
	if (bBlink)
		ucCMD |= 1;
	writeCommand(ucCMD);
 	
	return 0;
} /* lcd1602Control() */

//
// Write an ASCII string (up to 16 characters at a time)
// 
int LCD1602::writeString(char *text)
{
unsigned char ucTemp[2];
int i = 0;

	if (file_i2c < 0 || text == NULL)
		return 1;

	while (i<16 && *text)
	{
		ucTemp[0] = iBackLight | DATA | (*text & 0xf0);
		write(file_i2c, ucTemp, 1);
		usleep(PULSE_PERIOD);
		ucTemp[0] |= 4; // pulse E
		write(file_i2c, ucTemp, 1);
		usleep(PULSE_PERIOD);
		ucTemp[0] &= ~4;
		write(file_i2c, ucTemp, 1);
		usleep(PULSE_PERIOD);
		ucTemp[0] = iBackLight | DATA | (*text << 4);
		write(file_i2c, ucTemp, 1);
		ucTemp[0] |= 4; // pulse E
		write(file_i2c, ucTemp, 1);
		usleep(PULSE_PERIOD);
		ucTemp[0] &= ~4;
		write(file_i2c, ucTemp, 1);
		usleep(CMD_PERIOD);
		text++;
		i++;
	}
	return 0;
} /* WriteString() */

//
// Erase the display memory and reset the cursor to 0,0
//
int LCD1602::clear(void)
{
	if (file_i2c < 0)
		return 1;
	writeCommand(0x01); // clear the screen
	usleep(2000); // Espera 2 ms (ajusta según sea necesario)
	return 0;

} /* lcd1602Clear() */




//
// Set the LCD cursor position
//
int LCD1602::setCursor(int x, int y)
{
unsigned char cCmd;

	if (file_i2c < 0 || x < 0 || x > 15 || y < 0 || y > 1)
		return 1;

	cCmd = (y==0) ? 0x80 : 0xc0;
	cCmd |= x;
	writeCommand(cCmd);
	return 0;

} /* lcd1602SetCursor() */

void LCD1602::shutdown(void)
{
	iBackLight = 0; // turn off backlight
	writeCommand(0x08); // turn off display, cursor and blink	
	close(file_i2c);
	file_i2c = -1;
} /* lcd1602Shutdown() */