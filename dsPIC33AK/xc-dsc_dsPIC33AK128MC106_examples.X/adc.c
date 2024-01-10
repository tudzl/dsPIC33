/* Microchip Technology Inc. and its subsidiaries.  You may use this software 
 * and any derivatives exclusively with Microchip products. 
 * 
 * THIS SOFTWARE IS SUPPLIED BY MICROCHIP "AS IS".  NO WARRANTIES, WHETHER 
 * EXPRESS, IMPLIED OR STATUTORY, APPLY TO THIS SOFTWARE, INCLUDING ANY IMPLIED 
 * WARRANTIES OF NON-INFRINGEMENT, MERCHANTABILITY, AND FITNESS FOR A 
 * PARTICULAR PURPOSE, OR ITS INTERACTION WITH MICROCHIP PRODUCTS, COMBINATION 
 * WITH ANY OTHER PRODUCTS, OR USE IN ANY APPLICATION. 
 *
 * IN NO EVENT WILL MICROCHIP BE LIABLE FOR ANY INDIRECT, SPECIAL, PUNITIVE, 
 * INCIDENTAL OR CONSEQUENTIAL LOSS, DAMAGE, COST OR EXPENSE OF ANY KIND 
 * WHATSOEVER RELATED TO THE SOFTWARE, HOWEVER CAUSED, EVEN IF MICROCHIP HAS 
 * BEEN ADVISED OF THE POSSIBILITY OR THE DAMAGES ARE FORESEEABLE.  TO THE 
 * FULLEST EXTENT ALLOWED BY LAW, MICROCHIP'S TOTAL LIABILITY ON ALL CLAIMS 
 * IN ANY WAY RELATED TO THIS SOFTWARE WILL NOT EXCEED THE AMOUNT OF FEES, IF 
 * ANY, THAT YOU HAVE PAID DIRECTLY TO MICROCHIP FOR THIS SOFTWARE.
 *
 * MICROCHIP PROVIDES THIS SOFTWARE CONDITIONALLY UPON YOUR ACCEPTANCE OF THESE 
 * TERMS. 
 */

/*
 * File:   adc.c
 * Author: C68555
 *
 * Created on May 22, 2023, 2:46 PM
 */


#include "xc.h"
#include "examples.h"


void clock_ADC_for_40Msps_from_PLL2() {
    
    PLL2CONbits.ON = 1; //Enable PLL generator 2, if not already enabled
    
    //Select FRC as PLL2's clock source
    PLL2CONbits.NOSC = 1;
    //Request PLL2 clock switch
    PLL2CONbits.OSWEN = 1;
    //Wait for PLL2 clock switch to complete
    while(PLL2CONbits.OSWEN);
    
    //Set up PLL2 dividers to output 200MHz
    PLL2DIVbits.PLLPRE = 1; //Reference input will be 8MHz, no division
    PLL2DIVbits.PLLFBDIV = 120; //Fvco = 8MHz * 120 = 960MHz
    PLL2DIVbits.POSTDIV1 = 3; //Divide Fcvo by 3
    PLL2DIVbits.POSTDIV2 = 1; //Fpllo = Fvco / 3 / 1 = 320 MHz

    //The PLLSWEN bit controls changes to the PLL feedback divider.
    //Request PLL2 feedback divider switch
    PLL2CONbits.PLLSWEN = 1;
    //Wait for PLL2 feedback divider switch to complete
    while(PLL2CONbits.PLLSWEN);

    //The FOUTSWEN bit controls changes to the PLL output dividers.
    //Request PLL2 output divider switch
    PLL2CONbits.FOUTSWEN = 1;
    //Wait for PLL2 output divider switch to complete
    while(PLL2CONbits.FOUTSWEN);
    
    //Enable CLKGEN6
    CLK6CONbits.ON = 1;
    
    //Reset CLKGEN6 fractional divider for 1:1 ratio
    CLK6DIVbits.INTDIV = 0;
    CLK6DIVbits.FRACDIV = 0;
    //Request CLKGEN6 fractional divider switch
    CLK6CONbits.DIVSWEN = 1;
    //Wait for CLKGEN6 fractional divider switch to complete
    while(CLK6CONbits.DIVSWEN);

    //Set PLL2 Fout as new CLKGEN6 clock source
    CLK6CONbits.NOSC = 6;
    //Request CLKGEN6 clock switch
    CLK6CONbits.OSWEN = 1; 
    //Wait for CLKGEN6 clock switch to complete
    while (CLK6CONbits.OSWEN);
}

#ifdef ADC_SINGLE_CONVERSION_EXAMPLE
    
    // The channel output.
    long result = 0; 
    
    int main(){ 
        
        //Set up clock for 40Msps operation
        clock_ADC_for_40Msps_from_PLL2();
        
        // In this example channel 1 will be used.
        // Select single conversion mode.
        AD1CHCON1bits.MODE = 0;
        // Software trigger will start a conversion.
        AD1CHCON1bits.TRG1SRC = 1; 
        // Use a differential input.
        AD1CHCON1bits.DIFF = 1;
        // Select the AN9 analog positive input/pin for the signal.
        AD1CHCON1bits.PINSEL = 9;
        // Select the ANN1 analog negative input/pin for the signal.
        AD1CHCON1bits.NINSEL = 1;
        // Select signal sampling time (6.5 TADs = 81nS).
        AD1CHCON1bits.SAMC = 3;
        // Set ADC to RUN mode.
        AD1CONbits.MODE = 2;
        // Enable ADC.
        AD1CONbits.ON = 1;
        // Wait when ADC will be ready/calibrated.
        while(AD1CONbits.ADRDY == 0);
        // Trigger channel #1 in software and wait for the result.
        while(1){
            // Trigger channel # 1.
            AD1SWTRGbits.CH1TRG = 1;
            // Wait for a conversion ready flag.
            while(AD1STATbits.CH1RDY == 0);
            // Read result. It will clear the conversion ready flag.
            result = AD1DATA1;
        }
        return 1;
    }
#endif

#ifdef ADC_WINDOWED_CONVERSIONS_EXAMPLE

// The channel output from primary accumulator.
volatile long result = 0; 
// The number of accumulated samples.
volatile long number_of_accumulated_samples;

int main(){ 
    
    //Set up clock for 40Msps operation
    clock_ADC_for_40Msps_from_PLL2();
    
    // RA4/RP5 pin is a trigger input.
    // Switch to a digital input.
    ANSELAbits.ANSELA4 = 0;
    // Make an input. 
    TRISAbits.TRISA4 = 1;
    // Map external pin trigger to RP5/RA4.
    _ADTRIG31R = 5; 
    // In this example channel 3 will be used.
    // Set limit for the accumulated samples number.
    AD1CNT3bits.CNT3 = 0xffff; 
    // Window conversion mode.
    AD1CHCON3bits.MODE = 1;
    // Accumulation will be started/stopped from an external pin.
    AD1CHCON3bits.TRG1SRC = 31; 
    // Trigger all conversions from the ADC repeat timer.
    AD1CHCON3bits.TRG2SRC = 3; 
    // Select the AN5 analog positive input/pin.
    AD1CHCON3bits.PINSEL = 5;
    // Select signal sampling time (6.5 TADs = 81nS).
    AD1CHCON3bits.SAMC = 3;
    // Set period of the triggers timer (63 is maximum).
    AD1CONbits.RPTCNT = 60;
    // Set ADC to RUN mode.
    AD1CONbits.MODE = 2;
    // Enable ADC.
    AD1CONbits.ON = 1;
    // Wait when ADC will be ready/calibrated.
    while(AD1CONbits.ADRDY == 0);
    // Enable interrupt;
    _AD1CH3IE = 1;
    
    // Channel 3 is converted and results are accumulated until the RA4 pin is high.
    // On transition from high to low an interrupt is generated.
    while(1);
    
    return 1;
}

void __attribute__((interrupt)) _AD1CH3Interrupt(){
    
    // Read result in accumulator and clear CH3RDY flag.
    result = AD1DATA3;
    number_of_accumulated_samples = AD1CNT3bits.CNTSTAT3;
    result /= number_of_accumulated_samples;
    // Clear interrupt flag.
    _AD1CH3IF = 0;
    
}

#endif

#ifdef ADC_INTEGRATION_OF_THE_MULTIPLE_SAMPLES_EXAMPLE

// The channel output.
long result = 0; 

int main(){ 
    
    //Set up clock for 40Msps operation
    clock_ADC_for_40Msps_from_PLL2();
    
     // In this example channel 15 will be used.
     // Integration conversion mode.
     AD1CHCON15bits.MODE = 2;
     // Set number of conversions accumulated to 123.
     AD1CNT15 = 123;
     // Software trigger will start a conversions.
     AD1CHCON15bits.TRG1SRC = 1; 
     // Re-trigger back to back.
     AD1CHCON15bits.TRG2SRC = 2; 
     // Select the AN5 analog positive input/pin.
     AD1CHCON15bits.PINSEL = 5;
     // Select signal sampling time (6.5 TADs = 81nS).
     AD1CHCON15bits.SAMC = 3;
     // Set ADC to RUN mode.
     AD1CONbits.MODE = 2;
     // Enable ADC.
     AD1CONbits.ON = 1;
     // Wait when ADC will be ready/calibrated.
     while(AD1CONbits.ADRDY == 0);
     // Trigger channel #15 in software and wait for the 123 samples
     // accumulated result.
     while(1){
        // Trigger channel # 15.
        AD1SWTRGbits.CH15TRG = 1;
        // Wait for a conversion ready flag.
        while(AD1STATbits.CH15RDY == 0);
        // Read oversampling result. It will clear the conversion ready flag.
        result = AD1DATA15;
     }
     return 1;
}

#endif

#ifdef ADC_OVERSAMPLING_EXAMPLE

// The channel output.
long result = 0; 

int main(){ 
    
    //Set up clock for 40Msps operation
    clock_ADC_for_40Msps_from_PLL2();
    
    // In this example channel 4 will be used.
    // Oversampling conversion mode.
    AD1CHCON4bits.MODE = 3;
    // Set number of conversions accumulated to 16.
    AD1CHCON4bits.ACCNUM = 1;
    // The oversampling if started cannot be interrupted
    // by a high priority channels conversion requests.
    AD1CHCON4bits.ACCBRST = 1;
    // Software trigger will start a conversions.
    AD1CHCON4bits.TRG1SRC = 1; 
    // Re-trigger back to back.
    AD1CHCON4bits.TRG2SRC = 2; 
    // Select the AN5 analog positive input/pin.
    AD1CHCON4bits.PINSEL = 5;
    // Select signal sampling time (6.5 TADs = 81nS).
    AD1CHCON4bits.SAMC = 3;
    // Set ADC to RUN mode.
    AD1CONbits.MODE = 2;
    // Enable ADC.
    AD1CONbits.ON = 1;
    // Wait when ADC will be ready/calibrated.
    while(AD1CONbits.ADRDY == 0);
    // Trigger channel #4 in software and wait for the 16 samples
    // oversampling result.
    
    while(1){
        // Trigger channel # 4.
        AD1SWTRGbits.CH4TRG = 1;
        // Wait for a conversion ready flag.
        while(AD1STATbits.CH4RDY == 0);
        // Read oversampling result. It will clear the conversion ready flag.
        result = AD1DATA4;
    }
    return 1;
}


#endif

#ifdef ADC_COMPARATOR_EXAMPLE

// The channel output.
long result = 0; 

int main(){ 
    
    //Set up clock for 40Msps operation
    clock_ADC_for_40Msps_from_PLL2();
    
    // In this example channel 1 will be used.
    // Select single conversion mode.
    AD1CHCON1bits.MODE = 0;
    // Software trigger will start a conversion.
    AD1CHCON1bits.TRG1SRC = 1; 
    // Select the AN9 analog positive input/pin for the signal.
    AD1CHCON1bits.PINSEL = 9;
    // Select signal sampling time (6.5 TADs = 81nS).
    AD1CHCON1bits.SAMC = 3;
    // Enable the comparator for this channel.
    // Select out of bounds mode.
    AD1CHCON1bits.CMPMOD = 1;
    // Select low limit. To generate comparator event when AD1DATA1 < 1024.
    AD1CMPLO1 = 1024; 
    // Select high limit. To generate comparator event when AD1DATA1 > 3072.
    AD1CMPHI1 = 3072; 
    // Enable timer interrupt. 
    _AD1CMP1IE = 1;
    // Set ADC to RUN mode.
    AD1CONbits.MODE = 2;
    // Enable ADC.
    AD1CONbits.ON = 1;
    // Wait when ADC will be ready/calibrated.
    while(AD1CONbits.ADRDY == 0);
    // Trigger channel #1 in software and wait for the result.
    while(1){
        // Trigger channel # 1.
        AD1SWTRGbits.CH1TRG = 1;
        // Wait for a conversion ready flag.
        while(AD1STATbits.CH1RDY == 0);
        // Read result. It will clear the conversion ready flag.
        result = AD1DATA1;
    }
    return 1;
}

void __attribute__((interrupt)) _AD1CMP1Interrupt(){
    // Process the comparator event here.
    // Clear the comparator event flag.
    AD1CMPSTATbits.CH1CMP = 0; 
    // Clear the comparator flag.
    _AD1CMP1IF = 0;
}


#endif

#ifdef ADC_MULTIPLE_CHANNELS_SCAN_EXAMPLE

volatile long channel_2_an1;
volatile long channel_4_an2;
volatile long channel_6_an3;

// Define peripheral clock frequency.
#define FCY (150000000UL) // 150MHz
// Define the CCP1 timer frequency.
#define TIMER_FREQUENCY (100UL) // 1kHz

int main(){ 
    
    //Set up clock for 40Msps operation
    clock_ADC_for_40Msps_from_PLL2();
    
    // In this example channels ## 2, 4 and 6 will be scanned.
    // To scan channels they must be triggered from one source.
    // The channel with lowest number (#2) will be converted first.
    // The channel with highest number (#6) will be converted last.
    
    // CHANNEL 2
    // Single conversion mode.
    AD1CHCON2bits.MODE = 0;
    // CCP1 Timer starts conversion (same for all scanned channels).
    AD1CHCON2bits.TRG1SRC = 12;
    // Select the AN1 analog input/pin for the channel #2.
    AD1CHCON2bits.PINSEL = 1;
    // Select signal sampling time (6.5 TADs = 81nS).
    AD1CHCON2bits.SAMC = 3;
    
    // CHANNEL 4
    // Single conversion mode.
    AD1CHCON4bits.MODE = 0;
    // CCP1 Timer starts conversion (same for all scanned channels).
    AD1CHCON4bits.TRG1SRC = 12;
    // Select the AN2 analog input/pin for the channel #4.
    AD1CHCON4bits.PINSEL = 2;
    // Select signal sampling time (8.5 TADs = 106nS).
    AD1CHCON4bits.SAMC = 4; 
    
    // CHANNEL 6
    // Single conversion mode.
    AD1CHCON6bits.MODE = 0;
    // CCP1 Timer starts conversion (same for all scanned channels).
    AD1CHCON6bits.TRG1SRC = 12;
    // Select the AN3 analog input/pin for the channel #6.
    AD1CHCON6bits.PINSEL = 3;
    // Select signal sampling time (10.5 TADs = 131nS).
    AD1CHCON6bits.SAMC = 5; 
    
    // Set ADC to RUN mode.
    AD1CONbits.MODE = 2;
    // Enable ADC.
    AD1CONbits.ON = 1;
    // Wait when ADC will be ready/calibrated.
    while(AD1CONbits.ADRDY == 0);
    
    // Configure CCP1 Timer to trigger all channels (to scan).
    CCP1CON1bits.MOD = 0;
    // Set 32-bit timer.
    CCP1CON1bits.T32 = 1;
    // Set period.
    CCP1PR = FCY/TIMER_FREQUENCY;
    // Run timer.
    CCP1CON1bits.ON = 1;
    
    // Enable channel # 6 interrupt.
    // This channel is processed last and all other channels results 
    // will be ready in the channel #6 ISR.
    _AD1CH6IE = 1;
    
    while(1); 
    
    return 1;
}

// Channel # 6 interrupt (processed last). All channels
// results in the scan are available here.
void __attribute__((interrupt)) _AD1CH6Interrupt(){
    channel_2_an1 = AD1DATA2;
    channel_4_an2 = AD1DATA4;
    channel_6_an3 = AD1DATA6;
    _AD1CH6IF = 0; 
}

#endif

#ifdef ADC_SECOND_ORDER_LOW_PASS_FILTER_EXAMPLE

// VARIABLES OF THE SOFTWARE PART OF THE FILTER.
// These global variables are used in an interrupt.
// That's why they must be declared as "volatile".
// Input for the software part of the filter's first stage.
volatile long ch19_current_1 = 0;
// Input delayed for the first stage.
volatile long ch19_previous_1 = 0;
// Input for the software part of the filter's second stage.
volatile long ch19_current_2 = 0;
// Input delayed for the second stage.
volatile long ch19_previous_2 = 0;
// The filter output.
volatile long filtered_result = 0; 

// Define peripheral clock frequency.
#define FCY (150000000UL) // 150MHz
// Define the CCP1 timer frequency.
#define TIMER_FREQUENCY (100UL) // 1kHz

int main() { 
    
    //Set up clock for 40Msps operation
    clock_ADC_for_40Msps_from_PLL2();
    
    // This example assumes that dsPIC33AK128MC106 device is used.
    // dsPIC33AK128MC106 device has 3 channels with the secondary
    // accumulator implemented: ## 17, 18 and 19.
    // This example will use channel #19.
    // Enable accumulators roll-over to enable the secondary accumulator.
    AD1CHCON19bits.ACCRO = 1;
    // Select integration sampling mode.
    AD1CHCON19bits.MODE = 2;
    // CCP1 Timer starts conversions (1kHz frequency).
    AD1CHCON19bits.TRG1SRC = 12;
    // CCP1 Timer re-triggers (1kHz frequency).
    AD1CHCON19bits.TRG2SRC = 12;
    // Select the AN1 analog input/pin for the signal to b filtered.
    AD1CHCON19bits.PINSEL = 1;
    // Select signal sampling time (6.5 TADs = 81nS).
    AD1CHCON19bits.SAMC = 3;
    
    // Set number of conversions = 8 for the filter (sub-sampler).
    // The CH19RDY bit will be set after 8 conversions.
    // The conversions frequency is 1kHz defined by CCP1 Timer period.
    // The signal maximum frequency is in twice less = 500 Hz. 
    // The filter cut-off frequency is 500kHz/8 = 62.5 Hz.
    AD1CNT19 = 8;
    
    // Set ADC to RUN mode.
    AD1CONbits.MODE = 2;
    // Enable ADC.
    AD1CONbits.ON = 1;
    // Wait when ADC will be ready/calibrated.
    while(AD1CONbits.ADRDY == 0);
    
    // Configure CCP1 Timer to trigger the channel # 19.
    CCP1CON1bits.MOD = 0;
    // Set 32-bit timer.
    CCP1CON1bits.T32 = 1;
    // Set period.
    CCP1PR = FCY/TIMER_FREQUENCY;
    // Run timer.
    CCP1CON1bits.ON = 1;
    
    // Enable channel # 19 interrupt.
    _AD1CH19IE = 1;
    // The AN1 pin filtered result is available in the channel # 19 interrupt.
    while(1);
    
    return 1;
}

// Channel # 19 interrupt.
// Called when integration is finished (every AD1CNT19 = 8 conversions). 
void __attribute__((interrupt)) _AD1CH19Interrupt(){
    
    long primary_accumulator; 
    
    // Clear interrupt flag. If the interrupt is persistent then
    // to clear the flag it is required to read the ADC channel
    // result register first.
    primary_accumulator = AD1DATA19;
    _AD1CH19IF = 0; 
    
    // Process software part of the filter.
    ch19_current_1 = AD1ACC19;
    ch19_current_2 = ch19_previous_1 - ch19_current_1; 
    ch19_previous_1 = ch19_current_1;
    filtered_result = ch19_previous_2 - ch19_current_2;
    ch19_previous_2 = ch19_current_2;
    // Divide by 1:(8*8) or 1:64 or shift right by 6
    filtered_result >>= 6; 
}

#endif