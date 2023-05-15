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


/******************************************************************************
 * Main program
 ******************************************************************************/
void main(void)
{
    // Constants:
    const tip_id_t selected_id = SELECT_TIPID;
    
    // Variables:
    uint8_t bit_val;
    uint8_t ch_read_result[ADC_CHANS] = {0};
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
        ADC_ReadChannels(0, 3, ch_read_result); // Read ADC channels
                
        // Convert bit values into a byte:
        val_to_send = 0;
        bit_val = 1;
        for (uint8_t i=0; i<ADC_CHANS; i++)
        {
            if (ch_read_result[i] == 1)
            {
                val_to_send += bit_val;
            }
            bit_val <<= 1;
        }
        val_to_send += selected_id;

        Delay_ms(26);   // Delay before sending the value
        /*switch (selected_id)
        {
            // Read optocouplers:
            case SENSORS_1234:
            case SENSORS_5678:
            case SENSORS_RAW:
            //*****************************************************************
                ADC_ReadChannels(0, 3, ch_read_result); // Read ADC channels
                
                // Convert bit values into a byte:
                uint8_t new_val_to_send = 0;
                uint8_t bit_val = 1;
                for (uint8_t i=0; i<ADC_CHANS; i++)
                {
                    if (ch_read_result[i] == 1)
                        new_val_to_send += bit_val;

                    bit_val <<= 1;
                }
                
                if (selected_id == SENSORS_RAW)
                {
                    // Send the byte as is:
                    val_to_send = new_val_to_send;
                }
                else
                {
                    // Update only when a valid value is present:
                    switch (new_val_to_send)
                    {
                        case 1:
                        case 2:
                        case 3:
                        case 4:
                            val_to_send = new_val_to_send;
                            if (selected_id == SENSORS_5678)
                                val_to_send += 4;   // {1,2,3,4} -> {5,6,7,8}
                    }                
                }
                Delay_ms(26);   // Delay before sending the value
                break;

            // Read temperature sensor:
            case PROSCAN_T_SENSOR:
            //*****************************************************************
                // Read ADC channel 0 (GP0):
                ADC_ReadChannels(0, 0, ch_read_result); // Read ADC channels
                val_to_send = (ch_read_result[0])? ID_TEMP_LOW : ID_TEMP_HIGH;

                Delay_ms(29);   // Delay before sending the value
                break;

            // Wait before sending constant tip ID:
            default:
            //*****************************************************************
                Delay_ms(30);   // Delay before sending the value
        }*/
        //*********************************************************************
        Send_Sequence(val_to_send, 6);  // Send the result
    }
}
