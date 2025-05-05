#include <MDR32FxQI_port.h>
#include <MDR32FxQI_rst_clk.h>

void Delay(int waitTicks);

int main() {
  // Заводим структуру конфигурации вывода(-ов) порта GPIO
  PORT_InitTypeDef GPIOInitStruct;
  // Включаем тактирование порта C
  RST_CLK_PCLKcmd(RST_CLK_PCLK_PORTC, ENABLE);
  // Инициализируем структуру конфигурации вывода(-ов) порта значениями по
  // умолчанию
  PORT_StructInit(&GPIOInitStruct);
  // Изменяем значения по умолчанию на необходимые нам настройки
  GPIOInitStruct.PORT_Pin = PORT_Pin_2;
  GPIOInitStruct.PORT_OE = PORT_OE_OUT;
  GPIOInitStruct.PORT_SPEED = PORT_SPEED_SLOW;
  GPIOInitStruct.PORT_MODE = PORT_MODE_DIGITAL;
  // Применяем заполненную нами структуру для PORTC.
  PORT_Init(MDR_PORTC, &GPIOInitStruct);
  //
  while (1) {
    if (PORT_ReadInputDataBit (MDR_PORTC, PORT_Pin_2) == 0){
      PORT_SetBits(MDR_PORTC, PORT_Pin_2); // светоидиот на нашей плате
    }else{
      PORT_ResetBits(MDR_PORTC, PORT_Pin_2);
    }
    // Задержка
    Delay(100000);
  }
}

// Простейшая функция задержки, позднее мы заменим ее на реализацию через таймер
void Delay(int waitTicks) {
  int i;
  for (i = 0; i < waitTicks; i++) {
    __NOP();
  }
}