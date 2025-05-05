/**
 *******************************************************************************
 * @file    MDR32VF0xI_flash.c
 * @author  Milandr Application Team
 * @version V0.1.0
 * @date    16/04/2024
 * @brief   This file contains all the FLASH firmware functions.
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

/* Includes ------------------------------------------------------------------*/
#include "MDR32VF0xI_flash.h"
#include "MDR32VF0xI_utils.h"

/** @addtogroup MDR32VF0xI_StdPeriph_Driver MDR32VF0xI Standard Peripheral Driver
 * @{
 */

/** @addtogroup FLASH FLASH
 * @{
 */

/** @defgroup FLASH_Private_Types FLASH Private Types
 * @{
 */

#if defined(USE_MDR1206AFI) || defined(USE_MDR1206)
/**
 * @brief FLASH program size. Only used for MDR1206AFI (ID = 215).
 */
typedef enum {
    FLASH_PROGRAM_SIZE_1BYTE = (uint32_t)(0x01), /*!< Select 1 byte to programming. */
    FLASH_PROGRAM_SIZE_2BYTE = (uint32_t)(0x02), /*!< Select 2 bytes to programming. */
    FLASH_PROGRAM_SIZE_4BYTE = (uint32_t)(0x04)  /*!< Select 4 bytes to programming. */
} FLASH_ProgramSize_TypeDef;

#define IS_FLASH_PROGRAM_SIZE(SIZE) (((SIZE) == FLASH_PROGRAM_SIZE_1BYTE) || \
                                     ((SIZE) == FLASH_PROGRAM_SIZE_2BYTE) || \
                                     ((SIZE) == FLASH_PROGRAM_SIZE_4BYTE))
#endif

#if defined(USE_MDR32F02) || defined(USE_MDR1206FI) || defined(USE_MDR1206)
/**
 * @brief FLASH memory banks for mass erase.
 */
typedef enum {
    FLASH_ERASE_BANK_MAIN = ((uint32_t)0x00), /*!< Select the FLASH main bank to erase. */
    FLASH_ERASE_BANK_ALL  = ((uint32_t)0x01)  /*!< Select the FLASH main and information banks to erase. */
} FLASH_EraseMemoryBank_TypeDef;

#define IS_FLASH_ERASE_BANK(BANK) (((BANK) == FLASH_ERASE_BANK_MAIN) || \
                                   ((BANK) == FLASH_ERASE_BANK_ALL))
#endif

/** @} */ /* End of the group FLASH_Private_Types */

/** @defgroup FLASH_Private_Defines FLASH Private Defines
 * @{
 */

/**
 * @brief FLASH address.
 */
#define FLASH_ADDR_BYTE_MASK     ((uint32_t)0x3)
#define FLASH_ADDR_HALFWORD_MASK ((uint32_t)0x2)

#if defined(USE_MDR1206AFI) || defined(USE_MDR1206)
#define FLASH_ADDR_SECTOR_POS                   2
#define FLASH_ADDR_SECTOR_MASK                  ((uint32_t)0x1 << FLASH_ADDR_SECTOR_POS)
#define FLASH_GET_SECTOR_NUMBER(ADDR)           (((ADDR) & FLASH_ADDR_SECTOR_MASK) >> FLASH_ADDR_SECTOR_POS)

#define IS_FLASH_PROGRAM_START_BYTE(START_BYTE) (((START_BYTE) & ~FLASH_ADDR_BYTE_MASK) == 0)
#define IS_FLASH_PROGRAM_END_BYTE(END_BYTE)     ((END_BYTE) <= 4)
#endif

/**
 * @brief FLASH page address.
 */
#if defined(USE_MDR32F02)
#define GET_MAIN_PAGE_ADDR(PAGE) (((PAGE) << FLASH_ADR_MAIN_XADR_PAGE_Pos) & FLASH_ADR_MAIN_XADR_PAGE_Msk)
#define GET_INFO_PAGE_ADDR(PAGE) (((PAGE) << FLASH_ADR_INFO_XADR_PAGE_Pos) & FLASH_ADR_INFO_XADR_PAGE_Msk)
#elif defined(USE_MDR1206FI)
#define GET_MAIN_PAGE_ADDR(PAGE) (((PAGE) << FLASH_ADR_MAIN_XADR_PAGE_Pos) & (FLASH_ADR_MAIN_XADR_PAGE_Msk | FLASH_ADR_MAIN_BLOCK_Msk))
#define GET_INFO_PAGE_ADDR(PAGE) (((PAGE) << FLASH_ADR_INFO_XADR_PAGE_Pos) & (FLASH_ADR_INFO_XADR_PAGE_Msk | FLASH_ADR_INFO_BLOCK_Msk))
#elif defined(USE_MDR1206AFI)
#define GET_MAIN_PAGE_ADDR(PAGE) ((((PAGE) << FLASH_ADR_MAIN_PAGE_Pos) & FLASH_ADR_MAIN_PAGE_Msk) << 1)
#define GET_INFO_PAGE_ADDR(PAGE) ((((PAGE) << FLASH_ADR_INFO_PAGE_Pos) & FLASH_ADR_INFO_PAGE_Msk) << 1)
#elif defined(USE_MDR1206)
#define GET_MAIN_PAGE_ADDR_ID217(PAGE) (((PAGE) << FLASH_ADR_MAIN_XADR_PAGE_Pos) & (FLASH_ADR_MAIN_XADR_PAGE_Msk | FLASH_ADR_MAIN_BLOCK_Msk))
#define GET_INFO_PAGE_ADDR_ID217(PAGE) (((PAGE) << FLASH_ADR_INFO_XADR_PAGE_Pos) & (FLASH_ADR_INFO_XADR_PAGE_Msk | FLASH_ADR_INFO_BLOCK_Msk))
#define GET_MAIN_PAGE_ADDR_ID215(PAGE) ((((PAGE) << FLASH_ADR_MAIN_PAGE_Pos) & FLASH_ADR_MAIN_PAGE_Msk) << 1)
#define GET_INFO_PAGE_ADDR_ID215(PAGE) ((((PAGE) << FLASH_ADR_INFO_PAGE_Pos) & FLASH_ADR_INFO_PAGE_Msk) << 1)
#endif

/**
 * @brief FLASH page number.
 */
#if defined(USE_MDR32F02)
#define IS_FLASH_MAIN_PAGE_NUMBER(PAGE) (((PAGE) & ~(uint32_t)(FLASH_ADR_MAIN_XADR_PAGE_Msk >> FLASH_ADR_MAIN_XADR_PAGE_Pos)) == 0)
#define IS_FLASH_INFO_PAGE_NUMBER(PAGE) (((PAGE) & ~(uint32_t)(FLASH_ADR_INFO_XADR_PAGE_Msk >> FLASH_ADR_INFO_XADR_PAGE_Pos)) == 0)
#elif defined(USE_MDR1206FI)
#define GET_INFO_PAGE_NUMBER(ADDR)      (((ADDR) & (FLASH_ADR_INFO_XADR_PAGE_Msk | FLASH_ADR_INFO_BLOCK_Msk)) >> FLASH_ADR_INFO_XADR_PAGE_Pos)
#define IS_FLASH_MAIN_PAGE_NUMBER(PAGE) (((PAGE) & ~(uint32_t)((FLASH_ADR_MAIN_XADR_PAGE_Msk | FLASH_ADR_MAIN_BLOCK_Msk) >> FLASH_ADR_MAIN_XADR_PAGE_Pos)) == 0)
#define IS_FLASH_INFO_PAGE_NUMBER(PAGE) (((PAGE) & ~(uint32_t)((FLASH_ADR_INFO_XADR_PAGE_Msk | FLASH_ADR_INFO_BLOCK_Msk) >> FLASH_ADR_INFO_XADR_PAGE_Pos)) == 0)
#elif defined(USE_MDR1206AFI)
#define GET_INFO_PAGE_NUMBER(ADDR)      ((((ADDR) >> 1) & FLASH_ADR_INFO_PAGE_Msk) >> FLASH_ADR_INFO_PAGE_Pos)
#define IS_FLASH_MAIN_PAGE_NUMBER(PAGE) (((PAGE) & ~(uint32_t)(FLASH_ADR_MAIN_PAGE_Msk >> FLASH_ADR_MAIN_PAGE_Pos)) == 0)
#define IS_FLASH_INFO_PAGE_NUMBER(PAGE) ((((PAGE) & ~(uint32_t)(FLASH_ADR_INFO_PAGE_Msk >> FLASH_ADR_INFO_PAGE_Pos)) == 0) && ((PAGE) != 15))
#elif defined(USE_MDR1206)
#define GET_INFO_PAGE_NUMBER_ID217(ADDR)      (((ADDR) & (FLASH_ADR_INFO_XADR_PAGE_Msk | FLASH_ADR_INFO_BLOCK_Msk)) >> FLASH_ADR_INFO_XADR_PAGE_Pos)
#define GET_INFO_PAGE_NUMBER_ID215(ADDR)      ((((ADDR) >> 1) & FLASH_ADR_INFO_PAGE_Msk) >> FLASH_ADR_INFO_PAGE_Pos)
#define IS_FLASH_MAIN_PAGE_NUMBER_ID217(PAGE) (((PAGE) & ~(uint32_t)((FLASH_ADR_MAIN_XADR_PAGE_Msk | FLASH_ADR_MAIN_BLOCK_Msk) >> FLASH_ADR_MAIN_XADR_PAGE_Pos)) == 0)
#define IS_FLASH_INFO_PAGE_NUMBER_ID217(PAGE) (((PAGE) & ~(uint32_t)((FLASH_ADR_INFO_XADR_PAGE_Msk | FLASH_ADR_INFO_BLOCK_Msk) >> FLASH_ADR_INFO_XADR_PAGE_Pos)) == 0)
#define IS_FLASH_MAIN_PAGE_NUMBER_ID215(PAGE) (((PAGE) & ~(uint32_t)(FLASH_ADR_MAIN_PAGE_Msk >> FLASH_ADR_MAIN_PAGE_Pos)) == 0)
#define IS_FLASH_INFO_PAGE_NUMBER_ID215(PAGE) ((((PAGE) & ~(uint32_t)(FLASH_ADR_INFO_PAGE_Msk >> FLASH_ADR_INFO_PAGE_Pos)) == 0) && ((PAGE) != 15))
#endif

/**
 * @brief FLASH factory data.
 */
#if defined(USE_MDR1206FI) || defined(USE_MDR1206AFI) || defined(USE_MDR1206)
#if defined(USE_FLASH_FACTORY_DATA_PROTECTION)
#define FLASH_FACTORY_DATA_PAGE_NUMBER_ID217 (3)
#define FLASH_FACTORY_DATA_PAGE_NUMBER_ID215 (14)
#define FLASH_ADDR_INFO_WORD_MASK_ID217      (FLASH_ADR_INFO_BLOCK_Msk | FLASH_ADR_INFO_XADR_Msk | FLASH_ADR_YADR_Msk)
#define FLASH_ADDR_INFO_WORD_MASK_ID215      (((FLASH_ADR_INFO_PAGE_Msk | FLASH_ADR_ROW_Msk | FLASH_ADR_WORD_Msk) << 1) | FLASH_ADDR_SECTOR_MASK)
#if defined(USE_MDR1206FI)
#define FLASH_FACTORY_DATA_START_ADDR_ID217 (MDR_FLASH_SPECIAL_FIELDS_BASE & FLASH_ADDR_INFO_WORD_MASK_ID217)
#define FLASH_FACTORY_DATA_END_ADDR_ID217   ((MDR_FLASH_SPECIAL_FIELDS_BASE & FLASH_ADDR_INFO_WORD_MASK_ID217) + 0x1B)
#elif defined(USE_MDR1206AFI)
#define FLASH_FACTORY_DATA_START_ADDR_ID215 (MDR_FLASH_SPECIAL_FIELDS_BASE & FLASH_ADDR_INFO_WORD_MASK_ID215)
#define FLASH_FACTORY_DATA_END_ADDR_ID215   ((MDR_FLASH_SPECIAL_FIELDS_BASE & FLASH_ADDR_INFO_WORD_MASK_ID215) + 0x1B)
#elif defined(USE_MDR1206)
#define FLASH_FACTORY_DATA_START_ADDR_ID217 (MDR_FLASH_SPECIAL_FIELDS_BASE_ID217 & FLASH_ADDR_INFO_WORD_MASK_ID217)
#define FLASH_FACTORY_DATA_END_ADDR_ID217   ((MDR_FLASH_SPECIAL_FIELDS_BASE_ID217 & FLASH_ADDR_INFO_WORD_MASK_ID217) + 0x1B)
#define FLASH_FACTORY_DATA_START_ADDR_ID215 (MDR_FLASH_SPECIAL_FIELDS_BASE_ID215 & FLASH_ADDR_INFO_WORD_MASK_ID215)
#define FLASH_FACTORY_DATA_END_ADDR_ID215   ((MDR_FLASH_SPECIAL_FIELDS_BASE_ID215 & FLASH_ADDR_INFO_WORD_MASK_ID215) + 0x1B)
#endif
#endif
#endif

/** @} */ /* End of the group FLASH_Private_Defines */

/** @defgroup FLASH_Private_Functions_Declarations FLASH Private Functions Declarations
 * @{
 */

#if defined(USE_MDR1206AFI) || defined(USE_MDR1206)
__RAMFUNC static uint32_t FLASH_ConvertAddress(uint32_t Address, FLASH_MemoryBank_TypeDef Bank);
__RAMFUNC static void     FLASH_ProgramBytes(uint32_t Address, FLASH_MemoryBank_TypeDef Bank, uint32_t Data, FLASH_ProgramSize_TypeDef Size);
#endif

#if defined(USE_MDR32F02) || defined(USE_MDR1206FI)
__RAMFUNC static void FLASH_MassErase(FLASH_EraseMemoryBank_TypeDef Bank);
__RAMFUNC static void FLASH_ProgramCycle(uint32_t Command, uint32_t DelayProg);
#elif defined(USE_MDR1206AFI)
__RAMFUNC static void FLASH_MassErase(void);
__RAMFUNC static void FLASH_ProgramCycle(uint32_t StartByte, FLASH_ProgramSize_TypeDef Size, uint32_t Command, uint32_t DelayProg, uint32_t DelayAdsAdh);
#endif

#if defined(USE_MDR1206)
__RAMFUNC static uint32_t FLASH_ReadWord_ID217(uint32_t Address, FLASH_MemoryBank_TypeDef Bank);
__RAMFUNC static void     FLASH_ReadWordArrayBurst_ID217(const uint32_t* PtrAddressArray, uint32_t* PtrDataArray, uint32_t ArraySize, FLASH_MemoryBank_TypeDef Bank);
__RAMFUNC static void     FLASH_ErasePage_ID217(uint32_t Address, FLASH_MemoryBank_TypeDef Bank);
__RAMFUNC static void     FLASH_MassErase_ID217(FLASH_EraseMemoryBank_TypeDef Bank);
__RAMFUNC static void     FLASH_EraseAllPages_ID217(FLASH_MemoryBank_TypeDef Bank);
__RAMFUNC static void     FLASH_ProgramCycle_ID217(uint32_t Command, uint32_t DelayProg);
__RAMFUNC static void     FLASH_ProgramWord_ID217(uint32_t Address, FLASH_MemoryBank_TypeDef Bank, uint32_t Data);
__RAMFUNC static void     FLASH_ProgramHalfWord_ID217(uint32_t Address, FLASH_MemoryBank_TypeDef Bank, uint16_t Data);
__RAMFUNC static void     FLASH_ProgramByte_ID217(uint32_t Address, FLASH_MemoryBank_TypeDef Bank, uint8_t Data);
__RAMFUNC static void     FLASH_ProgramWordArrayBurst_ID217(const uint32_t* PtrAddressArray, const uint32_t* PtrDataArray, uint32_t ArraySize, FLASH_MemoryBank_TypeDef Bank);

__RAMFUNC static uint32_t FLASH_ReadWord_ID215(uint32_t Address, FLASH_MemoryBank_TypeDef Bank);
__RAMFUNC static void     FLASH_ReadWordArrayBurst_ID215(const uint32_t* PtrAddressArray, uint32_t* PtrDataArray, uint32_t ArraySize, FLASH_MemoryBank_TypeDef Bank);
__RAMFUNC static void     FLASH_ErasePage_ID215(uint32_t Address, FLASH_MemoryBank_TypeDef Bank);
__RAMFUNC static void     FLASH_MassErase_ID215(void);
__RAMFUNC static void     FLASH_EraseAllPages_ID215(FLASH_MemoryBank_TypeDef Bank);
__RAMFUNC static void     FLASH_ProgramCycle_ID215(uint32_t StartByte, FLASH_ProgramSize_TypeDef Size, uint32_t Command, uint32_t DelayProg, uint32_t DelayAdsAdh);
__RAMFUNC static void     FLASH_ProgramWord_ID215(uint32_t Address, FLASH_MemoryBank_TypeDef Bank, uint32_t Data);
__RAMFUNC static void     FLASH_ProgramHalfWord_ID215(uint32_t Address, FLASH_MemoryBank_TypeDef Bank, uint16_t Data);
__RAMFUNC static void     FLASH_ProgramByte_ID215(uint32_t Address, FLASH_MemoryBank_TypeDef Bank, uint8_t Data);
__RAMFUNC static void     FLASH_ProgramWordArrayBurst_ID215(const uint32_t* PtrAddressArray, const uint32_t* PtrDataArray, uint32_t ArraySize, FLASH_MemoryBank_TypeDef Bank);
#endif

#if defined(USE_MDR1206FI) || defined(USE_MDR1206AFI) || defined(USE_MDR1206)
#if defined(USE_FLASH_FACTORY_DATA_PROTECTION)
__RAMFUNC static uint32_t FLASH_GetInfoPageNumber(uint32_t PageAddress);
#endif
#endif

/** @} */ /* End of the group FLASH_Private_Functions_Declarations */

/** @addtogroup FLASH_Private_Functions FLASH Private Functions
 * @{
 */

/** @addtogroup FLASH_Functions_Not_Allowed_to_Be_Located_in_FLASH Functions Not Allowed to Be Located in the FLASH
 * @{
 */

/** @addtogroup FLASH_Register_Access_Functions FLASH Register Access Functions
 * @{
 */

#if defined(USE_MDR1206FI) || defined(USE_MDR1206AFI) || defined(USE_MDR1206)
#if defined(USE_FLASH_FACTORY_DATA_PROTECTION)
/**
 * @brief  Get the FLASH page number for the information bank.
 * @param  PageAddress: The FLASH page address.
 * @return The FLASH page number for the selected address.
 */
__RAMFUNC static uint32_t FLASH_GetInfoPageNumber(uint32_t PageAddress)
{
    uint32_t PageNumber;

#if defined(USE_MDR1206FI) || defined(USE_MDR1206AFI)
    PageNumber = GET_INFO_PAGE_NUMBER(PageAddress);
#elif defined(USE_MDR1206)
    if (GetChipID() == 217) {
        PageNumber = GET_INFO_PAGE_NUMBER_ID217(PageAddress);
    } else {
        PageNumber = GET_INFO_PAGE_NUMBER_ID215(PageAddress);
    }
#endif

    return PageNumber;
}
#endif
#endif

#if defined(USE_MDR32F02) || defined(USE_MDR1206FI) || defined(USE_MDR1206)
/**
 * @brief  Cycle for programming an 32-bit value.
 *         The initial programming procedure must first be completed.
 * @note   This function can be used only for MDR32F02 or MDR1206FI (ID = 217).
 * @param  Command: The command value, to which is added the set/clear of PROG2 bit.
 * @param  DelayProg: FLASH_TPROG_US delay time in loops.
 * @return None.
 */
#if defined(USE_MDR32F02) || defined(USE_MDR1206FI)
__RAMFUNC static void FLASH_ProgramCycle(uint32_t Command, uint32_t DelayProg)
#elif defined(USE_MDR1206)
__RAMFUNC static void FLASH_ProgramCycle_ID217(uint32_t Command, uint32_t DelayProg)
#endif
{
    Command |= FLASH_CMD_YE;
    MDR_FLASH->CMD = Command;
    DELAY_PROGRAM_WaitLoopsAsm(DelayProg);

    Command &= ~FLASH_CMD_YE;
    MDR_FLASH->CMD = Command;
}
#endif

#if defined(USE_MDR1206AFI) || defined(USE_MDR1206)
/**
 * @brief  Convert the FLASH memory word address to a special address required for register access.
 * @note   This function can be used only for MDR1206AFI (ID = 215).
 * @param  Address: A FLASH memory word address.
 * @param  Bank: @ref FLASH_MemoryBank_TypeDef - select a FLASH bank (the main bank or the information bank).
 *         This parameter can be one of the following values:
 *             @arg FLASH_BANK_MAIN: The FLASH main bank;
 *             @arg FLASH_BANK_INFO: The FLASH information bank.
 * @return Address required for register access.
 */
__RAMFUNC static uint32_t FLASH_ConvertAddress(uint32_t Address, FLASH_MemoryBank_TypeDef Bank)
{
    if (FLASH_GET_SECTOR_NUMBER(Address) == 0) {
        Address = (Address >> 1) & ((Bank == FLASH_BANK_INFO) ? ~FLASH_ADR_INFO_SECTOR_Msk : ~FLASH_ADR_MAIN_SECTOR_Msk);
    } else {
        Address = (Address >> 1) | ((Bank == FLASH_BANK_INFO) ? FLASH_ADR_INFO_SECTOR_Msk : FLASH_ADR_MAIN_SECTOR_Msk);
    }

    return Address;
}

/**
 * @brief  Cycle for programming an 8-bit values within a 32-bit word.
 *         The initial programming procedure must first be completed.
 * @note   This function can be used only for MDR1206AFI (ID = 215).
 * @param  StartByte: Byte from which programming begins. Must be from 0 to 3.
 * @param  Size: @ref FLASH_ProgramSize_TypeDef - select a number of bytes for programming. Should be StartByte + Size <= 4.
 * @param  Command: The command value, to which is added the set/clear of PROG2 bit.
 * @param  DelayProg: FLASH_TPROG_PROGRAM_US delay time in loops.
 * @param  DelayAdsAdh: FLASH_TADS_PROGRAM_US/FLASH_TADH_PROGRAM_US delay time in loops.
 * @return None.
 */
#if defined(USE_MDR1206AFI)
__RAMFUNC static void FLASH_ProgramCycle(uint32_t StartByte, FLASH_ProgramSize_TypeDef Size, uint32_t Command, uint32_t DelayProg, uint32_t DelayAdsAdh)
#elif defined(USE_MDR1206)
__RAMFUNC static void FLASH_ProgramCycle_ID215(uint32_t StartByte, FLASH_ProgramSize_TypeDef Size, uint32_t Command, uint32_t DelayProg, uint32_t DelayAdsAdh)
#endif
{
    uint32_t Byte;

    /* Check the parameters. */
    assert_param(IS_FLASH_PROGRAM_START_BYTE(StartByte));
    assert_param(IS_FLASH_PROGRAM_SIZE(Size));
    assert_param(IS_FLASH_PROGRAM_END_BYTE(StartByte + (uint32_t)Size));

    for (Byte = StartByte; Byte < StartByte + (uint32_t)Size; Byte++) {
        MDR_FLASH->CTRL = 1 << Byte;
        DELAY_PROGRAM_WaitLoopsAsm(DelayAdsAdh);

        Command |= FLASH_CMD_PROG2;
        MDR_FLASH->CMD = Command;
        DELAY_PROGRAM_WaitLoopsAsm(DelayProg);

        Command &= ~FLASH_CMD_PROG2;
        MDR_FLASH->CMD = Command;
        DELAY_PROGRAM_WaitLoopsAsm(DelayAdsAdh);
    }
}

/**
 * @brief   Program the specified number of 8-bit FLASH memory values.
 * @warning The same address (8-bit data) can't be programmed again before next erase.
 * @note    This function can be used only for MDR1206AFI (ID = 215).
 * @note    The core clock from SystemCoreClock is used to calculate the delay time.
 *          Before calling FLASH_ProgramByte(), you must update SystemCoreClock by calling SystemCoreClockUpdate().
 * @param   Address: A FLASH memory address.
 * @param   Bank: @ref FLASH_MemoryBank_TypeDef - select a FLASH bank (the main bank or the information bank).
 *                This parameter can be one of the following values:
 *                    @arg FLASH_BANK_MAIN: The FLASH main bank;
 *                    @arg FLASH_BANK_INFO: The FLASH information bank.
 * @param   Data: 32-bit data whose bytes will be program.
 * @param   Size: @ref FLASH_ProgramSize_TypeDef - select a number of bytes for programming.
 * @return  None.
 */
__RAMFUNC static void FLASH_ProgramBytes(uint32_t Address, FLASH_MemoryBank_TypeDef Bank, uint32_t Data, FLASH_ProgramSize_TypeDef Size)
{
    uint32_t Command;
    uint32_t DelayConst;
    uint32_t DelayProg;
    uint32_t DelayAdsAdh;
    uint32_t DelayPgs;
    uint32_t StartByte;

    /* Check the parameters. */
    assert_param(IS_FLASH_MEMORY_BANK(Bank));
    assert_param(IS_FLASH_PROGRAM_SIZE(Size));

#if defined(USE_FLASH_FACTORY_DATA_PROTECTION)
    if (Bank == FLASH_BANK_INFO) {
        if (((Address & FLASH_ADDR_INFO_WORD_MASK_ID215) >= FLASH_FACTORY_DATA_START_ADDR_ID215) &&
            ((Address & FLASH_ADDR_INFO_WORD_MASK_ID215) <= FLASH_FACTORY_DATA_END_ADDR_ID215)) {
            return;
        }
    }
#endif

    if (Size == FLASH_PROGRAM_SIZE_4BYTE) {
        Address &= ~FLASH_ADDR_FOUR_BYTE_ALIGN_MASK;
    } else if (Size == FLASH_PROGRAM_SIZE_2BYTE) {
        Address &= ~FLASH_ADDR_TWO_BYTE_ALIGN_MASK;
    }

    StartByte   = Address & FLASH_ADDR_BYTE_MASK;
    DelayConst  = DELAY_PROGRAM_GET_CONST(SystemCoreClock);
    DelayProg   = DELAY_PROGRAM_GET_LOOPS_US(FLASH_TPROG_PROGRAM_US, DelayConst);
    DelayAdsAdh = DELAY_PROGRAM_GET_LOOPS_US(FLASH_TADS_PROGRAM_US, DelayConst);
    DelayPgs    = DELAY_PROGRAM_GET_LOOPS_US(FLASH_TPGS_PROGRAM_US, DelayConst);

    Command = FLASH_CMD_CON | FLASH_CMD_TMEN;
    Command |= (Bank == FLASH_BANK_INFO) ? FLASH_CMD_NVR : 0;
    MDR_FLASH->CMD = Command;

    MDR_FLASH->ADR = FLASH_ConvertAddress(Address, Bank);
    MDR_FLASH->DI  = Data;

    Command |= FLASH_CMD_CE | FLASH_CMD_PROG;
    MDR_FLASH->CMD = Command;
    DELAY_PROGRAM_WaitLoopsAsm(DELAY_PROGRAM_GET_LOOPS_US(FLASH_TNVS_PROGRAM_US, DelayConst));

    Command |= FLASH_CMD_WE;
    MDR_FLASH->CMD = Command;
    DELAY_PROGRAM_WaitLoopsAsm(DelayPgs);

#if defined(USE_MDR1206AFI)
    FLASH_ProgramCycle(StartByte, Size, Command, DelayProg, DelayAdsAdh);
#elif defined(USE_MDR1206)
    FLASH_ProgramCycle_ID215(StartByte, Size, Command, DelayProg, DelayAdsAdh);
#endif

    Command &= ~FLASH_CMD_WE;
    MDR_FLASH->CMD = Command;
    DELAY_PROGRAM_WaitLoopsAsm(DELAY_PROGRAM_GET_LOOPS_US(FLASH_TRCV_PROGRAM_US, DelayConst));

    Command &= ~(FLASH_CMD_CE | FLASH_CMD_PROG);
    MDR_FLASH->CMD = Command;
    MDR_FLASH->CMD = FLASH_CMD_TMEN;

    DELAY_PROGRAM_WaitLoopsAsm(DELAY_PROGRAM_GET_LOOPS_US(FLASH_TRW_PROGRAM_US, DelayConst));
}
#endif

/** @} */ /* End of the group FLASH_Register_Access_Functions */

/** @} */ /* End of the group FLASH_Functions_Not_Allowed_to_Be_Located_in_FLASH */

/** @} */ /* End of the group FLASH_Private_Functions */

/** @addtogroup FLASH_Exported_Functions FLASH Exported Functions
 * @{
 */

/** @addtogroup FLASH_Functions_Allowed_to_Be_Located_in_FLASH Functions Allowed to Be Located in the FLASH
 * @{
 */

/** @addtogroup FLASH_Control_Functions FLASH Control Functions
 * @{
 */

/**
 * @brief  Lock/Unlock access to FLASH memory via registers (locked by default).
 * @param  LockState: @ref FlagStatus - a new lock state of the access to FLASH memory via registers.
 *         This parameter must be one of the @ref FlagStatus values:
 *             @arg RESET: Unlock the access;
 *             @arg SET: Lock the access.
 * @return None.
 */
void FLASH_SetRegistersLock(FlagStatus LockState)
{
    /* Check the parameters. */
    assert_param(IS_BIT_STATUS(LockState));

    if (LockState != RESET) {
        /* Lock access if SET. */
        MDR_FLASH->KEY = FLASH_KEY_KEY_LOCK;
    } else {
        /* Unlock access if RESET. */
        MDR_FLASH->KEY = FLASH_KEY_KEY_UNLOCK;
    }
}

/**
 * @brief  Get the current state of access to FLASH memory via registers (locked by default).
 * @param  None.
 * @return @ref FlagStatus - lock state:
 *             @arg RESET: Unlocked;
 *             @arg SET: Locked.
 */
FlagStatus FLASH_GetRegistersLock(void)
{
    if (MDR_FLASH->KEY == FLASH_KEY_KEY_UNLOCK) {
        /* Access to FLASH memory via registers is unlocked. */
        return RESET;
    } else {
        /* Access to FLASH memory via registers is locked. */
        return SET;
    }
}

/** @} */ /* End of the group FLASH_Control_Functions */

/** @addtogroup FLASH_Special_Fields_Control_Functions FLASH Special Fields Control Functions
 * @{
 */

#if defined(USE_MDR1206FI) || defined(USE_MDR1206AFI) || defined(USE_MDR1206)
/**
 * @brief   Program protection bits for selected FLASH memory regions.
 * @warning After programming the JTAG interface is blocked.
 * @param   ProtectionRegions: Protection regions.
 *          This parameter must be any combination of @ref FLASH_ProtectionRegions_TypeDef values.
 * @return  None.
 */
void FLASH_ProgramProtection(uint32_t ProtectionRegions)
{
    uint32_t Protection;
    uint32_t ProtectionAddr;

    /* Check the parameters. */
    assert_param(IS_FLASH_PROTECTION_REGIONS(ProtectionRegions));

#if defined(USE_MDR1206FI) || defined(USE_MDR1206AFI)
    ProtectionAddr = (uint32_t)&MDR_FLASH_SPECIAL_FIELDS->PROTECTION;
#elif defined(USE_MDR1206)
    if (GetChipID() == 217) {
        ProtectionAddr = (uint32_t)&MDR_FLASH_SPECIAL_FIELDS_ID217->PROTECTION;
    } else {
        ProtectionAddr = (uint32_t)&MDR_FLASH_SPECIAL_FIELDS_ID215->PROTECTION;
    }
#endif

    Protection = FLASH_SPECIAL_FIELDS_PROTECTION_KEY_UNLOCK | (~ProtectionRegions & FLASH_SPECIAL_FIELDS_PROTECTION_REGIONS_Msk);
    FLASH_ProgramWord(ProtectionAddr, FLASH_BANK_INFO, Protection);
}

/**
 * @brief  Read protection state of the FLASH memory regions.
 * @param  None.
 * @return A combined value of the individual protection states (@ref FLASH_ProtectionRegions_TypeDef).
 */
uint32_t FLASH_ReadProtection(void)
{
    uint32_t Protection;

#if defined(USE_MDR1206FI) || defined(USE_MDR1206AFI)
    Protection = MDR_FLASH_SPECIAL_FIELDS->PROTECTION;
#elif defined(USE_MDR1206)
    if (GetChipID() == 217) {
        Protection = MDR_FLASH_SPECIAL_FIELDS_ID217->PROTECTION;
    } else {
        Protection = MDR_FLASH_SPECIAL_FIELDS_ID215->PROTECTION;
    }
#endif

    if ((Protection & FLASH_SPECIAL_FIELDS_PROTECTION_KEY_Msk) == FLASH_SPECIAL_FIELDS_PROTECTION_KEY_UNLOCK) {
        Protection = ~Protection & FLASH_SPECIAL_FIELDS_PROTECTION_REGIONS_Msk;
    } else {
        Protection = 0;
    }

    return Protection;
}

/**
 * @brief  Program factory data (UNIQUE_ID, TRIMs) and checksum (TRIM_CS) into FLASH memory.
 * @note   If USE_FLASH_FACTORY_DATA_PROTECTION is defined, then factory data programming is not performed.
 * @param  FactoryData: The pointer to the @ref FLASH_FactoryData_TypeDef structure
 *         that contains identification information and trim values.
 * @return None.
 */
void FLASH_ProgramFactoryData(const FLASH_FactoryData_TypeDef* FactoryData)
{
    uint32_t                         TrimWord;
    uint8_t                          TrimCS;
    uint32_t                         Index;
    uint8_t*                         PtrFactoryData;
    MDR_FLASH_SpecialFields_TypeDef* FlashFactoryData;

#if defined(USE_MDR1206FI) || defined(USE_MDR1206AFI)
    FlashFactoryData = MDR_FLASH_SPECIAL_FIELDS;
#elif defined(USE_MDR1206)
    if (GetChipID() == 217) {
        FlashFactoryData = MDR_FLASH_SPECIAL_FIELDS_ID217;
    } else {
        FlashFactoryData = MDR_FLASH_SPECIAL_FIELDS_ID215;
    }
#endif
    TrimWord = *((uint32_t*)&FactoryData->TrimHSI);

    /* Checksum calculation. */
    PtrFactoryData = (uint8_t*)FactoryData;
    for (TrimCS = 0, Index = 0; Index < 24; Index++) {
        TrimCS += PtrFactoryData[Index];
    }

    /* Program UNIQUE_ID, TRIMs and checksum (TRIM_CS) into FLASH memory. */
    for (Index = 0; Index < 5; Index++) {
        FLASH_ProgramWord((uint32_t)&FlashFactoryData->UNIQUE_ID[Index], FLASH_BANK_INFO, FactoryData->UniqueID[Index]);
    }
    FLASH_ProgramWord((uint32_t)&FlashFactoryData->TRIM_HSI, FLASH_BANK_INFO, TrimWord);
    FLASH_ProgramByte((uint32_t)&FlashFactoryData->TRIM_CS, FLASH_BANK_INFO, TrimCS);
}

/**
 * @brief  Read factory data (UNIQUE_ID, TRIMs) from FLASH memory and verify the checksum (TRIM_CS).
 * @param  FactoryData: The pointer to the @ref FLASH_FactoryData_TypeDef structure
 *         where identification information and trim values should be stored.
 * @return @ref ErrorStatus - checksum verification result.
 */
ErrorStatus FLASH_ReadFactoryData(FLASH_FactoryData_TypeDef* FactoryData)
{
    uint8_t                          ReadTrimCS;
    uint8_t                          CalcTrimCS;
    uint32_t                         Index;
    uint8_t*                         PtrFactoryData;
    MDR_FLASH_SpecialFields_TypeDef* FlashFactoryData;

#if defined(USE_MDR1206FI) || defined(USE_MDR1206AFI)
    FlashFactoryData = MDR_FLASH_SPECIAL_FIELDS;
#elif defined(USE_MDR1206)
    if (GetChipID() == 217) {
        FlashFactoryData = MDR_FLASH_SPECIAL_FIELDS_ID217;
    } else {
        FlashFactoryData = MDR_FLASH_SPECIAL_FIELDS_ID215;
    }
#endif

    /* Read UNIQUE_ID, TRIMs and checksum (TRIM_CS) from FLASH memory. */
    for (Index = 0; Index < 5; Index++) {
        FactoryData->UniqueID[Index] = FlashFactoryData->UNIQUE_ID[Index];
    }
    *((uint32_t*)&FactoryData->TrimHSI) = *((uint32_t*)&FlashFactoryData->TRIM_HSI);
    ReadTrimCS                          = FlashFactoryData->TRIM_CS;

    /* Checksum calculation. */
    PtrFactoryData = (uint8_t*)FactoryData;
    for (CalcTrimCS = 0, Index = 0; Index < 24; Index++) {
        CalcTrimCS += PtrFactoryData[Index];
    }

    /* Comparison of checksums. */
    if (ReadTrimCS != CalcTrimCS)
        return ERROR;
    else
        return SUCCESS;
}
#endif

/** @} */ /* End of the group FLASH_Special_Fields_Control_Functions */

#if defined(USE_MDR1206FI) || defined(USE_MDR1206AFI) || defined(USE_MDR1206)
/** @addtogroup FLASH_JTAG_Locking_Functions FLASH JTAG Locking Functions
 * @{
 */

/**
 * @brief  Lock/Unlock write access to the CHIP_ID_CTRL register that performs JTAG blocking.
 *         The CHIP_ID_CTRL register are locked by default.
 * @param  LockState: @ref FlagStatus - a new lock state of the CHIP_ID_CTRL register.
 *         This parameter must be one of the @ref FlagStatus values:
 *             @arg RESET: Unlock the write access;
 *             @arg SET: Lock the write access.
 * @return None.
 */
void FLASH_JTAG_SetRegistersLock(FlagStatus LockState)
{
    /* Check the parameters. */
    assert_param(IS_BIT_STATUS(LockState));

    if (LockState != RESET) {
        /* Lock write access if SET. */
        MDR_FLASH->OTP_KEY = FLASH_OTP_KEY_OTP_KEY_LOCK;
    } else {
        /* Unlock write access if RESET. */
        MDR_FLASH->OTP_KEY = FLASH_OTP_KEY_OTP_KEY_UNLOCK;
    }
}

/**
 * @brief  Get the current CHIP_ID_CTRL register write access lock state.
 *         The CHIP_ID_CTRL register are locked by default.
 * @param  None.
 * @return @ref FlagStatus - write lock state:
 *             @arg RESET: Unlocked;
 *             @arg SET: Locked.
 */
FlagStatus FLASH_JTAG_GetRegistersLock(void)
{
    if (MDR_FLASH->OTP_KEY == FLASH_OTP_KEY_OTP_KEY_UNLOCK) {
        /* CHIP_ID_CTRL write access is unlocked. */
        return RESET;
    } else {
        /* CHIP_ID_CTRL write access is locked. */
        return SET;
    }
}

/**
 * @brief   Lock the JTAG interface permanently by programming the OTP memory.
 * @warning After programming the OTP memory, the JTAG interface will be locked forever.
 * @note    The core clock from SystemCoreClock is used to calculate the delay time.
 *          Before calling FLASH_ErasePage(), you must update SystemCoreClock by calling SystemCoreClockUpdate().
 * @param   None.
 * @return  None.
 */
void FLASH_JTAG_SetLockForever(void)
{
    uint32_t Iteration;
    uint32_t OTPProg;
    uint32_t DelayConst;

    OTPProg    = FLASH_CHIP_ID_CTRL_OTP_PROG;
    DelayConst = DELAY_PROGRAM_GET_CONST(SystemCoreClock);

    MDR_FLASH->CHIP_ID_CTRL = OTPProg;
    for (Iteration = 0; Iteration < 5; Iteration++) {
        DELAY_PROGRAM_WaitLoopsAsm(DELAY_PROGRAM_GET_LOOPS_MS(1, DelayConst));
        OTPProg ^= FLASH_CHIP_ID_CTRL_OTP_PROG;
        MDR_FLASH->CHIP_ID_CTRL = OTPProg;
    }
}

/** @} */ /* End of the group FLASH_JTAG_Locking_Functions */
#endif

/** @} */ /* End of the group FLASH_Functions_Allowed_to_Be_Located_in_FLASH */

/** @addtogroup FLASH_Functions_Not_Allowed_to_Be_Located_in_FLASH Functions Not Allowed to Be Located in the FLASH
 * @{
 */

#if defined(USE_MDR32F02) || defined(USE_MDR1206FI) || defined(USE_MDR1206)
/** @addtogroup FLASH_Control_Functions FLASH Control Functions
 * @{
 */

/**
 * @brief  Set a FLASH latency value. Unlocking FLASH is not required before calling this function.
 * @param  Latency: @ref FLASH_Latency_TypeDef - specify a FLASH latency value in cycles.
 * @return None.
 */
__RAMFUNC void FLASH_SetLatency(FLASH_Latency_TypeDef Latency)
{
    /* Check the parameters. */
    assert_param(IS_FLASH_LATENCY(Latency));

    /* Set the new latency value. */
#if defined(USE_MDR32F02) || defined(USE_MDR1206FI)
    MDR_FLASH->CMD = (uint32_t)Latency | FLASH_CMD_TMR;
#elif defined(USE_MDR1206)
    if (GetChipID() == 217) {
        MDR_FLASH->CMD = (uint32_t)Latency | FLASH_CMD_TMR;
    }
#endif
#if defined(USE_MDR32F02)
    *((volatile uint32_t*)FLASH_BASE); /* Errata 0011 workaround. */
#endif
}

/**
 * @brief  Get a FLASH latency value. Unlocking FLASH is not required before calling this function.
 * @param  None.
 * @return @ref FLASH_Latency_TypeDef - a FLASH latency value in cycles.
 */
__RAMFUNC FLASH_Latency_TypeDef FLASH_GetLatency(void)
{
#if defined(USE_MDR1206)
    FLASH_Latency_TypeDef Latency = FLASH_LATENCY_CYCLE_1;

    if (GetChipID() == 217) {
        Latency = (FLASH_Latency_TypeDef)(MDR_FLASH->CMD & FLASH_CMD_DELAY_Msk);
    }
#endif
    /* Return the current latency value. */
#if defined(USE_MDR32F02) || defined(USE_MDR1206FI)
    return (FLASH_Latency_TypeDef)(MDR_FLASH->CMD & FLASH_CMD_DELAY_Msk);
#elif defined(USE_MDR1206)
    return Latency;
#endif
}

/** @} */ /* End of the group FLASH_Control_Functions */
#endif

/** @addtogroup FLASH_Register_Access_Functions FLASH Register Access Functions
 * @{
 */

/**
 * @brief  Get the FLASH page address (can be used for FLASH_ErasePage()).
 * @param  PageNumber: The FLASH page number.
 *         This parameter is a value:
 *         - MDR32F02: main bank - from 0 to 63, information bank - from 0 to 1;
 *         - MDR1206FI: main bank - from 0 to 127, information bank - from 0 to 3;
 *         - MDR1206AFI: main bank - from 0 to 511, information bank - from 0 to 14.
 * @param  Bank: @ref FLASH_MemoryBank_TypeDef - select a FLASH bank (the main bank or the information bank).
 *         This parameter can be one of the following values:
 *             @arg FLASH_BANK_MAIN: The FLASH main bank;
 *             @arg FLASH_BANK_INFO: The FLASH information bank.
 * @return The address of selected FLASH page.
 */
__RAMFUNC uint32_t FLASH_GetPageAddress(uint32_t PageNumber, FLASH_MemoryBank_TypeDef Bank)
{
    uint32_t PageAddress;

    /* Check the parameters. */
    assert_param(IS_FLASH_MEMORY_BANK(Bank));
#if defined(USE_MDR32F02) || defined(USE_MDR1206FI) || defined(USE_MDR1206AFI)
    assert_param((Bank == FLASH_BANK_MAIN) ? IS_FLASH_MAIN_PAGE_NUMBER(PageNumber) : IS_FLASH_INFO_PAGE_NUMBER(PageNumber));
#elif defined(USE_MDR1206)
    if (GetChipID() == 217) {
        assert_param((Bank == FLASH_BANK_MAIN) ? IS_FLASH_MAIN_PAGE_NUMBER_ID217(PageNumber) : IS_FLASH_INFO_PAGE_NUMBER_ID217(PageNumber));
    } else {
        assert_param((Bank == FLASH_BANK_MAIN) ? IS_FLASH_MAIN_PAGE_NUMBER_ID215(PageNumber) : IS_FLASH_INFO_PAGE_NUMBER_ID215(PageNumber));
    }
#endif

#if defined(USE_MDR32F02) || defined(USE_MDR1206FI) || defined(USE_MDR1206AFI)
    if (Bank == FLASH_BANK_MAIN) {
        PageAddress = GET_MAIN_PAGE_ADDR(PageNumber);
    } else {
        PageAddress = GET_INFO_PAGE_ADDR(PageNumber);
    }
#elif defined(USE_MDR1206)
    if (GetChipID() == 217) {
        if (Bank == FLASH_BANK_MAIN) {
            PageAddress = GET_MAIN_PAGE_ADDR_ID217(PageNumber);
        } else {
            PageAddress = GET_INFO_PAGE_ADDR_ID217(PageNumber);
        }
    } else {
        if (Bank == FLASH_BANK_MAIN) {
            PageAddress = GET_MAIN_PAGE_ADDR_ID215(PageNumber);
        } else {
            PageAddress = GET_INFO_PAGE_ADDR_ID215(PageNumber);
        }
    }
#endif

    return PageAddress;
}

/**
 * @brief  Read a 16-bit FLASH memory value.
 * @note   Hardware supports only 32-bit word reading.
 * @param  Address: The FLASH memory half word address (two bytes aligned).
 * @param  Bank: @ref FLASH_MemoryBank_TypeDef - select a FLASH bank (the main bank or the information bank).
 *         This parameter can be one of the following values:
 *             @arg FLASH_BANK_MAIN: The FLASH main bank;
 *             @arg FLASH_BANK_INFO: The FLASH information bank.
 * @return The selected FLASH memory halfword.
 */
__RAMFUNC uint16_t FLASH_ReadHalfWord(uint32_t Address, FLASH_MemoryBank_TypeDef Bank)
{
    uint32_t Data;
    uint32_t Shift;

    /* Check the parameters. */
    assert_param(IS_FLASH_MEMORY_BANK(Bank));
    assert_param(IS_FLASH_ADDR_TWO_BYTE_ALIGNED(Address));

    Data = FLASH_ReadWord(Address & ~FLASH_ADDR_BYTE_MASK, Bank);

    Shift = (Address & FLASH_ADDR_HALFWORD_MASK) * 8;
    Data >>= Shift;

    return (uint16_t)Data;
}

/**
 * @brief  Read an 8-bit FLASH memory value.
 * @note   Hardware supports only 32-bit word reading.
 * @param  Address: The FLASH memory byte address.
 * @param  Bank: @ref FLASH_MemoryBank_TypeDef - select a FLASH bank (the main bank or the information bank).
 *         This parameter can be one of the following values:
 *             @arg FLASH_BANK_MAIN: The main FLASH;
 *             @arg FLASH_BANK_INFO: The FLASH information bank.
 * @return The selected FLASH memory byte.
 */
__RAMFUNC uint8_t FLASH_ReadByte(uint32_t Address, FLASH_MemoryBank_TypeDef Bank)
{
    uint32_t Data;
    uint32_t Shift;

    /* Check the parameters. */
    assert_param(IS_FLASH_MEMORY_BANK(Bank));

    Data = FLASH_ReadWord(Address & ~FLASH_ADDR_BYTE_MASK, Bank);

    Shift = (Address & FLASH_ADDR_BYTE_MASK) * 8;
    Data >>= Shift;

    return (uint8_t)Data;
}

#if defined(USE_MDR32F02) || defined(USE_MDR1206FI) || defined(USE_MDR1206)
/**
 * @brief  Read a 32-bit FLASH memory value.
 * @note   This function can be used only for MDR32F02 or MDR1206FI (ID = 217).
 * @param  Address: A FLASH memory word address (four bytes aligned).
 * @param  Bank: @ref FLASH_MemoryBank_TypeDef - select a FLASH bank (the main bank or the information bank).
 *         This parameter can be one of the following values:
 *             @arg FLASH_BANK_MAIN: The FLASH main bank;
 *             @arg FLASH_BANK_INFO: The FLASH information bank.
 * @return The selected FLASH memory word.
 */
#if defined(USE_MDR32F02) || defined(USE_MDR1206FI)
__RAMFUNC uint32_t FLASH_ReadWord(uint32_t Address, FLASH_MemoryBank_TypeDef Bank)
#elif defined(USE_MDR1206)
__RAMFUNC static uint32_t FLASH_ReadWord_ID217(uint32_t Address, FLASH_MemoryBank_TypeDef Bank)
#endif
{
    uint32_t Command;
    uint32_t Data;

    /* Check the parameters. */
    assert_param(IS_FLASH_MEMORY_BANK(Bank));
    assert_param(IS_FLASH_ADDR_FOUR_BYTE_ALIGNED(Address));

    Command = (MDR_FLASH->CMD & FLASH_CMD_DELAY_Msk) | FLASH_CMD_CON | FLASH_CMD_TMR;
    Command |= (Bank == FLASH_BANK_INFO) ? FLASH_CMD_IFREN : 0;
    MDR_FLASH->CMD = Command;
    MDR_FLASH->ADR = Address;
    MDR_FLASH->CMD = Command | FLASH_CMD_XE | FLASH_CMD_YE;
    MDR_FLASH->CMD = Command | FLASH_CMD_XE | FLASH_CMD_YE | FLASH_CMD_SE;
    MDR_FLASH->DO; /* Idle reading for delay. */
    MDR_FLASH->DO; /* Idle reading for delay. */
    Data           = MDR_FLASH->DO;
    Command        = (Command & FLASH_CMD_DELAY_Msk) | FLASH_CMD_TMR;
    MDR_FLASH->CMD = Command;

#if defined(USE_MDR32F02)
    Command = *((volatile uint32_t*)FLASH_BASE); /* Errata 0011 workaround. */
#endif

    return Data;
}

/**
 * @brief  Read the array of the 32-bit FLASH memory values in the packet mode.
 * @note   This function can be used only for MDR32F02 and MDR1206FI (ID = 217).
 * @param  PtrAddressArray: The pointer to the array with FLASH memory words addresses to read (four bytes aligned).
 * @param  PtrDataArray: The pointer to the array where the words read from the FLASH MEMORY will be stored.
 * @param  ArraySize: The size of address and data arrays.
 * @param  Bank: @ref FLASH_MemoryBank_TypeDef - select a FLASH bank (the main bank or the information bank).
 *         This parameter can be one of the following values:
 *             @arg FLASH_BANK_MAIN: The FLASH main bank;
 *             @arg FLASH_BANK_INFO: The FLASH information bank.
 * @return None.
 */
#if defined(USE_MDR32F02) || defined(USE_MDR1206FI)
__RAMFUNC void FLASH_ReadWordArrayBurst(const uint32_t* PtrAddressArray, uint32_t* PtrDataArray, uint32_t ArraySize, FLASH_MemoryBank_TypeDef Bank)
#elif defined(USE_MDR1206)
__RAMFUNC static void FLASH_ReadWordArrayBurst_ID217(const uint32_t* PtrAddressArray, uint32_t* PtrDataArray, uint32_t ArraySize, FLASH_MemoryBank_TypeDef Bank)
#endif
{
    uint32_t Command;
    uint32_t Index;

    /* Check the parameters. */
    assert_param(IS_FLASH_MEMORY_BANK(Bank));
    assert_param(ArraySize > 0);
#if (USE_ASSERT_INFO != 0)
    for (Index = 0; Index < ArraySize; Index++) {
        assert_param(IS_FLASH_ADDR_FOUR_BYTE_ALIGNED(PtrAddressArray[Index]));
    }
#endif

    Command = (MDR_FLASH->CMD & FLASH_CMD_DELAY_Msk) | FLASH_CMD_CON | FLASH_CMD_TMR;
    Command |= (Bank == FLASH_BANK_INFO) ? FLASH_CMD_IFREN : 0;
    MDR_FLASH->CMD = Command;

    for (Index = 0; Index < ArraySize; Index++) {
        MDR_FLASH->CMD = Command | FLASH_CMD_XE | FLASH_CMD_YE;
        MDR_FLASH->ADR = PtrAddressArray[Index];
        MDR_FLASH->CMD = Command | FLASH_CMD_XE | FLASH_CMD_YE | FLASH_CMD_SE;
        MDR_FLASH->DO; /* Idle reading for delay. */
        MDR_FLASH->DO; /* Idle reading for delay. */
        PtrDataArray[Index] = MDR_FLASH->DO;
    }
    MDR_FLASH->CMD = (Command & FLASH_CMD_DELAY_Msk) | FLASH_CMD_TMR;

#if defined(USE_MDR32F02)
    Index = *((volatile uint32_t*)FLASH_BASE); /* Errata 0011 workaround. */
#endif
}

/**
 * @brief  Erase one page (4 KByte) of a selected FLASH memory bank.
 * @note   This function can be used only for MDR32F02 and MDR1206FI (ID = 217).
 * @note   The core clock from SystemCoreClock is used to calculate the delay time.
 *         Before calling FLASH_ErasePage(), you must update SystemCoreClock by calling SystemCoreClockUpdate().
 * @param  Address: Page address in the FLASH memory.
 * @param  Bank: @ref FLASH_MemoryBank_TypeDef - select a FLASH bank (the main bank or the information bank).
 *         This parameter can be one of the following values:
 *             @arg FLASH_BANK_MAIN: The FLASH main bank;
 *             @arg FLASH_BANK_INFO: The FLASH information bank.
 * @return None.
 */
#if defined(USE_MDR32F02) || defined(USE_MDR1206FI)
__RAMFUNC void FLASH_ErasePage(uint32_t Address, FLASH_MemoryBank_TypeDef Bank)
#elif defined(USE_MDR1206)
__RAMFUNC static void FLASH_ErasePage_ID217(uint32_t Address, FLASH_MemoryBank_TypeDef Bank)
#endif
{
    uint32_t Command;
    uint32_t DelayConst;

    /* Check the parameters. */
    assert_param(IS_FLASH_MEMORY_BANK(Bank));

#if defined(USE_MDR1206FI) || defined(USE_MDR1206)
#if defined(USE_FLASH_FACTORY_DATA_PROTECTION)
    if (Bank == FLASH_BANK_INFO) {
        if (FLASH_GetInfoPageNumber(Address) == FLASH_FACTORY_DATA_PAGE_NUMBER_ID217) {
            return;
        }
    }
#endif
#endif

    DelayConst = DELAY_PROGRAM_GET_CONST(SystemCoreClock);

    Command = (MDR_FLASH->CMD & FLASH_CMD_DELAY_Msk) | FLASH_CMD_CON | FLASH_CMD_TMR;
    Command |= (Bank == FLASH_BANK_INFO) ? FLASH_CMD_IFREN : 0;
    MDR_FLASH->CMD = Command;

    MDR_FLASH->ADR = Address;
    Command |= FLASH_CMD_XE | FLASH_CMD_ERASE;
    MDR_FLASH->CMD = Command;
    DELAY_PROGRAM_WaitLoopsAsm(DELAY_PROGRAM_GET_LOOPS_US(FLASH_TNVS_US, DelayConst));

    Command |= FLASH_CMD_NVSTR;
    MDR_FLASH->CMD = Command;
    DELAY_PROGRAM_WaitLoopsAsm(DELAY_PROGRAM_GET_LOOPS_US(FLASH_TERASE_US, DelayConst));

    Command &= ~FLASH_CMD_ERASE;
    MDR_FLASH->CMD = Command;
    DELAY_PROGRAM_WaitLoopsAsm(DELAY_PROGRAM_GET_LOOPS_US(FLASH_TNVH_US, DelayConst));

    Command &= ~(FLASH_CMD_XE | FLASH_CMD_NVSTR);
    MDR_FLASH->CMD = Command;
    MDR_FLASH->CMD = (Command & FLASH_CMD_DELAY_Msk) | FLASH_CMD_TMR;
}

/**
 * @brief  Erase the main bank or the main and the information banks.
 * @note   This function can be used only for MDR32F02 and MDR1206FI (ID = 217).
 * @param  Bank: @ref FLASH_EraseMemoryBank_TypeDef - select a FLASH bank to erase.
 *         This parameter can be one of the following values:
 *             @arg FLASH_ERASE_BANK_MAIN: The FLASH main bank;
 *             @arg FLASH_ERASE_BANK_ALL:  The FLASH main and information banks.
 * @return None.
 */
#if defined(USE_MDR32F02) || defined(USE_MDR1206FI)
__RAMFUNC static void FLASH_MassErase(FLASH_EraseMemoryBank_TypeDef Bank)
#elif defined(USE_MDR1206)
__RAMFUNC static void FLASH_MassErase_ID217(FLASH_EraseMemoryBank_TypeDef Bank)
#endif
{
    uint32_t Command;
    uint32_t DelayConst;
#if defined(USE_MDR1206FI) || defined(USE_MDR1206)
    uint32_t Block;
#endif

    /* Check the parameters. */
    assert_param(IS_FLASH_ERASE_BANK(Bank));

    DelayConst = DELAY_PROGRAM_GET_CONST(SystemCoreClock);

    Command = (MDR_FLASH->CMD & FLASH_CMD_DELAY_Msk) | FLASH_CMD_CON | FLASH_CMD_TMR;
    Command |= (Bank == FLASH_ERASE_BANK_ALL) ? FLASH_CMD_IFREN : 0;
    MDR_FLASH->CMD = Command;

#if defined(USE_MDR1206FI) || defined(USE_MDR1206)
    for (Block = 0; Block < 2; Block++) {
        MDR_FLASH->ADR = (Block << FLASH_ADR_MAIN_BLOCK_Pos) | (Block << FLASH_ADR_INFO_BLOCK_Pos);
#endif
        Command |= FLASH_CMD_XE | FLASH_CMD_MAS1 | FLASH_CMD_ERASE;
        MDR_FLASH->CMD = Command;
        DELAY_PROGRAM_WaitLoopsAsm(DELAY_PROGRAM_GET_LOOPS_US(FLASH_TNVS_US, DelayConst));

        Command |= FLASH_CMD_NVSTR;
        MDR_FLASH->CMD = Command;
        DELAY_PROGRAM_WaitLoopsAsm(DELAY_PROGRAM_GET_LOOPS_US(FLASH_TME_US, DelayConst));

        Command &= ~FLASH_CMD_ERASE;
        MDR_FLASH->CMD = Command;
        DELAY_PROGRAM_WaitLoopsAsm(DELAY_PROGRAM_GET_LOOPS_US(FLASH_TNVH1_US, DelayConst));

        Command &= ~(FLASH_CMD_XE | FLASH_CMD_MAS1 | FLASH_CMD_NVSTR);
        MDR_FLASH->CMD = Command;
#if defined(USE_MDR1206FI) || defined(USE_MDR1206)
    }
#endif
    MDR_FLASH->CMD = (Command & FLASH_CMD_DELAY_Msk) | FLASH_CMD_TMR;
}

/**
 * @brief  Erase all pages of a selected FLASH memory bank.
 * @note   This function can be used only for MDR32F02 and MDR1206FI (ID = 217).
 * @param  Bank: @ref FLASH_MemoryBank_TypeDef - select a FLASH bank.
 *         This parameter can be one of the following values:
 *             @arg FLASH_BANK_MAIN: The FLASH main bank;
 *             @arg FLASH_BANK_INFO: The FLASH information bank.
 * @return None.
 */
#if defined(USE_MDR32F02) || defined(USE_MDR1206FI)
__RAMFUNC void FLASH_EraseAllPages(FLASH_MemoryBank_TypeDef Bank)
#elif defined(USE_MDR1206)
__RAMFUNC static void FLASH_EraseAllPages_ID217(FLASH_MemoryBank_TypeDef Bank)
#endif
{
    uint32_t Page;
    uint32_t NumOfPages;

    /* Check the parameters. */
    assert_param(IS_FLASH_MEMORY_BANK(Bank));

#if defined(USE_MDR32F02)
    NumOfPages = 2;
#elif defined(USE_MDR1206FI) || defined(USE_MDR1206)
    NumOfPages = 4;
#endif

#if defined(USE_MDR32F02) || defined(USE_MDR1206FI)
    if (Bank == FLASH_BANK_MAIN) {
        FLASH_MassErase(FLASH_ERASE_BANK_MAIN);
    } else if (Bank == FLASH_BANK_INFO) {
        for (Page = 0; Page < NumOfPages; Page++) {
            FLASH_ErasePage(Page << FLASH_ADR_INFO_XADR_PAGE_Pos, FLASH_BANK_INFO);
        }
    }
#elif defined(USE_MDR1206)
    if (Bank == FLASH_BANK_MAIN) {
        FLASH_MassErase_ID217(FLASH_ERASE_BANK_MAIN);
    } else if (Bank == FLASH_BANK_INFO) {
        for (Page = 0; Page < NumOfPages; Page++) {
            FLASH_ErasePage_ID217(Page << FLASH_ADR_INFO_XADR_PAGE_Pos, FLASH_BANK_INFO);
        }
    }
#endif
}

/**
 * @brief  Erase the main and the information banks.
 * @note   This function can be used only for MDR32F02 and MDR1206FI (ID = 217).
 * @param  None.
 * @return None.
 */
#if defined(USE_MDR32F02) || defined(USE_MDR1206FI)
__RAMFUNC void FLASH_EraseAllBanks(void)
#elif defined(USE_MDR1206)
__RAMFUNC static void FLASH_EraseAllBanks_ID217(void)
#endif
{
#if defined(USE_MDR32F02)
    FLASH_MassErase(FLASH_ERASE_BANK_ALL);
#elif defined(USE_MDR1206FI)
#if defined(USE_FLASH_FACTORY_DATA_PROTECTION)
    FLASH_MassErase(FLASH_ERASE_BANK_MAIN);
    FLASH_EraseAllPages(FLASH_BANK_INFO);
#else
    FLASH_MassErase(FLASH_ERASE_BANK_ALL);
#endif
#elif defined(USE_MDR1206)
#if defined(USE_FLASH_FACTORY_DATA_PROTECTION)
    FLASH_MassErase_ID217(FLASH_ERASE_BANK_MAIN);
    FLASH_EraseAllPages_ID217(FLASH_BANK_INFO);
#else
    FLASH_MassErase_ID217(FLASH_ERASE_BANK_ALL);
#endif
#endif
}

/**
 * @brief   Program a 32-bit FLASH memory value.
 * @warning To count the correct delay times, the core clock must be at least 8 MHz.
 * @warning The same address (32-bit data) can't be programmed more than twice before next erase.
 * @note    This function can be used only for MDR32F02 and MDR1206FI (ID = 217).
 * @note    The core clock from SystemCoreClock is used to calculate the delay time.
 *          Before calling FLASH_ProgramWord(), you must update SystemCoreClock by calling SystemCoreClockUpdate().
 * @param   Address: A FLASH memory word address (four bytes aligned).
 * @param   Bank: @ref FLASH_MemoryBank_TypeDef - select a FLASH bank (the main bank or the information bank).
 *          This parameter can be one of the following values:
 *              @arg FLASH_BANK_MAIN: The FLASH main bank;
 *              @arg FLASH_BANK_INFO: The FLASH information bank.
 * @param   Data: 32-bit data to be programmed.
 * @return  None.
 */
#if defined(USE_MDR32F02) || defined(USE_MDR1206FI)
__RAMFUNC void FLASH_ProgramWord(uint32_t Address, FLASH_MemoryBank_TypeDef Bank, uint32_t Data)
#elif defined(USE_MDR1206)
__RAMFUNC static void FLASH_ProgramWord_ID217(uint32_t Address, FLASH_MemoryBank_TypeDef Bank, uint32_t Data)
#endif
{
    uint32_t Command;
    uint32_t DelayConst;
    uint32_t DelayProg;

    /* Check the parameters. */
    assert_param(IS_FLASH_MEMORY_BANK(Bank));
    assert_param(IS_FLASH_ADDR_FOUR_BYTE_ALIGNED(Address));

#if defined(USE_MDR1206FI) || defined(USE_MDR1206)
#if defined(USE_FLASH_FACTORY_DATA_PROTECTION)
    if (Bank == FLASH_BANK_INFO) {
        if (((Address & FLASH_ADDR_INFO_WORD_MASK_ID217) >= FLASH_FACTORY_DATA_START_ADDR_ID217) &&
            ((Address & FLASH_ADDR_INFO_WORD_MASK_ID217) <= FLASH_FACTORY_DATA_END_ADDR_ID217)) {
            return;
        }
    }
#endif
#endif

    DelayConst = DELAY_PROGRAM_GET_CONST(SystemCoreClock);
    DelayProg  = DELAY_PROGRAM_GET_LOOPS_US(FLASH_TPROG_US, DelayConst);

    Command = (MDR_FLASH->CMD & FLASH_CMD_DELAY_Msk) | FLASH_CMD_CON | FLASH_CMD_TMR;
    Command |= (Bank == FLASH_BANK_INFO) ? FLASH_CMD_IFREN : 0;
    MDR_FLASH->CMD = Command;

    MDR_FLASH->ADR = Address;
    MDR_FLASH->DI  = Data;

    Command |= FLASH_CMD_XE | FLASH_CMD_PROG;
    MDR_FLASH->CMD = Command;
    DELAY_PROGRAM_WaitLoopsAsm(DELAY_PROGRAM_GET_LOOPS_US(FLASH_TNVS_US, DelayConst));

    Command |= FLASH_CMD_NVSTR;
    MDR_FLASH->CMD = Command;
    DELAY_PROGRAM_WaitLoopsAsm(DELAY_PROGRAM_GET_LOOPS_US(FLASH_TPGS_US, DelayConst));

#if defined(USE_MDR32F02) || defined(USE_MDR1206FI)
    FLASH_ProgramCycle(Command, DelayProg);
#elif defined(USE_MDR1206)
    FLASH_ProgramCycle_ID217(Command, DelayProg);
#endif

    Command &= ~FLASH_CMD_PROG;
    MDR_FLASH->CMD = Command;
    DELAY_PROGRAM_WaitLoopsAsm(DELAY_PROGRAM_GET_LOOPS_US(FLASH_TNVH_US, DelayConst));

    Command &= ~(FLASH_CMD_XE | FLASH_CMD_NVSTR);
    MDR_FLASH->CMD = Command;
    MDR_FLASH->CMD = (Command & FLASH_CMD_DELAY_Msk) | FLASH_CMD_TMR;

    DELAY_PROGRAM_WaitLoopsAsm(DELAY_PROGRAM_GET_LOOPS_US(FLASH_TRCV_US, DelayConst));
}

/**
 * @brief   Program a 16-bit FLASH memory value.
 * @warning To count the correct delay times, the core clock must be at least 8 MHz.
 * @warning The same address (32-bit data) can't be programmed more than twice before next erase.
 * @note    This function can be used only for MDR32F02 and MDR1206FI (ID = 217).
 * @note    Hardware supports only 32-bit word programming.
 * @note    The core clock from SystemCoreClock is used to calculate the delay time.
 *          Before calling FLASH_ProgramHalfWord(), you must update SystemCoreClock by calling SystemCoreClockUpdate().
 * @param   Address: A FLASH memory half word address (two bytes aligned).
 * @param   Bank: @ref FLASH_MemoryBank_TypeDef - select a FLASH bank (the main bank or the information bank).
 *          This parameter can be one of the following values:
 *              @arg FLASH_BANK_MAIN: The FLASH main bank;
 *              @arg FLASH_BANK_INFO: The FLASH information bank.
 * @param   Data: 16-bit data to be programmed.
 * @return  None.
 */
#if defined(USE_MDR32F02) || defined(USE_MDR1206FI)
__RAMFUNC void FLASH_ProgramHalfWord(uint32_t Address, FLASH_MemoryBank_TypeDef Bank, uint16_t Data)
#elif defined(USE_MDR1206)
__RAMFUNC static void FLASH_ProgramHalfWord_ID217(uint32_t Address, FLASH_MemoryBank_TypeDef Bank, uint16_t Data)
#endif
{
    uint32_t Mask;
    uint32_t Tmp;
    uint32_t Shift;
    uint32_t ProgData;

    /* Check the parameters. */
    assert_param(IS_FLASH_MEMORY_BANK(Bank));
    assert_param(IS_FLASH_ADDR_TWO_BYTE_ALIGNED(Address));

    Shift    = (Address & FLASH_ADDR_HALFWORD_MASK) * 8;
    ProgData = (uint32_t)Data << Shift;
    Mask     = (uint32_t)0xFFFF << Shift;
    Address &= ~FLASH_ADDR_BYTE_MASK;

#if defined(USE_MDR32F02) || defined(USE_MDR1206FI)
    Tmp      = FLASH_ReadWord(Address, Bank);
    ProgData = (Tmp & ~Mask) | (ProgData & Mask);
    FLASH_ProgramWord(Address, Bank, ProgData);
#elif defined(USE_MDR1206)
    Tmp      = FLASH_ReadWord_ID217(Address, Bank);
    ProgData = (Tmp & ~Mask) | (ProgData & Mask);
    FLASH_ProgramWord_ID217(Address, Bank, ProgData);
#endif
}

/**
 * @brief   Program an 8-bit FLASH memory value.
 * @warning To count the correct delay times, the core clock must be at least 8 MHz.
 * @warning The same address (32-bit data) can't be programmed more than twice before next erase.
 * @note    This function can be used only for MDR32F02 and MDR1206FI (ID = 217).
 * @note    Hardware supports only 32-bit word programming.
 * @note    The core clock from SystemCoreClock is used to calculate the delay time.
 *          Before calling FLASH_ProgramByte(), you must update SystemCoreClock by calling SystemCoreClockUpdate().
 * @param   Address: A FLASH memory byte address.
 * @param   Bank: @ref FLASH_MemoryBank_TypeDef - select a FLASH bank (the main bank or the information bank).
 *          This parameter can be one of the following values:
 *              @arg FLASH_BANK_MAIN: The FLASH main bank;
 *              @arg FLASH_BANK_INFO: The FLASH information bank.
 * @param   Data: 8-bit data to be programmed.
 * @return  None.
 */
#if defined(USE_MDR32F02) || defined(USE_MDR1206FI)
__RAMFUNC void FLASH_ProgramByte(uint32_t Address, FLASH_MemoryBank_TypeDef Bank, uint8_t Data)
#elif defined(USE_MDR1206)
__RAMFUNC static void FLASH_ProgramByte_ID217(uint32_t Address, FLASH_MemoryBank_TypeDef Bank, uint8_t Data)
#endif
{
    uint32_t Mask;
    uint32_t Tmp;
    uint32_t Shift;
    uint32_t ProgData;

    /* Check the parameters. */
    assert_param(IS_FLASH_MEMORY_BANK(Bank));

    Shift    = (Address & FLASH_ADDR_BYTE_MASK) * 8;
    ProgData = (uint32_t)Data << Shift;
    Mask     = (uint32_t)0xFF << Shift;
    Address &= ~FLASH_ADDR_BYTE_MASK;

#if defined(USE_MDR32F02) || defined(USE_MDR1206FI)
    Tmp      = FLASH_ReadWord(Address, Bank);
    ProgData = (Tmp & ~Mask) | (ProgData & Mask);
    FLASH_ProgramWord(Address, Bank, ProgData);
#elif defined(USE_MDR1206)
    Tmp      = FLASH_ReadWord_ID217(Address, Bank);
    ProgData = (Tmp & ~Mask) | (ProgData & Mask);
    FLASH_ProgramWord_ID217(Address, Bank, ProgData);
#endif
}

/**
 * @brief   Program an array of the 32-bit FLASH memory values in the packet mode.
 * @warning To count the correct delay times, the core clock must be at least 8 MHz.
 * @warning The same address (32-bit data) can't be programmed more than twice before next erase.
 * @note    This function can be used only for MDR32F02 and MDR1206FI (ID = 217).
 * @note    The core clock from SystemCoreClock is used to calculate the delay time.
 *          Before calling FLASH_ProgramWordArrayBurst(), you must update SystemCoreClock by calling SystemCoreClockUpdate().
 * @param   PtrAddressArray: The pointer to the array with FLASH memory words addresses to write (four bytes aligned).
 * @param   PtrDataArray: The pointer to the data array to program.
 * @param   ArraySize: The size of address and data arrays.
 * @param   Bank: @ref FLASH_MemoryBank_TypeDef - select a FLASH bank (the main bank or the information bank).
 *          This parameter can be one of the following values:
 *              @arg FLASH_BANK_MAIN: The FLASH main bank;
 *              @arg FLASH_BANK_INFO: The FLASH information bank.
 * @return  None.
 */
#if defined(USE_MDR32F02) || defined(USE_MDR1206FI)
__RAMFUNC void FLASH_ProgramWordArrayBurst(const uint32_t* PtrAddressArray, const uint32_t* PtrDataArray, uint32_t ArraySize, FLASH_MemoryBank_TypeDef Bank)
#elif defined(USE_MDR1206)
__RAMFUNC static void FLASH_ProgramWordArrayBurst_ID217(const uint32_t* PtrAddressArray, const uint32_t* PtrDataArray, uint32_t ArraySize, FLASH_MemoryBank_TypeDef Bank)
#endif
{
    uint32_t Command;
    uint32_t DelayConst;
    uint32_t DelayProg;
    uint32_t Index;

    /* Check the parameters. */
    assert_param(IS_FLASH_MEMORY_BANK(Bank));
    assert_param(ArraySize > 0);
#if (USE_ASSERT_INFO != 0)
    for (Index = 0; Index < ArraySize; Index++) {
        assert_param(IS_FLASH_ADDR_FOUR_BYTE_ALIGNED(PtrAddressArray[Index]));
    }
#endif

    DelayConst = DELAY_PROGRAM_GET_CONST(SystemCoreClock);
    DelayProg  = DELAY_PROGRAM_GET_LOOPS_US(FLASH_TPROG_US, DelayConst);

    Command = (MDR_FLASH->CMD & FLASH_CMD_DELAY_Msk) | FLASH_CMD_CON | FLASH_CMD_TMR;
    Command |= (Bank == FLASH_BANK_INFO) ? FLASH_CMD_IFREN : 0;
    MDR_FLASH->CMD = Command;

    Index = 0;
    while (Index < ArraySize) {
#if defined(USE_MDR1206FI) || defined(USE_MDR1206)
#if defined(USE_FLASH_FACTORY_DATA_PROTECTION)
        if (Bank == FLASH_BANK_INFO) {
            if (((PtrAddressArray[Index] & FLASH_ADDR_INFO_WORD_MASK_ID217) >= FLASH_FACTORY_DATA_START_ADDR_ID217) &&
                ((PtrAddressArray[Index] & FLASH_ADDR_INFO_WORD_MASK_ID217) <= FLASH_FACTORY_DATA_END_ADDR_ID217)) {
                Index++;
                continue;
            }
        }
#endif
#endif
        MDR_FLASH->ADR = PtrAddressArray[Index];
        MDR_FLASH->DI  = PtrDataArray[Index];

        Command |= FLASH_CMD_XE | FLASH_CMD_PROG;
        MDR_FLASH->CMD = Command;
        DELAY_PROGRAM_WaitLoopsAsm(DELAY_PROGRAM_GET_LOOPS_US(FLASH_TNVS_US, DelayConst));

        Command |= FLASH_CMD_NVSTR;
        MDR_FLASH->CMD = Command;
        DELAY_PROGRAM_WaitLoopsAsm(DELAY_PROGRAM_GET_LOOPS_US(FLASH_TPGS_US, DelayConst));

#if defined(USE_MDR32F02) || defined(USE_MDR1206FI)
        FLASH_ProgramCycle(Command, DelayProg);
#elif defined(USE_MDR1206)
        FLASH_ProgramCycle_ID217(Command, DelayProg);
#endif
        Index++;
        /* Can be programmed with reduced delays (without 30 us delay) if only YADR is changed. */
        while ((Index < ArraySize) && ((PtrAddressArray[Index - 1] & ~FLASH_ADR_YADR_Msk) == (PtrAddressArray[Index] & ~FLASH_ADR_YADR_Msk))) {
#if defined(USE_MDR1206FI) || defined(USE_MDR1206)
#if defined(USE_FLASH_FACTORY_DATA_PROTECTION)
            if (Bank == FLASH_BANK_INFO) {
                if (((PtrAddressArray[Index] & FLASH_ADDR_INFO_WORD_MASK_ID217) >= FLASH_FACTORY_DATA_START_ADDR_ID217) &&
                    ((PtrAddressArray[Index] & FLASH_ADDR_INFO_WORD_MASK_ID217) <= FLASH_FACTORY_DATA_END_ADDR_ID217)) {
                    Index++;
                    continue;
                }
            }
#endif
#endif
            MDR_FLASH->ADR = PtrAddressArray[Index];
            MDR_FLASH->DI  = PtrDataArray[Index];
#if defined(USE_MDR32F02) || defined(USE_MDR1206FI)
            FLASH_ProgramCycle(Command, DelayProg);
#elif defined(USE_MDR1206)
            FLASH_ProgramCycle_ID217(Command, DelayProg);
#endif
            Index++;
        }
        Command &= ~FLASH_CMD_PROG;
        MDR_FLASH->CMD = Command;
        DELAY_PROGRAM_WaitLoopsAsm(DELAY_PROGRAM_GET_LOOPS_US(FLASH_TNVH_US, DelayConst));

        Command &= ~(FLASH_CMD_XE | FLASH_CMD_NVSTR);
        MDR_FLASH->CMD = Command;
        DELAY_PROGRAM_WaitLoopsAsm(DELAY_PROGRAM_GET_LOOPS_US(FLASH_TRCV_US, DelayConst));
    }

    MDR_FLASH->CMD = (Command & FLASH_CMD_DELAY_Msk) | FLASH_CMD_TMR;
}
#endif

#if defined(USE_MDR1206AFI) || defined(USE_MDR1206)
/**
 * @brief  Read a 32-bit FLASH memory value.
 * @note   This function can be used only for MDR1206AFI (ID = 215).
 * @param  Address: A FLASH memory word address (four bytes aligned).
 * @param  Bank: @ref FLASH_MemoryBank_TypeDef - select a FLASH bank (the main bank or the information bank).
 *         This parameter can be one of the following values:
 *             @arg FLASH_BANK_MAIN: The FLASH main bank;
 *             @arg FLASH_BANK_INFO: The FLASH information bank.
 * @return The selected FLASH memory word.
 */
#if defined(USE_MDR1206AFI)
__RAMFUNC uint32_t FLASH_ReadWord(uint32_t Address, FLASH_MemoryBank_TypeDef Bank)
#elif defined(USE_MDR1206)
__RAMFUNC static uint32_t FLASH_ReadWord_ID215(uint32_t Address, FLASH_MemoryBank_TypeDef Bank)
#endif
{
    uint32_t Command;
    uint32_t Data;

    /* Check the parameters. */
    assert_param(IS_FLASH_MEMORY_BANK(Bank));
    assert_param(IS_FLASH_ADDR_FOUR_BYTE_ALIGNED(Address));

    Command = FLASH_CMD_CON | FLASH_CMD_TMEN;
    Command |= (Bank == FLASH_BANK_INFO) ? FLASH_CMD_NVR : 0;
    MDR_FLASH->CMD = Command;
    MDR_FLASH->ADR = FLASH_ConvertAddress(Address, Bank);
    MDR_FLASH->CMD = Command | FLASH_CMD_CE | FLASH_CMD_RE;
    MDR_FLASH->DO; /* Idle reading for delay. */
    MDR_FLASH->DO; /* Idle reading for delay. */
    Data           = MDR_FLASH->DO;
    MDR_FLASH->CMD = FLASH_CMD_TMEN;

    return Data;
}

/**
 * @brief  Read the array of the 32-bit FLASH memory values in the packet mode.
 * @note   This function can be used only for for MDR1206AFI (ID = 215).
 * @param  PtrAddressArray: The pointer to the array with FLASH memory words addresses to read (four bytes aligned).
 * @param  PtrDataArray: The pointer to the array where the words read from the FLASH memory will be stored.
 * @param  ArraySize: The size of address and data arrays.
 * @param  Bank: @ref FLASH_MemoryBank_TypeDef - select a FLASH bank (the main bank or the information bank).
 *         This parameter can be one of the following values:
 *             @arg FLASH_BANK_MAIN: The FLASH main bank;
 *             @arg FLASH_BANK_INFO: The FLASH information bank.
 * @return None.
 */
#if defined(USE_MDR1206AFI)
__RAMFUNC void FLASH_ReadWordArrayBurst(const uint32_t* PtrAddressArray, uint32_t* PtrDataArray, uint32_t ArraySize, FLASH_MemoryBank_TypeDef Bank)
#elif defined(USE_MDR1206)
__RAMFUNC static void FLASH_ReadWordArrayBurst_ID215(const uint32_t* PtrAddressArray, uint32_t* PtrDataArray, uint32_t ArraySize, FLASH_MemoryBank_TypeDef Bank)
#endif
{
    uint32_t Command;
    uint32_t Index;

    /* Check the parameters. */
    assert_param(IS_FLASH_MEMORY_BANK(Bank));
    assert_param(ArraySize > 0);
#if (USE_ASSERT_INFO != 0)
    for (Index = 0; Index < ArraySize; Index++) {
        assert_param(IS_FLASH_ADDR_FOUR_BYTE_ALIGNED(PtrAddressArray[Index]));
    }
#endif

    Command = FLASH_CMD_CON | FLASH_CMD_TMEN;
    Command |= (Bank == FLASH_BANK_INFO) ? FLASH_CMD_NVR : 0;
    MDR_FLASH->CMD = Command;

    for (Index = 0; Index < ArraySize; Index++) {
        MDR_FLASH->CMD = Command | FLASH_CMD_CE;
        MDR_FLASH->ADR = FLASH_ConvertAddress(PtrAddressArray[Index], Bank);
        MDR_FLASH->CMD = Command | FLASH_CMD_CE | FLASH_CMD_RE;
        MDR_FLASH->DO; /* Idle reading for delay. */
        MDR_FLASH->DO; /* Idle reading for delay. */
        PtrDataArray[Index] = MDR_FLASH->DO;
    }
    MDR_FLASH->CMD = FLASH_CMD_TMEN;
}

/**
 * @brief  Erase one page (1 KByte) of a selected FLASH memory bank.
 * @note   This function can be used only for MDR1206AFI (ID = 215).
 * @note   The core clock from SystemCoreClock is used to calculate the delay time.
 *         Before calling FLASH_ErasePage(), you must update SystemCoreClock by calling SystemCoreClockUpdate().
 * @param  Address: Page address in the FLASH memory.
 * @param  Bank: @ref FLASH_MemoryBank_TypeDef - select a FLASH bank (the main bank or the information bank).
 *         This parameter can be one of the following values:
 *             @arg FLASH_BANK_MAIN: The FLASH main bank;
 *             @arg FLASH_BANK_INFO: The FLASH information bank.
 * @return None.
 */
#if defined(USE_MDR1206AFI)
__RAMFUNC void FLASH_ErasePage(uint32_t Address, FLASH_MemoryBank_TypeDef Bank)
#elif defined(USE_MDR1206)
__RAMFUNC static void FLASH_ErasePage_ID215(uint32_t Address, FLASH_MemoryBank_TypeDef Bank)
#endif
{
    uint32_t Command;
    uint32_t DelayConst;
    uint32_t Sector;

    /* Check the parameters. */
    assert_param(IS_FLASH_MEMORY_BANK(Bank));

#if defined(USE_FLASH_FACTORY_DATA_PROTECTION)
    if (Bank == FLASH_BANK_INFO) {
        if (FLASH_GetInfoPageNumber(Address) == FLASH_FACTORY_DATA_PAGE_NUMBER_ID215) {
            return;
        }
    }
#endif

    DelayConst = DELAY_PROGRAM_GET_CONST(SystemCoreClock);

    Command = FLASH_CMD_CON | FLASH_CMD_TMEN;
    Command |= (Bank == FLASH_BANK_INFO) ? FLASH_CMD_NVR : 0;
    MDR_FLASH->CMD = Command;

    for (Sector = 0; Sector < 2; Sector++) {
        Address        = (Address & ~FLASH_ADDR_SECTOR_MASK) | (Sector << FLASH_ADDR_SECTOR_POS);
        MDR_FLASH->ADR = FLASH_ConvertAddress(Address, Bank);

        Command |= FLASH_CMD_CE | FLASH_CMD_ERASE;
        MDR_FLASH->CMD = Command;
        DELAY_PROGRAM_WaitLoopsAsm(DELAY_PROGRAM_GET_LOOPS_US(FLASH_TNVS_SECTOR_ERASE_US, DelayConst));

        Command |= FLASH_CMD_WE;
        MDR_FLASH->CMD = Command;
        DELAY_PROGRAM_WaitLoopsAsm(DELAY_PROGRAM_GET_LOOPS_US(FLASH_TERASE_SECTOR_ERASE_US, DelayConst));

        Command &= ~FLASH_CMD_WE;
        MDR_FLASH->CMD = Command;
        DELAY_PROGRAM_WaitLoopsAsm(DELAY_PROGRAM_GET_LOOPS_US(FLASH_TRCV_SECTOR_ERASE_US, DelayConst));

        Command &= ~(FLASH_CMD_CE | FLASH_CMD_ERASE);
        MDR_FLASH->CMD = Command;
    }
    MDR_FLASH->CMD = FLASH_CMD_TMEN;

    DELAY_PROGRAM_WaitLoopsAsm(DELAY_PROGRAM_GET_LOOPS_US(FLASH_TRW_SECTOR_ERASE_US, DelayConst));
}

/**
 * @brief  Erase the main memory bank.
 * @note   This function can be used only for MDR1206AFI (ID = 215).
 * @param  None.
 * @return None.
 */
#if defined(USE_MDR1206AFI)
__RAMFUNC static void FLASH_MassErase(void)
#elif defined(USE_MDR1206)
__RAMFUNC static void FLASH_MassErase_ID215(void)
#endif
{
    uint32_t Command;
    uint32_t DelayConst;
    uint32_t Sector;

    DelayConst = DELAY_PROGRAM_GET_CONST(SystemCoreClock);

    Command        = FLASH_CMD_CON | FLASH_CMD_TMEN;
    MDR_FLASH->CMD = Command;

    for (Sector = 0; Sector < 2; Sector++) {
        MDR_FLASH->ADR = Sector << FLASH_ADR_MAIN_SECTOR_Pos;
        Command |= FLASH_CMD_CE | FLASH_CMD_CHIP | FLASH_CMD_ERASE;
        MDR_FLASH->CMD = Command;
        DELAY_PROGRAM_WaitLoopsAsm(DELAY_PROGRAM_GET_LOOPS_US(FLASH_TNVS_CHIP_ERASE_US, DelayConst));

        Command |= FLASH_CMD_WE;
        MDR_FLASH->CMD = Command;
        DELAY_PROGRAM_WaitLoopsAsm(DELAY_PROGRAM_GET_LOOPS_US(FLASH_TERASE_CHIP_ERASE_US, DelayConst));

        Command &= ~FLASH_CMD_WE;
        MDR_FLASH->CMD = Command;
        DELAY_PROGRAM_WaitLoopsAsm(DELAY_PROGRAM_GET_LOOPS_US(FLASH_TRCV_CHIP_ERASE_US, DelayConst));

        Command &= ~(FLASH_CMD_CE | FLASH_CMD_CHIP | FLASH_CMD_ERASE);
        MDR_FLASH->CMD = Command;
    }
    MDR_FLASH->CMD = FLASH_CMD_TMEN;

    DELAY_PROGRAM_WaitLoopsAsm(DELAY_PROGRAM_GET_LOOPS_US(FLASH_TRW_CHIP_ERASE_US, DelayConst));
}

/**
 * @brief  Erase all pages of a selected FLASH memory bank.
 * @note   This function can be used only for MDR1206AFI (ID = 215).
 * @param  Bank: @ref FLASH_MemoryBank_TypeDef - select a FLASH bank.
 *         This parameter can be one of the following values:
 *             @arg FLASH_BANK_MAIN: The FLASH main bank;
 *             @arg FLASH_BANK_INFO: The FLASH information bank.
 * @return None.
 */
#if defined(USE_MDR1206AFI)
__RAMFUNC void FLASH_EraseAllPages(FLASH_MemoryBank_TypeDef Bank)
#elif defined(USE_MDR1206)
__RAMFUNC static void FLASH_EraseAllPages_ID215(FLASH_MemoryBank_TypeDef Bank)
#endif
{
    uint32_t Page;

    /* Check the parameters. */
    assert_param(IS_FLASH_MEMORY_BANK(Bank));

#if defined(USE_MDR1206AFI)
    if (Bank == FLASH_BANK_MAIN) {
        FLASH_MassErase();
    } else if (Bank == FLASH_BANK_INFO) {
        for (Page = 0; Page < 15; Page++) {
            FLASH_ErasePage(GET_INFO_PAGE_ADDR(Page), FLASH_BANK_INFO);
        }
    }
#elif defined(USE_MDR1206)
    if (Bank == FLASH_BANK_MAIN) {
        FLASH_MassErase_ID215();
    } else if (Bank == FLASH_BANK_INFO) {
        for (Page = 0; Page < 15; Page++) {
            FLASH_ErasePage_ID215(GET_INFO_PAGE_ADDR_ID215(Page), FLASH_BANK_INFO);
        }
    }
#endif
}

/**
 * @brief  Erase the main and the information banks.
 * @note   This function can be used only for MDR1206AFI (ID = 215).
 * @param  None.
 * @return None.
 */
#if defined(USE_MDR1206AFI)
__RAMFUNC void FLASH_EraseAllBanks(void)
#elif defined(USE_MDR1206)
__RAMFUNC static void FLASH_EraseAllBanks_ID215(void)
#endif
{
#if defined(USE_MDR1206AFI)
    FLASH_MassErase();
    FLASH_EraseAllPages(FLASH_BANK_INFO);
#elif defined(USE_MDR1206)
    FLASH_MassErase_ID215();
    FLASH_EraseAllPages_ID215(FLASH_BANK_INFO);
#endif
}

/**
 * @brief   Program a 32-bit FLASH memory value.
 * @warning To count the correct delay times, the core clock must be at least 8 MHz.
 * @warning The same address (8-bit data) can't be programmed again before next erase.
 * @note    This function can be used only for MDR1206AFI (ID = 215).
 * @note    Hardware supports only 8-bit data programming.
 * @note    The core clock from SystemCoreClock is used to calculate the delay time.
 *          Before calling FLASH_ProgramWord(), you must update SystemCoreClock by calling SystemCoreClockUpdate().
 * @param   Address: A FLASH memory word address (four bytes aligned).
 * @param   Bank: @ref FLASH_MemoryBank_TypeDef - select a FLASH bank (the main bank or the information bank).
 *          This parameter can be one of the following values:
 *              @arg FLASH_BANK_MAIN: The FLASH main bank;
 *              @arg FLASH_BANK_INFO: The FLASH information bank.
 * @param   Data: 32-bit data to be programmed.
 * @return  None.
 */
#if defined(USE_MDR1206AFI)
__RAMFUNC void FLASH_ProgramWord(uint32_t Address, FLASH_MemoryBank_TypeDef Bank, uint32_t Data)
#elif defined(USE_MDR1206)
__RAMFUNC static void FLASH_ProgramWord_ID215(uint32_t Address, FLASH_MemoryBank_TypeDef Bank, uint32_t Data)
#endif
{
    /* Check the parameters. */
    assert_param(IS_FLASH_MEMORY_BANK(Bank));
    assert_param(IS_FLASH_ADDR_FOUR_BYTE_ALIGNED(Address));

    FLASH_ProgramBytes(Address, Bank, Data, FLASH_PROGRAM_SIZE_4BYTE);
}

/**
 * @brief   Program a 16-bit FLASH memory value.
 * @warning To count the correct delay times, the core clock must be at least 8 MHz.
 * @warning The same address (8-bit data) can't be programmed again before next erase.
 * @note    This function can be used only for MDR1206AFI (ID = 215).
 * @note    Hardware supports only 8-bit data programming.
 * @note    The core clock from SystemCoreClock is used to calculate the delay time.
 *          Before calling FLASH_ProgramHalfWord(), you must update SystemCoreClock by calling SystemCoreClockUpdate().
 * @param   Address: A FLASH memory half word address (two bytes aligned).
 * @param   Bank: @ref FLASH_MemoryBank_TypeDef - select a FLASH bank (the main bank or the information bank).
 *          This parameter can be one of the following values:
 *              @arg FLASH_BANK_MAIN: The FLASH main bank;
 *              @arg FLASH_BANK_INFO: The FLASH information bank.
 * @param   Data: 16-bit data to be programmed.
 * @return  None.
 */
#if defined(USE_MDR1206AFI)
__RAMFUNC void FLASH_ProgramHalfWord(uint32_t Address, FLASH_MemoryBank_TypeDef Bank, uint16_t Data)
#elif defined(USE_MDR1206)
__RAMFUNC static void FLASH_ProgramHalfWord_ID215(uint32_t Address, FLASH_MemoryBank_TypeDef Bank, uint16_t Data)
#endif
{
    uint32_t Shift;
    uint32_t ProgData;

    /* Check the parameters. */
    assert_param(IS_FLASH_MEMORY_BANK(Bank));
    assert_param(IS_FLASH_ADDR_TWO_BYTE_ALIGNED(Address));

    Shift    = (Address & FLASH_ADDR_HALFWORD_MASK) * 8;
    ProgData = (uint32_t)Data << Shift;

    FLASH_ProgramBytes(Address, Bank, ProgData, FLASH_PROGRAM_SIZE_2BYTE);
}

/**
 * @brief   Program an 8-bit FLASH memory value.
 * @warning To count the correct delay times, the core clock must be at least 8 MHz.
 * @warning The same address (8-bit data) can't be programmed again before next erase.
 * @note    This function can be used only for MDR1206AFI (ID = 215).
 * @note    The core clock from SystemCoreClock is used to calculate the delay time.
 *          Before calling FLASH_ProgramByte(), you must update SystemCoreClock by calling SystemCoreClockUpdate().
 * @param   Address: A FLASH memory byte address.
 * @param   Bank: @ref FLASH_MemoryBank_TypeDef - select a FLASH bank (the main bank or the information bank).
 *          This parameter can be one of the following values:
 *              @arg FLASH_BANK_MAIN: The FLASH main bank;
 *              @arg FLASH_BANK_INFO: The FLASH information bank.
 * @param   Data: 8-bit data to be programmed.
 * @return  None.
 */
#if defined(USE_MDR1206AFI)
__RAMFUNC void FLASH_ProgramByte(uint32_t Address, FLASH_MemoryBank_TypeDef Bank, uint8_t Data)
#elif defined(USE_MDR1206)
__RAMFUNC static void FLASH_ProgramByte_ID215(uint32_t Address, FLASH_MemoryBank_TypeDef Bank, uint8_t Data)
#endif
{
    uint32_t Shift;
    uint32_t ProgData;

    /* Check the parameters. */
    assert_param(IS_FLASH_MEMORY_BANK(Bank));

    Shift    = (Address & FLASH_ADDR_BYTE_MASK) * 8;
    ProgData = (uint32_t)Data << Shift;

    FLASH_ProgramBytes(Address, Bank, ProgData, FLASH_PROGRAM_SIZE_1BYTE);
}

/**
 * @brief   Program an array of the 32-bit FLASH memory values in the packet mode.
 * @warning To count the correct delay times, the core clock must be at least 8 MHz.
 * @warning The same address (8-bit data) can't be programmed again before next erase.
 * @note    This function can be used only for MDR1206AFI (ID = 215).
 * @note    Hardware supports only 8-bit data programming.
 * @note    The core clock from SystemCoreClock is used to calculate the delay time.
 *          Before calling FLASH_ProgramWordArrayBurst(), you must update SystemCoreClock by calling SystemCoreClockUpdate().
 * @param   PtrAddressArray: The pointer to the array with FLASH memory words addresses to write (four bytes aligned).
 * @param   PtrDataArray: The pointer to the data array to program.
 * @param   ArraySize: The size of address and data arrays.
 * @param   Bank: @ref FLASH_MemoryBank_TypeDef - select a FLASH bank (the main bank or the information bank).
 *          This parameter can be one of the following values:
 *              @arg FLASH_BANK_MAIN: The FLASH main bank;
 *              @arg FLASH_BANK_INFO: The FLASH information bank.
 * @return  None.
 */
#if defined(USE_MDR1206AFI)
__RAMFUNC void FLASH_ProgramWordArrayBurst(const uint32_t* PtrAddressArray, const uint32_t* PtrDataArray, uint32_t ArraySize, FLASH_MemoryBank_TypeDef Bank)
#elif defined(USE_MDR1206)
__RAMFUNC static void FLASH_ProgramWordArrayBurst_ID215(const uint32_t* PtrAddressArray, const uint32_t* PtrDataArray, uint32_t ArraySize, FLASH_MemoryBank_TypeDef Bank)
#endif
{
    uint32_t Command;
    uint32_t DelayConst;
    uint32_t DelayProg;
    uint32_t DelayAdsAdh;
    uint32_t DelayPgs;
    uint32_t Sector;
    uint32_t Index;
    uint32_t PreviousIndex;

    /* Check the parameters. */
    assert_param(IS_FLASH_MEMORY_BANK(Bank));
    assert_param(ArraySize > 0);
#if (USE_ASSERT_INFO != 0)
    for (Index = 0; Index < ArraySize; Index++) {
        assert_param(IS_FLASH_ADDR_FOUR_BYTE_ALIGNED(PtrAddressArray[Index]));
    }
#endif

    DelayConst  = DELAY_PROGRAM_GET_CONST(SystemCoreClock);
    DelayProg   = DELAY_PROGRAM_GET_LOOPS_US(FLASH_TPROG_PROGRAM_US, DelayConst);
    DelayAdsAdh = DELAY_PROGRAM_GET_LOOPS_US(FLASH_TADS_PROGRAM_US, DelayConst);
    DelayPgs    = DELAY_PROGRAM_GET_LOOPS_US(FLASH_TPGS_PROGRAM_US, DelayConst);

    Command = FLASH_CMD_CON | FLASH_CMD_TMEN;
    Command |= (Bank == FLASH_BANK_INFO) ? FLASH_CMD_NVR : 0;
    MDR_FLASH->CMD = Command;

    for (Sector = 0; Sector < 2; Sector++) {
        for (Index = 0; Index < ArraySize;) {
#if defined(USE_FLASH_FACTORY_DATA_PROTECTION)
            if (Bank == FLASH_BANK_INFO) {
                if (((PtrAddressArray[Index] & FLASH_ADDR_INFO_WORD_MASK_ID215) >= FLASH_FACTORY_DATA_START_ADDR_ID215) &&
                    ((PtrAddressArray[Index] & FLASH_ADDR_INFO_WORD_MASK_ID215) <= FLASH_FACTORY_DATA_END_ADDR_ID215)) {
                    Index++;
                    continue;
                }
            }
#endif
            if (FLASH_GET_SECTOR_NUMBER(PtrAddressArray[Index]) == Sector) {
                MDR_FLASH->ADR = FLASH_ConvertAddress(PtrAddressArray[Index], Bank);
                MDR_FLASH->DI  = PtrDataArray[Index];

                Command |= FLASH_CMD_CE | FLASH_CMD_PROG;
                MDR_FLASH->CMD = Command;
                DELAY_PROGRAM_WaitLoopsAsm(DELAY_PROGRAM_GET_LOOPS_US(FLASH_TNVS_PROGRAM_US, DelayConst));

                Command |= FLASH_CMD_WE;
                MDR_FLASH->CMD = Command;
                DELAY_PROGRAM_WaitLoopsAsm(DelayPgs);

#if defined(USE_MDR1206AFI)
                FLASH_ProgramCycle(0, FLASH_PROGRAM_SIZE_4BYTE, Command, DelayProg, DelayAdsAdh);
#elif defined(USE_MDR1206)
                FLASH_ProgramCycle_ID215(0, FLASH_PROGRAM_SIZE_4BYTE, Command, DelayProg, DelayAdsAdh);
#endif
                PreviousIndex = Index;
                Index++;
                /* Can be programmed with reduced delays if only ROW address is changed. */
                for (; Index < ArraySize; Index++) {
#if defined(USE_FLASH_FACTORY_DATA_PROTECTION)
                    if (Bank == FLASH_BANK_INFO) {
                        if (((PtrAddressArray[Index] & FLASH_ADDR_INFO_WORD_MASK_ID215) >= FLASH_FACTORY_DATA_START_ADDR_ID215) &&
                            ((PtrAddressArray[Index] & FLASH_ADDR_INFO_WORD_MASK_ID215) <= FLASH_FACTORY_DATA_END_ADDR_ID215)) {
                            Index++;
                            continue;
                        }
                    }
#endif
                    if (FLASH_GET_SECTOR_NUMBER(PtrAddressArray[Index]) == Sector) {
                        if ((PtrAddressArray[PreviousIndex] & ~(FLASH_ADR_ROW_Msk << 1)) == (PtrAddressArray[Index] & ~(FLASH_ADR_ROW_Msk << 1))) {
                            MDR_FLASH->ADR = FLASH_ConvertAddress(PtrAddressArray[Index], Bank);
                            MDR_FLASH->DI  = PtrDataArray[Index];
#if defined(USE_MDR1206AFI)
                            FLASH_ProgramCycle(0, FLASH_PROGRAM_SIZE_4BYTE, Command, DelayProg, DelayAdsAdh);
#elif defined(USE_MDR1206)
                            FLASH_ProgramCycle_ID215(0, FLASH_PROGRAM_SIZE_4BYTE, Command, DelayProg, DelayAdsAdh);
#endif
                            PreviousIndex = Index;
                        } else {
                            break;
                        }
                    }
                }
                Command &= ~FLASH_CMD_WE;
                MDR_FLASH->CMD = Command;
                DELAY_PROGRAM_WaitLoopsAsm(DELAY_PROGRAM_GET_LOOPS_US(FLASH_TRCV_PROGRAM_US, DelayConst));

                Command &= ~(FLASH_CMD_CE | FLASH_CMD_PROG);
                MDR_FLASH->CMD = Command;
                DELAY_PROGRAM_WaitLoopsAsm(DELAY_PROGRAM_GET_LOOPS_US(FLASH_TRW_PROGRAM_US, DelayConst));
            } else {
                Index++;
            }
        }
    }
    MDR_FLASH->CMD = FLASH_CMD_TMEN;
}
#endif

#if defined(USE_MDR1206)
/**
 * @brief  Read a 32-bit FLASH memory value.
 * @note   Common function for MDR1206FI (ID = 217) and MDR1206AFI (ID = 215).
 * @param  Address: A FLASH memory word address (four bytes aligned).
 * @param  Bank: @ref FLASH_MemoryBank_TypeDef - select a FLASH bank (the main bank or the information bank).
 *         This parameter can be one of the following values:
 *             @arg FLASH_BANK_MAIN: The FLASH main bank;
 *             @arg FLASH_BANK_INFO: The FLASH information bank.
 * @return The selected FLASH memory word.
 */
__RAMFUNC uint32_t FLASH_ReadWord(uint32_t Address, FLASH_MemoryBank_TypeDef Bank)
{
    uint32_t Data;

    if (GetChipID() == 217) {
        Data = FLASH_ReadWord_ID217(Address, Bank);
    } else {
        Data = FLASH_ReadWord_ID215(Address, Bank);
    }

    return Data;
}

/**
 * @brief  Read the array of the 32-bit FLASH memory values in the packet mode.
 * @note   Common function for MDR1206FI (ID = 217) and MDR1206AFI (ID = 215).
 * @param  PtrAddressArray: The pointer to the array with FLASH memory words addresses to read (four bytes aligned).
 * @param  PtrDataArray: The pointer to the array where the words read from the FLASH memory will be stored.
 * @param  ArraySize: The size of address and data arrays.
 * @param  Bank: @ref FLASH_MemoryBank_TypeDef - select a FLASH bank (the main bank or the information bank).
 *         This parameter can be one of the following values:
 *             @arg FLASH_BANK_MAIN: The FLASH main bank;
 *             @arg FLASH_BANK_INFO: The FLASH information bank.
 * @return None.
 */
__RAMFUNC void FLASH_ReadWordArrayBurst(const uint32_t* PtrAddressArray, uint32_t* PtrDataArray, uint32_t ArraySize, FLASH_MemoryBank_TypeDef Bank)
{
    if (GetChipID() == 217) {
        FLASH_ReadWordArrayBurst_ID217(PtrAddressArray, PtrDataArray, ArraySize, Bank);
    } else {
        FLASH_ReadWordArrayBurst_ID215(PtrAddressArray, PtrDataArray, ArraySize, Bank);
    }
}

/**
 * @brief  Erase one page of a selected FLASH memory bank:
 *         - MDR1206FI (ID = 217): page is 4 KByte;
 *         - MDR1206AFI (ID = 215): page is 1 KByte.
 * @note   Common function for MDR1206FI (ID = 217) and MDR1206AFI (ID = 215).
 * @note   The core clock from SystemCoreClock is used to calculate the delay time.
 *         Before calling FLASH_ErasePage(), you must update SystemCoreClock by calling SystemCoreClockUpdate().
 * @param  Address: Page address in the FLASH memory.
 * @param  Bank: @ref FLASH_MemoryBank_TypeDef - select a FLASH bank (the main bank or the information bank).
 *         This parameter can be one of the following values:
 *             @arg FLASH_BANK_MAIN: The FLASH main bank;
 *             @arg FLASH_BANK_INFO: The FLASH information bank.
 * @return None.
 */
__RAMFUNC void FLASH_ErasePage(uint32_t Address, FLASH_MemoryBank_TypeDef Bank)
{
    if (GetChipID() == 217) {
        FLASH_ErasePage_ID217(Address, Bank);
    } else {
        FLASH_ErasePage_ID215(Address, Bank);
    }
}

/**
 * @brief  Erase all pages of a selected FLASH memory bank.
 * @note   Common function for MDR1206FI (ID = 217) and MDR1206AFI (ID = 215).
 * @param  Bank: @ref FLASH_MemoryBank_TypeDef - select a FLASH bank.
 *         This parameter can be one of the following values:
 *             @arg FLASH_BANK_MAIN: The FLASH main bank;
 *             @arg FLASH_BANK_INFO: The FLASH information bank.
 * @return None.
 */
__RAMFUNC void FLASH_EraseAllPages(FLASH_MemoryBank_TypeDef Bank)
{
    if (GetChipID() == 217) {
        FLASH_EraseAllPages_ID217(Bank);
    } else {
        FLASH_EraseAllPages_ID215(Bank);
    }
}

/**
 * @brief  Erase the main and the information banks.
 * @note   Common function for MDR1206FI (ID = 217) and MDR1206AFI (ID = 215).
 * @param  None.
 * @return None.
 */
__RAMFUNC void FLASH_EraseAllBanks(void)
{
    if (GetChipID() == 217) {
        FLASH_EraseAllBanks_ID217();
    } else {
        FLASH_EraseAllBanks_ID215();
    }
}

/**
 * @brief   Program a 32-bit FLASH memory value.
 * @warning To count the correct delay times, the core clock must be at least 8 MHz.
 * @warning Programming the same address:
 *          - MDR1206FI (ID = 217): the same address (32-bit data) can't be programmed more than twice before next erase;
 *          - MDR1206AFI (ID = 215): the same address (8-bit data) can't be programmed again before next erase.
 * @note    Common function for MDR1206FI (ID = 217) and MDR1206AFI (ID = 215).
 * @note    Hardware supports only:
 *          - MDR1206FI (ID = 217): 32-bit data programming;
 *          - MDR1206AFI (ID = 215): 8-bit data programming.
 * @note    The core clock from SystemCoreClock is used to calculate the delay time.
 *          Before calling FLASH_ProgramWord(), you must update SystemCoreClock by calling SystemCoreClockUpdate().
 * @param   Address: A FLASH memory word address (four bytes aligned).
 * @param   Bank: @ref FLASH_MemoryBank_TypeDef - select a FLASH bank (the main bank or the information bank).
 *          This parameter can be one of the following values:
 *              @arg FLASH_BANK_MAIN: The FLASH main bank;
 *              @arg FLASH_BANK_INFO: The FLASH information bank.
 * @param   Data: 32-bit data to be programmed.
 * @return  None.
 */
__RAMFUNC void FLASH_ProgramWord(uint32_t Address, FLASH_MemoryBank_TypeDef Bank, uint32_t Data)
{
    if (GetChipID() == 217) {
        FLASH_ProgramWord_ID217(Address, Bank, Data);
    } else {
        FLASH_ProgramWord_ID215(Address, Bank, Data);
    }
}

/**
 * @brief   Program a 16-bit FLASH memory value.
 * @warning To count the correct delay times, the core clock must be at least 8 MHz.
 * @warning Programming the same address:
 *          - MDR1206FI (ID = 217): the same address (32-bit data) can't be programmed more than twice before next erase;
 *          - MDR1206AFI (ID = 215): the same address (8-bit data) can't be programmed again before next erase.
 * @note    Common function for MDR1206FI (ID = 217) and MDR1206AFI (ID = 215).
 * @note    Hardware supports only:
 *          - MDR1206FI (ID = 217): 32-bit data programming;
 *          - MDR1206AFI (ID = 215): 8-bit data programming.
 * @note    The core clock from SystemCoreClock is used to calculate the delay time.
 *          Before calling FLASH_ProgramHalfWord(), you must update SystemCoreClock by calling SystemCoreClockUpdate().
 * @param   Address: A FLASH memory half word address (two bytes aligned).
 * @param   Bank: @ref FLASH_MemoryBank_TypeDef - select a FLASH bank (the main bank or the information bank).
 *          This parameter can be one of the following values:
 *              @arg FLASH_BANK_MAIN: The FLASH main bank;
 *              @arg FLASH_BANK_INFO: The FLASH information bank.
 * @param   Data: 16-bit data to be programmed.
 * @return  None.
 */
__RAMFUNC void FLASH_ProgramHalfWord(uint32_t Address, FLASH_MemoryBank_TypeDef Bank, uint16_t Data)
{
    if (GetChipID() == 217) {
        FLASH_ProgramHalfWord_ID217(Address, Bank, Data);
    } else {
        FLASH_ProgramHalfWord_ID215(Address, Bank, Data);
    }
}

/**
 * @brief   Program an 8-bit FLASH memory value.
 * @warning To count the correct delay times, the core clock must be at least 8 MHz.
 * @warning Programming the same address:
 *          - MDR1206FI (ID = 217): the same address (32-bit data) can't be programmed more than twice before next erase;
 *          - MDR1206AFI (ID = 215): the same address (8-bit data) can't be programmed again before next erase.
 * @note    Common function for MDR1206FI (ID = 217) and MDR1206AFI (ID = 215).
 * @note    Hardware supports only:
 *          - MDR1206FI (ID = 217): 32-bit data programming;
 *          - MDR1206AFI (ID = 215): 8-bit data programming.
 * @note    The core clock from SystemCoreClock is used to calculate the delay time.
 *          Before calling FLASH_ProgramByte(), you must update SystemCoreClock by calling SystemCoreClockUpdate().
 * @param   Address: A FLASH memory byte address.
 * @param   Bank: @ref FLASH_MemoryBank_TypeDef - select a FLASH bank (the main bank or the information bank).
 *          This parameter can be one of the following values:
 *              @arg FLASH_BANK_MAIN: The FLASH main bank;
 *              @arg FLASH_BANK_INFO: The FLASH information bank.
 * @param   Data: 8-bit data to be programmed.
 * @return  None.
 */
__RAMFUNC void FLASH_ProgramByte(uint32_t Address, FLASH_MemoryBank_TypeDef Bank, uint8_t Data)
{
    if (GetChipID() == 217) {
        FLASH_ProgramByte_ID217(Address, Bank, Data);
    } else {
        FLASH_ProgramByte_ID215(Address, Bank, Data);
    }
}

/**
 * @brief   Program an array of the 32-bit FLASH memory values in the packet mode.
 * @warning To count the correct delay times, the core clock must be at least 8 MHz.
 * @warning Programming the same address:
 *          - MDR1206FI (ID = 217): the same address (32-bit data) can't be programmed more than twice before next erase;
 *          - MDR1206AFI (ID = 215): the same address (8-bit data) can't be programmed again before next erase.
 * @note    Common function for MDR1206FI (ID = 217) and MDR1206AFI (ID = 215).
 * @note    Hardware supports only:
 *          - MDR1206FI (ID = 217): 32-bit data programming;
 *          - MDR1206AFI (ID = 215): 8-bit data programming.
 * @note    The core clock from SystemCoreClock is used to calculate the delay time.
 *          Before calling FLASH_ProgramWordArrayBurst(), you must update SystemCoreClock by calling SystemCoreClockUpdate().
 * @param   PtrAddressArray: The pointer to the array with FLASH memory words addresses to write (four bytes aligned).
 * @param   PtrDataArray: The pointer to the data array to program.
 * @param   ArraySize: The size of address and data arrays.
 * @param   Bank: @ref FLASH_MemoryBank_TypeDef - select a FLASH bank (the main bank or the information bank).
 *          This parameter can be one of the following values:
 *              @arg FLASH_BANK_MAIN: The FLASH main bank;
 *              @arg FLASH_BANK_INFO: The FLASH information bank.
 * @return  None.
 */
__RAMFUNC void FLASH_ProgramWordArrayBurst(const uint32_t* PtrAddressArray, const uint32_t* PtrDataArray, uint32_t ArraySize, FLASH_MemoryBank_TypeDef Bank)
{
    if (GetChipID() == 217) {
        FLASH_ProgramWordArrayBurst_ID217(PtrAddressArray, PtrDataArray, ArraySize, Bank);
    } else {
        FLASH_ProgramWordArrayBurst_ID215(PtrAddressArray, PtrDataArray, ArraySize, Bank);
    }
}
#endif

/** @} */ /* End of the group FLASH_Register_Access_Functions */

/** @} */ /* End of the group FLASH_Functions_Not_Allowed_to_Be_Located_in_FLASH */

/** @} */ /* End of the group FLASH_Exported_Functions */

/** @} */ /* End of the group FLASH */

/** @} */ /* End of the group MDR32VF0xI_StdPeriph_Driver */

/*********************** (C) COPYRIGHT 2024 Milandr ****************************
 *
 * END OF FILE MDR32VF0xI_flash.c */

