#include <stdio.h>
#include <mosquitto.h>

void on_message(struct mosquitto *mosca, void *userdata, const struct mosquitto_message *message){
    // Manejar el mensaje recibido aquí
    printf("Mensaje recibido en el topic %s: %s\n", message->topic, (char *)message->payload);
}

int main(void){
    int rc;
    struct mosquitto *mosca;

    mosquitto_lib_init();

    mosca = mosquitto_new("mqtt_server", true, NULL);

    // Configurar el callback para manejar los mensajes recibidos
    mosquitto_message_callback_set(mosca, on_message);

    // Iniciar el servidor MQTT en el puerto 1883
    rc = mosquitto_connect(mosca, "127.0.0.1", 1883, 1024);

    if(rc){
        printf("Error al iniciar el servidor MQTT. Código de retorno: %d\n", rc);
        return -1;
    }

    printf("Servidor MQTT iniciado en el puerto 1883...\n");

    // Entrar en el bucle de red para manejar eventos
    mosquitto_loop_forever(mosca, -1, 1);

    // Cleanup
    mosquitto_destroy(mosca);
    mosquitto_lib_cleanup();

    return 0;
}
