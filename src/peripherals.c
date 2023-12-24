//Connection with peripherals modules. LCD & LEDs
#include "../lib/peripherals.h"
#include "../lib/lcd1602.h"
#include "../lib/defines.h"


void peripheralsInit(void){
    int rc = lcd1602Init(1, 0x27);
    if (rc){
		printf("Initialization failed; aborting...\n");
		return 0;
	}

	usleep(2000);
    lcd1602SetCursor(0,0);
    lcd1602WriteString("\x04  PioT Guard  \x04");
	lcd1602SetCursor(0,1);
	lcd1602WriteString("Initializing...");
	lcd1602Control(1,0,0); // backlight on, underline off, blink block on 
    for(int i=0; i < 2; i++){
        sleep(1);
        lcd1602SetCursor(0,0);
        lcd1602WriteString("\x05");
        lcd1602SetCursor(15,0);
        lcd1602WriteString("\x05");
        sleep(1);
        lcd1602SetCursor(0,0);
        lcd1602WriteString("\x04");
        lcd1602SetCursor(15,0);
        lcd1602WriteString("\x04");
    }
    lcd1602Clear();
    lcd1602SetCursor(0,0);
}

void peripheralsShutdown(void){
    lcd1602Shutdown();
}

void peripheralsENABLE(void){
    lcd1602WriteString("ALARM ENABLED");
    lcd1602SetCursor(1,7);
    lcd1602WriteString()
}

void peripheralsDISABLE(){
    
}

//====================================
//=======MAIN LOOP PERIPHERALS========
//====================================
int main(void){
     
    while(1){
        switch (mode)
        {
        case INIT:
            peripheralsInit();
            break;
        case ENABLED:
            /* code */
            break;
        case DISABLED:
            /* code */
            break;
        case SHUTDOWN:
            /* code */
            break;
        default:
            break;
        }
    }

} /* main()*/