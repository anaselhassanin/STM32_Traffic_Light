#include "lcd.h"
#include "stm32f1xx_hal.h"
#include "string.h"

static void LCD_Enable(void);
static void LCD_Send4Bits(uint8_t data);

void LCD_Init(void)
{
  HAL_Delay(50);
  LCD_SendCommand(0x33);
  LCD_SendCommand(0x32);
  LCD_SendCommand(0x28);
  LCD_SendCommand(0x0C);
  LCD_SendCommand(0x06);
  LCD_Clear();
}

void LCD_SendCommand(uint8_t cmd)
{
  HAL_GPIO_WritePin(LCD_RS_GPIO_Port, LCD_RS_Pin, GPIO_PIN_RESET);
  LCD_Send4Bits(cmd >> 4);
  LCD_Send4Bits(cmd & 0x0F);
  HAL_Delay(2);
}

void LCD_SendData(uint8_t data)
{
  HAL_GPIO_WritePin(LCD_RS_GPIO_Port, LCD_RS_Pin, GPIO_PIN_SET);
  LCD_Send4Bits(data >> 4);
  LCD_Send4Bits(data & 0x0F);
  HAL_Delay(2);
}

void LCD_Print(char *str)
{
  while (*str) {
    LCD_SendData(*str++);
  }
}

void LCD_Clear(void)
{
  LCD_SendCommand(0x01);
  HAL_Delay(2);
}

void LCD_SetCursor(uint8_t row, uint8_t col)
{
  uint8_t addr = (row == 0) ? 0x80 : 0xC0;
  addr += col;
  LCD_SendCommand(addr);
}

static void LCD_Enable(void)
{
  HAL_GPIO_WritePin(LCD_EN_GPIO_Port, LCD_EN_Pin, GPIO_PIN_SET);
  HAL_Delay(1);
  HAL_GPIO_WritePin(LCD_EN_GPIO_Port, LCD_EN_Pin, GPIO_PIN_RESET);
  HAL_Delay(1);
}

static void LCD_Send4Bits(uint8_t data)
{
  HAL_GPIO_WritePin(LCD_D4_GPIO_Port, LCD_D4_Pin, (data & 0x01) ? GPIO_PIN_SET : GPIO_PIN_RESET);
  HAL_GPIO_WritePin(LCD_D5_GPIO_Port, LCD_D5_Pin, (data & 0x02) ? GPIO_PIN_SET : GPIO_PIN_RESET);
  HAL_GPIO_WritePin(LCD_D6_GPIO_Port, LCD_D6_Pin, (data & 0x04) ? GPIO_PIN_SET : GPIO_PIN_RESET);
  HAL_GPIO_WritePin(LCD_D7_GPIO_Port, LCD_D7_Pin, (data & 0x08) ? GPIO_PIN_SET : GPIO_PIN_RESET);
  LCD_Enable();
}
