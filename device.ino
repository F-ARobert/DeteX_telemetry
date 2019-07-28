#include "AZ3166WiFi.h"
#include "DevKitMQTTClient.h"
#include "AzureIotHub.h"
#include "SystemTickCounter.h"
#include "wiring.h"
#include "Serial.h"
#include <time.h>


#include "timing_mngmt.h"
#include "EventBase.pb.h"
#include "LightControl.pb.h"
#include "pb_encode.h"
#include "pb_decode.h"


/* DEFINES */
#define LIDAR_ON 0

#if LIDAR_ON
/* LIDAR SET UP */

#include "lidar.h"
// You need to create an driver instance
RPLidar lidar;


/* END LIDAR SET UP */
#endif

static bool hasWifi = false;
static bool hasIoTHub = false;


/* Global variable ****************************/
telemetry_table_t tele_tab;
telemetry_data_t t_data;

/* Initialize tickers */
Ticker lidar_send;
Ticker sensors_read;

Timer lidar_timer;

int lidar_time;

/* Global functions */
void lidar_time_read(void);
void read_all_sensors(void);

/* Assign function pointers */
void (*read_sensors_ptr)(void) = &read_all_sensors;
void (*lidar_time_read_ptr)(void) = &lidar_time_read;

#if LIDAR_ON
 
#endif


void setup() {

#if LIDAR_ON
  lidar.begin(Serial);

  pinMode(RPLIDAR_MOTOR, OUTPUT);
#endif


  /* Sensor intialization */
  init_onboard_sensors();
  tele_tab = telemetry_init();
  
  if (WiFi.begin() == WL_CONNECTED)
  {
    hasWifi = true;
    Screen.print(1, "Running...");

    if (!DevKitMQTTClient_Init())
    {
      hasIoTHub = false;
      return;
    }
    hasIoTHub = true;
  }
  else
  {
    hasWifi = false;
    Screen.print(1, "No Wi-Fi");
  }

  /* Set up iunterupts and timers */
  lidar_timer.start();
  lidar_send.attach(lidar_time_read_ptr,0.1);
  sensors_read.attach(read_sensors_ptr,5.0);
}

void loop() {
  // put your main code here, to run repeatedly:
  


  #if LIDAR_ON
    run_lidar(lidar);
  #endif


  if (tele_tab.count == 12){
    calc_average(&t_data, &tele_tab);
    /* For testing purposes */
    Serial.printf("---------- New Output ----------------\n");
    Serial.printf("Lidar timer read : %d\n", lidar_time);

    Serial.printf("%.2f Celsius\n", t_data.temperature);
    Serial.printf("%.2f %% humidity\n",t_data.humidity);
    Serial.printf("%.2f Pa\n", t_data.pressure);
    Serial.printf("Magnetic field: x %d, y %d, z %d\n", t_data.mag_field.x, t_data.mag_field.y,t_data.mag_field.z);

    /* End testing */
    if (hasIoTHub && hasWifi)
      {
        uint8_t buff[1280];
        size_t msg_length;
        bool status;

        EventBase msg_telemetry = EventBase_init_zero;
      
        pb_callback_t id;
        pb_callback_t device_id;
        pb_callback_t payload;

        char* id_string = "5d2b572f3dd05300015cad67";
        char* device_id_string = "ele400-equipe4";
        char* payload_string = "5";

        id.arg = id_string;
        device_id.arg = device_id_string;
        payload.arg = payload_string;


        msg_telemetry.id = id;
        msg_telemetry.deviceId = device_id;
        msg_telemetry.deviceTime = millis();
        msg_telemetry.version = 1;
        msg_telemetry.commandId;
        msg_telemetry.payload = payload;

        pb_ostream_t stream = pb_ostream_from_buffer(buff,sizeof(buff));

        status = pb_encode(&stream, EventBase_fields, &msg_telemetry);
        msg_length = stream.bytes_written;

        // replace the following line with your data sent to Azure IoTHub
        snprintf((char*)buff, msg_length, "{\"topic\":\"iot\"}");
        
        if (DevKitMQTTClient_SendEvent((char*)buff))
        {
          Screen.print(1, "Sending...");
        }
        else
        {
          Screen.print(1, "Failure...");
        }
        // delay(2000);
      }


    tele_tab = telemetry_init();
  }

}

/********* TO PUT UN SEPERATE HEADER FILE ********************/
void lidar_time_read(void){
  lidar_time = lidar_timer.read_ms();
  lidar_timer.reset();
}

void read_all_sensors(void){
  Serial.printf("Lidar timer read : %d\n", lidar_time);
  Screen.print(1,"Hello!");
  read_sensors(&tele_tab);
}