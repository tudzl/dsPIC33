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
 * File:   main.c
 * Author: C68555
 *
 * Created on May 22, 2023, 2:42 PM
 */

/*
 * File:   main.c
 * Author: Zell
 * V0.1, added uart init codes, RC7 TX working OK, original codes not working
 * Modified on Jan 16, 2024,  
 */


#include "xc.h"
#include "examples.h"
#include "stdio.h"
#include "string.h"

//Some code examples are complete programs and define their own main(),
//if one of those is used, comment this main() out.
//#define ADC_SINGLE_CONVERSION_EXAMPLE  comment out to disable this example, see example.h

int UART_MCP2221_init(void);

int main(void) {
    
    UART_MCP2221_init();
    //printf("<This is the hello world demo for dsPIC33AK device test!>");
    
    IFS2bits.U1TXIF = 0; // Clear TX interrupt flag
    U1TXREG = "Z" ;
    IEC2bits.U1TXIE = 0;
    while(1){
        
        //Re-transmit periodically
        if (IEC2bits.U1TXIE == 0) { //Check if TX interrupt was disabled
            //Delay
            for (uint16_t i = 0; i < 0x3000; i++);
            //Re-enable TX interrupt to resume transmission
            IEC2bits.U1TXIE = 1;
        }
        

}
    
    return 0;
}

