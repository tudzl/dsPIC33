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
 * File:   qei.c
 * Author: C68555
 *
 * Created on September 19, 2023, 5:07 PM
 */


#include "xc.h"
#include "examples.h"

#ifdef QEI_HOMING_INITIALIZATION_EXAMPLE

int main()
{
    //Remap of QEA, QEB, Index pins
    _TRISD5 = 1;
    _QEIHOME1R = 54;
    
    _TRISD9 = 1;
    _QEIINDX1R = 58;
    
    _TRISD10 = 1;
    _QEIA1R = 59;
            
    _TRISD6 = 1;
    _QEIB1R = 55;
    
    //Clear position counter
    POS1CNT = 0;
    //Enable QEI interrupt
    _QEI1IE = 1;
    //Enable QEI interrupt on initializing the position counter on completion of homing process
    QEI1STATbits.PCIIEN = 1;
    
    QEI1IOCbits.QEAPOL = 1; //QEA is inverted
    QEI1IOCbits.QEBPOL = 1; //QEB is inverted
    
    QEI1CONbits.CCM = 0; // Position counter in quadrature mode
    QEI1CONbits.PIMOD = 3; // Initialize the position counter on completion of homing process
    QEI1CONbits.ON = 1; // Enable QEI module
    
    while(1);
    
}

void __attribute__((__interrupt__)) _QEI1Interrupt(void)
{
     _QEI1IF = 0;
     // Check if the source of interrupt is homing initialization
     if(QEI1STATbits.PCIIRQ) 
     {
        // User application code goes here
     }
}

#endif