/**
 *******************************************************************************
 * @file    MDR32VF0xI_port.h
 * @author  Milandr Application Team
 * @version V0.1.0
 * @date    22/03/2024
 * @brief   This file contains all the functions prototypes for the PORT
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
#ifndef MDR32VF0xI_PORT_H
#define MDR32VF0xI_PORT_H

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "MDR32VF0xI_config.h"

/** @addtogroup MDR32VF0xI_StdPeriph_Driver MDR32VF0xI Standard Peripheral Driver
 * @{
 */

/** @addtogroup PORT
 * @{
 */

/** @defgroup PORT_Exported_Types PORT Exported Types
 * @{
 */

/**
 * @brief PORT pins enumeration.
 */
typedef enum {
    PORT_PIN_0   = PORT_RXTX_RXTX0,   /*!< Pin 0 is selected. */
    PORT_PIN_1   = PORT_RXTX_RXTX1,   /*!< Pin 1 is selected. */
    PORT_PIN_2   = PORT_RXTX_RXTX2,   /*!< Pin 2 is selected. */
    PORT_PIN_3   = PORT_RXTX_RXTX3,   /*!< Pin 3 is selected. */
    PORT_PIN_4   = PORT_RXTX_RXTX4,   /*!< Pin 4 is selected. */
    PORT_PIN_5   = PORT_RXTX_RXTX5,   /*!< Pin 5 is selected. */
    PORT_PIN_6   = PORT_RXTX_RXTX6,   /*!< Pin 6 is selected. */
    PORT_PIN_7   = PORT_RXTX_RXTX7,   /*!< Pin 7 is selected. */
    PORT_PIN_8   = PORT_RXTX_RXTX8,   /*!< Pin 8 is selected. */
    PORT_PIN_9   = PORT_RXTX_RXTX9,   /*!< Pin 9 is selected. */
    PORT_PIN_10  = PORT_RXTX_RXTX10,  /*!< Pin 10 is selected. */
    PORT_PIN_11  = PORT_RXTX_RXTX11,  /*!< Pin 11 is selected. */
    PORT_PIN_12  = PORT_RXTX_RXTX12,  /*!< Pin 12 is selected. */
    PORT_PIN_13  = PORT_RXTX_RXTX13,  /*!< Pin 13 is selected. */
    PORT_PIN_14  = PORT_RXTX_RXTX14,  /*!< Pin 14 is selected. */
    PORT_PIN_15  = PORT_RXTX_RXTX15,  /*!< Pin 15 is selected. */
    PORT_PIN_ALL = PORT_RXTX_RXTX_Msk /*!< All pins are selected. */
} PORT_Pin_TypeDef;

#define IS_PORT_PIN(PIN) ((((PIN) & ~PORT_RXTX_RXTX_Msk) == 0x00) && \
                          ((PIN) != 0x00))

#define IS_GET_PORT_PIN(PIN) (((PIN) == PORT_PIN_0) ||  \
                              ((PIN) == PORT_PIN_1) ||  \
                              ((PIN) == PORT_PIN_2) ||  \
                              ((PIN) == PORT_PIN_3) ||  \
                              ((PIN) == PORT_PIN_4) ||  \
                              ((PIN) == PORT_PIN_5) ||  \
                              ((PIN) == PORT_PIN_6) ||  \
                              ((PIN) == PORT_PIN_7) ||  \
                              ((PIN) == PORT_PIN_8) ||  \
                              ((PIN) == PORT_PIN_9) ||  \
                              ((PIN) == PORT_PIN_10) || \
                              ((PIN) == PORT_PIN_11) || \
                              ((PIN) == PORT_PIN_12) || \
                              ((PIN) == PORT_PIN_13) || \
                              ((PIN) == PORT_PIN_14) || \
                              ((PIN) == PORT_PIN_15))

/**
 * @brief PORT pin analog-digital mode enumeration.
 */
typedef enum {
    PORT_MODE_ANALOG  = ((uint32_t)0x0),
    PORT_MODE_DIGITAL = ((uint32_t)0x1)
} PORT_Mode_TypeDef;

#define IS_PORT_MODE(MODE) (((MODE) == PORT_MODE_ANALOG) || \
                            ((MODE) == PORT_MODE_DIGITAL))

/**
 * @brief PORT pin direction (OE) enumeration.
 */
typedef enum {
    PORT_DIRECTION_IN  = ((uint32_t)0x0),
    PORT_DIRECTION_OUT = ((uint32_t)0x1)
} PORT_Direction_TypeDef;

#define IS_PORT_DIR(DIR) (((DIR) == PORT_DIRECTION_IN) || ((DIR) == PORT_DIRECTION_OUT))

/**
 * @brief PORT pin function enumeration.
 */
typedef enum {
    PORT_FUNCTION_PORT        = PORT_FUNC_MODE0_PORT,
    PORT_FUNCTION_MAIN        = PORT_FUNC_MODE0_MAIN,
    PORT_FUNCTION_ALTERNATIVE = PORT_FUNC_MODE0_ALTERNATIVE,
    PORT_FUNCTION_OVERRIDDEN  = PORT_FUNC_MODE0_OVERRIDDEN
} PORT_Function_TypeDef;

#define IS_PORT_FUNC(FUNC) (((FUNC) == PORT_FUNCTION_PORT) ||        \
                            ((FUNC) == PORT_FUNCTION_MAIN) ||        \
                            ((FUNC) == PORT_FUNCTION_ALTERNATIVE) || \
                            ((FUNC) == PORT_FUNCTION_OVERRIDDEN))

/**
 * @brief PORT pin power (PWR) enumeration.
 */
typedef enum {
#if defined(USE_MDR1206AFI)
    PORT_POWER_INCREASED_UPTO_4mA = ((uint32_t)0x0),
    PORT_POWER_NOMINAL_UPTO_2mA   = ((uint32_t)0x1)
#else
    PORT_POWER_NOMINAL_UPTO_2mA   = ((uint32_t)0x0),
    PORT_POWER_INCREASED_UPTO_4mA = ((uint32_t)0x1)
#endif
} PORT_Power_TypeDef;

#define IS_PORT_POWER(PWR) (((PWR) == PORT_POWER_NOMINAL_UPTO_2mA) || \
                            ((PWR) == PORT_POWER_INCREASED_UPTO_4mA))

#if defined(USE_MDR32F02_REV_2) || defined(USE_MDR1206FI) || defined(USE_MDR1206AFI) || defined(USE_MDR1206)
/**
 * @brief PORT pin pull-up resister usage enumeration.
 */
typedef enum {
    PORT_PULL_UP_OFF = ((uint32_t)0x0),
    PORT_PULL_UP_ON  = ((uint32_t)0x1)
} PORT_PullUp_TypeDef;

#define IS_PORT_PULL_UP(PULL_UP) (((PULL_UP) == PORT_PULL_UP_OFF) || \
                                  ((PULL_UP) == PORT_PULL_UP_ON))
#endif

/**
 * @brief PORT pin pull-down resister usage enumeration.
 */
typedef enum {
    PORT_PULL_DOWN_OFF = ((uint32_t)0x0),
    PORT_PULL_DOWN_ON  = ((uint32_t)0x1)
} PORT_PullDown_TypeDef;

#define IS_PORT_PULL_DOWN(PULL_DOWN) (((PULL_DOWN) == PORT_PULL_DOWN_ON) || \
                                      ((PULL_DOWN) == PORT_PULL_DOWN_OFF))

/**
 * @brief PORT init structure definition.
 */
typedef struct
{
    uint16_t PORT_Pin;                     /*!< Specify PORT pins to be configured.
                                                This parameter can be any combination of the @ref PORT_Pin_TypeDef values. */
    PORT_Mode_TypeDef PORT_Mode;           /*!< Specify the operation mode for the selected pins.
                                                This parameter can be a value of the @ref PORT_Mode_TypeDef. */
    PORT_Direction_TypeDef PORT_Direction; /*!< Specify in/out mode for the selected pins.
                                                This parameter can be a value of the @ref PORT_Direction_TypeDef. */
    PORT_Function_TypeDef PORT_Function;   /*!< Specify operation function for the selected pins.
                                                This parameter can be a value of the @ref PORT_Function_TypeDef. */
    PORT_Power_TypeDef PORT_Power;         /*!< Specify power for the selected pins.
                                                This parameter can be a value of the @ref PORT_Power_TypeDef. */
#if defined(USE_MDR32F02_REV_2) || defined(USE_MDR1206FI) || defined(USE_MDR1206AFI) || defined(USE_MDR1206)
    PORT_PullUp_TypeDef PORT_PullUp; /*!< Specify pull-up state for the selected pins.
                                          This parameter can be a value of the @ref PORT_PullUp_TypeDef. */
#endif
    PORT_PullDown_TypeDef PORT_PullDown; /*!< Specify pull-down state for the selected pins.
                                              This parameter can be a value of the @ref PORT_PullDown_TypeDef. */
} PORT_InitTypeDef;

/** @} */ /* End of the group PORT_Exported_Types */

/** @defgroup PORT_Exported_Defines PORT Exported Defines
 * @{
 */

/** @defgroup PORT_JTAG_define PORT JTAG Defines
 * @{
 */
#if defined(USE_JTAG)
#define PORT_JTAG      MDR_PORTA /*!< The port containing JTAG interface. */
#define PORT_JTAG_MASK (PORT_OE_OE6 | PORT_OE_OE7 | \
                        PORT_OE_OE8 | PORT_OE_OE9) /*!< JTAG pins: OE, ANALOG mask. */
#define PORT_JTAG_MASK1 (PORT_FUNC_MODE6_Msk | PORT_FUNC_MODE7_Msk | \
                         PORT_FUNC_MODE8_Msk | PORT_FUNC_MODE9_Msk) /*!< JTAG pins: FUNC mask. */
#define PORT_JTAG_MASK2 (PORT_PWR_PWR6 | PORT_PWR_PWR7 | \
                         PORT_PWR_PWR8 | PORT_PWR_PWR9) /*!< JTAG pins: PWR mask. */
#if defined(USE_MDR32F02_REV_2) || defined(USE_MDR1206FI) || defined(USE_MDR1206AFI) || defined(USE_MDR1206)
#define PORT_JTAG_MASK3 (PORT_PULL_PULL_DOWN6 | PORT_PULL_PULL_DOWN7 | \
                         PORT_PULL_PULL_DOWN8 | PORT_PULL_PULL_DOWN9 | \
                         PORT_PULL_PULL_UP6 | PORT_PULL_PULL_UP7 |     \
                         PORT_PULL_PULL_UP8 | PORT_PULL_PULL_UP9) /*!< JTAG pins: PULL mask. */
#else
#define PORT_JTAG_MASK3 (PORT_PULL_PULL_DOWN6 | PORT_PULL_PULL_DOWN7 | \
                         PORT_PULL_PULL_DOWN8 | PORT_PULL_PULL_DOWN9) /*!< JTAG pins: PULL mask. */
#endif

#define JTAG_PINS(PORT)  (((PORT) == PORT_JTAG) ? PORT_JTAG_MASK : 0)
#define JTAG_PINS1(PORT) (((PORT) == PORT_JTAG) ? PORT_JTAG_MASK1 : 0)
#define JTAG_PINS2(PORT) (((PORT) == PORT_JTAG) ? PORT_JTAG_MASK2 : 0)
#define JTAG_PINS3(PORT) (((PORT) == PORT_JTAG) ? PORT_JTAG_MASK3 : 0)

#else

#define JTAG_PINS(PORT)  0
#define JTAG_PINS1(PORT) 0
#define JTAG_PINS2(PORT) 0
#define JTAG_PINS3(PORT) 0

#endif

#define IS_NOT_JTAG_PIN(PORT, PIN) (((PIN) & JTAG_PINS(PORT)) == 0x00)

/** @} */ /* End of the group PORT_JTAG_define */

/** @} */ /* End of the group PORT_Exported_Defines */

/** @defgroup PORT_Exported_Functions PORT Exported Functions
 * @{
 */

void PORT_DeInit(MDR_PORT_TypeDef* PORTx);
void PORT_Init(MDR_PORT_TypeDef* PORTx, const PORT_InitTypeDef* PORT_InitStruct);
void PORT_StructInit(PORT_InitTypeDef* PORT_InitStruct);

BitStatus PORT_ReadInputDataBit(MDR_PORT_TypeDef* PORTx, PORT_Pin_TypeDef PORT_Pin);
uint32_t  PORT_ReadInputData(MDR_PORT_TypeDef* PORTx);

BitStatus PORT_ReadOutputDataBit(MDR_PORT_TypeDef* PORTx, PORT_Pin_TypeDef PORT_Pin);
uint32_t  PORT_ReadOutputData(MDR_PORT_TypeDef* PORTx);

void PORT_SetBits(MDR_PORT_TypeDef* PORTx, uint32_t PORT_Pin);
void PORT_ResetBits(MDR_PORT_TypeDef* PORTx, uint32_t PORT_Pin);

void PORT_WriteBit(MDR_PORT_TypeDef* PORTx, uint32_t PORT_Pin, BitStatus BitVal);

void PORT_Write(MDR_PORT_TypeDef* PORTx, uint32_t PortVal);

void PORT_ToggleBit(MDR_PORT_TypeDef* PORTx, uint32_t PORT_Pin);

/** @} */ /* End of the group PORT_Exported_Functions */

/** @} */ /* End of the group PORT */

/** @} */ /* End of the group MDR32VF0xI_StdPeriph_Driver */

#ifdef __cplusplus
} // extern "C" block end
#endif

#endif /* MDR32VF0xI_PORT_H */

/*********************** (C) COPYRIGHT 2024 Milandr ****************************
 *
 * END OF FILE MDR32VF0xI_port.h */

