#include "app.h"

void vApplicationStackOverflowHook(TaskHandle_t pxTask, char *pcTaskName)
{
  while (1)
  {
    __NOP();
  }
}
void vApplicationIdleHook(void)
{
  while (1)
  {
    __NOP();
  }
}

void exampleTask(void *pvParameters)
{
  (void) pvParameters; // убираем warning
  for (;;)
  { 
    __NOP();       
    vTaskDelay(1);
  }
}

int main(void)
{
  CLK_Init_80_mhz();
  
  xTaskCreate(exampleTask, "exampleTask", configMINIMAL_STACK_SIZE, NULL, tskIDLE_PRIORITY + 1, NULL);

  vTaskStartScheduler();
  while (1)
  {
    __NOP();
  }
  return 0;
}
