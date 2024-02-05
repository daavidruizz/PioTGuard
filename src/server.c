#include <stdlib.h>
#include <syslog.h>
#include <signal.h>
#include <stdio.h>
#include <pthread.h>
#include <mosquitto.h>
#include "defines.h"

struct mosquitto *mosca;

void handleSignal(int signum){
    printf("\nSaliendo...\n");
    syslog(LOG_ERR, "Senal recibida %d. Saliendo...", signum);
    mosquitto_destroy(mosca);
    mosquitto_lib_cleanup();
    closelog();
    exit(signum);
}

void message_callback(struct mosquitto *mosq, void *userdata, const struct mosquitto_message *message){
    if(message->payloadlen){
        syslog(LOG_INFO, "Topic %s: %s", (char *)message->topic, (char *)message->payload);
        //printf("Mensaje recibido en el topic %s: %s", (char *)message->topic, (char *)message->payload);
        
    } else {
        syslog(LOG_INFO, "VACIO Topic %s", (char *)message->topic);
        //printf("Mensaje vacío en el topic %s", (char *)message->topic);
    }
}

void* MQTTComms(void* args){
    int rc;
    mosquitto_lib_init();
    
    //Creamos el cliente que registra todo
    mosca = mosquitto_new("PioTGuard-Server", true, NULL);
    rc = mosquitto_connect(mosca, LOOPBACK, PORT, 1024);

    if(rc){
        syslog(LOG_ERR, "Error al iniciar el servidor MQTT. Código %d", rc);
        mosquitto_destroy(mosca);
        mosquitto_lib_cleanup();
        closelog();
        exit(-1);
    }

    syslog(LOG_INFO, "Broker MQTT activo en el puerto %d" ,PORT);

    for(int i = 0; i < N_TOPICS; i++){
        switch (i)
        {
        case 0:
            rc = mosquitto_subscribe(mosca, NULL, PIOT, 0);
            break;
        case 1:
            rc = mosquitto_subscribe(mosca, NULL, PIOT CONFIG, 0);
            break;
        case 2:
            rc = mosquitto_subscribe(mosca, NULL, APP INFO, 0);
            break;
        case 3:
            rc = mosquitto_subscribe(mosca, NULL, APP CONFIG ALARM, 0);
            break;
        case 4:
            rc = mosquitto_subscribe(mosca, NULL, SENSORS INFO , 0);
            break;
        case 5:
            rc = mosquitto_subscribe(mosca, NULL, DOOR_SENSOR READ, 0);
            break;
        case 6:
            rc = mosquitto_subscribe(mosca, NULL, MQ2_SENSOR READ, 0);
            break;
        case 7:
            rc = mosquitto_subscribe(mosca, NULL, PRESENCE_SENSOR READ, 0);
            break;
        default:
            break;
        }

        if(rc){
            syslog(LOG_ERR, "Error al suscribirse al topic. Código %d", rc);
            mosquitto_destroy(mosca);
            mosquitto_lib_cleanup();
            closelog();
        }
    }
    
    // Configuración del callback para manejar los mensajes recibidos
    mosquitto_message_callback_set(mosca, message_callback);

    while(1){
        mosquitto_loop(mosca,-1,1);
    }
    
    mosquitto_destroy(mosca);
    mosquitto_lib_cleanup();
    closelog();
}

int main(void){

    // Configurar el manejador de señal 
    if (signal(SIGTERM, handleSignal) == SIG_ERR) {
        perror("Exit. -15 Signal");
    }else if(signal(SIGINT, handleSignal) == SIG_ERR){
        perror("Exit. Ctrl+C");
    }
    
    openlog("PioTGuard-Server", LOG_PID | LOG_NDELAY, LOG_USER);
    syslog(LOG_INFO, "Inicializando PioTGuard-Server...");

    pthread_t threadMQTT;

    int rcMQTT = pthread_create(&threadMQTT, NULL, MQTTComms, &mosca);

    if(rcMQTT){
        syslog(LOG_ERR, "Error al inicializar el hilo de comunicaciones MQTT");
        exit(-1);
    }
    
    pthread_join(threadMQTT,NULL);

    return 0;
}