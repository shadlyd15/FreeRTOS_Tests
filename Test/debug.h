#ifndef _DEBUG_H_
#define _DEBUG_H_

#define DEBUG
#define DEBUG_BAUDRATE  (19200)
#define DEBUG_TICK      (5)

    #if defined(DEBUG)
        #include "Arduino.h"
        #include <semphr.h>  // add the FreeRTOS functions for Semaphores (or Flags).
        
        SemaphoreHandle_t xDebugSemaphore;
        #define DEBUG_INIT(x)   Serial.begin(x); \
                                if ( xDebugSemaphore == NULL ){   \
                                    xDebugSemaphore = xSemaphoreCreateMutex();  \
                                    if ( ( xDebugSemaphore ) != NULL ) \
                                    xSemaphoreGive( ( xDebugSemaphore ) );  \
                                }   \

        #define DEBUG(x)    if ( xSemaphoreTake( xDebugSemaphore, ( TickType_t ) DEBUG_TICK ) == pdTRUE ){  \
                                Serial.print(x);  \
                                xSemaphoreGive( xDebugSemaphore ); \
                            }   \
                            
        #define PLACE(x)    x
        
    #else
        #define DEBUG_INIT(x)
        #define DEBUG(x)
        #define PLACE(x)
    #endif // DEBUG

#endif  // __DEBUG__