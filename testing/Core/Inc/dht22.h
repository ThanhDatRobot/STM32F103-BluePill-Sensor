/*
 * dht22.h
 *
 *  Created on: Oct 23, 2024
 *      Author: otaku
 */

#ifndef INC_DHT22_H_
#define INC_DHT22_H_

#include "main.h"

// Define the port and pin connected to the DHT22
#define DHT22_PORT GPIOB
#define DHT22_PIN GPIO_PIN_9

// Function prototypes
void DHT22_Init(TIM_HandleTypeDef *htim);
uint8_t DHT22_Start(void);
uint8_t DHT22_Read(void);
uint8_t DHT22_ReadData(float *temperature, float *humidity);

#endif /* INC_DHT22_H_ */
