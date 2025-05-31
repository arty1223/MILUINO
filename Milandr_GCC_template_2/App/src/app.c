#include "app.h"

#define __ASM __asm
#define __NOP() __ASM volatile("nop")
int main(void)
{
  while (1)
  {
    __NOP();
  }
}
