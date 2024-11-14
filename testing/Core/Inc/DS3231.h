/*
 * DS3231.h
 *
 *  Created on: Nov 9, 2024
 *      Author: otaku
 */

#ifndef INC_DS3231_H_
#define INC_DS3231_H_

#include "stm32f1xx_hal.h" // Đảm bảo rằng bạn đã bao gồm thư viện HAL STM32

// Địa chỉ I2C của DS3231
#define DS3231_ADDRESS 0xD0

// Định nghĩa các thanh ghi của DS3231
#define DS3231_REG_TIME 0x00
#define DS3231_REG_DATE 0x04

// Cấu trúc lưu trữ thời gian
typedef struct
{
    uint8_t sec;    // Giây
    uint8_t min;    // Phút
    uint8_t hour;   // Giờ
    uint8_t day;    // Ngày
    uint8_t month;  // Tháng
    uint8_t year;   // Năm
} DS3231_TimeTypeDef;

// Hàm khởi tạo DS3231
HAL_StatusTypeDef DS3231_Init(I2C_HandleTypeDef *hi2c);

// Hàm đọc thời gian từ DS3231
HAL_StatusTypeDef DS3231_GetTime(I2C_HandleTypeDef *hi2c, DS3231_TimeTypeDef *time);

// Hàm ghi thời gian vào DS3231
HAL_StatusTypeDef DS3231_SetTime(I2C_HandleTypeDef *hi2c, DS3231_TimeTypeDef *time);


#endif /* INC_DS3231_H_ */
