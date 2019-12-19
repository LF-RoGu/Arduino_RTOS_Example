/*
  Author: Luis Fernando Rodriguez
  Date: 02/11/19
  Team: Rangers GDL
*/
#include <Arduino_FreeRTOS.h>
//#include <task.h>
#include <semphr.h>

#define RTOS_TASK_PRIORITY 5

SemaphoreHandle_t g_mutex;

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  // set the digital pin as output:
  pinMode(LED_BUILTIN, OUTPUT);

  /* Setup Semaphr*/
  if(g_mutex == NULL)
  {
    g_mutex = xSemaphoreCreateMutex();
  }
/*--------------------------------------------------*/
/*---------------------- Tasks ---------------------*/
/*--------------------------------------------------*/
  /* Create task*/
  xTaskCreate(led_blink_seq_one,"led_blink_seq_one",128,NULL,RTOS_TASK_PRIORITY - 1,NULL);
  xTaskCreate(led_blink_seq_two,"led_blink_seq_two",128,NULL,RTOS_TASK_PRIORITY - 2,NULL);
  /* Start scheduler*/
  vTaskStartScheduler();
}

void loop() {
  // put your main code here, to run repeatedly:

}

void led_blink_seq_one(void* pvParameters)
{
  for(;;)
  {
    /*
     * See if we can obtain or "Take" the Serial Semaphore.
     * If the semaphore is not available, wait 5 ticks of the Scheduler to see if it becomes free.
     */
    if( xSemaphoreTake( g_mutex, ( TickType_t ) 5 ) == pdTRUE )
    {
      Serial.println("led_blink_seq_one");
      digitalWrite(LED_BUILTIN, LOW);    // turn the LED off by making the voltage LOW
      /* Task Delay*/
      vTaskDelay(1000/portTICK_PERIOD_MS);
      /* Now free or "Give" the Serial Port for others.*/
      xSemaphoreGive( g_mutex );
    }
    vTaskDelay(1);
  }
}

void led_blink_seq_two(void* pvParameters)
{
  for(;;)
  {
    /*
     * See if we can obtain or "Take" the Serial Semaphore.
     * If the semaphore is not available, wait 5 ticks of the Scheduler to see if it becomes free.
     */
    if( xSemaphoreTake( g_mutex, ( TickType_t ) 5 ) == pdTRUE )
    {
      Serial.println("led_blink_seq_two");
      digitalWrite(LED_BUILTIN, LOW);    // turn the LED off by making the voltage LOW
      /* Task Delay*/
      vTaskDelay(2000/portTICK_PERIOD_MS);
      /* Now free or "Give" the Serial Port for others.*/
      xSemaphoreGive( g_mutex );
    }
    vTaskDelay(1);
  }
}
