/*
 * DS3231.c
 *
 *  Created on: Nov 9, 2024
 *      Author: otaku
 */

/*
 * DS3231.c
 *
 * Thư viện này cung cấp các hàm để giao tiếp với module đồng hồ thực DS3231 thông qua giao thức I2C.
 * Bao gồm các chức năng để khởi tạo, đọc và ghi thời gian vào DS3231.
 *
 * Các hàm chính trong thư viện:
 *
 * 1. DS3231_Init:
 *    - Hàm này khởi tạo module DS3231 và đặt tất cả các giá trị mặc định của đồng hồ, ví dụ như giờ và phút ban đầu bằng 0.
 *    - Sử dụng I2C để truyền dữ liệu khởi tạo đến DS3231.
 *
 * 2. DS3231_GetTime:
 *    - Hàm này đọc dữ liệu thời gian từ DS3231 và lưu vào cấu trúc DS3231_TimeTypeDef.
 *    - Dữ liệu thời gian bao gồm giây, phút, giờ, ngày, tháng và năm.
 *    - Dữ liệu từ DS3231 được lưu dưới dạng BCD, vì vậy cần chuyển đổi về dạng giá trị thực tế (ví dụ: từ BCD sang decimal).
 *    - Trả về trạng thái của phép đọc (HAL_OK hoặc lỗi).
 *
 * 3. DS3231_SetTime:
 *    - Hàm này ghi thời gian vào DS3231.
 *    - Thời gian cần được chuyển đổi từ dạng decimal sang BCD trước khi ghi vào các thanh ghi của DS3231.
 *    - Bao gồm các trường: giây, phút, giờ, ngày, tháng, và năm.
 *    - Trả về trạng thái của phép ghi (HAL_OK hoặc lỗi).
 *
 * Các thanh ghi và địa chỉ của DS3231:
 * - DS3231_ADDRESS: Địa chỉ I2C của DS3231 (thường là 0x68).
 * - DS3231_REG_TIME: Thanh ghi bắt đầu của thời gian (bao gồm giây, phút, giờ, ngày, tháng và năm).
 *
 * Ghi chú:
 * - Hàm DS3231_GetTime yêu cầu truyền một con trỏ đến cấu trúc DS3231_TimeTypeDef để lưu dữ liệu thời gian.
 * - DS3231_SetTime yêu cầu một cấu trúc DS3231_TimeTypeDef chứa giá trị thời gian cần thiết.
 * - Các phép toán chuyển đổi BCD sang decimal và ngược lại được thực hiện trong các hàm này để đảm bảo dữ liệu đúng định dạng khi truyền qua I2C.
 	 Cách dùng, ví dụ mẫu:
 	 DS3231_TimeTypeDef time;

int main(void)
{
    // Khởi tạo HAL
    HAL_Init();

    // Khởi tạo I2C (giả sử bạn đã cấu hình I2C trên STM32CubeMX)
    MX_I2C1_Init();  // Hàm này được sinh ra bởi STM32CubeMX

    // Khởi tạo DS3231
    if (DS3231_Init(&hi2c1) != HAL_OK)
    {
        // Lỗi khi khởi tạo DS3231
        Error_Handler();
    }

    // Đặt thời gian cho DS3231 (ví dụ: 12:30:45, ngày 9 tháng 11 năm 2024)
    time.sec = 45;
    time.min = 30;
    time.hour = 12;
    time.day = 9;
    time.month = 11;
    time.year = 24;  // Năm 2024 (chỉ sử dụng 2 chữ số cuối)

    if (DS3231_SetTime(&hi2c1, &time) != HAL_OK)
    {
        // Lỗi khi ghi thời gian
        Error_Handler();
    }

    // Đọc lại thời gian từ DS3231
    if (DS3231_GetTime(&hi2c1, &time) != HAL_OK)
    {
        // Lỗi khi đọc thời gian
        Error_Handler();
    }

    // Hiển thị thời gian đã đọc được (giả sử bạn sử dụng UART để debug)
    printf("Current time: %02d:%02d:%02d, Date: %02d/%02d/%02d\n",
           time.hour, time.min, time.sec, time.day, time.month, time.year);

    // Tiếp tục vòng lặp chính
    while (1)
    {
        // Các tác vụ khác của chương trình
    }
}

// Hàm xử lý lỗi (được gọi khi có lỗi trong khởi tạo hoặc giao tiếp)
void Error_Handler(void)
{
    // Chạy mã xử lý lỗi, ví dụ như nháy LED hoặc đưa ra thông báo
    while (1)
    {
        // Đèn LED nháy hoặc chờ xử lý
    }
}

 */

#include "ds3231.h"

// Hàm khởi tạo DS3231
HAL_StatusTypeDef DS3231_Init(I2C_HandleTypeDef *hi2c)
{
    uint8_t data[2] = {0x00, 0x00}; // Đặt tất cả giá trị mặc định (ví dụ như giờ và phút bằng 0)
    return HAL_I2C_Master_Transmit(hi2c, DS3231_ADDRESS, data, sizeof(data), HAL_MAX_DELAY);
}

// Hàm đọc thời gian từ DS3231
HAL_StatusTypeDef DS3231_GetTime(I2C_HandleTypeDef *hi2c, DS3231_TimeTypeDef *time)
{
    uint8_t data[7];

    // Đọc 7 byte dữ liệu từ thanh ghi TIME
    HAL_StatusTypeDef status = HAL_I2C_Mem_Read(hi2c, DS3231_ADDRESS, DS3231_REG_TIME, I2C_MEMADD_SIZE_8BIT, data, 7, HAL_MAX_DELAY);

    if (status != HAL_OK)
        return status;

    // Chuyển đổi dữ liệu BCD thành giá trị thực tế
    time->sec = ((data[0] & 0xF0) >> 4) * 10 + (data[0] & 0x0F);
    time->min = ((data[1] & 0xF0) >> 4) * 10 + (data[1] & 0x0F);
    time->hour = ((data[2] & 0xF0) >> 4) * 10 + (data[2] & 0x0F);
    time->day = ((data[3] & 0xF0) >> 4) * 10 + (data[3] & 0x0F);
    time->month = ((data[5] & 0xF0) >> 4) * 10 + (data[5] & 0x0F);
    time->year = ((data[6] & 0xF0) >> 4) * 10 + (data[6] & 0x0F);

    return HAL_OK;
}

// Hàm ghi thời gian vào DS3231
HAL_StatusTypeDef DS3231_SetTime(I2C_HandleTypeDef *hi2c, DS3231_TimeTypeDef *time)
{
    uint8_t data[7];

    // Chuyển đổi giá trị thực tế thành BCD
    data[0] = ((time->sec / 10) << 4) | (time->sec % 10);
    data[1] = ((time->min / 10) << 4) | (time->min % 10);
    data[2] = ((time->hour / 10) << 4) | (time->hour % 10);
    data[3] = ((time->day / 10) << 4) | (time->day % 10);
    data[4] = 0x00; // Để trống cho thanh ghi tháng (có thể không cần thiết)
    data[5] = ((time->month / 10) << 4) | (time->month % 10);
    data[6] = ((time->year / 10) << 4) | (time->year % 10);

    return HAL_I2C_Mem_Write(hi2c, DS3231_ADDRESS, DS3231_REG_TIME, I2C_MEMADD_SIZE_8BIT, data, 7, HAL_MAX_DELAY);
}



