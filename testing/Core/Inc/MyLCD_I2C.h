/******************************************************************************************************************
@File:  	MyLCD_I2C.h
@Author:  	Mai Thanh Dat
@Github:	https://github.com/ThanhDatRobot
******************************************************************************************************************/

#ifndef __MYLCD_I2C_H
#define __MYLCD_I2C_H

#include "stm32f1xx_hal.h"

// Define modes and control bits
#define MYLCD_COMMAND 0
#define MYLCD_DATA    1

// Backlight control
#define MYLCD_BACKLIGHT    0x08
#define MYLCD_NOBACKLIGHT  0x00

// LCD commands
#define MYLCD_CLEARDISPLAY 0x01
#define MYLCD_RETURNHOME   0x02
#define MYLCD_ENTRYMODESET 0x04
#define MYLCD_DISPLAYCONTROL 0x08
#define MYLCD_CURSORSHIFT  0x10
#define MYLCD_FUNCTIONSET  0x20
#define MYLCD_SETDDRAMADDR 0x80

// Control flags for display entry mode
#define MYLCD_ENTRYLEFT           0x02
#define MYLCD_ENTRYSHIFTINCREMENT 0x01
#define MYLCD_ENTRYSHIFTDECREMENT 0x00

// Display on/off control flags
#define MYLCD_DISPLAYON   0x04
#define MYLCD_DISPLAYOFF  0x00
#define MYLCD_CURSORON    0x02
#define MYLCD_CURSOROFF   0x00
#define MYLCD_BLINKON     0x01
#define MYLCD_BLINKOFF    0x00

// Function set flags
#define MYLCD_8BITMODE    0x10
#define MYLCD_4BITMODE    0x00
#define MYLCD_2LINE       0x08
#define MYLCD_1LINE       0x00
#define MYLCD_5x10DOTS    0x04
#define MYLCD_5x8DOTS     0x00

// Structure to store LCD parameters
typedef struct {
	I2C_HandleTypeDef* I2C;
	uint8_t ADDRESS;
	uint8_t COLUMNS;
	uint8_t ROWS;
	uint8_t ENTRYMODE;
	uint8_t DISPLAYCTRL;
	uint8_t CURSORSHIFT;
	uint8_t FUNCTIONSET;
	uint8_t BACKLIGHT;
} MyLCD_I2C;

void MyLCD_Init(MyLCD_I2C* LCD, I2C_HandleTypeDef* hi2c_LCD, uint8_t Address, uint8_t Columns, uint8_t Rows);
void MyLCD_SetCursor(MyLCD_I2C* LCD, uint8_t Xpos, uint8_t Ypos);
void MyLCD_WriteChar(MyLCD_I2C* LCD, char character);
void MyLCD_WriteString(MyLCD_I2C* LCD, char *String);
void MyLCD_Clear(MyLCD_I2C* LCD);
void MyLCD_ReturnHome(MyLCD_I2C* LCD);
void MyLCD_CursorOn(MyLCD_I2C* LCD);
void MyLCD_CursorOff(MyLCD_I2C* LCD);
void MyLCD_BlinkOn(MyLCD_I2C* LCD);
void MyLCD_BlinkOff(MyLCD_I2C* LCD);
void MyLCD_SetBacklight(MyLCD_I2C* LCD, uint8_t state);

#endif /* __MYLCD_I2C_H */
