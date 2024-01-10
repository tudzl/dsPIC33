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
 * File:   dac.c
 * Author: C68555
 *
 * Created on August 18, 2023, 11:48 AM
 */

#include "xc.h"
#include "dac.h"
#include "examples.h"


void DAC_configuration_for_pulse_stretcher() {
    /* Pulse Stretcher Configuration */
    DAC1CONbits.CMPPOL = 0; //Comparator output is non-inverted.
    //Pulse stretcher will stretch pulses on comparator output rising edge.
    //To stretch pulses on falling edge, set CMPPOL = 1 for inverted output.
}

void DAC_configuration_for_digital_filter() {
    DACCTRL1bits.FCLKDIV = 1; /* Filter Clk Divide by 2 */
    DAC1CONbits.FLTREN = 1; /* Filter enabled */
}

void DAC_configure_for_1V_output() {
    /* DAC Register Settings */
    
    DAC1DATbits.DACDAT = 0x4D9; /* DAC Output set to 1V (AVDD = 3.3V)*/
    
    DAC1CONbits.DACOEN = 1; /* Enable DAC 1 output on pin DACOUT1 */
    DAC1CONbits.DACEN = 1; /* Enable DAC 1 */
    DACCTRL1bits.ON = 1; /* Turn ON all DACs */
}


void DAC_triangle_wave_mode_configuration() {
    /* Triangle Wave Mode Settings */
    
    DAC1DATbits.DACLOW = 0x100; /* Lower data value */
    DAC1DATbits.DACDAT = 0xF00; /* Upper data value */
    
    SLP1DATbits.SLPDAT = 0x1; /* Slope rate, counts per step */
    SLP1CONbits.TWME = 1; /* Enable Triangle Mode */
    SLP1CONbits.SLOPEN = 1; /* Enable Slope Mode */
    
    DAC1CONbits.DACOEN = 1; /* Enable DAC 1 output on pin DACOUT1 */
    DAC1CONbits.DACEN = 1; /* Enable DAC 1 */
    DACCTRL1bits.ON = 1; /* Turn ON all DACs */
}


#ifdef DAC_WITH_SLOPE_COMPENSATION_EXAMPLE

void DAC_initialize_with_slope_compensation() {

    
    //PWM clock settings
    PCLKCONbits.MCLKSEL = 1; //Use CLKGEN5 as PWM MCLK
    PG1CONbits.CLKSEL = 1; // Use PWM Clock selected by MCLKSEL, undivided and unscaled
    
    //PWM clocking based on 200MHz MCLK from CLKGEN5. Each unit is 1/16 of an MCLK period.
    PG1PER = 1000 << 4; // PWM frequency is 200 kHz, 5 uS period
    PG1DC = 950 << 4; // 95% duty cycle, 4.75 uS on time 
    
    PG1IOCONbits.PENH = 1; // PWM Generator controls the PWMxH output pin
    PG1IOCONbits.PENL = 1; // PWM Generator controls the PWMxL output pin 
    
    // PWM PCI setup, use CLDAT when comparator 1 trips
    PG1CLPCIbits.PSS = 0b11011; // PCI source is Comparator 1 
    PG1CLPCIbits.AQSS = 2; // LEB active as Acceptance Qualifier 
    PG1CLPCIbits.AQPS = 1; // Invert Acceptance Qualifier (LEB not active)
    PG1CLPCIbits.TERM = 1; // Auto terminate as Termination Event
    PG1CLPCIbits.ACP = 3; // Latched PCI Acceptance Criteria
    PG1IOCONbits.CLDAT = 0b01; // PWM1L = 1 and PWM1H = 0 if CL event is active
    PG1LEBbits.PHR = 1; // Rising edge of PWMxH triggers the LEB counter
    
    PG1LEB = 100 << 4; // 500 nS LEB timer 
    
    // PWM to DAC Trigger setup 
    PG1TRIGA = 300 << 4; // ADC Trigger 1 at 1.5 uS, used as SLPSTRT
    PG1EVTbits.ADTR1EN1 = 1; // PGxTRIGA as trigger source for ADC Trigger 1
    PG1TRIGB = 800 << 4; // ADC Trigger 2 at 4 uS, used as SLPSTOPA
    PG1EVTbits.ADTR2EN2 = 1; // PGxTRIGB as trigger source for ADC Trigger 2
    PG1CONbits.ON = 1; // Enable PWM
    
    // DAC Configuration
    DACCTRL2bits.SSTIME = 0x8A; // Default value 552 nS @ 500MHz
    DACCTRL2bits.TMODTIME = 0x55;// Default value 340 nS @ 500MHz
    
    DAC1DATbits.DACDAT = 2703; // 2.17v steady state value
    DAC1DATbits.DACLOW = 1113; // 0.89v, value at the end slope
    
    
    SLP1DATbits.SLPDAT = 41; // Slope = (2703-1113)*16/((4u-1.5u)/4n)
    SLP1CONbits.SLOPEN = 1; // Enable Slope compensation 
    SLP1CONbits.SLPSTRT = 1; // PWM1 ADC Trigger 1
    SLP1CONbits.SLPSTOPA = 1; // PWM1 ADC Trigger 2
    SLP1CONbits.SLPSTOPB = 1; // Comparator 1
    DAC1CONbits.CBE = 1; // Enable comparator blanking
    DAC1CONbits.TMCB = 125; // 125 * 4 nS = 500 nS blanking time
    DAC1CONbits.DACOEN = 1; // Enable DAC output 
    DAC1CONbits.DACEN = 1; // Enable DAC 1
    DACCTRL1bits.ON = 1; // Enable DAC system

}

int main(void) {
    
    configure_CLKGEN7_for_500MHz_from_PLL2_VCODIV();
    configure_CLKGEN5_from_PLL2_Fout();
    DAC_initialize_with_slope_compensation();
    
    while(1);
}

#endif
 

#ifdef DAC_SETTINGS_FOR_HYSTERETIC_MODE_EXAMPLE
 
void DAC_settings_for_hysteretic_mode() {

    /* PWM Clock Selection */
    PCLKCONbits.MCLKSEL = 1; // Master Clock Source is CLKGEN5
    PG1CONbits.CLKSEL = 1; // Clock selected by MCLKSEL    
    
    PG1IOCONbits.PENH = 1; // Enable H output
    PG1IOCONbits.PENL = 1; // Enable L output 
    PG1IOCONbits.FFDAT = 0b11; // FF PCI data is 0b11

    // PPS setup
    RPINR6bits.PCI8R = 172; // 'PWM_Req_On' from DAC to PCI8
    RPINR6bits.PCI9R = 173; // 'PWM_Req_Off' from DAC to PCI9 

    // FF PCI setup
    PG1FFPCIbits.TSYNCDIS = 1; // Terminate immediately
    PG1FFPCIbits.TERM = 0b111; // PCI 9 (PWM_Req_Off)
    PG1FFPCIbits.PSS = 8; // PCI 8 (PWM_Req_On)
    PG1FFPCIbits.ACP = 0b100; // Latched rising edge
    PG1CONbits.ON = 1; // Enable PG1

    // DAC initialization 
       
    DAC1DATbits.DACLOW = 0x400; // Lower cmp limit, 0.825 V
    DAC1DATbits.DACDAT = 0xC00; // Upper cmp limit, 2.475 V
    
    DAC1CONbits.CBE = 1; // Enable comparator blanking 
    DAC1CONbits.TMCB = 100; // 2/500 MHz * 100 = 400 nS
    SLP1CONbits.HCFSEL = 1; // 1 = PWM1H 
    SLP1CONbits.HME = 1; // Hysteretic Mode
    DAC1CONbits.INSEL = 1; // CMP1B input
    DAC1CONbits.DACOEN = 1; // Output DAC voltage to DACOUT1 pin
    DAC1CONbits.DACEN = 1; // Enable DAC module 
    DACCTRL1bits.ON = 1; // Enable DAC1
}
 
int main(void) {
    
    configure_CLKGEN7_for_500MHz_from_PLL2_VCODIV();
    configure_CLKGEN5_from_PLL2_Fout();
    DAC_settings_for_hysteretic_mode();
    
    while(1);
    
}

#endif
 
void configure_CLKGEN7_for_500MHz_from_PLL2_VCODIV() {
        
    PLL2CONbits.ON = 1; //Enable PLL generator 2, if not already enabled
    
    //Select FRC as PLL2's clock source
    PLL2CONbits.NOSC = 1;
    //Request PLL2 clock switch
    PLL2CONbits.OSWEN = 1;
    //Wait for PLL2 clock switch to complete
    while(PLL2CONbits.OSWEN);
    
    PLL2DIVbits.PLLPRE = 1; //Reference input will be 8MHz, no division
    PLL2DIVbits.PLLFBDIV = 125; //Fvco = 8MHz * 125 = 1000MHz
    PLL2DIVbits.POSTDIV1 = 5; //Divide Fcvo by 5
    PLL2DIVbits.POSTDIV2 = 1; //Fpllo = Fvco / 5 / 1 = 200 MHz

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
    
    VCO2DIVbits.INTDIV = 1; //Divided VCO clock is 1000MHz / 2 = 500MHz
    //Request VCO divider switch
    PLL2CONbits.DIVSWEN = 1;
    //Wait for VCO divider switch to complete
    while(PLL2CONbits.DIVSWEN);
    
    //Enable CLKGEN7, if not already enabled
    CLK7CONbits.ON = 1;
    
    //Set CLKGEN7 fractional divider to divide by 1
    CLK7DIVbits.INTDIV = 0;
    CLK7DIVbits.FRACDIV = 0;
    //Request CLKGEN7 fractional divider switch
    CLK7CONbits.DIVSWEN = 1;
    //Wait for CLKGEN7 fractional divider switch to complete
    while(CLK7CONbits.DIVSWEN);

    //Set PLL2 VCODIV as new CLKGEN7 clock source
    CLK7CONbits.NOSC = 8;
    //Request CLKGEN7 clock switch
    CLK7CONbits.OSWEN = 1; 
    //Wait for CLKGEN7 clock switch to complete
    while (CLK7CONbits.OSWEN);
}

void configure_CLKGEN5_from_PLL2_Fout() {
        
    //This assumes PLL2 is running and has been configured previously to a valid
    //Fout frequency (maximum of 400MHz can be used as input to the PWM)
    
    //Enable CLKGEN5, if not already enabled
    CLK5CONbits.ON = 1;
    
    //Set CLKGEN5 fractional divider to divide by 1
    CLK5DIVbits.INTDIV = 0;
    CLK5DIVbits.FRACDIV = 0;
    //Request CLKGEN5 fractional divider switch
    CLK5CONbits.DIVSWEN = 1;
    //Wait for CLKGEN5 fractional divider switch to complete
    while(CLK5CONbits.DIVSWEN);

    //Set PLL2 Fout as new CLKGEN5 clock source
    CLK5CONbits.NOSC = 6;
    //Request CLKGEN5 clock switch
    CLK5CONbits.OSWEN = 1; 
    //Wait for CLKGEN5 clock switch to complete
    while (CLK5CONbits.OSWEN);
}