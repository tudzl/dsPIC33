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
 * File:   sccp.c
 * Author: C68555
 *
 * Created on August 21, 2023, 9:29 AM
 */


#include "xc.h"
#include "sccp.h"
#include "examples.h"


void SCCP1_setup_for_input_capture_mode_every_rising_edge() {
    CCP1CON1bits.CCSEL=1; // Input capture mode
    CCP1CON1bits.CLKSEL=0; // Set the clock source (1/2 speed UPB bus, 4MHz)
    CCP1CON1bits.T32=0; // 16-bit Dual Timer mode
    CCP1CON1bits.MOD= 1; // Capture ever rising edge of the event
    
    CCP1CON2bits.ICS= 0; // Capture source is ICS pin
    CCP1CON1bits.OPS=0; // Interrupt on every input capture event
    
    CCP1CON1bits.TMRPS=0; // Set the clock pre-scaler (1:1)
    CCP1CON1bits.ON=1; // Enable CCP/input capture
}

#ifdef SCCP_INPUT_CAPTURE_MODE_EVERY_RISING_EDGE_EXAMPLE

int main(void) {
    
    //Map SCCP1 input capture input to RC10
    _TRISC10 = 1; //RC10 is input
    _ICM1R = 43; //Map ICM1 input function to RP43 (RC10)
    
    SCCP1_setup_for_input_capture_mode_every_rising_edge();
    
    _CCP1IE = 1;
    
    while(1);
}

//Define array to store captured pulse widths
#define CAPTURE_VALUES_LEN 32
uint32_t capture_values[CAPTURE_VALUES_LEN];
uint32_t capture_values_index = 0;

uint32_t last_ccp1buf = 0;

void __attribute__((__interrupt__)) _CCP1Interrupt() {
    //Captured pulse width is the difference between current and previous capture values
    uint32_t current_ccp1buf = CCP1BUF;
    uint32_t capture_result = current_ccp1buf - last_ccp1buf;
    //Save current CCP1BUF reading for comparison with next reading
    last_ccp1buf = current_ccp1buf;

    //Store captured pulse width
    capture_values[capture_values_index++] = capture_result;
    if (capture_values_index >= CAPTURE_VALUES_LEN) {
        capture_values_index = 0;
    }
    
    _CCP1IF = 0;
}

#endif

void SCCP1_setup_for_dual_edge_buffered_compare_mode() {

    // Set CCP operating mode
    CCP1CON1bits.CCSEL = 0;     // Set SCCP operating mode (OC mode)
    CCP1CON1bits.MOD = 0b0101;  // Set mode (Buffered Dual-Compare/PWM mode)
    //Configure SCCP Timebase
    CCP1CON1bits.T32 = 0;       // Set timebase width (16-bit)
    CCP1CON1bits.TMRSYNC = 0;   // Time base not synchronized to system clock
    CCP1CON1bits.CLKSEL = 1;    // Set the clock source (CLKGEN12)
    CCP1CON1bits.TMRPS = 0b00;  // Set the clock pre-scaler (1:1)
    CCP1CON1bits.TRIGEN = 0;    // Set Sync/Triggered mode (Synchronous)
    CCP1CON1bits.SYNC = 0b00000;// Select Sync/Trigger source (Self-sync)
    
    //Configure SCCP output for PWM signal
    CCP1CON2bits.OCAEN = 1; // Enable desired output signals (OC1A)

    CCP1CON3bits.POLACE = 0;    // Configure output polarity (Active High)
    CCP1TMR = 0x0000;           // Initialize timer prior to enable module.
    CCP1PR = 0x0000FFFF;        // Configure timebase period
    CCP1RA = 0x00001000;        // Set the rising edge compare value
    CCP1RB = 0x00008000;        // Set the falling edge compare value
    CCP1CON1bits.ON = 1;        // Turn on SCCP module
    
}

#ifdef SCCP_DUAL_EDGE_BUFFERED_COMPARE_MODE_EXAMPLE

int main(void) {
    
    //Map SCCP1 output to RC11
    _RP44R = 24; //Map RP44 (RC11) to SCCP1 output function
    _TRISC11 = 0; //RC11 is output
    
    configure_CLKGEN12_for_200MHz_from_PLL2_Fout();
    
    SCCP1_setup_for_dual_edge_buffered_compare_mode();

    while(1); 

}

#endif

void SCCP1_setup_for_synchronous_16_bit_dual_timer_mode() {

    CCP1CON1bits.TRIGEN = 0; // Set Sync/Triggered mode (Synchronous Mode)
    CCP1CON1bits.SYNC = 0; // rolls over at FFFFh or match with period register (self sync)
    CCP1CON1bits.T32 = 0; // 16 bit dual timer mode
    CCP1CON1bits.TMRSYNC = 0; //Time base not synchronized to system clock

    CCP1CON1bits.CLKSEL = 0; // Set the clock source (1/2 speed UPB clock)
    CCP1CON1bits.TMRPS = 0; // Set the clock pre-scaler (1:1)
    CCP1PR = 0x00000FFF; // Period of 1.02ms with clock @4Mhz
    CCP1CON1bits.ON = 1; // Start the Timer

}

#ifdef SCCP_SYNCHRONOUS_OPERATION_16_BIT_DUAL_TIMER_MODE_EXAMPLE

int main(void) {
    
    SCCP1_setup_for_synchronous_16_bit_dual_timer_mode();
    
    //Enable CCP1 Timer interrupt
    _CCT1IE = 1;
    
    //Set RC4 as digital output
    _TRISC4 = 0;
    
    while(1);
}

void __attribute__((__interrupt__)) _CCT1Interrupt() {
    _CCT1IF = 0;
    //Toggle RC4
    _LATC4 ^= 1;
}

#endif

void SCCP1_setup_for_timer_triggered_operation_16_bit_dual_timer_mode() {
    
    CCP1CON1bits.TRIGEN=1;      // Set Sync/Triggered mode (Triggered Mode)   
    CCP1CON1bits.SYNC = 0b01001;// INT0 as trigger
    CCP1CON1bits.T32=0;         // 16 bit dual timer mode
    CCP1CON1bits.TMRSYNC = 0;   // Time base not synchronized to system clock
    CCP1CON1bits.CLKSEL = 0;    // Set the clock source (Tcy)
    CCP1CON1bits.TMRPS = 0;     // Set the clock pre-scaler (1:1)
    
    CCP1PR = 0x00000FFF; // 32-bit SCCP1 period register
    
    CCP1CON1bits.ON=1; // Enable the Timer
}

#ifdef SCCP_TIMER_TRIGGERED_OPERATION_16_BIT_DUAL_TIMER_MODE_EXAMPLE

int main(void) {
    
    //Set RB0 (INT0) as digital
    _ANSELB0 = 0;
    _TRISB0 = 1; //Input
    
    SCCP1_setup_for_timer_triggered_operation_16_bit_dual_timer_mode();
    
    //Timer will not run until the first time INT0 is asserted, then, the timer will run continuously.
    
    //Enable SCCP1 timer interrupt
    _CCT1IE = 1;
    
    //Set RC4 as digital output
    _TRISC4 = 0;
    
    while(1);
}

void __attribute__((__interrupt__)) _CCT1Interrupt() {
    _CCT1IF = 0;
    //Toggle RC4
    _LATC4 ^= 1;
}

#endif

void configure_CLKGEN12_for_200MHz_from_PLL2_Fout() {
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
    
    //Enable CLKGEN12, if not already enabled
    CLK12CONbits.ON = 1;
    
    //Set CLKGEN12 fractional divider to divide by 1
    CLK12DIVbits.INTDIV = 0;
    CLK12DIVbits.FRACDIV = 0;
    //Request CLKGEN12 fractional divider switch
    CLK12CONbits.DIVSWEN = 1;
    //Wait for CLKGEN12 fractional divider switch to complete
    while(CLK12CONbits.DIVSWEN);

    //Set PLL2 Fout as new CLKGEN12 clock source
    CLK12CONbits.NOSC = 6;
    //Request CLKGEN12 clock switch
    CLK12CONbits.OSWEN = 1; 
    //Wait for CLKGEN12 clock switch to complete
    while (CLK12CONbits.OSWEN);
}