#include "general.h"
#include "hal.h"
#include "esp_adc/adc_oneshot.h"
#include "esp_rom_sys.h"
#include "esp_timer.h"

// ============================================
//  Internal state
// ============================================
static adc_oneshot_unit_handle_t adc1_handle;

// ============================================
//  hInitIO
// ============================================
tByte hInitIO(void) {
    // --- Button (input, pull-up) ---
    gpio_reset_pin(PIN_TASTER);
    gpio_set_direction(PIN_TASTER, GPIO_MODE_INPUT);
    gpio_set_pull_mode(PIN_TASTER, GPIO_PULLUP_ONLY);

    // --- Relay (output, start OFF) ---
    gpio_reset_pin(PIN_RELAY_1);
    gpio_set_direction(PIN_RELAY_1, GPIO_MODE_OUTPUT);
    gpio_set_level(PIN_RELAY_1, 0);

    // --- Status LED (output, start OFF) ---
    gpio_reset_pin(PIN_STATUS_LED);
    gpio_set_direction(PIN_STATUS_LED, GPIO_MODE_OUTPUT);
    gpio_set_level(PIN_STATUS_LED, 0);

    // --- HC-SR04 ---
    gpio_reset_pin(PIN_HCSR04_TRIG);
    gpio_set_direction(PIN_HCSR04_TRIG, GPIO_MODE_OUTPUT);
    gpio_set_level(PIN_HCSR04_TRIG, 0);

    gpio_reset_pin(PIN_HCSR04_ECHO);
    gpio_set_direction(PIN_HCSR04_ECHO, GPIO_MODE_INPUT);

    // --- DHT22 (configured at read time, starts as input) ---
    gpio_reset_pin(PIN_DHT22_DATA);
    gpio_set_direction(PIN_DHT22_DATA, GPIO_MODE_INPUT);
    gpio_set_pull_mode(PIN_DHT22_DATA, GPIO_PULLUP_ONLY);

    // --- Potentiometer (ADC1) ---
    adc_oneshot_unit_init_cfg_t adc_cfg = {
        .unit_id = ADC_UNIT_1,
    };
    adc_oneshot_new_unit(&adc_cfg, &adc1_handle);

    adc_oneshot_chan_cfg_t chan_cfg = {
        .atten = ADC_ATTEN_DB_12,   // 0–3.3V range
        .bitwidth = ADC_BITWIDTH_12,
    };
    adc_oneshot_config_channel(adc1_handle, PIN_POTI_ADC_CHANNEL, &chan_cfg);

    // --- I2C for OLED is initialized separately ---

    return 0;
}

// ============================================
//  HC-SR04 helper – measure distance in cm
// ============================================
static float hcsr04_read_cm(void) {
    // Send 10µs trigger pulse
    gpio_set_level(PIN_HCSR04_TRIG, 1);
    esp_rom_delay_us(10);
    gpio_set_level(PIN_HCSR04_TRIG, 0);

    // Wait for echo HIGH
    int64_t timeout = esp_timer_get_time() + 30000; // 30ms timeout
    while (gpio_get_level(PIN_HCSR04_ECHO) == 0) {
        if (esp_timer_get_time() > timeout) return -1.0f;
    }

    int64_t start = esp_timer_get_time();

    // Wait for echo LOW
    timeout = start + 30000;
    while (gpio_get_level(PIN_HCSR04_ECHO) == 1) {
        if (esp_timer_get_time() > timeout) return -1.0f;
    }

    int64_t duration = esp_timer_get_time() - start;

    // Speed of sound ≈ 343 m/s → distance = duration / 58.0 (in cm)
    return (float)duration / 58.0f;
}

// ============================================
//  DHT22 helper – simplified read
//  (In production, use a proper library)
// ============================================
static tByte dht22_read(float* temp, float* hum) {
    // Placeholder – use a DHT library for ESP-IDF
    // e.g. esp-idf-lib DHT component
    *temp = 0.0f;
    *hum = 0.0f;
    return 1;  // 1 = not implemented yet
}

// ============================================
//  readInputs
// ============================================
void readInputs(inpVector* inVec) {
    // Button (active LOW with pull-up)
    inVec->taster = !gpio_get_level(PIN_TASTER);

    // Potentiometer
    int raw = 0;
    adc_oneshot_read(adc1_handle, PIN_POTI_ADC_CHANNEL, &raw);
    inVec->poti = (uint16_t)raw;

    // DHT22
    dht22_read(&inVec->temperature, &inVec->humidity);

    // HC-SR04 water level
    inVec->waterLevel_cm = hcsr04_read_cm();
}

// ============================================
//  writeOutputs
// ============================================
void writeOutputs(outpVector* outVec) {
    gpio_set_level(PIN_RELAY_1, outVec->relay_1);
    gpio_set_level(PIN_STATUS_LED, outVec->statusLed);
}

// ============================================
//  readByte / writeByte – for future expansion
// ============================================
tByte readByte(const gpio_num_t pinGroup[8]) {
    tByte result = 0;
    for (int bit = 0; bit < 8; bit++) {
        if (gpio_get_level(pinGroup[bit])) {
            result |= (1 << bit);
        }
    }
    return result;
}

void writeByte(const gpio_num_t pinGroup[8], tByte value) {
    for (int bit = 0; bit < 8; bit++) {
        gpio_set_level(pinGroup[bit], (value >> bit) & 0x01);
    }
}
