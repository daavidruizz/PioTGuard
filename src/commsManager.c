#include <stdio.h>
#include "../lib/mosquitto.h"

int main(void){
    int rc;
    struct mosquitto *mosca;
    mosquitto_lib_init();    

    mosca = mosquitto_new("mqtt1",true,NULL);

    rc = mosquitto_connect(mosca,"127.0.0.1",1883,60);

    if(rc != 0){
        printf("Client could not connect to broker!. Error: %s\n",mosquitto_strerror(rc));
        mosquitto_destroy(mosca);
        return -1;
    }
    printf("Connected to the broker!\n");

    mosquitto_publish(mosca,NULL,"/",20,"RPI mosca rafa feo",0,true);

    while(1){

    }
    mosquitto_disconnect(mosca);
    mosquitto_destroy(mosca);
    mosquitto_lib_cleanup();
    return 0;
    
}
