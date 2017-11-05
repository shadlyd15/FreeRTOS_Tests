#include <Arduino_FreeRTOS.h>
#include <semphr.h>  // add the FreeRTOS functions for Semaphores (or Flags).
#include "debug.h"
#include "RTClib.h"

// define two Tasks for DigitalRead & AnalogRead
void TaskDS3231( void *pvParameters );
void TaskLCD( void *pvParameters );
void TaskButton( void *pvParameters );
void TaskRelay( void *pvParameters );

int freeRam(){
    extern int __heap_start, *__brkval; 
        int v; 
        return (int) &v - (__brkval == 0 ? (int) &__heap_start : (int) __brkval);
        
}

void TaskLCD( void *pvParameters ){
    while(1){    
        int free_memory;
        DEBUG(F("[OK] Free Memory : "));
        DEBUG(freeRam());
        DEBUG(F("\n[OK] Device Working !!!\n"));
        vTaskDelay(50);  // one tick delay (15ms) in between reads for stability
    }
}

void TaskDS3231( void *pvParameters ){

    static RTC_DS3231 rtc;
    static DateTime_t DataTime;

    DEBUG(F("[OK] DS3231 Task Started\n"));
    
    if (!rtc.begin()) {
        DEBUG(F("[ERROR] I2C Device Not Found\n"));
    }
    else{
        DEBUG(F("[OK] I2C Device Found\n"));
        if(rtc.lostPower())
            DEBUG(F("[ERROR] Power Was Lost !!! Set Time Again !!!\n"));
    }

    while(1){
      
        if(rtc.lostPower()){
            DEBUG(F("[ERROR] Power Was Lost !!! Set Time Again !!!\n"));
        }
        else{
          if(rtc.now(&DataTime)){
              DEBUG(F("Time : "));
              DEBUG(DataTime.hour);
              DEBUG(F(":"));
              DEBUG(DataTime.minute);
              DEBUG(F(":"));
              DEBUG(DataTime.second);
              DEBUG(F("\n"));
          }
          else{
              DEBUG(F("[ERROR] I2C Device Not Responding !!!\n"));
              if(rtc.restart()){
                DEBUG(F("[OK] I2C Device Restarted\n"));
              }
              else{
                DEBUG(F("[ERROR] I2C Device Failed To Restart\n"));
              }
          }
  
//          DEBUG(F("[OK] RTC Task Running !!!\n"));        
        }
        
        vTaskDelay(50);  // one tick delay (15ms) in between reads for stability
    }
}
// the setup function runs once when you press reset or power the board
void setup() {

  DEBUG_INIT(DEBUG_BAUDRATE);
  DEBUG(F("[OK] FreeRTOS Load Scheduler Started\n"));

//   delay(1000);

  // Now set up two Tasks to run independently.
  xTaskCreate(
    TaskDS3231
    ,  (const portCHAR *)"RTC DS3231"  // A name just for humans
    ,  128  // This stack size can be checked & adjusted by reading the Stack Highwater
    ,  NULL
    ,  2  // Priority, with 3 (configMAX_PRIORITIES - 1) being the highest, and 0 being the lowest.
    ,  NULL );

    xTaskCreate(
    TaskLCD
    ,  (const portCHAR *)"LCD Task"  // A name just for humans
    ,  128  // This stack size can be checked & adjusted by reading the Stack Highwater
    ,  NULL
    ,  2  // Priority, with 3 (configMAX_PRIORITIES - 1) being the highest, and 0 being the lowest.
    ,  NULL );
  // Now the Task scheduler, which takes over control of scheduling individual Tasks, is automatically started.
}

void loop()
{
  // Empty. Things are done in Tasks.
}

/*--------------------------------------------------*/
/*---------------------- Tasks ---------------------*/
/*--------------------------------------------------*/
