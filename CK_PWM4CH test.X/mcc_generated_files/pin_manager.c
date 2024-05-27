/**
  PIN MANAGER Generated Driver File

  @Company:
    Microchip Technology Inc.

  @File Name:
    pin_manager.c

  @Summary:
    This is the generated manager file for the PIC24 / dsPIC33 / PIC32MM MCUs device.  This manager
    configures the pins direction, initial state, analog setting.
    The peripheral pin select, PPS, configuration is also handled by this manager.

  @Description:
    This source file provides implementations for PIN MANAGER.
    Generation Information :
        Product Revision  :  PIC24 / dsPIC33 / PIC32MM MCUs - 1.171.4
        Device            :  dsPIC33CK256MP506
    The generated drivers are tested against the following:
        Compiler          :  XC16 v2.10
        MPLAB 	          :  MPLAB X v6.05
*/

/*
    (c) 2020 Microchip Technology Inc. and its subsidiaries. You may use this
    software and any derivatives exclusively with Microchip products.

    THIS SOFTWARE IS SUPPLIED BY MICROCHIP "AS IS". NO WARRANTIES, WHETHER
    EXPRESS, IMPLIED OR STATUTORY, APPLY TO THIS SOFTWARE, INCLUDING ANY IMPLIED
    WARRANTIES OF NON-INFRINGEMENT, MERCHANTABILITY, AND FITNESS FOR A
    PARTICULAR PURPOSE, OR ITS INTERACTION WITH MICROCHIP PRODUCTS, COMBINATION
    WITH ANY OTHER PRODUCTS, OR USE IN ANY APPLICATION.

    IN NO EVENT WILL MICROCHIP BE LIABLE FOR ANY INDIRECT, SPECIAL, PUNITIVE,
    INCIDENTAL OR CONSEQUENTIAL LOSS, DAMAGE, COST OR EXPENSE OF ANY KIND
    WHATSOEVER RELATED TO THE SOFTWARE, HOWEVER CAUSED, EVEN IF MICROCHIP HAS
    BEEN ADVISED OF THE POSSIBILITY OR THE DAMAGES ARE FORESEEABLE. TO THE
    FULLEST EXTENT ALLOWED BY LAW, MICROCHIP'S TOTAL LIABILITY ON ALL CLAIMS IN
    ANY WAY RELATED TO THIS SOFTWARE WILL NOT EXCEED THE AMOUNT OF FEES, IF ANY,
    THAT YOU HAVE PAID DIRECTLY TO MICROCHIP FOR THIS SOFTWARE.

    MICROCHIP PROVIDES THIS SOFTWARE CONDITIONALLY UPON YOUR ACCEPTANCE OF THESE
    TERMS.
*/


/**
    Section: Includes
*/

#include <xc.h>
#include <stdio.h>
#include "pin_manager.h"
extern uint8_t SW1_pressed;
/**
 Section: File specific functions
*/
void (*SW1_RC11_InterruptHandler)(void) = NULL;

/**
 Section: Driver Interface Function Definitions
*/
void PIN_MANAGER_Initialize (void)
{
    /****************************************************************************
     * Setting the Output Latch SFR(s)
     ***************************************************************************/
    LATA = 0x0000;
    LATB = 0x0000;
    LATC = 0x0000;
    LATD = 0x0000;

    /****************************************************************************
     * Setting the GPIO Direction SFR(s)
     ***************************************************************************/
    TRISA = 0x001F;
    TRISB = 0x03DD;
    TRISC = 0xFFFF;
    TRISD = 0x5FF4;

    /****************************************************************************
     * Setting the Weak Pull Up and Weak Pull Down SFR(s)
     ***************************************************************************/
    CNPDA = 0x0000;
    CNPDB = 0x0000;
    CNPDC = 0x0000;
    CNPDD = 0x0000;
    CNPUA = 0x0000;
    CNPUB = 0x0000;
    CNPUC = 0x0000;
    CNPUD = 0x0000;

    /****************************************************************************
     * Setting the Open Drain SFR(s)
     ***************************************************************************/
    ODCA = 0x0000;
    ODCB = 0x0000;
    ODCC = 0x0000;
    ODCD = 0x0000;

    /****************************************************************************
     * Setting the Analog/Digital Configuration SFR(s)
     ***************************************************************************/
    ANSELA = 0x001F;
    ANSELB = 0x009D;
    ANSELC = 0x00CF;
    ANSELD = 0x0C00;
    
    /****************************************************************************
     * Set the PPS
     ***************************************************************************/
    __builtin_write_RPCON(0x0000); // unlock PPS

    RPOR22bits.RP77R = 0x000E;    //RD13->INTERNAL OSCILLATOR:REFO1
    RPOR17bits.RP67R = 0x0001;    //RD3->UART1:U1TX
    RPINR18bits.U1RXR = 0x0044;    //RD4->UART1:U1RX

    __builtin_write_RPCON(0x0800); // lock PPS
    
    /****************************************************************************
     * Interrupt On Change: negative
     ***************************************************************************/
    CNEN1Cbits.CNEN1C11 = 1;    //Pin : RC11
    /****************************************************************************
     * Interrupt On Change: flag
     ***************************************************************************/
    CNFCbits.CNFC11 = 0;    //Pin : RC11
    /****************************************************************************
     * Interrupt On Change: config
     ***************************************************************************/
    CNCONCbits.CNSTYLE = 1;    //Config for PORTC
    CNCONCbits.ON = 1;    //Config for PORTC
    
    /* Initialize IOC Interrupt Handler*/
    SW1_RC11_SetInterruptHandler(&SW1_RC11_CallBack);
    
    /****************************************************************************
     * Interrupt On Change: Interrupt Enable
     ***************************************************************************/
    IFS1bits.CNCIF = 0; //Clear CNCI interrupt flag
    IEC1bits.CNCIE = 1; //Enable CNCI interrupt
}

void __attribute__ ((weak)) SW1_RC11_CallBack(void)
{
    SW1_pressed = 1;

}

void SW1_RC11_SetInterruptHandler(void (* InterruptHandler)(void))
{ 
    IEC1bits.CNCIE = 0; //Disable CNCI interrupt
    SW1_RC11_InterruptHandler = InterruptHandler; 
    IEC1bits.CNCIE = 1; //Enable CNCI interrupt
}

void SW1_RC11_SetIOCInterruptHandler(void *handler)
{ 
    SW1_RC11_SetInterruptHandler(handler);
}

/* Interrupt service routine for the CNCI interrupt. */
void __attribute__ (( interrupt, no_auto_psv )) _CNCInterrupt ( void )
{
    if(IFS1bits.CNCIF == 1)
    {
        if(CNFCbits.CNFC11 == 1)
        {
            if(SW1_RC11_InterruptHandler) 
            { 
                SW1_RC11_InterruptHandler(); 
            }
            
            CNFCbits.CNFC11 = 0;  //Clear flag for Pin - RC11

        }
        
        
        // Clear the flag
        IFS1bits.CNCIF = 0;
    }
}

