/**
  ******************************************************************************
  * @file    MDR32FxQI_eth.h
  * @author  Milandr Application Team
  * @version V2.1.1i
  * @date    23/07/2024
  * @brief   This file contains all the functions prototypes for the ETHERNET
  *          firmware library.
  ******************************************************************************
  * <br><br>
  *
  * THE PRESENT FIRMWARE IS FOR GUIDANCE ONLY. IT AIMS AT PROVIDING CUSTOMERS
  * WITH CODING INFORMATION REGARDING MILANDR'S PRODUCTS IN ORDER TO FACILITATE
  * THE USE AND SAVE TIME. MILANDR SHALL NOT BE HELD LIABLE FOR ANY
  * DIRECT, INDIRECT OR CONSEQUENTIAL DAMAGES RESULTING
  * FROM THE CONTENT OF SUCH FIRMWARE AND/OR A USE MADE BY CUSTOMERS OF THE
  * CODING INFORMATION CONTAINED HEREIN IN THEIR PRODUCTS.
  *
  * <h2><center>&copy; COPYRIGHT 2025 Milandr</center></h2>
  ******************************************************************************
  */

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __MDR32FxQI_ETH_H
#define __MDR32FxQI_ETH_H

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "MDR32FxQI_config.h"

/** @addtogroup __MDR32FxQI_StdPeriph_Driver MDR32FxQI Standard Peripherial Driver
  * @{
  */

#if defined (USE_K1986VE1xI)
/** @addtogroup ETHERNET
  * @{
  */

/** @defgroup ETH_Exported_Types ETHERNET Exported Types
  * @{
  */

/**
  * @brief ETHERNET PHY Mode
  */
typedef enum
{
    ETH_PHY_MODE_10BaseT_Half_Duplex                   = (uint32_t)(0x0 << ETH_PHY_CONTROL_MODE_Pos),
    ETH_PHY_MODE_10BaseT_Full_Duplex                   = (uint32_t)(0x1 << ETH_PHY_CONTROL_MODE_Pos),
    ETH_PHY_MODE_100BaseT_Half_Duplex                  = (uint32_t)(0x2 << ETH_PHY_CONTROL_MODE_Pos),
    ETH_PHY_MODE_100BaseT_Full_Duplex                  = (uint32_t)(0x3 << ETH_PHY_CONTROL_MODE_Pos),
    ETH_PHY_MODE_100BaseT_Half_Duplex_With_Auto_Tuning = (uint32_t)(0x4 << ETH_PHY_CONTROL_MODE_Pos),
    ETH_PHY_MODE_Repeater                              = (uint32_t)(0x5 << ETH_PHY_CONTROL_MODE_Pos),
    ETH_PHY_MODE_Low_Consumption                       = (uint32_t)(0x6 << ETH_PHY_CONTROL_MODE_Pos),
    ETH_PHY_MODE_AutoNegotiation                       = (uint32_t)(0x7 << ETH_PHY_CONTROL_MODE_Pos)
} ETH_PHY_MODE;

#define IS_ETH_PHY_MODE(MODE)        (((MODE) & (~ETH_PHY_CONTROL_MODE_Msk)) == 0)

/**
  * @brief ETHERNET PHY Interface
  */
typedef enum
{
    ETH_PHY_INTERFACE_FIBER_OPTIC    = (uint32_t)(0x01 << ETH_PHY_CONTROL_FX_EN_Pos),
    ETH_PHY_INTERFACE_ETHERNET_802_3 = (uint32_t)(0x00 << ETH_PHY_CONTROL_FX_EN_Pos)
} ETH_PHY_INTERFACE;

#define IS_ETH_PHY_INTERFACE(INTERFACE)  (((INTERFACE) == ETH_PHY_INTERFACE_FIBER_OPTIC) || \
                                          ((INTERFACE) == ETH_PHY_INTERFACE_ETHERNET_802_3))

/**
  * @brief ETHERNET DBG Mode
  */
typedef enum
{
    ETH_DBG_MODE_FREE_RUN = (uint32_t)(0x0 << ETH_G_CFGh_DBG_MODE_Pos),
    ETH_DBG_MODE_HALT     = (uint32_t)(0x2 << ETH_G_CFGh_DBG_MODE_Pos),
    ETH_DBG_MODE_STOP     = (uint32_t)(0x3 << ETH_G_CFGh_DBG_MODE_Pos)
} ETH_DBG_MODE;

#define IS_ETH_DBG_MODE(MODE)   (((MODE) == ETH_DBG_MODE_FREE_RUN) | \
                                 ((MODE) == ETH_DBG_MODE_HALT)     | \
                                 ((MODE) == ETH_DBG_MODE_STOP))

/**
  * @brief ETHERNET Buffer Mode
  */
typedef enum
{
    ETH_BUFFER_MODE_LINEAR                    = (uint32_t) (0x0 << ETH_G_CFGl_BUFF_MODE_Pos),
    ETH_BUFFER_MODE_AUTOMATIC_CHANGE_POINTERS = (uint32_t) (0x1 << ETH_G_CFGl_BUFF_MODE_Pos),
    ETH_BUFFER_MODE_FIFO                      = (uint32_t) (0x2 << ETH_G_CFGl_BUFF_MODE_Pos)
} ETH_BUFFER_MODE;

#define IS_ETH_BUFFER_MODE(MODE)  (((MODE) == ETH_BUFFER_MODE_LINEAR)                    | \
                                   ((MODE) == ETH_BUFFER_MODE_AUTOMATIC_CHANGE_POINTERS) | \
                                   ((MODE) == ETH_BUFFER_MODE_FIFO))

/**
  * @brief ETHERNET Transmitter bits order
  */
typedef enum
{
    ETH_TRANSMITTER_BITS_ORDER_LSB = (uint32_t)(0 << ETH_X_CFG_MSB1st_Pos),
    ETH_TRANSMITTER_BITS_ORDER_MSB = (uint32_t)(1 << ETH_X_CFG_MSB1st_Pos)
} ETH_TRANSMITTER_BITS_ORDER;

#define IS_ETH_TRANSMITTER_BITS_ORDER(ORDER)  (((ORDER) == ETH_TRANSMITTER_BITS_ORDER_LSB ) || \
                                               ((ORDER) == ETH_TRANSMITTER_BITS_ORDER_MSB))

/**
  * @brief ETHERNET Transmitter Endain (little endian or big endian)
  */
typedef enum
{
    ETH_TRANSMITTER_BE_LITTLE_ENDIAN = (uint32_t)(0x0 << ETH_X_CFG_BE_Pos),
    ETH_TRANSMITTER_BE_BIG_ENDIAN    = (uint32_t)(0x1 << ETH_X_CFG_BE_Pos)
} ETH_TRANSMITTER_BE;

#define IS_ETH_TRANSMITTER_BE(BE)   (((BE) == ETH_TRANSMITTER_BE_LITTLE_ENDIAN) | \
                                     ((BE) == ETH_TRANSMITTER_BE_BIG_ENDIAN))

/**
  * @brief ETHERNET Transmitter Event Mode
  */
typedef enum
{
    ETH_TRANSMITTER_EVENT_MODE_XFIFO_EMPTY        = (uint32_t)(0x0 << ETH_X_CFG_EVNT_MODE_Pos),
    ETH_TRANSMITTER_EVENT_MODE_XFIFO_ALMOST_EMPTY = (uint32_t)(0x1 << ETH_X_CFG_EVNT_MODE_Pos),
    ETH_TRANSMITTER_EVENT_MODE_XFIFO_HALF_FULL    = (uint32_t)(0x2 << ETH_X_CFG_EVNT_MODE_Pos),
    ETH_TRANSMITTER_EVENT_MODE_XFIFO_ALMOST_FULL  = (uint32_t)(0x3 << ETH_X_CFG_EVNT_MODE_Pos),
    ETH_TRANSMITTER_EVENT_MODE_XFIFO_FULL         = (uint32_t)(0x4 << ETH_X_CFG_EVNT_MODE_Pos),
    ETH_TRANSMITTER_EVENT_MODE_PAKET_SENT         = (uint32_t)(0x5 << ETH_X_CFG_EVNT_MODE_Pos),
    ETH_TRANSMITTER_EVENT_MODE_READ_WORD          = (uint32_t)(0x6 << ETH_X_CFG_EVNT_MODE_Pos),
    ETH_TRANSMITTER_EVENT_MODE_START_TRANSMIT     = (uint32_t)(0x7 << ETH_X_CFG_EVNT_MODE_Pos)
} ETH_TRANSMITTER_EVENT_MODE;

#define ETH_TRANSMITTER_EVENT_MODE_MASK      (uint32_t)(0x7 << ETH_X_CFG_EVNT_MODE_Pos)

#define IS_ETH_TRANSMITTER_EVENT_MODE(MODE)  (((MODE) & (~ETH_TRANSMITTER_EVENT_MODE_MASK)) == 0)

/**
  * @brief ETHERNET Receiver Endain (little endian or big endian)
  */
typedef enum
{
    ETH_RECEIVER_BE_LITTLE_ENDIAN = (uint32_t)(0x0 << ETH_R_CFG_BE_Pos),
    ETH_RECEIVER_BE_BIG_ENDIAN    = (uint32_t)(0x1 << ETH_R_CFG_BE_Pos)
} ETH_RECEIVER_BE;

#define IS_ETH_RECEIVER_BE(BE)  (((BE) == ETH_RECEIVER_BE_LITTLE_ENDIAN) | \
                                 ((BE) == ETH_RECEIVER_BE_BIG_ENDIAN))

/**
  * @brief ETHERNET Receiver bits order
  */
typedef enum
{
    ETH_RECEIVER_BITS_ORDER_LSB = (uint32_t)(0 << ETH_R_CFG_MSB1st_Pos),
    ETH_RECEIVER_BITS_ORDER_MSB = (uint32_t)(0 << ETH_R_CFG_MSB1st_Pos)
} ETH_RECEIVER_BITS_ORDER;

#define IS_ETH_RECEIVER_BITS_ORDER(ORDER)   (((ORDER) == ETH_RECEIVER_BITS_ORDER_LSB ) || \
                                             ((ORDER) == ETH_RECEIVER_BITS_ORDER_MSB))

/**
  * @brief ETHERNET Receiver Event Mode
  */
typedef enum
{
    ETH_RECEIVER_EVENT_MODE_RFIFO_NOT_EMPTY        = (uint32_t)(0x0 << ETH_R_CFG_EVNT_MODE_Pos),
    ETH_RECEIVER_EVENT_MODE_RFIFO_NOT_ALMOST_EMPTY = (uint32_t)(0x1 << ETH_R_CFG_EVNT_MODE_Pos),
    ETH_RECEIVER_EVENT_MODE_RFIFO_HALF_EMPTY       = (uint32_t)(0x2 << ETH_R_CFG_EVNT_MODE_Pos),
    ETH_RECEIVER_EVENT_MODE_RFIFO_NOT_ALMOST_FULL  = (uint32_t)(0x3 << ETH_R_CFG_EVNT_MODE_Pos),
    ETH_RECEIVER_EVENT_MODE_RFIFO_NOT_FULL         = (uint32_t)(0x4 << ETH_R_CFG_EVNT_MODE_Pos),
    ETH_RECEIVER_EVENT_MODE_PACET_RECEIVED         = (uint32_t)(0x5 << ETH_R_CFG_EVNT_MODE_Pos),
    ETH_RECEIVER_EVENT_MODE_WORD_RECEIVED          = (uint32_t)(0x6 << ETH_R_CFG_EVNT_MODE_Pos),
    ETH_RECEIVER_EVENT_MODE_PACET_DROPED           = (uint32_t)(0x7 << ETH_R_CFG_EVNT_MODE_Pos)
} ETH_RECEIVER_EVENT_MODE;

#define ETH_RECEIVER_EVENT_MODE_MASK                        (uint32_t)(0x7 << ETH_R_CFG_EVNT_MODE_Pos)

#define IS_ETH_RECEIVER_EVENT_MODE(MODE)                    (((MODE) & (~ETH_RECEIVER_EVENT_MODE_MASK)) == 0)

/**
  * @brief ETHERNET Interrupt defintions
  */
typedef enum
{
    ETH_MAC_IT_MII_RDY  = (uint32_t)(0x00008000),
    ETH_MAC_IT_MDIO_INT = (uint32_t)(0x00004000),
    ETH_MAC_IT_CRS_LOST = (uint32_t)(0x00001000),
    ETH_MAC_IT_LC       = (uint32_t)(0x00000800),
    ETH_MAC_IT_UNDF     = (uint32_t)(0x00000400),
    ETH_MAC_IT_XF_ERR   = (uint32_t)(0x00000200),
    ETH_MAC_IT_XF_OK    = (uint32_t)(0x00000100),
    ETH_MAC_IT_SF       = (uint32_t)(0x00000080),
    ETH_MAC_IT_LF       = (uint32_t)(0x00000040),
    ETH_MAC_IT_CF       = (uint32_t)(0x00000020),
    ETH_MAC_IT_CRC_ERR  = (uint32_t)(0x00000010),
    ETH_MAC_IT_SMB_ERR  = (uint32_t)(0x00000008),
    ETH_MAC_IT_OVF      = (uint32_t)(0x00000004),
    ETH_MAC_IT_MISSED_F = (uint32_t)(0x00000002),
    ETH_MAC_IT_RF_OK    = (uint32_t)(0x00000001)
} ETH_IT_Def;

#define IS_ETH_MAC_IT(IT) (((IT) & 0xFFFF2000) == 0)

/**
  * @brief ETHERNET Flags
  */
typedef enum
{
    ETH_MAC_FLAG_R_EMPTY  = ((uint32_t)0x00000001),
    ETH_MAC_FLAG_R_AEMPTY = ((uint32_t)0x00000002),
    ETH_MAC_FLAG_R_HALF   = ((uint32_t)0x00000004),
    ETH_MAC_FLAG_R_AFULL  = ((uint32_t)0x00000008),
    ETH_MAC_FLAG_R_FULL   = ((uint32_t)0x00000010),
    ETH_MAC_FLAG_X_EMPTY  = ((uint32_t)0x00000100),
    ETH_MAC_FLAG_X_AEMPTY = ((uint32_t)0x00000200),
    ETH_MAC_FLAG_X_HALF   = ((uint32_t)0x00000400),
    ETH_MAC_FLAG_X_AFULL  = ((uint32_t)0x00000800),
    ETH_MAC_FLAG_X_FULL   = ((uint32_t)0x00001000)
} ETH_MAC_FLAGx;

#define IS_ETH_MAC_FLAG(FLAG)   ((((FLAG) & 0xFFFFE0E0) == 0) | \
                                (!((FLAG) == 0)))

/**
  * @brief ETHERNET Clock Source
  */
typedef enum
{
    ETH_PHY_CLOCK_SOURCE_HSI      = (uint32_t)(0x00000000),
    ETH_PHY_CLOCK_SOURCE_HSE      = (uint32_t)(0x10000000),
    ETH_PHY_CLOCK_SOURCE_PLLCPU_0 = (uint32_t)(0x20000000),
    ETH_PHY_CLOCK_SOURCE_HSE2     = (uint32_t)(0x30000000)
} ETH_Clock_Source;

#define IS_ETH_CLOCK_SOURCE(CLOCK_SOURCE)   (((CLOCK_SOURCE) == ETH_PHY_CLOCK_SOURCE_HSI)      | \
                                             ((CLOCK_SOURCE) == ETH_PHY_CLOCK_SOURCE_HSE)      | \
                                             ((CLOCK_SOURCE) == ETH_PHY_CLOCK_SOURCE_PLLCPU_0) | \
                                             ((CLOCK_SOURCE) == ETH_PHY_CLOCK_SOURCE_HSE2))

/**
  * @brief ETHERNET Clock BRG
  */
typedef enum
{
    ETH_HCLKdiv1   = ((uint32_t)0x00),
    ETH_HCLKdiv2   = ((uint32_t)0x01),
    ETH_HCLKdiv4   = ((uint32_t)0x02),
    ETH_HCLKdiv8   = ((uint32_t)0x03),
    ETH_HCLKdiv16  = ((uint32_t)0x04),
    ETH_HCLKdiv32  = ((uint32_t)0x05),
    ETH_HCLKdiv64  = ((uint32_t)0x06),
    ETH_HCLKdiv128 = ((uint32_t)0x07)
} ETH_Clock_BRG;

#define IS_ETH_CLOCK_BRG(BRG)   (((BRG) & ~0x7) == 0)

/**
  * @brief ETHERNET PHY Clock HCLKdiv
  */
typedef enum
{
    ETH_PHY_HCLKdiv1   = ((uint32_t)0x00 << RST_CLK_ETH_CLOCK_PHY_BRG_Pos),
    ETH_PHY_HCLKdiv2   = ((uint32_t)0x01 << RST_CLK_ETH_CLOCK_PHY_BRG_Pos),
    ETH_PHY_HCLKdiv4   = ((uint32_t)0x02 << RST_CLK_ETH_CLOCK_PHY_BRG_Pos),
    ETH_PHY_HCLKdiv8   = ((uint32_t)0x03 << RST_CLK_ETH_CLOCK_PHY_BRG_Pos),
    ETH_PHY_HCLKdiv16  = ((uint32_t)0x04 << RST_CLK_ETH_CLOCK_PHY_BRG_Pos),
    ETH_PHY_HCLKdiv32  = ((uint32_t)0x05 << RST_CLK_ETH_CLOCK_PHY_BRG_Pos),
    ETH_PHY_HCLKdiv64  = ((uint32_t)0x06 << RST_CLK_ETH_CLOCK_PHY_BRG_Pos),
    ETH_PHY_HCLKdiv128 = ((uint32_t)0x07 << RST_CLK_ETH_CLOCK_PHY_BRG_Pos)
} ETH_PHY_Clock_HCLKdiv;

#define IS_ETH_PHY_HCLKdiv(BRG)   (((BRG) & ~0x00070000UL) == 0)

/**
  * @brief ETHERNET Clock Number
  */
typedef enum
{
    ETH_CLK1 = ((uint32_t)0x01000000),
    ETH_CLK2 = ((uint32_t)0x40000000)
} ETH_Clock_Number;

#define IS_ETH_CLK(ETH_CLK) (((ETH_CLK) == ETH_CLK1) || \
                             ((ETH_CLK) == ETH_CLK2))

/**--------------------------------------------------------------------------**/
/**
  * @brief            Description of common PHY registers
  */
/**--------------------------------------------------------------------------**/

/**
  * @brief ETHERNET PHY FLAGs
  */
typedef enum
{
    ETH_PHY_FLAG_MDINT         = ((uint32_t)0x00000400),
    ETH_PHY_FLAG_MDO           = ((uint32_t)0x00000200),
    ETH_PHY_FLAG_FX_VALID      = ((uint32_t)0x00000100),
    ETH_PHY_FLAG_COL           = ((uint32_t)0x000000C0),
    ETH_PHY_FLAG_CRS           = ((uint32_t)0x00000020),
    ETH_PHY_FLAG_READY         = ((uint32_t)0x00000010),
    ETH_PHY_FLAG_FULL_DUPLEX   = ((uint32_t)0x00000008),
    ETH_PHY_FLAG_CARRIER_SENSE = ((uint32_t)0x00000004),
    ETH_PHY_FLAG_LINK          = ((uint32_t)0x00000002),
    ETH_PHY_FLAG_100MBIT       = ((uint32_t)0x00000001)
} ETH_PHY_FLAGx;

#define IS_ETH_PHY_FLAG(FLAG)   (((FLAG) == ETH_PHY_FLAG_MDINT)         || \
                                 ((FLAG) == ETH_PHY_FLAG_MDO)           || \
                                 ((FLAG) == ETH_PHY_FLAG_FX_VALID)      || \
                                 ((FLAG) == ETH_PHY_FLAG_COL)           || \
                                 ((FLAG) == ETH_PHY_FLAG_CRS)           || \
                                 ((FLAG) == ETH_PHY_FLAG_READY)         || \
                                 ((FLAG) == ETH_PHY_FLAG_FULL_DUPLEX)   || \
                                 ((FLAG) == ETH_PHY_FLAG_CARRIER_SENSE) || \
                                 ((FLAG) == ETH_PHY_FLAG_LINK)          || \
                                 ((FLAG) == ETH_PHY_FLAG_100MBIT))

/**
  * @brief ETHERNET PHY Register address
  */
typedef enum
{
    PHY_BCR      =  0,  /*!< Basic Control Register */
    PHY_BSR      =  1,  /*!< Basic Status Register */
    PHY_ID1      =  2,
    PHY_ID2      =  3,
    PHY_ATU      =  4,
    PHY_OPP_ATU  =  5,
    PHY_EATU     =  6,
    PHY_EMODE    = 18,
    PHY_IT_FLAGs = 29,
    PHY_IT_MASKs = 30,
    PHY_ECTR     = 31
} ETH_PHY_Reg_Addr;

#define IS_ETH_PHYReg(PHYreg)   ((PHYreg <= 6UL)  || \
                                 (PHYreg == 18UL) || (PHYreg == 29UL)  || \
                                 (PHYreg == 30UL) || (PHYreg == 31UL))

/**
 * @brief ETHERNET MDIO MDC prescaler
 */
typedef enum {
    ETH_MDIO_MDC_PRESCALER_DIV_16  = ((uint32_t)0x00 << ETH_MDIO_CTRL_DIV_Pos), /*!< Selects ETH_CLK divided by 16 to generate the MDC clock */
    ETH_MDIO_MDC_PRESCALER_DIV_32  = ((uint32_t)0x01 << ETH_MDIO_CTRL_DIV_Pos), /*!< Selects ETH_CLK divided by 32 to generate the MDC clock */
    ETH_MDIO_MDC_PRESCALER_DIV_48  = ((uint32_t)0x02 << ETH_MDIO_CTRL_DIV_Pos), /*!< Selects ETH_CLK divided by 48 to generate the MDC clock */
    ETH_MDIO_MDC_PRESCALER_DIV_64  = ((uint32_t)0x03 << ETH_MDIO_CTRL_DIV_Pos), /*!< Selects ETH_CLK divided by 64 to generate the MDC clock */
    ETH_MDIO_MDC_PRESCALER_DIV_80  = ((uint32_t)0x04 << ETH_MDIO_CTRL_DIV_Pos), /*!< Selects ETH_CLK divided by 80 to generate the MDC clock */
    ETH_MDIO_MDC_PRESCALER_DIV_96  = ((uint32_t)0x05 << ETH_MDIO_CTRL_DIV_Pos), /*!< Selects ETH_CLK divided by 96 to generate the MDC clock */
    ETH_MDIO_MDC_PRESCALER_DIV_112 = ((uint32_t)0x06 << ETH_MDIO_CTRL_DIV_Pos), /*!< Selects ETH_CLK divided by 112 to generate the MDC clock */
    ETH_MDIO_MDC_PRESCALER_DIV_128 = ((uint32_t)0x07 << ETH_MDIO_CTRL_DIV_Pos)  /*!< Selects ETH_CLK divided by 128 to generate the MDC clock */
} ETH_MDIO_MDC_PRESCALER;

#define IS_ETH_MDIO_MDC_PRESCALER(VALUE) (((VALUE) == ETH_MDIO_MDC_PRESCALER_DIV_16 ) || \
                                          ((VALUE) == ETH_MDIO_MDC_PRESCALER_DIV_32 ) || \
                                          ((VALUE) == ETH_MDIO_MDC_PRESCALER_DIV_48 ) || \
                                          ((VALUE) == ETH_MDIO_MDC_PRESCALER_DIV_64 ) || \
                                          ((VALUE) == ETH_MDIO_MDC_PRESCALER_DIV_80 ) || \
                                          ((VALUE) == ETH_MDIO_MDC_PRESCALER_DIV_96 ) || \
                                          ((VALUE) == ETH_MDIO_MDC_PRESCALER_DIV_112) || \
                                          ((VALUE) == ETH_MDIO_MDC_PRESCALER_DIV_128))

/**
  * @brief ETHERNET Init Structure definition
  * @note  The user should not configure all the ETH_InitTypeDef structure's fields.
  *        By calling the ETH_StructInit function the structure’s fields are set to their default values.
  *        Only the parameters that will be set to a non-default value should be configured.
  */
typedef struct
{
    /**
      * @brief PHY
      */
    uint32_t                   ETH_PHY_Address;                /*!< Addres PHY.
                                                                    This parameter can be a value from 0 to 0x1F. */
    ETH_PHY_MODE               ETH_PHY_Mode;                   /*!< Selects the PHY MODE.
                                                                    This parameter can be a value of @ref ETH_PHY_MODE. */
    ETH_PHY_INTERFACE          ETH_PHY_Interface;              /*!< Selects the PHY interface.
                                                                    This parameter can be a value of @ref ETH_PHY_INTERFACE. */
    /**
      * @brief MDIO
      */
    ETH_MDIO_MDC_PRESCALER     ETH_MDIO_MDC_Prescaler;         /*!< Specifies the MDC Prescaler configuration (MDC = ETH_CLK / ETH_MDIO_MDC_Prescaler).
                                                                    This parameter can be a value of @ref ETH_MDIO_MDC_PRESCALER */
    /**
      * @brief MAC General Config
      */
    uint32_t                   ETH_Dilimiter;                  /*!< Defines the boundaries of the transmitter and receiver buffers.
                                                                    This parameter can be a value from 0 to 0x1FFF. */
    ETH_DBG_MODE               ETH_DBG_Mode;                   /*!< Select the mode of operation in debug mode.
                                                                    This parameter can be a value of @ref ETH_DBG_MODE. */
    FunctionalState            ETH_DBG_XF;                     /*!< Enable or disable automatic change of the transmitter FIFO pointers.
                                                                    This parameter can be a value of @ref FunctionalState. */
    FunctionalState            ETH_DBG_RF;                     /*!< Enable or disable automatic change of the receiver FIFO pointers.
                                                                    This parameter can be a value of @ref FunctionalState. */
    FunctionalState            ETH_Loopback_Mode;              /*!< Selects or not the internal MAC Loopback mode.
                                                                    This parameter can be: ENABLE or DISABLE. */
    BitStatus                  ETH_Receiver_RST;               /*!< Receiver reset bit.
                                                                    This parameter can be a value of @ref BitStatus. */
    BitStatus                  ETH_Transmitter_RST;            /*!< Transmitter reset bit.
                                                                    This parameter can be a value of @ref BitStatus. */
    FunctionalState            ETH_Register_CLR;               /*!< Permission or prohibition of discharge status registers his reading.
                                                                    This parameter can be: ENABLE or DISABLE. */
    ETH_BUFFER_MODE            ETH_Buffer_Mode;                /*!< Set the operating mode buffers.
                                                                    This parameter can be a value of @ref ETH_BUFFER_MODE */
    FunctionalState            ETH_Extension_Mode;             /*!< Enable or disable mode additions to the size of short packets slotTime field "Extension".
                                                                    This parameter can be: ENABLE or DISABLE. */
    FunctionalState            ETH_HalfDuplex_Mode;            /*!< Enable or disable half-duplex operation.
                                                                    This parameter can be: ENABLE or DISABLE. */
    FunctionalState            ETH_DTRM;                       /*!< Enable or disable mode deterministic delivery time.
                                                                    This parameter can be: ENABLE or DISABLE. */
    FunctionalState            ETH_Pause;                      /*!< Enable or disable the automatic packet processing PAUSE.
                                                                    This parameter can be: ENABLE or DISABLE. */
    uint32_t                   ETH_ColWnd;                     /*!< Set the size of the "window collisions".
                                                                    This parameter can be a value from 0 to 0xFF. */
    /**
     * @brief MAC Transmitter Config
     */
    FunctionalState            ETH_Transmitter_State;          /*!< The state of the transmitter.
                                                                    This parameter can be: ENABLE or DISABLE. */
    ETH_TRANSMITTER_BE         ETH_Transmitter_BE;             /*!< Set the sequence of bytes in the word transmitter.
                                                                    This parameter can be a value of @ref ETH_TRANSMITTER_BE. */
    ETH_TRANSMITTER_BITS_ORDER ETH_Transmitter_Bits_Order;     /*!< The order of the bits in the transmission of data bytes.
                                                                    This parameter can be a value of @ref ETH_TRANSMITTER_BITS_ORDER. */
    ETH_TRANSMITTER_EVENT_MODE ETH_Transmitter_Event_Mode;     /*!< Defines mode of operation withdrawal EVNT[1].
                                                                    This parameter can be a value of @ref ETH_TRANSMITTER_EVENT_MODE. */
    FunctionalState            ETH_Automatic_Pad_Strip;        /*!< Enable or disable addition to the minimum packet length PAD-s.
                                                                    This parameter can be: ENABLE or DISABLE. */
    FunctionalState            ETH_Automatic_Preamble;         /*!< Enable or disable supplement packet preamble.
                                                                    This parameter can be: ENABLE or DISABLE. */
    FunctionalState            ETH_Automatic_CRC_Strip;        /*!< Enable or disable supplement package automatically calculated CRC.
                                                                    This parameter can be: ENABLE or DISABLE. */
    FunctionalState            ETH_Automatic_IPG;              /*!< Enable or disable curing system pauses between sending packets.
                                                                    This parameter can be: ENABLE or DISABLE. */
    uint32_t                   ETH_Retry_Counter;              /*!< Maximum number of attempts to send a packet.
                                                                    This parameter can be a value from 0 to 0xF. */
    /**
     * @brief MAC Receiver Config
     */
    FunctionalState            ETH_Receiver_State;             /*!< The state of the receiver.
                                                                    This parameter can be a value of @ref FunctionalState. */
    ETH_RECEIVER_BE            ETH_Receiver_BE;                /*!< Set the sequence of bytes in the word receiver.
                                                                    This parameter can be a value of @ref ETH_RECEIVER_BE. */
    ETH_RECEIVER_BITS_ORDER    ETH_Receiver_Bits_Order;        /*!< Set the sequence of bits at the reception of data bytes.
                                                                    This parameter can be a value of @ref ETH_RECEIVER_BITS_ORDER. */
    ETH_RECEIVER_EVENT_MODE    ETH_Receiver_Event_Mode;        /*!< Defines mode of operation withdrawal EVNT[0].
                                                                    This parameter can be a value of @ref ETH_RECEIVER_EVENT_MODE. */
    FunctionalState            ETH_Receive_All_Packets;        /*!< Selects or not all frames reception by the MAC (No filtering).
                                                                    This parameter can be a value of @ref FunctionalState. */
    FunctionalState            ETH_Short_Frames_Reception;     /*!< Enable or disable the reception of packets of less than the minimum.
                                                                    This parameter can be a value of @ref FunctionalState. */
    FunctionalState            ETH_Long_Frames_Reception;      /*!< Enable or disable the reception of packets longer than the maximum.
                                                                    This parameter can be a value of @ref FunctionalState. */
    FunctionalState            ETH_Broadcast_Frames_Reception; /*!< Enable or disable Receiving packet broadcast MAC-address.
                                                                    This parameter can be a value of @ref FunctionalState. */
    FunctionalState            ETH_Error_CRC_Frames_Reception; /*!< Enable or disable receiving packets with errors.
                                                                    This parameter can be a value of @ref FunctionalState. */
    FunctionalState            ETH_Control_Frames_Reception;   /*!< Enable or disable receiving control packets.
                                                                    This parameter can be a value of @ref FunctionalState. */
    FunctionalState            ETH_Unicast_Frames_Reception;   /*!< Enable or disable receiving packets with MAC-address specified in the register MAC_Address.
                                                                    This parameter can be a value of @ref FunctionalState. */
    FunctionalState            ETH_Source_Addr_HASH_Filter;    /*!< Enable or disable receiving packets with a multicast MAC-address-filtered HAS-table.
                                                                    This parameter can be a value of @ref FunctionalState. */
    /**
     * @brief MAC Address
     */
    uint16_t                   ETH_MAC_Address[3];                    /*!< Individual MAC address */
    /**
     * @brief Hash Table
     */
    uint32_t                   ETH_Hash_Table_High;                 /*!< This field holds the higher 32 bits of Hash table.  */
    uint32_t                   ETH_Hash_Table_Low;                  /*!< This field holds the lower 32 bits of Hash table.  */
    /**
     * @brief Time settings
     */
    uint32_t                   ETH_IPG;                            /*!< The value of inter-packet interval for full duplex
                                                                       This parameter can be a value from 0 to 0xFFFF. */
    uint32_t                   ETH_PSC;                            /*!< Prescaler value increment values for BAG and JitterWnd.
                                                                       This parameter can be a value from 0 to 0xFFFF. */
    uint32_t                   ETH_BAG;                            /*!< The period value of the packets.
                                                                       This parameter can be a value from 0 to 0xFFFF. */
    uint32_t                   ETH_JitterWnd;                        /*!< Jitter value of packets transmitted.
                                                                       This parameter can be a value from 0 to 0xFFFF. */
} ETH_InitTypeDef;

/**
 * @brief Bit-field structure of the state of the packet reception
 */
typedef struct
{
    uint32_t Length  :16; /*!< The number of bytes in the packet including header and CRC. */
    uint32_t PF_ERR  :1;  /*!< A sign package PAUSE. */
    uint32_t CF_ERR  :1;  /*!< A sign Management Pack (filtering by MAC and special tags in the field length - 13.14 - octets). */
    uint32_t LF_ERR  :1;  /*!< A sign excess packet length 1518 octets. */
    uint32_t SF_ERR  :1;  /*!< A sign of lack of packet length 64 octets. */
    uint32_t LEN_ERR :1;  /*!< A sign mismatch between the actual length and the length specified in the length field - 13.14 octets. */
    uint32_t DN_ERR  :1;  /*!< A sign bit of the packet is not a multiple of 8. */
    uint32_t CRC_ERR :1;  /*!< A sign mismatch packet CRC. */
    uint32_t SMB_ERR :1;  /*!< A sign of the presence in the packet error nibbles. */
    uint32_t MCA     :1;  /*!< A sign group package (MAC matches HASH). */
    uint32_t BCA     :1;  /*!< A sign of the broadcast packet (MAC = FF:FF:FF:FF:FF:FF). */
    uint32_t UCA     :1;  /*!< A sign individual package (MAC corresponds to the set). */
} ETH_StatusPacketReceptionBitFileds;

/**
 * @brief The state of the packet reception    Unioun definition
 */
typedef union
{
    uint32_t Status;                           /*!< Specifies the fully status Word. */
    ETH_StatusPacketReceptionBitFileds Fields; /*!< Specifies the particular field of the status word.*/
} ETH_StatusPacketReceptionTypeDef;

/** @} */ /* End of group ETH_Exported_Types */


/** @defgroup ETH_Exported_Constants ETHERNET Exported Constants
  * @{
  */

/** @defgroup PHY_Read_Write_Timeouts PHY Read write Timeouts
  * @{
  */

#define PHY_READ_TO             ((uint32_t)0x0004FFFF)
#define PHY_WRITE_TO            ((uint32_t)0x0004FFFF)

/** @} */ /* End of group PHY_Read_Write_Timeouts */

/** @} */ /* End of group ETH_Exported_Constants */


/** @defgroup ETH_Exported_Functions ETHERNET Exported Functions
  * @{
  */

void ETH_ClockDeInit(void);
void ETH_PHY_ClockConfig(ETH_Clock_Source clock_source, ETH_PHY_Clock_HCLKdiv PHY_HCLKdiv);
void ETH_BRGInit(ETH_Clock_BRG ETH_HCLKdiv);
void ETH_ClockCMD(ETH_Clock_Number ETHx, FunctionalState NewState);
void ETH_PHY_Reset(MDR_ETHERNET_TypeDef * ETHERNETx);
void ETH_PHYCmd(MDR_ETHERNET_TypeDef * ETHERNETx, FunctionalState NewState);
void ETH_DeInit(MDR_ETHERNET_TypeDef * ETHERNETx);
void ETH_StructInit(ETH_InitTypeDef * ETH_InitStruct);
void ETH_Init(MDR_ETHERNET_TypeDef * ETHERNETx, ETH_InitTypeDef * ETH_InitStruct);
void ETH_Start(MDR_ETHERNET_TypeDef * ETHERNETx);
void ETH_MACTransmissionCmd(MDR_ETHERNET_TypeDef * ETHERNETx, FunctionalState NewState);
void ETH_MACReceptionCmd(MDR_ETHERNET_TypeDef * ETHERNETx, FunctionalState NewState);
uint16_t ETH_GetMACITStatusRegister(MDR_ETHERNET_TypeDef * ETHERNETx);
FlagStatus ETH_GetMACITStatus(MDR_ETHERNET_TypeDef * ETHERNETx, uint32_t ETH_MAC_FLAG);
uint32_t ETH_GetMACStatusRegister(MDR_ETHERNET_TypeDef * ETHERNETx);
void ETH_MACITConfig(MDR_ETHERNET_TypeDef * ETHERNETx, uint32_t ETH_MAC_IT, FunctionalState NewState);
void ETH_MACAddressConfig(MDR_ETHERNET_TypeDef * ETHERNETx, uint16_t *MacAddr);
void ETH_GetMACAddress(MDR_ETHERNET_TypeDef * ETHERNETx, uint16_t *Addr);
FlagStatus ETH_GetPHYStatus(MDR_ETHERNET_TypeDef * ETHERNETx, uint16_t ETH_PHY_FLAG);
FlagStatus ETH_GetPHYAutonegStatus(MDR_ETHERNET_TypeDef * ETHERNETx);
uint16_t ETH_ReadPHYRegister(MDR_ETHERNET_TypeDef * ETHERNETx, uint16_t PHYAddress, ETH_PHY_Reg_Addr PHYReg);
ErrorStatus ETH_WritePHYRegister(MDR_ETHERNET_TypeDef * ETHERNETx, uint16_t PHYAddress, ETH_PHY_Reg_Addr PHYReg, uint16_t PHYValue);
uint32_t ETH_ReceivedFrame(MDR_ETHERNET_TypeDef * ETHERNETx, uint32_t * ptr_InputBuffer);
void ETH_SendFrame(MDR_ETHERNET_TypeDef * ETHERNETx, uint32_t * ptr_OutputBuffer, uint32_t BufLen);
uint16_t ETH_GetRxFrameCount(MDR_ETHERNET_TypeDef * ETHERNETx);
void ETH_DecrementRxFrameCount(MDR_ETHERNET_TypeDef * ETHERNETx);
uint16_t ETH_GetTxBufferFreeSize(MDR_ETHERNET_TypeDef * ETHERNETx);
void ETH_DMAPrepare(void);
void ETH_DMAFrameRx(uint32_t * DstBuf, uint32_t BufferSize, uint32_t * SrcBuf);
void ETH_DMAFrameTx(uint32_t * DstBuf, uint32_t BufferSize, uint32_t *  SrcBuf);
void ETH_CheckMode10BaseT(MDR_ETHERNET_TypeDef * ETHERNETx);

FlagStatus ETH_GetFlagStatus(MDR_ETHERNET_TypeDef * ETHERNETx, uint16_t ETH_MAC_FLAG);

/** @} */ /* End of group ETH_Exported_Functions */

#endif /* #if defined (USE_K1986VE1xI) */

/** @} */ /* End of group ETHERNET */

/** @} */ /* End of group __MDR32FxQI_StdPeriph_Driver */

#ifdef __cplusplus
} // extern "C" block end
#endif

#endif /* __MDR32FxQI_ETH_H */

/*********************** (C) COPYRIGHT 2025 Milandr ****************************
*
* END OF FILE MDR32FxQI_eth.h */


