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
 * File:   clc.c
 * Author: C68555
 *
 * Created on August 18, 2023, 12:27 PM
 */


#include "xc.h"
#include "examples.h"


#ifdef CLC_FSK_MODULATION_OF_UART_DATA_EXAMPLE

//UART clocking settings for CPU @8MHz, UART1 clocked from 1/2 speed UPB bus @4MHz
#define FP 4000000
#define BAUDRATE 9600
//UART Baud rate calculation for fractional baud rate divider
#define BRGVAL (FP/BAUDRATE)

#define TRANSMIT_CHAR_SIZE 16
uint8_t transmitChar[TRANSMIT_CHAR_SIZE] = "ABCDEFGHIJKLMNOP";
uint8_t transmitCharIndex = 0;

void CLC1_configure_for_FSK_modulation_of_UART_data() {
    
    /*Select input source for CLC1*/
    CLC1SELbits.DS1 = 1; //Peripheral clock as input source to modulate high signal
    CLC1SELbits.DS2 = 3; //UART TX data as input source
    CLC1SELbits.DS4 = 1; //CLC2 output as input source
    
    //Configure Gates of CLC1
    CLC1GLSbits.G2D2T = 1; // Gate 2 selects Data 2 (here UART TX data)
    CLC1GLSbits.G1D1T = 1; // Gate 1 selects Data 1(here peripheral clock)
    CLC1GLSbits.G3D2N = 1; // Gate 3 selects inverted Data 2 (here UART TX data)
    CLC1GLSbits.G4D4T = 1; // Gate 4 selects Data 4 (here CLC2 output)
    
    /*Select input source for CLC2*/
    CLC2SELbits.DS1 = 3; //LPRC as input source to modulate logic low signal
    
    //Configure Gates of CLC2
    CLC2GLSbits.G1D1T = 1; // All gates select data 1 as input (here LPRC)
    CLC2GLSbits.G2D1T = 1;
    CLC2GLSbits.G3D1T = 1; 
    CLC2GLSbits.G4D1T = 1;
    
    //Configure CLC1 mode and output
    CLC1CONbits.LCOE = 1; // Enable CLC output on IO
    CLC1CONbits.MODE = 0; // Select AND-OR logic function
    
    //Configure CLC2 mode and output
    CLC2CONbits.LCOE = 1; // Enable CLC output on IO
    CLC2CONbits.MODE = 2; // Select AND logic function
    
    //Configure I/O for CLC1 output
    _RP44R = 47; //Map CLC1OUT output function to RP44 (RC11)
    _TRISC11 = 0; //RC11 as output
    
    /* UART configuration goes here*/
    
    //Configure I/O for UART
    _RP41R = 9;     //Assign UART1 TX output functionality to RP41 (RC8)
    _TRISC8 = 0;    //Set RC8 as output

    U1CONbits.MODE = 0;    // Asynchronous 8-bit UART
    U1CONbits.CLKSEL = 0; // 1/2 speed UPB clock as Baud Clock source
    U1CONbits.STSEL = 0;   // 1 stop bit

    //Use fractional baud rate divider
    U1CONbits.CLKMOD = 1;   
    U1BRG = BRGVAL; // Baud Rate setting for 9600

    U1CONbits.TXEN = 1; // Enable UART TX.
     
    //Turn on CLCs
    CLC1CONbits.ON = 1; // Enable CLC1
    CLC2CONbits.ON = 1; // Enable CLC2
    
    /*Turn on UART here*/
    U1CONbits.ON = 1; // Enable UART
    
}

int main(void) {
    
    CLC1_configure_for_FSK_modulation_of_UART_data();
     
    while(1) {
        
        //Delay
        for (int i = 0; i < 0x123; i++);
        
        //Transmit one character at a time
        while(!U1STATbits.TXBE);
        U1TXREG = transmitChar[transmitCharIndex++];
        if (transmitCharIndex >= TRANSMIT_CHAR_SIZE) {transmitCharIndex = 0;}
    }
}

#endif