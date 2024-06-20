#include "main.h"
#include "flash.h"
#include "stm32f1xx_hal_flash.h"
#include "sys.h"

#define FLASH_START_ADDR 0x0801f000 // 写入的起始地址

extern void FLASH_PageErase(uint32_t PageAddress);
extern HAL_StatusTypeDef FLASH_WaitForLastOperation(uint32_t Timeout);

void Flash_Write(uint32_t TypeProgram, uint32_t Address, uint64_t(*Data), uint32_t len)
{

  HAL_FLASH_Unlock();

  FLASH_PageErase(Address);           // 擦除这个扇区
  CLEAR_BIT(FLASH->CR, FLASH_CR_PER); // 清除CR寄存器的PER位，此操作应该在FLASH_PageErase()中完成！
                                      // 但是HAL库里面并没有做，应该是HAL库bug！
  if (TypeProgram == TYPEPROGRAM_CHAR)
  {
    for (int var = 0; var < len / 2; var++)
    {
      uint8_t data1 = (uint8_t)*Data;
      uint8_t data2 = (uint8_t) * (Data + 1);
      uint64_t datar = data2 << 8 | data1;
      HAL_FLASH_Program(FLASH_TYPEPROGRAM_HALFWORD, Address, datar);
      Address += 2;
      Data += 2;
    }
  }
  else
  {
    for (int var = 0; var < len; var++)
    {
      HAL_FLASH_Program(TypeProgram, Address, *Data);
      Address += (1 << TypeProgram);
      Data += 1;
    }
  }
  HAL_FLASH_Lock();
}

void Flash_Read(uint32_t TypeProgram, uint32_t Address, uint64_t(*Data), uint32_t len)
{
  int32_t i = 0;
  if (TypeProgram == TYPEPROGRAM_CHAR)
  {
    for (i = 0; i < len; i++, Address += 1, Data++)
    {
      *Data = *(uint8_t *)Address;
    }
  }
  else if (TypeProgram == FLASH_TYPEPROGRAM_HALFWORD)
  {
    for (i = 0; i < len; i++, Address += 2, Data++)
    {
      *Data = *(uint16_t *)Address;
    }
  }
  else if (TypeProgram == TYPEPROGRAM_WORD)
  {
    for (i = 0; i < len; i++, Address += 4, Data++)
    {
      *Data = *(uint32_t *)Address;
    }
  }
  else if (TypeProgram == TYPEPROGRAM_DOUBLEWORD)
  {
    for (i = 0; i < len; i++, Address += 8, Data++)
    {
      *Data = *(uint64_t *)Address;
    }
  }
}

void MY_Flash_Init(void)
{
  uint64_t flash_read_buf[2];
  Flash_Read(FLASH_TYPEPROGRAM_HALFWORD, FLASH_START_ADDR, flash_read_buf, 2); // 从指定页的地址读FLASH
  Fumes_Threshold_L = flash_read_buf[0];
  Fumes_Threshold_H = flash_read_buf[1];
}

void MY_Flash_Write(void)
{
  uint64_t flash_write_buf[2];
  flash_write_buf[0] = Fumes_Threshold_L;
  flash_write_buf[1] = Fumes_Threshold_H;
  Flash_Write(FLASH_TYPEPROGRAM_HALFWORD, FLASH_START_ADDR, flash_write_buf, 2);
}