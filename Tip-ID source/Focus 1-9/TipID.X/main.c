/*
 * File:    main.c
 * Project: TipID
 * Version: TipID_0101_210420_01
 * Updated: 2021-04-20
 * 
 * Revision History
 * ================
 * TipID_0101_210420_01:
 * Added raw sensors variant with tip IDs = {0x0...0xF}.
 * Changed optocouplers threshold from 1.2V to 1.5V.
 * 
 * TipID_0101_201029_01:
 * Added optocouplers variant with tip IDs = {5,6,7,8}
 * 
 * TipID_0101_200728_01:
 * Improved timings; code refactoring.
 * 
 * TipID_0101_191218_01:
 * Initial version.
 * Supports optocouplers, constant IDs (pre-compiled), temperature sensor.
 * 
 */

/******************************************************************************
 * PIC12F617 Configuration Bit Settings
 ******************************************************************************/
#pragma config FOSC = INTOSCIO  // Oscillator Selection bits (INTOSCIO oscillator: I/O function on RA4/AN3/T1G/OSC2/CLKOUT, I/O function on RA5/T1CKI/OSC1/CLKIN)
#pragma config WDTE = OFF       // Watchdog Timer Enable bit (WDT disabled and can be enabled by SWDTEN bit of the WDTCON register)
#pragma config PWRTE = OFF      // Power-up Timer Enable bit (PWRT disabled)
#pragma config MCLRE = ON       // MCLR Pin Function Select bit (MCLR pin is MCLR function and weak internal pull-up is enabled)
#pragma config CP = OFF         // Code Protection bit (Program memory is not code protected)
#pragma config IOSCFS = 8MHZ    // Internal Oscillator Frequency Select (8 MHz)
#pragma config BOREN = ON       // Brown-out Reset Selection bits (BOR enabled)
#pragma config WRT = OFF        // Flash Program Memory Self Write Enable bits (Write protection off)

// #pragma config statements should precede project file includes.
// Use project enums instead of #define for ON and OFF.

#include <xc.h>
#include "func.h"

uint8_t gray_code[9] = {0,1,3,2,6,7,5,4,12};
uint8_t index_gray_code[16] = {0,1,3,2,6,7,4,5,0xFF,0xFF,0xFF,0xFF,8,0xFF,0xFF,0xFF};

/******************************************************************************
 * Main program
 ******************************************************************************/
void main(void)
{
    // Constants:
    const tip_id_t selected_id = SELECT_TIPID;
    
    // Variables:
    uint8_t bit_val;
    uint8_t ch_read_result[OPTOCOUPLER_CHANNELS] = {0};
    uint8_t val_to_send = 0;
    
    // GPIO configuration:
    OUT_PIN_DIR = DIR_OUTPUT;   // All pins input, except OUT_PIN
    OUT_PIN = PIN_LOW;
    
    // ADC initialization:
    ADC_Init();

    ADC_InitThreshold(OPTO_VOLTAGE);
    //*************************************************************************
    // Sensors threshold / constant ID initialization:
    /*switch (selected_id)
    {
        // Optocouplers threshold:
        case SENSORS_1234:
        case SENSORS_5678:
        case SENSORS_RAW:
            ADC_InitThreshold(OPTO_VOLTAGE);
            break;
            
        // Temperature sensor threshold:
        case PROSCAN_T_SENSOR:
            ADC_InitThreshold(TEMP_VOLTAGE);
            val_to_send = ID_TEMP_LOW;            
            break;
            
        // Constant tip ID:
        default:
            val_to_send = (uint8_t)SELECT_TIPID;
    }*/
    
    //*************************************************************************    
    // Infinite loop:
    while (1)
    {
        ADC_ReadChannels(0, (OPTOCOUPLER_CHANNELS - 1), ch_read_result); // Read ADC channels
                
        // Convert bit values into a byte:
        val_to_send = 0;
        bit_val = 1;
        for (uint8_t i=0; i<OPTOCOUPLER_CHANNELS; i++)
        {
            if (ch_read_result[i] == 1)
            {
                val_to_send += bit_val;
            }
            bit_val <<= 1;
        }
        val_to_send = index_gray_code[val_to_send] + selected_id;

        Delay_ms(26);   // Delay before sending the value

        //*********************************************************************
        Send_Sequence(val_to_send, 6);  // Send the result
    }
}
