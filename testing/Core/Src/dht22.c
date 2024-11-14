/*
 * dht22.c
 *
 *  Created on: Oct 23, 2024
 *      Author: otaku
 */

#include "dht22.h"

static TIM_HandleTypeDef *DHT22_htim;

uint8_t RH1, RH2, TC1, TC2, SUM, CHECK;
uint32_t pMillis, cMillis;
float tCelsius = 0;
float RH = 0;

// Delay function using the hardware timer
void microDelay (uint16_t delay)
{
    __HAL_TIM_SET_COUNTER(DHT22_htim, 0);
    while (__HAL_TIM_GET_COUNTER(DHT22_htim) < delay);
}

// Initialize DHT22 with the timer handle
void DHT22_Init(TIM_HandleTypeDef *htim)
{
    DHT22_htim = htim;
    HAL_TIM_Base_Start(DHT22_htim);
}

uint8_t DHT22_Start(void)
{
    uint8_t Response = 0;
    GPIO_InitTypeDef GPIO_InitStructPrivate = {0};
    GPIO_InitStructPrivate.Pin = DHT22_PIN;
    GPIO_InitStructPrivate.Mode = GPIO_MODE_OUTPUT_PP;
    GPIO_InitStructPrivate.Speed = GPIO_SPEED_FREQ_LOW;
    GPIO_InitStructPrivate.Pull = GPIO_NOPULL;
    HAL_GPIO_Init(DHT22_PORT, &GPIO_InitStructPrivate); // set the pin as output
    HAL_GPIO_WritePin(DHT22_PORT, DHT22_PIN, 0);   // pull the pin low
    microDelay(1300);   // wait for 1300us
    HAL_GPIO_WritePin(DHT22_PORT, DHT22_PIN, 1);   // pull the pin high
    microDelay(30);   // wait for 30us
    GPIO_InitStructPrivate.Mode = GPIO_MODE_INPUT;
    GPIO_InitStructPrivate.Pull = GPIO_PULLUP;
    HAL_GPIO_Init(DHT22_PORT, &GPIO_InitStructPrivate); // set the pin as input
    microDelay(40);
    if (!(HAL_GPIO_ReadPin(DHT22_PORT, DHT22_PIN)))
    {
        microDelay(80);
        if ((HAL_GPIO_ReadPin(DHT22_PORT, DHT22_PIN))) Response = 1;
    }
    pMillis = HAL_GetTick();
    cMillis = HAL_GetTick();
    while ((HAL_GPIO_ReadPin(DHT22_PORT, DHT22_PIN)) && pMillis + 2 > cMillis)
    {
        cMillis = HAL_GetTick();
    }
    return Response;
}

uint8_t DHT22_Read(void)
{
    uint8_t a, b = 0;
    for (a = 0; a < 8; a++)
    {
        pMillis = HAL_GetTick();
        cMillis = HAL_GetTick();
        while (!(HAL_GPIO_ReadPin(DHT22_PORT, DHT22_PIN)) && pMillis + 2 > cMillis)
        {
            cMillis = HAL_GetTick();
        }
        microDelay(40);   // wait for 40 us
        if (!(HAL_GPIO_ReadPin(DHT22_PORT, DHT22_PIN)))   // if the pin is low
            b &= ~(1 << (7 - a));
        else
            b |= (1 << (7 - a));
        pMillis = HAL_GetTick();
        cMillis = HAL_GetTick();
        while ((HAL_GPIO_ReadPin(DHT22_PORT, DHT22_PIN)) && pMillis + 2 > cMillis)
        {
            cMillis = HAL_GetTick();
        }
    }
    return b;
}




// Hàm đọc cả nhiệt độ và độ ẩm từ DHT22
uint8_t DHT22_ReadData(float *temperature, float *humidity)
{
    if (DHT22_Start())
    {
        RH1 = DHT22_Read(); // First 8 bits of humidity
        RH2 = DHT22_Read(); // Second 8 bits of humidity
        TC1 = DHT22_Read(); // First 8 bits of temperature
        TC2 = DHT22_Read(); // Second 8 bits of temperature
        SUM = DHT22_Read(); // Checksum byte

        CHECK = RH1 + RH2 + TC1 + TC2;
        if (CHECK == SUM)
        {
            // Tính độ ẩm
            *humidity = (float)((RH1 << 8) | RH2) / 10;

            // Tính nhiệt độ
            if (TC1 > 127) // Kiểm tra bit dấu để xác định nhiệt độ âm
            {
                *temperature = (float)TC2 / 10 * (-1);
            }
            else
            {
                *temperature = (float)((TC1 << 8) | TC2) / 10;
            }
            return 1;  // Thành công
        }
        else
        {
            return 0;  // Sai checksum
        }
    }
    return 0;  // Không kết nối được cảm biến
}
