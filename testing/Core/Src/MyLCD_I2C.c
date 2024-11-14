/******************************************************************************************************************
@File:  	MyLCD_I2C.c
@Author:  	Mai Thanh Dat
@Github:	https://github.com/ThanhDatRobot
******************************************************************************************************************/

#include "MyLCD_I2C.h"

//************************** Low Level Functions ***************************************************************//
static void MyLCD_Delay(uint16_t Time)
{
	HAL_Delay(Time);
}

static void MyLCD_WriteI2C(MyLCD_I2C* LCD, uint8_t Data, uint8_t Mode)
{
	char Data_H;
	char Data_L;
	uint8_t Data_I2C[4];
	Data_H = Data & 0xF0;
	Data_L = (Data << 4) & 0xF0;

	if (LCD->BACKLIGHT)
	{
		Data_H |= MYLCD_BACKLIGHT;
		Data_L |= MYLCD_BACKLIGHT;
	}

	if (Mode == MYLCD_DATA)
	{
		Data_H |= 0x01;  // RS = 1 for data
		Data_L |= 0x01;
	}
	else
	{
		Data_H &= ~0x01; // RS = 0 for command
		Data_L &= ~0x01;
	}

	Data_I2C[0] = Data_H | 0x04;  // EN = 1
	MyLCD_Delay(1);
	Data_I2C[1] = Data_H & ~0x04; // EN = 0
	Data_I2C[2] = Data_L | 0x04;  // EN = 1
	MyLCD_Delay(1);
	Data_I2C[3] = Data_L & ~0x04; // EN = 0

	HAL_I2C_Master_Transmit(LCD->I2C, LCD->ADDRESS, Data_I2C, 4, 1000);
}

//************************** High Level Functions ***************************************************************//
void MyLCD_Init(MyLCD_I2C* LCD, I2C_HandleTypeDef* hi2c_LCD, uint8_t Address, uint8_t Columns, uint8_t Rows)
{
	LCD->I2C = hi2c_LCD;
	LCD->ADDRESS = Address;
	LCD->COLUMNS = Columns;
	LCD->ROWS = Rows;
	
	LCD->FUNCTIONSET = MYLCD_FUNCTIONSET | MYLCD_4BITMODE | MYLCD_2LINE | MYLCD_5x8DOTS;
	LCD->ENTRYMODE = MYLCD_ENTRYMODESET | MYLCD_ENTRYLEFT | MYLCD_ENTRYSHIFTDECREMENT;
	LCD->DISPLAYCTRL = MYLCD_DISPLAYCONTROL | MYLCD_DISPLAYON | MYLCD_CURSOROFF | MYLCD_BLINKOFF;
	LCD->CURSORSHIFT = MYLCD_CURSORSHIFT | 0x00;  // Cursor shift with no movement
	LCD->BACKLIGHT = MYLCD_BACKLIGHT;

	MyLCD_Delay(50);
	MyLCD_WriteI2C(LCD, 0x33, MYLCD_COMMAND);
	MyLCD_WriteI2C(LCD, 0x32, MYLCD_COMMAND);
	MyLCD_WriteI2C(LCD, 0x28, MYLCD_COMMAND); // 4-bit, 2-line, 5x8 dots

	MyLCD_WriteI2C(LCD, LCD->DISPLAYCTRL, MYLCD_COMMAND);
	MyLCD_WriteI2C(LCD, MYLCD_CLEARDISPLAY, MYLCD_COMMAND);
	MyLCD_Delay(5);
	MyLCD_WriteI2C(LCD, MYLCD_RETURNHOME, MYLCD_COMMAND);
	MyLCD_Delay(5);
}

void MyLCD_SetCursor(MyLCD_I2C* LCD, uint8_t Xpos, uint8_t Ypos)
{
	uint8_t DDRAM_ADDRESS = 0x00;

	if (Xpos >= LCD->COLUMNS) Xpos = LCD->COLUMNS - 1;
	if (Ypos >= LCD->ROWS) Ypos = LCD->ROWS - 1;

	switch (Ypos)
	{
		case 0: DDRAM_ADDRESS = 0x00 + Xpos; break;
		case 1: DDRAM_ADDRESS = 0x40 + Xpos; break;
		case 2: DDRAM_ADDRESS = 0x14 + Xpos; break;
		case 3: DDRAM_ADDRESS = 0x54 + Xpos; break;
	}

	MyLCD_WriteI2C(LCD, MYLCD_SETDDRAMADDR | DDRAM_ADDRESS, MYLCD_COMMAND);
}

void MyLCD_WriteChar(MyLCD_I2C* LCD, char character)
{
	MyLCD_WriteI2C(LCD, character, MYLCD_DATA);
}

void MyLCD_WriteString(MyLCD_I2C* LCD, char *String)
{
	while (*String) MyLCD_WriteChar(LCD, *String++);
}

void MyLCD_Clear(MyLCD_I2C* LCD)
{
	MyLCD_WriteI2C(LCD, MYLCD_CLEARDISPLAY, MYLCD_COMMAND);
	MyLCD_Delay(5);
}

void MyLCD_ReturnHome(MyLCD_I2C* LCD)
{
	MyLCD_WriteI2C(LCD, MYLCD_RETURNHOME, MYLCD_COMMAND);
	MyLCD_Delay(5);
}

void MyLCD_CursorOn(MyLCD_I2C* LCD)
{
	LCD->DISPLAYCTRL |= MYLCD_CURSORON;
	MyLCD_WriteI2C(LCD, LCD->DISPLAYCTRL, MYLCD_COMMAND);
}

void MyLCD_CursorOff(MyLCD_I2C* LCD)
{
	LCD->DISPLAYCTRL &= ~MYLCD_CURSORON;
	MyLCD_WriteI2C(LCD, LCD->DISPLAYCTRL, MYLCD_COMMAND);
}

void MyLCD_BlinkOn(MyLCD_I2C* LCD)
{
	LCD->DISPLAYCTRL |= MYLCD_BLINKON;
	MyLCD_WriteI2C(LCD, LCD->DISPLAYCTRL, MYLCD_COMMAND);
}

void MyLCD_BlinkOff(MyLCD_I2C* LCD)
{
	LCD->DISPLAYCTRL &= ~MYLCD_BLINKON;
	MyLCD_WriteI2C(LCD, LCD->DISPLAYCTRL, MYLCD_COMMAND);
}

void MyLCD_SetBacklight(MyLCD_I2C* LCD, uint8_t state)
{
	LCD->BACKLIGHT = state ? MYLCD_BACKLIGHT : MYLCD_NOBACKLIGHT;
	MyLCD_WriteI2C(LCD, 0, MYLCD_COMMAND);
}
