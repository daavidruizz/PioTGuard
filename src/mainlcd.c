//
// LCD1602 2 line by 16 character LCD demo
//
// Written by Larry Bank - 12/7/2017
// Copyright (c) 2017 BitBank Software, Inc.
// bitbank@pobox.com
//
// This program is free software: you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with this program.  If not, see <http://www.gnu.org/licenses/>.
//

#include <unistd.h>
#include <stdio.h>
#include <string.h>
#include <stdint.h>
//#include <lcd1602.h>
#include "../lib/lcd1602.h"

int main(int argc, char *argv[])
{

char Character[] = {
    0x04, // 0b00100
    0x0A, // 0b01010
    0x0E, // 0b01110
    0x0A, // 0b01010
    0x0A, // 0b01010
    0x0A, // 0b01010
    0x11, // 0b10001
    0x0E  // 0b01110
};

char porcentaje[] = {
    0b00100101,
};

int rc;
	rc = lcd1602Init(1, 0x27);
	if (rc)
	{
		printf("Initialization failed; aborting...\n");
		return 0;
	}
    lcd1602CreateChar(0x00,Character);
	usleep(2000);
    lcd1602WriteString("BitBank LCD1602");
	lcd1602SetCursor(0,1);
	lcd1602WriteString("actualizando...");
	lcd1602Control(1,0,0); // backlight on, underline off, blink block on 
    sleep(1);
    printf("limpiando...\n");
    lcd1602Clear();
    lcd1602WriteString("hola de nuevo...");
    lcd1602SetCursor(0,1);
	lcd1602WriteString("ktal");
    lcd1602SetCursor(6,1);
    lcd1602WriteString("\x00"); //envio posicion CGRAM
	getchar();
	lcd1602Shutdown();
return 0;
} /* main() */
