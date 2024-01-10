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
 * File:   ptg.h
 * Author: 
 * Comments:
 * Revision history: 
 */

// This is a guard condition so that contents of this file are not included
// more than once.  
#ifndef PTG_H
#define	PTG_H

#include <xc.h> // include processor files - each processor file is guarded.  

/* PTG Commands */
#define PTGCTRL(x) ((0b00000000) | ((x) & 0b00001111))
#define PTGADD(x) ((0b00010000) | ((x) & 0b00000111))
#define PTGCOPY(x) ((0b00011000) | ((x) & 0b00000111))
#define PTGSTRB(x) ((0b00100000) | ((x) & 0b00011111))
#define PTGWHI(x) ((0b01000000) | ((x) & 0b00001111))
#define PTGWLO(x) ((0b01010000) | ((x) & 0b00001111))
#define PTGIRQ(x) ((0b01110000) | ((x) & 0b00001111))
#define PTGTRIG(x) ((0b10000000) | ((x) & 0b00011111))
#define PTGJMP(x) ((0b10100000) | ((x) & 0b00011111))
#define PTGJMPC0(x) ((0b11000000) | ((x) & 0b00011111))
#define PTGJMPC1(x) ((0b11100000) | ((x) & 0b00011111))
#define PTGNOP 0

// Used with PTGCTRL command
typedef enum
{
    ptgNop = 0b0000,  
    stepDelayDisable = 0b0010,
    stepDelayEnable = 0b0110,
    t0Wait = 0b1000,
    t1Wait = 0b1001,
    softTriggerLevelWait = 0b1010,
    softTriggerEdgeWait = 0b1011,
    c0Strobe = 0b1100,
    c1Strobe = 0b1101,
    l0Strobe = 0b1110,
    triggerGenerate = 0b1111,
} CTRL_T;

// Used with PTGADD and PTGCOPY commands
typedef enum
{
    c0Limit = 0b0000,
    c1Limit = 0b0001,
    t0Limit = 0b0010,
    t1Limit = 0b0011,
    stepDelay = 0b0100,
    literal0 = 0b0101,
} ADD_COPY_T;

#ifdef	__cplusplus
extern "C" {
#endif /* __cplusplus */

    // TODO If C++ is being used, regular C code needs function names to have C 
    // linkage so the functions can be used by the c code. 

#ifdef	__cplusplus
}
#endif /* __cplusplus */

#endif	/* PTG_H */

