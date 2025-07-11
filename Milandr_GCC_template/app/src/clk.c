#include "clk.h"


void CLK_Init_80_mhz(void)
{
    RST_CLK_DeInit();

    /* Enable HSE (High Speed External) clock */
    RST_CLK_HSEconfig(RST_CLK_HSE_ON);
    while(RST_CLK_HSEstatus() == ERROR) {}

    /* Configures the CPU_PLL clock source (CPU_CLK = HSE_CLK * 10 = 8 MHz * 10 = 80 MHz) */
    RST_CLK_CPU_PLLconfig(RST_CLK_CPU_PLLsrcHSEdiv1, RST_CLK_CPU_PLLmul10);
    /* Enables the CPU_PLL */
    RST_CLK_CPU_PLLcmd(ENABLE);
    while(RST_CLK_CPU_PLLstatus() == ERROR) {}

    /* Select the CPU_PLL output as input for CPU_C2_SEL */
    RST_CLK_CPU_PLLuse(ENABLE);
    /* Set CPUClk Prescaler (CPU_C3_SEL = CPU_C2) */
    RST_CLK_CPUclkPrescaler(RST_CLK_CPUclkDIV1);

    /* Enables the RST_CLK_PCLK_EEPROM */
    RST_CLK_PCLKcmd(RST_CLK_PCLK_EEPROM, ENABLE);
    /* Sets the code latency value (CPU_CLK up to 100 MHz) */
    EEPROM_SetLatency(EEPROM_Latency_3);
    /* Disables the RST_CLK_PCLK_EEPROM */
    RST_CLK_PCLKcmd(RST_CLK_PCLK_EEPROM, DISABLE);
    
    /* Enables the RST_CLK_PCLK_BKP */
    RST_CLK_PCLKcmd(RST_CLK_PCLK_BKP, ENABLE);
    /* Setting the parameters of the voltage regulator SelectRI and LOW in the BKP controller (CPU_CLK = 80 MHz) */
    BKP_DUccMode(BKP_DUcc_upto_80MHz);

    /* Select the CPU clock source */
    RST_CLK_CPUclkSelection(RST_CLK_CPUclkCPU_C3);

    SystemCoreClockUpdate();
    // MDR_RST_CLK->HS_CONTROL |= 0b01; //включить HSE
    // while(MDR_RST_CLK->CLOCK_STATUS & RST_CLK_CLOCK_STATUS_HSE_RDY != 1); //ждем запуска HSE

    // MDR_RST_CLK->CPU_CLOCK |= 0b10; // CPU_C1 - HSE    

    // MDR_RST_CLK->PLL_CONTROL |= 4 << RST_CLK_PLL_CONTROL_PLL_CPU_MUL_Pos | 1 << RST_CLK_PLL_CONTROL_PLL_CPU_ON_Pos; //включить PLL, множитель 4+1  
    // // MDR_RST_CLK->PLL_CONTROL |= 9 << RST_CLK_PLL_CONTROL_PLL_CPU_MUL_Pos | 1 << RST_CLK_PLL_CONTROL_PLL_CPU_ON_Pos; //включить PLL, множитель 9+1  
    // while(MDR_RST_CLK->CLOCK_STATUS & RST_CLK_CLOCK_STATUS_PLL_CPU_RDY_Pos != 1); //ждем запуска PLL_CPU
    // MDR_RST_CLK->CPU_CLOCK |= 0b100; // CPU_C2 - PLL_CPU

    // MDR_EEPROM->CMD &= ~EEPROM_CMD_DELAY_Msk | 0x18; // выставляем задержку EEPROM в 3 цикла
    // #define LDO_TRIM  *(uint8_t *) 0x08000FA0 + *(uint8_t *) 0x08000FA1 + *(uint8_t *) 0x08000FA2
    // MDR_BKP->REG_0E |= LDO_TRIM << BKP_REG_0E_TRIM_Pos;
    // MDR_BKP->REG_0E |= 0b111111; // Выбор режима работы встроенного регулятора напряжения частота более 80 МГц; + нагрузка 100 ом
    

    // MDR_RST_CLK->CPU_CLOCK |= 0b01 << RST_CLK_CPU_CLOCK_HCLK_SEL_Pos; // CPU_C3 - HCLK
}