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
 * File:   wdt.c
 * Author: C68555
 *
 * Created on August 24, 2023, 12:40 PM
 */


#include "xc.h"
#include "examples.h"


#ifdef WDT_CONFIGURATION_BITS_EXAMPLE

//Code example to configure Run and Sleep WDT for 1sec, HW-controlled, via config fuses 
// FWDT
#pragma config FWDT_WINEN = ON // Watchdog Timer Window Enable bit (Watchdog Timer operates in Window mode)
#pragma config FWDT_SWDTMPS = PS1024 // Sleep Mode Watchdog Timer Post Scaler select bits (1:1024)
#pragma config FWDT_RCLKSEL = BPRC256 // Watchdog Timer Clock select bits (WDT Run Mode uses BFRC:256)
#pragma config FWDT_RWDTPS = PS1024 // Run Mode Watchdog Timer Post Scaler select bits (1:1024)
#pragma config FWDT_WDTWIN = WIN25 // Watchdog Timer Window Size Select bits (WDT Window is 25% of WDT period)
#pragma config FWDT_WDTRSTEN = ON // Watchdog Timer Reset Enable bit (WDT event generates a reset)
#pragma config FWDT_WDTEN = HW // Watchdog Timer Enable bit (WDT is enabled in hardware)
//This means that the WDT is always enabled, with WDTCON settings defined by the above config bits (read-only)

#endif


#ifdef WDT_RUN_MODE_EXAMPLE

// FWDT
#pragma config FWDT_WINEN = ON // Watchdog Timer Window Enable bit (Watchdog Timer operates in Window mode)
#pragma config FWDT_SWDTMPS = PS1024 // Sleep Mode Watchdog Timer Post Scaler select bits (1:1024)
#pragma config FWDT_RCLKSEL = BPRC256 // Watchdog Timer Clock select bits (WDT Run Mode uses BFRC:256)
#pragma config FWDT_RWDTPS = PS1024 // Run Mode Watchdog Timer Post Scaler select bits (1:1024)
#pragma config FWDT_WDTWIN = WIN25 // Watchdog Timer Window Size Select bits (WDT Window is 25% of WDT period)
#pragma config FWDT_WDTRSTEN = ON // Watchdog Timer Reset Enable bit (WDT event generates a reset)
#pragma config FWDT_WDTEN = SW // Watchdog Timer Enable bit (WDT is controlled by software, use WDTCON.ON bit)

int main(void) {
    
    //Set RC4 to observe reset.
    _LATC4 = 1;
    _TRISC4 = 0;
    //Externally pull the pin down, and it will go low once a reset occurs.
    
    //Configure Run mode WDT for a period of 1s.
    WDTCONbits.RMCLK = 3; // BFRC/256 (31.25kHz) as Run WDT Clock
    WDTCONbits.RMPS = 10; // Run Postscaler 1024
    WDTCONbits.ON = 1; // Enable WDT
    
    while(1); //Device will reset with 1s period as WDT is not cleared.
  
}

#endif

#ifdef WDT_SLEEP_MODE_EXAMPLE

// FWDT
#pragma config FWDT_WINEN = ON // Watchdog Timer Window Enable bit (Watchdog Timer operates in Window mode)
#pragma config FWDT_SWDTMPS = PS1024 // Sleep Mode Watchdog Timer Post Scaler select bits (1:1024)
#pragma config FWDT_RCLKSEL = BPRC256 // Watchdog Timer Clock select bits (WDT Run Mode uses BFRC:256)
#pragma config FWDT_RWDTPS = PS1024 // Run Mode Watchdog Timer Post Scaler select bits (1:1024)
#pragma config FWDT_WDTWIN = WIN25 // Watchdog Timer Window Size Select bits (WDT Window is 25% of WDT period)
#pragma config FWDT_WDTRSTEN = ON // Watchdog Timer Reset Enable bit (WDT event generates a reset)
#pragma config FWDT_WDTEN = SW // Watchdog Timer Enable bit (WDT is controlled by software, use WDTCON.ON bit)

int main(void) {
    
    //Configure RC4 as output indicator
    _LATC4 = 1;
    _TRISC4 = 0;
    
    //Sleep mode WDT uses the fixed clock source of BFRC/256 (31.25kHz).
    //The 1024 prescaler will result in a period of 1s.
    WDTCONbits.SMPS = 10; // Sleep Postscaler 1024
    WDTCONbits.ON = 1; // Enable WDT

    //The Sleep mode WDT will wake the device periodically. 
    //RC4 indicates the WDT timeout period.
    while(1) {
        asm volatile("BTG LATC, #4");
        Sleep();
    }
    
}

#endif


#ifdef WDT_WINDOWED_CLEAR_EXAMPLE

// FWDT
#pragma config FWDT_WINEN = ON // Watchdog Timer Window Enable bit (Watchdog Timer operates in Window mode)
#pragma config FWDT_SWDTMPS = PS1024 // Sleep Mode Watchdog Timer Post Scaler select bits (1:1024)
#pragma config FWDT_RCLKSEL = BPRC256 // Watchdog Timer Clock select bits (WDT Run Mode uses BFRC:256)
#pragma config FWDT_RWDTPS = PS1024 // Run Mode Watchdog Timer Post Scaler select bits (1:1024)
#pragma config FWDT_WDTWIN = WIN25 // Watchdog Timer Window Size Select bits (WDT Window is 25% of WDT period)
#pragma config FWDT_WDTRSTEN = ON // Watchdog Timer Reset Enable bit (WDT event generates a reset)
#pragma config FWDT_WDTEN = SW // Watchdog Timer Enable bit (WDT is controlled by software, use WDTCON.ON bit)

#define FCY 8000000
#include <libpic30.h>


int main(void) {
    
    //Set RC4 high to observe reset.
    _LATC4 = 1;
    _TRISC4 = 0;
    //Externally pull RC4 down, and it will go low if a reset occurs.
    
    //Configure Run mode WDT for 1s period
    WDTCONbits.RMCLK = 3; // BFRC/256 (31.25kHz) as Run WDT Clock
    WDTCONbits.RMPS = 10; // Run Postscaler 1024
    WDTCONbits.ON = 1; // Enable WDT
    
    //With the WDT in Windowed mode, window is during the last "N"% of the period.
    //Here, the last 25% of the WDT period has been selected using FWDT_WDTWIN.
    //For a 1ms period, this requires a delay of at least 750ms from when the WDT
    //first started, or when it was last cleared, before clearing the WDT.
    while(1) {
        __delay_ms(800); // 800ms delay
        ClrWdt(); // Clears Run Counter after 800ms delay
    }
}
 
#endif