// Main program for the PioTGuard-Server
#include <unistd.h>
#include <stdio.h>
#include <string.h>
#include <stdint.h>
#include <signal.h>
#include <stdlib.h>
#include <pthread.h>
#include "../lib/lcd1602.h"
#define I2C_PORT 0x27

static FILE *fp;

// Estructura para pasar datos al hilo de control
struct ControlThreadData {
    // Puedes agregar datos adicionales según sea necesario
};

void* lcdLoop(void* data);

void handleSignal(int signum){
    printf("\nSe recibió la señal Ctrl+C (SIGINT). Saliendo...\n");
    lcd1602Shutdown();
    // Cerrar el archivo
    fclose(fp);
    exit(signum);
}

//====================================
//==========CDONTROL LCD==============
//====================================
void* controlLCD(void* data){

    // Configurar el manejador de señal para Ctrl+C
    if (signal(SIGINT, handleSignal) == SIG_ERR) {
        perror("Error al configurar el manejador de señal");
    }
    

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
    int temp = -1;

        char buf[32];

    // Abrir el archivo que contiene la temperatura
    fp = fopen("/sys/class/thermal/thermal_zone0/temp", "r");
    if (fp == NULL) {
        perror("Error abriendo el archivo de temperatura");
    }

    while(1){
        sleep(1);
        rewind(fp);
        // Leer la temperatura desde el archivo
        if (fgets(buf, sizeof(buf), fp) == NULL) {
            perror("Error leyendo la temperatura");
            break;
        }
        temp = atoi(buf);
        sprintf(buf, "T -> %.2f", temp / 1000.0);
        lcd1602SetCursor(3,0);
        lcd1602WriteString(buf);
        fflush(fp);
    }
    return NULL;
} 

int main(int argc, char *argv[]){
    
    // Manejo de señales kill -15
    signal(SIGTERM, handleSignal);

    // Inicializar datos para el hilo de control
    struct ControlThreadData controlData;
    // Puedes configurar la estructura con los datos necesarios para el hilo de control

    // Crear el hilo de control
    pthread_t controlThreadId;
    if (pthread_create(&controlThreadId, NULL, controlLCD, (void*)&controlData) != 0) {
        perror("Error al crear el hilo de control");
        return -1;
    }

    int i = 0;
    while(1){
        printf("Contando: %d\n",i);
        sleep(1);
    }

    return 0;
}/* main() */
