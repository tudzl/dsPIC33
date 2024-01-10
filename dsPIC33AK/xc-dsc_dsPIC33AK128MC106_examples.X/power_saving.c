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
 * File:   power_saving.c
 * Author: C68555
 *
 * Created on August 24, 2023, 12:59 PM
 */


#include "xc.h"
#include "examples.h"


#ifdef IDLE_MODE_WAKE_USING_TIMER1

//When waking up the device using an interrupt, 2 things may happen, depending on the IPL.
//If the IPL of the interrupt is greater than of the CPU, the ISR will be vectored to.
//If the IPL of the interrupt is not greater, execution will resume after the PWRSAV instruction.

int main() {
   
    //Zero Timer1 timer register and set period
    TMR1 = 0x00000000;
    PR1 =  0x0000ABCD;
   
    _T1IF = 0; //Clear Timer1 interrupt flag
    _T1IE = 1; //Enable Timer1 interrupt
    
    //When the CPU IPL is greater than the Timer1 IPL, the Timer1 ISR will wake the
    //device up from power saving mode, but the ISR will not be vectored to.
    //The CPU's IPL (SR[2:0]) resets to 0b000, and therefore must be increased
    //in order for an ISR to have a lower IPL than the CPU.
    
    //Increase the CPU IPL to 2 (from reset value of 0) 
    asm volatile("BSET SR, #6");
    //Decrease the Timer1 IPL to 1 (from reset value of 4)
    _T1IP = 1;
    //Comment the above lines out for the Timer1 ISR to execute when the device wakes up.
    
    T1CONbits.TCS = 0; //Select UPB clock as Timer1 clock source
    T1CONbits.TON = 1; //Enable Timer1
    
    Idle();
    
    //RC7 high indicates the device woke from Idle
    _LATC7 = 1;
    _TRISC7 = 0;
    
    while(1);
    
    return 0;
}

void __attribute__((__interrupt__)) _T1Interrupt() {
    _T1IF = 0;
    
    //RC2 high indicates Timer1 ISR was vectored to
    _LATC2 = 1;
    _TRISC2 = 0;
    
}

#endif

#ifdef SLEEP_MODE_WAKE_USING_EXTERNAL_INTERRUPT

int main() {
    
    //Set RD9 as input
    _TRISD9 = 1;
    //Map external interrupt 1 function to RD9 (RP58)
    _INT1R = 58;
    
    //Delay before enabling interrupt to allow signal to stabilize
    for (int i = 0; i < 8; i++);
    
    
    //Clear INT1 flag
    _INT1IF = 0;
    //Enable INT1 interrupt
    _INT1IE = 1;
    
    //Enter sleep mode
    Sleep();
    
    //A high-to-low edge on RD9 will trigger the INT1 interrupt, waking the device.
    
    //RC7 high indicates device woke up from sleep
    _LATC7 = 1;
    _TRISC7 = 0;
    
    while(1);
    
    return 0;
}

void __attribute__((__interrupt__)) _Int1Interrupt() {
    _INT1IF = 0;
}

#endif


#ifdef PMD_SCCP_DISABLE_EXAMPLE

int main() {
       
    //Select RC7 as output
    _TRISC7 = 0;
    //Map SCCP1 output function to RC7 (RP40)
    _RP40R = 24;
    
    //Set CCP1 up for dual buffered output capture mode
    CCP1CON1bits.MOD = 0b0101;
    CCP1PR = 0xBEEF;
    CCP1RA = 0x1234;
    CCP1RB = 0x4321;
    
    //Enable SCCP1
    CCP1CON1bits.ON = 1;
    
    //Set RD9 as input
    _TRISD9 = 1;
    //Wait for a low level on RD9
    while(_RD9);
       
    //Disable SCCP1 using PMD
   _CCP1MD = 1;
   
   //If SCCP1 is re-enabled at this point, its SFRs are in reset state and need to be re-initialized.
    
    while(1);
    
    return 0;
}

#endif