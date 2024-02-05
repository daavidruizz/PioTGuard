#ifndef DEFINES_H
#define DEFINES_H

//=====================
//==PERIPHERALS COMMs==
//=====================
#define INIT
#define ENABLED
#define DISABLED
#define SHUTDOWN


//=====================
//========LCD==========
//===================== 
#define I2C_PORT 0x27
#define eyeL 0x04
#define eyeR 0x05
#define bell 0x01
#define enableMap 0x02
#define disableMap 0x03


//=====================
//=====MQTT COMMS======
//=======TOPICS========
//=====================
#define PORT 1883
#define LOOPBACK "127.0.0.1"
#define N_TOPICS 10

#define PIOT "/piot"
#define CONFIG "/config"
#define SENSORS "/sensor"
#define APP "/app"
#define ALARM "/alarm"
#define DOOR_SENSOR "/door"
#define PRESENCE_SENSOR "/presence"
#define MQ2_SENSOR "/smoke"
#define INFO "/info"
#define READ "/read"

//=====================
//=====MQTT COMMS======
//========MSGS=========
//=====================
#define ENABLE "enable"
#define DISABLE "disable"



#endif