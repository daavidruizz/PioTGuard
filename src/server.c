#include <stdio.h>
#include <mosquitto.h>
#include "defines.h"
#include <syslog.h>

void message_callback(struct mosquitto *mosq, void *userdata, const struct mosquitto_message *message){
    if(message->payloadlen){
        syslog(LOG_INFO, "Mensaje recibido en el topic %s: %s", (char *)message->topic, (char *)message->payload);
    } else {
        syslog(LOG_INFO, "Mensaje vacío en el topic %s", (char *)message->topic);
    }
}

int main(void){

    openlog("PioTGuard-Server", LOG_PID | LOG_NDELAY, LOG_USER);
    syslog(LOG_INFO, "Inicializando PioTGuard-Server...")
    
    int rc;
    struct mosquitto *mosca;
    
    mosquitto_lib_init();

    mosca = mosquitto_new("PioTGuard-Server", true, NULL);
    rc = mosquitto_connect(mosca, LOOPBACK, PORT, 1024);

    if(rc){
        syslog(LOG_ERR, "Error al iniciar el servidor MQTT. Código %d", rc);
        mosquitto_destroy(mosca);
        mosquitto_lib_cleanup();
        closelog();
        return -1;
    }

    syslog(LOG_INFO, "Broker MQTT activo en el puerto " PORT);

    rc = mosquitto_subscribe(mosca, NULL, PIOT,0);

    if(rc){
        syslog(LOG_ERR, "Error al suscribirse al topic. Código %d", rc);
        mosquitto_destroy(mosca);
        mosquitto_lib_cleanup();
        closelog();
    }
    
    // Configuración del callback para manejar los mensajes recibidos
    mosquitto_message_callback_set(mosca, message_callback);

}