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
 * File:   timer1.c
 * Author: C68555
 *
 * Created on August 23, 2023, 5:05 PM
 */


#include "xc.h"
#include "timer1.h"
#include "examples.h"


void timer1_configure_for_synchronous_clock_counter() {
    T1CON = 0; // Stop timer and clear control register

    T1CONbits.TCS = 0; //Use Internal clock source (1/2 speed UPB clock)
    T1CONbits.TCKPS = 0b00; //1:1 prescaler
    T1CONbits.TGATE = 0; //Not gated
    
    TMR1 = 0; // Clear timer register
    PR1 = 0x12345678; // Load period register
    T1CONbits.TON = 1; // Start timer
}

#ifdef TIMER1_SYNCHRONOUS_CLOCK_COUNTER_EXAMPLE

int main(void) {
    
    timer1_configure_for_synchronous_clock_counter();
    _T1IE = 1; //Enable Timer1 interrupt
    
    while(1);
}

void __attribute__((__interrupt__)) _T1Interrupt() {
    _T1IF = 0;
}

#endif

void timer1_configure_for_synchronous_external_counter() {
    T1CON = 0; // Stop timer and clear control register
    
    //T1CON = 0x00000106; // Set prescaler at 1:1, external clock source (33C TECS selection in original)
    
    T1CONbits.TCS = 1; //Use external clock source selected by TECS
    T1CONbits.TSYNC = 1; //Synchronizes the External Clock input
    T1CONbits.TCKPS = 0b00; //1:1 prescaler
    T1CONbits.TECS = 0b00; //Select T1CK as external clock source
    
    TMR1 = 0; // Clear timer register
    PR1 = 0x12345678; // Load period register
    T1CONbits.TON = 1; // Start timer
}

#ifdef TIMER1_SYNCHRONOUS_EXTERNAL_COUNTER_EXAMPLE

int main(void) {
    
    _TRISD5 = 1; //Set RD5 as input
    _T1CKR = 54; //Map T1CK input function to RD5 (RP54)
    
    timer1_configure_for_synchronous_external_counter();
    _T1IE = 1;  //Enable Timer1 interrupt
    
    while(1);
}

void __attribute__((__interrupt__)) _T1Interrupt() {
    _T1IF = 0;
}

#endif

void timer1_configure_for_gated_timer() {
    T1CON = 0; // Stop timer and clear control register
    
    T1CONbits.TCKPS = 0b00; //1:1 prescaler
    T1CONbits.TGATE = 1; //Gated timer mode
    T1CONbits.TCS = 0; //Use internal clock
    
    TMR1 = 0; // Clear timer register
    PR1 = 0xFFFFFFFF; //Timer will roll over if it reaches maximum value
    
    T1CONbits.TON = 1; // Start timer
}


#ifdef TIMER1_GATED_TIMER_EXAMPLE

int main(void) {
    
    _TRISD5 = 1; //Set RD5 as input
    _T1CKR = 54; //Map T1CK input function to RD5 (RP54)
    
    timer1_configure_for_gated_timer();
    _T1IE = 1;  //Enable Timer1 interrupt
    
    while(1);
}

uint32_t gated_buffer[128];
uint32_t gated_buffer_index = 0;

void __attribute__((__interrupt__)) _T1Interrupt() {
    _T1IF = 0;
    gated_buffer[gated_buffer_index++] = TMR1;
    if (gated_buffer_index >= 128) {
        gated_buffer_index = 0;
    }
    //Reset timer for consistent values in gated mode
    TMR1 = 0;
}

#endif

void timer1_configure_for_asynchronous_counter() {

    T1CON = 0; // Stop timer and clear control register
    TMR1 = 0; // Clear timer register
    
    T1CONbits.TCS = 1; //Use external clock source
    T1CONbits.TECS = 0b00; //Select T1CK as external clock source
    T1CONbits.TSYNC = 0; //External clock is asynchronous to 1/2 speed UPB clock
    T1CONbits.TCKPS = 0b00; //1:1 prescaler
    
    PR1 = 0x12345678; // Load period register
    T1CONbits.TON = 1; // Start timer
}

#ifdef TIMER1_ASYNCHRONOUS_COUNTER_EXAMPLE

int main(void) {

    _TRISD5 = 1; //Set RD5 as input
    _T1CKR = 54; //Map T1CK input function to RD5 (RP54)
    
    timer1_configure_for_asynchronous_counter();
    _T1IE = 1;  //Enable Timer1 interrupt
    
    while(1);
}

void __attribute__((__interrupt__)) _T1Interrupt() {
    _T1IF = 0;
}

#endif