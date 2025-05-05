/**
 *******************************************************************************
 * @file    system_MDR32VF0xI_it.h
 * @author  Milandr Application Team
 * @version V0.1.0
 * @date    28/03/2024
 * @brief   This file contains all the functions prototypes for the IT
 *          firmware library.
 *******************************************************************************
 * <br><br>
 *
 * THE PRESENT FIRMWARE IS FOR GUIDANCE ONLY. IT AIMS AT PROVIDING CUSTOMERS
 * WITH CODING INFORMATION REGARDING MILANDR'S PRODUCTS IN ORDER TO FACILITATE
 * THE USE AND SAVE TIME. MILANDR SHALL NOT BE HELD LIABLE FOR ANY
 * DIRECT, INDIRECT OR CONSEQUENTIAL DAMAGES RESULTING
 * FROM THE CONTENT OF SUCH FIRMWARE AND/OR A USE MADE BY CUSTOMERS OF THE
 * CODING INFORMATION CONTAINED HEREIN IN THEIR PRODUCTS.
 *
 * <h2><center>&copy; COPYRIGHT 2024 Milandr</center></h2>
 *******************************************************************************
 */

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef SYSTEM_MDR32VF0xI_IT_H
#define SYSTEM_MDR32VF0xI_IT_H

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "system_MDR32VF0xI_config.h"

/** @addtogroup DEVICE_SUPPORT Device Support
 * @{
 */

/** @addtogroup MDR32VF0xI_DEVICE MDR32VF0xI
 *  @{
 */

/** @addtogroup MDR32VF0xI_System_IT MDR32VF0xI System IT
 * @{
 */

/** @addtogroup MDR32VF0xI_System_IT_Exported_Types MDR32VF0xI System IT Exported Types
 * @{
 */

#if defined(USE_MDR1206FI) || defined(USE_MDR1206AFI) || defined(USE_MDR1206)
/**
 *  @brief MDR1206 interrupt privilege modes.
 */
typedef enum {
    IT_PRIVILEGE_MODE_IRQ_M = CORE_PRIVILEGE_MODE_MACHINE,
    IT_PRIVILEGE_MODE_IRQ_U = CORE_PRIVILEGE_MODE_USER
} IT_PrivilegeModeIRQ_TypeDef;

#define IS_IT_PRIVILEGE_MODE_IRQ(MODE) (((MODE) == IT_PRIVILEGE_MODE_IRQ_M) || \
                                        ((MODE) == IT_PRIVILEGE_MODE_IRQ_U))
#endif

/** @} */ /* End of the group MDR32VF0xI_System_IT_Exported_Types */

/** @addtogroup MDR32VF0xI_System_IT_Exported_Defines MDR32VF0xI System IT Exported Defines
 * @{
 */

/**
 *  @brief IT CSR MSTATUS.
 */
#if defined(USE_MDR1206FI) || defined(USE_MDR1206AFI) || defined(USE_MDR1206)
#define CSR_MSTATUS_UIE_Pos 0
#define CSR_MSTATUS_UIE_Msk (0x1 << CSR_MSTATUS_UIE_Pos)
#define CSR_MSTATUS_UIE     CSR_MSTATUS_UIE_Msk
#endif

#define CSR_MSTATUS_MIE_Pos 3
#define CSR_MSTATUS_MIE_Msk (0x1 << CSR_MSTATUS_MIE_Pos)
#define CSR_MSTATUS_MIE     CSR_MSTATUS_MIE_Msk

#if defined(USE_MDR1206FI) || defined(USE_MDR1206AFI) || defined(USE_MDR1206)
#define CSR_MSTATUS_UPIE_Pos 4
#define CSR_MSTATUS_UPIE_Msk (0x1 << CSR_MSTATUS_UPIE_Pos)
#define CSR_MSTATUS_UPIE     CSR_MSTATUS_UPIE_Msk
#endif

#define CSR_MSTATUS_MPIE_Pos 7
#define CSR_MSTATUS_MPIE_Msk (0x1 << CSR_MSTATUS_MPIE_Pos)
#define CSR_MSTATUS_MPIE     CSR_MSTATUS_MPIE_Msk

#define CSR_MSTATUS_MPP_Pos  11
#define CSR_MSTATUS_MPP_Msk  (0x3 << CSR_MSTATUS_MPP_Pos)

#if defined(USE_MDR1206FI) || defined(USE_MDR1206AFI) || defined(USE_MDR1206)
#define CSR_MSTATUS_TW_Pos 21
#define CSR_MSTATUS_TW_Msk (0x1 << CSR_MSTATUS_TW_Pos)
#define CSR_MSTATUS_TW     CSR_MSTATUS_TW_Msk
#endif

#if defined(USE_MDR1206FI) || defined(USE_MDR1206AFI) || defined(USE_MDR1206)
/**
 *  @brief IT CSR USTATUS.
 */
#define CSR_USTATUS_UIE_Pos  0
#define CSR_USTATUS_UIE_Msk  (0x1 << CSR_USTATUS_UIE_Pos)
#define CSR_USTATUS_UIE      CSR_USTATUS_UIE_Msk

#define CSR_USTATUS_UPIE_Pos 4
#define CSR_USTATUS_UPIE_Msk (0x1 << CSR_USTATUS_UPIE_Pos)
#define CSR_USTATUS_UPIE     CSR_USTATUS_UPIE_Msk
#endif

/** @} */ /* End of the group MDR32VF0xI_System_IT_Exported_Defines */

/** @addtogroup MDR32VF0xI_System_IT_Exported_Variables MDR32VF0xI System IT Exported Variables
 * @{
 */

/**
 * @brief IT exception vector table.
 */
extern IRQHandler_TypeDef ExceptionVectorTable[];

/**
 * @brief IT interrupt vector table.
 */
#if defined(USE_MDR32F02)
extern IRQHandler_TypeDef InterruptVectorTable[];
#elif defined(USE_MDR1206FI) || defined(USE_MDR1206AFI) || defined(USE_MDR1206)
extern __VECTOR_TABLE_ALIGNED IRQHandler_TypeDef InterruptVectorTable[];
#endif

#if defined(USE_MDR32F02)
/**
 * @brief IT external interrupt vector table.
 */
extern IRQHandler_TypeDef ExtInterruptVectorTable[];
#endif

/** @} */ /* End of the group MDR32VF0xI_System_IT_Exported_Variables */

/** @addtogroup MDR32VF0xI_System_IT_Exported_Functions MDR32VF0xI System IT Exported Functions
 * @{
 */

#if defined(USE_MDR32F02)
void       IT_GlobalEnableIRQ(void);
void       IT_GlobalDisableIRQ(void);
FlagStatus IT_GetGlobalEnableIRQ(void);
#elif defined(USE_MDR1206FI) || defined(USE_MDR1206AFI) || defined(USE_MDR1206)
void       IT_GlobalEnableIRQ(IT_PrivilegeModeIRQ_TypeDef PrivilegeModeIRQ);
void       IT_GlobalDisableIRQ(IT_PrivilegeModeIRQ_TypeDef PrivilegeModeIRQ);
FlagStatus IT_GetGlobalEnableIRQ(IT_PrivilegeModeIRQ_TypeDef PrivilegeModeIRQ);
#endif

/** @} */ /* End of the group MDR32VF0xI_System_IT_Exported_Functions */

/** @addtogroup MDR32VF0xI_System_IT_Exported_IRQ_Handlers MDR32VF0xI System IT Exported IRQ Handlers
 * @{
 */

/**
 * @brief IT trap handler.
 */
__INTERRUPT_MACHINE __TRAP_HANDLER_ALIGNED void Trap_IRQHandler(void);

#if defined(__GNUC__)
/**
 * @brief IT interrupt handlers.
 */
void MSIP_IRQHandler(void);
void MTIP_IRQHandler(void);
#if defined(USE_MDR1206FI) || defined(USE_MDR1206AFI) || defined(USE_MDR1206)
void USIP_IRQHandler(void);
void UTIP_IRQHandler(void);
void CSIP_IRQHandler(void);
#endif
void DMA_IRQHandler(void);
void UART1_IRQHandler(void);
void UART2_IRQHandler(void);
void SSP1_IRQHandler(void);
void POWER_IRQHandler(void);
void WWDG_IRQHandler(void);
void TIMER1_IRQHandler(void);
void TIMER2_IRQHandler(void);
void ADC_IRQHandler(void);
void I2C_IRQHandler(void);
void BKP_IRQHandler(void);
void EXT_INT1_IRQHandler(void);
void EXT_INT2_IRQHandler(void);
void EXT_INT3_IRQHandler(void);
void ADCUI_F1_IRQHandler(void);
void ADCUI_F2_IRQHandler(void);
void ADCUI_F03_IRQHandler(void);
void RANDOM_IRQHandler(void);
void USART_IRQHandler(void);
void UART3_IRQHandler(void);
void SSP2_IRQHandler(void);
void SSP3_IRQHandler(void);
void TIMER3_IRQHandler(void);
void TIMER4_IRQHandler(void);
void UART4_IRQHandler(void);
#if defined(USE_MDR32F02)
void L_BLOCK_IRQHandler(void);
void SENSORS_IRQHandler(void);
void CLK_MEASURE_IRQHandler(void);
void OTP_IRQHandler(void);
#endif

/**
 * @brief IT exception handlers.
 */
void TrapIAM_IRQHandler(void);
void TrapIAF_IRQHandler(void);
void TrapII_IRQHandler(void);
void TrapEBREAK_IRQHandler(void);
void TrapLAM_IRQHandler(void);
void TrapLAF_IRQHandler(void);
void TrapSAM_IRQHandler(void);
void TrapSAF_IRQHandler(void);
void TrapECALLM_IRQHandler(void);
#if defined(USE_MDR1206FI) || defined(USE_MDR1206AFI) || defined(USE_MDR1206)
void TrapECALLU_IRQHandler(void);
#endif
void Unexpected_IRQHandler(void);
#endif

/** @} */ /* End of the group MDR32VF0xI_System_IT_Exported_IRQ_Handlers */

/** @} */ /* End of the group MDR32VF0xI_System_IT */

/** @} */ /* End of the group MDR32VF0xI_DEVICE */

/** @} */ /* End of the group DEVICE_SUPPORT */

#ifdef __cplusplus
} // extern "C" block end
#endif

#endif /* system_MDR32VF0xI_IT_H */

/*********************** (C) COPYRIGHT 2024 Milandr ****************************
 *
 * END OF FILE system_MDR32VF0xI_it.h */

