/**
 * @headerfile      General
 * @brief           File with common includes and definitions
 *
 * @details         This header provides common standard library includes used
 *                  throughout the labyrinth project.
 *
*/
#pragma once

#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>

#include "driver/gpio.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_adc/adc_oneshot.h"


// ============================================
//  PIN MAPPING – Steuerungsplatine (ESP32-S3)
// ============================================

// --- Sensors (Inputs) ---
#define PIN_DHT22_DATA       GPIO_NUM_4      // DHT22 data line

#define PIN_HCSR04_TRIG      GPIO_NUM_5      // Ultrasonic trigger (output)
#define PIN_HCSR04_ECHO      GPIO_NUM_6      // Ultrasonic echo   (input)

#define PIN_TASTER           GPIO_NUM_7      // Button (digital input)
#define PIN_POTI             GPIO_NUM_2      // Potentiometer (ADC1_CH1)
#define PIN_POTI_ADC_CHANNEL ADC_CHANNEL_1

// --- Actuators (Outputs) ---
#define PIN_RELAY_1          GPIO_NUM_15     // Relay → LED Licht (mock)
#define PIN_STATUS_LED       GPIO_NUM_1      // Status LED direct

// --- OLED Display (I2C) ---
#define PIN_OLED_SDA         GPIO_NUM_11     // I2C Data
#define PIN_OLED_SCL         GPIO_NUM_12     // I2C Clock
#define OLED_I2C_ADDR        0x3C            // Typical SSD1306 address

typedef uint8_t tByte;

/*    StateMachine Inputs                */
typedef struct {
    bool    taster;
    uint16_t poti;          // 0–4095 (12-bit ADC)
    float   temperature;
    float   humidity;
    float   waterLevel_cm;
} inpVector;

/*    StateMachine Outputs               */
typedef struct {
    bool relay_1;
    bool statusLed;
} outpVector;
