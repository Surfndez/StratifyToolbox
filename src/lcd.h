/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __LCD_H
#define __LCD_H

#include <mcu/types.h>

void LCD_IO_Init();
void LCD_IO_WriteMultipleData(u8 *pData, u32 Size);
void LCD_IO_WriteReg(u8 Reg);
void LCD_IO_WriteDataBlock(const u8 * data, int nbyte);
void LCD_IO_WriteData8(u8 RegValue);
void LCD_IO_WriteData(u16 RegValue);
u16 LCD_IO_ReadData();
void LCD_IO_Delay(u32 delay);
void LCD_IO_Assert_CS();
void LCD_IO_Deassert_CS();

#endif /* __LCD_H */

