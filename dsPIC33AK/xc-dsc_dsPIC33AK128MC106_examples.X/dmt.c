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
 * File:   dmt.c
 * Author: C68555
 *
 * Created on August 24, 2023, 2:38 PM
 */


#include "xc.h"
#include "examples.h"


#ifdef DMT_CLEARING_BEFORE_COUNT_MATCH_EXAMPLE

int main() {
    
    //Configure DMT count and window intervals
    PSCNT = 0x0000FFFF;
    PSINTV = 0x000000FF;
    //Enable DMT
    DMTCONbits.ON = 1;
    
    _Bool pre_cleared = 0;
    
    while(1) {

        while(DMTCNT == 0); //Wait for DMT to start counting before clear sequence
        
        //1) Assign correct pre-clear value to DMTPRECLR
        DMTPRECLRbits.STEP1 = 0b01000000;
        //2) Wait for DMT Clear window to open (DMTCNT >= PSINTV)
        while(!DMTSTATbits.WINOPN);
        //3) Assign correct clear value to DMTCLR
        DMTCLRbits.STEP2 = 0b00001000;
    }
}

void __attribute__((__interrupt__)) _GeneralTrap() {
    if (INTCON5bits.DMT) {
        while(1);
    }
}

#endif



#ifdef DMT_CLEARING_AFTER_DMT_EVENT_EXAMPLE

int main() {
    
    //Configure DMT count and window intervals
    PSCNT = 0x00000FFF;
    PSINTV = 0x000000FF;
    //Enable DMT
    DMTCONbits.ON = 1;
    
    while(1); //Intentionally not clearing the DMT, trap will be taken.
}

void __attribute__((__interrupt__)) _GeneralTrap() {
    if (INTCON5bits.DMT) {
        
        //Clear the DMT after it has run out by writing in the proper sequence
        PPPCbits.NMISTEP1 = 0b01000000;
        PPCbits.NMISTEP2 = 0b10001000;
        
        //Despite being able to clear the DMT, INTCON5bits.DMT cannot be cleared
        //except by device reset. Therefore, this ISR cannot be returned from.

        //Can execute any code needed for a safe shutdown here
        
        //Reset the device
        asm volatile("RESET");
        
    }
}

#endif
