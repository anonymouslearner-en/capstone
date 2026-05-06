/**
 * @name            Digitaler Prototyp | Capstone 2026
 *
 * @brief           Berechnet Binomialkoeffizienten mit vereinfachtem Algorithmus
 *
 * @note            Nach Projektvorgabe und Verfügbarkeit der Wokwi-Bauteile
 */


#include "general.h"
#include "hal.h"

typedef enum {
    STATE_LED_OFF = 0,
    STATE_LED_ON
} appState_t;


 /**  ▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓
                     ▶ HAUPTPROGRAMM ◀
 ▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓ **/

void app_main(void) {
    tByte retVal = 0;
    outpVector outputs = {0};
    appState_t state = STATE_LED_OFF;

    /*    [1] Initialisation              */
    // Hardware initialisieren
    retVal |= hInitIO();
    // Automat in den Startzustand setzen
    writeOutputs(&outputs);

    /** ==================================================================
                      [2] Main State Machine Loop ⬇
    ================================================================== **/
    while (retVal == 0) {
        switch (state) {
            case STATE_LED_OFF:
                outputs.statusLed = false;
                outputs.relay_1 = false;
                writeOutputs(&outputs);
                vTaskDelay(pdMS_TO_TICKS(500));
                state = STATE_LED_ON;
                break;

            case STATE_LED_ON:
                outputs.statusLed = true;
                outputs.relay_1 = false;
                writeOutputs(&outputs);
                vTaskDelay(pdMS_TO_TICKS(500));
                state = STATE_LED_OFF;
                break;

            default:
                retVal = 1;
                break;
        }
    }

    /** ==================================================================
                      [3] Error Handling ⬇
    ================================================================== **/
    while (1) {

    }

}
