/* 
 * File:   func.h
 * Project: TipID
 * 
 */

#ifndef __FUNC_H
#define	__FUNC_H

// Exported includes:
#include <stdint.h>


/******************************************************************************
 * Configuration
 ******************************************************************************/
// Tip IDs:
typedef enum
{
    FOCUS               = 1,    // {1 - 9}
    PIXEL_694           = 10,
    PIXEL_1064          = 11,   // {11 - 15}
    VASQULAR            = 16,   // {16 - 24}
    PEN_LIKE            = 25    // {25 - 27}
} tip_id_t;

//*****************************************************************************

// -> Choose a constant  base tip ID: FOCUS,PIXEL_694,PIXEL_1064,VASQULAR,PEN_LIKE
#define SELECT_TIPID    PEN_LIKE

// Uncomment for the new PCB 2.0 (tested with FemiLift):
#define NEW_PCB

//*****************************************************************************

// Optocouplers:
#define OPTO_VOLTAGE        1.5 // Optocoupler threshold, Volts (double)

// Temperature sensor:
#define TEMP_VOLTAGE        2.5 // Temperature threshold, Volts (double)

// Tip IDs to send with temperature sensor:
#define ID_TEMP_LOW         13  // Value to send below temp. threshold
#define ID_TEMP_HIGH        15  // Value to send above temp. threshold

// ADC configuration:
#define ADC_CHANS           4   // Number of channels to read (usually 4)
#define ADC_HYST            2   // Hysteresis, ADC steps above & below threshold

// 1.0 ms delay - change to fine-tune the pulse frequency:
#define DELAY_MS            150

//*****************************************************************************

#ifdef  NEW_PCB
// Output pin: GP5/TRISA5 for new PCB
#define OUT_PIN             GPIObits.GP5
#define OUT_PIN_DIR         TRISIObits.TRISA5
// Invert signal polarity for the new PCB:
#define PIN_HIGH            0
#define PIN_LOW             1

#else // OLD PCB
// Output pin: GP4/TRISA4 for old FemiLift/Smart
#define OUT_PIN             GPIObits.GP4
#define OUT_PIN_DIR         TRISIObits.TRISA4
// Do NOT invert signal polarity for the old PCB:
#define PIN_HIGH            1
#define PIN_LOW             0
#endif


/******************************************************************************
 * General definitions
 ******************************************************************************/
#define DIR_OUTPUT  0
#define DIR_INPUT   1


/******************************************************************************
 * Macros
 ******************************************************************************/
#define SendBit1()  do {    \
        OUT_PIN = PIN_HIGH; \
        Delay_ms(1);        \
        OUT_PIN = PIN_LOW;  \
        Delay_ms(1);        \
    } while (0)

#define SendBit0()  do {    \
        OUT_PIN = PIN_HIGH; \
        Delay_ms(1);        \
        OUT_PIN = PIN_LOW;  \
        Delay_ms(2);        \
    } while (0)


/******************************************************************************
 * Exported functions
 ******************************************************************************/
void ADC_InitThreshold(double value);
void ADC_Init(void);
uint8_t ADC_ReadChannel(uint8_t channel);
void ADC_ReadChannels(uint8_t first_ch, uint8_t last_ch, uint8_t* result);
void Delay_ms(unsigned int ms);
void Send_Sequence(uint8_t val, uint8_t bits);


#endif	/* __FUNC_H */

