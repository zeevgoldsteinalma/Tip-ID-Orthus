/* 
 * File:   func.c
 * Project: TipID
 * 
 */

#include "func.h"
#include <xc.h>
#include <math.h>

/******************************************************************************
 * Static variables
 ******************************************************************************/
uint8_t ThrHigh, ThrLow;

//*****************************************************************************
void ADC_InitThreshold(double value)
{
    double th_val  = value * 255.0/5.0; // Threshold: 0..5V -> 0..255
    ThrHigh = (uint8_t)(th_val) + ADC_HYST;   // High;
    ThrLow  = (uint8_t)(th_val) - ADC_HYST;   // Low;
}

//*****************************************************************************
void ADC_Init(void)
{
    ANSELbits.ADCS  = 0;        // Fosc/2
    ADCON0bits.ADFM = 0;        // Format: left-justified
    ADCON0bits.VCFG = 0;        // Reference: Vdd
    ADCON0bits.CHS  = 0;        // Select ANS0 (GP0)
}

//*****************************************************************************
uint8_t ADC_ReadChannel(uint8_t channel)
{
    ADCON0bits.ADON = 1;            // Turn ON the ADC
    ADCON0bits.CHS  = channel;      // Select ADC channel
    Delay_ms(1);                    // Delay - NOT required?
    ADCON0bits.GO_nDONE = 1;        // Start conversion
    while (ADCON0bits.GO_nDONE);    // Wait until done
    uint8_t a_val = ADRESH;         // Read the HIGH 8 BITS only!
    ADCON0bits.ADON = 0;            // Turn OFF the ADC
    return a_val;
}

//*****************************************************************************
void ADC_ReadChannels(uint8_t first_ch, uint8_t last_ch, uint8_t *result)
{
    for (uint8_t i=first_ch; i<=last_ch; i++)
    {
        uint8_t a_val = ADC_ReadChannel(i); // Measure analog value

        // Compare to threshold values, convert to 0 or 1 (inverted).
        // NOTE: if the value is between thresholds, result doesn't change!
        if (a_val >= ThrHigh)
            result[i] = 0;
        else if (a_val <= ThrLow)
            result[i] = 1;
   }
}

//*****************************************************************************
void Delay_ms(unsigned int ms)
{
    unsigned int del = ms*DELAY_MS;
    while (--del);
}

//*****************************************************************************
void Send_Sequence(uint8_t val, uint8_t bits)
{
    SendBit1();     // Start
    Delay_ms(10);

    // Send bits from LSB to MSB:
    for (uint8_t i=0; i<bits; i++)
    {
        if (val & 0x01)
            SendBit1();
        else
            SendBit0();
        val >>= 1;
    }
    SendBit1();     // Stop
}

//*****************************************************************************
