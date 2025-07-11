#include "app.h"

#define __ASM __asm
#define __NOP() __ASM volatile("nop")
int main(void)
{
  CLK_Init_80_mhz();
  initSystick();
  
  while (1)
  {
    __NOP();
  }
}
