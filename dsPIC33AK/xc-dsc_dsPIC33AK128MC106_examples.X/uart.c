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
 * File:   uart.c
 * Author: C68555
 *
 * Created on July 28, 2023, 3:36 PM
 */

#include "xc.h"
#include "examples.h"


#ifdef UART_TRANSMISSION_WITH_MCP2221

//Clock settings for CPU @8MHz, UART1 clocked from 1/2 speed UPB bus @4MHz
#define FP 4000000
#define BAUDRATE 9600
//Baud rate calculation for fractional baud rate divider
#define BRGVAL (FP/BAUDRATE)

#define TRANSMIT_CHAR_SIZE 16
uint8_t transmitChar[TRANSMIT_CHAR_SIZE] = "ABCDEFGHIJKLMNOP";
uint8_t transmitCount = 0;

int UART_MCP2221_init(void){
    
        //clock_CPU_from_FRC(); //added by zell
    //clock_CPU_for_200MIPS_from_PLL1();  //added by zell
    //clock_CPU_from_FRC();
    //enable_POSC_for_8MHz_crystal();
    //Configure I/O
    //_RP41R = 9;     //Assign UART1 TX output functionality to RP41 (RC8)
   // _TRISC8 = 0;    //Set RC8 as output
    
    //Select RC7 as output, UART_TX-->MCP2221, by Zell
    _RP40R = 9;     //Assign UART1 TX output functionality to RP40 (RC7)
    _TRISC7 = 0;

    U1CONbits.MODE = 0;    // Asynchronous 8-bit UART
    U1CONbits.CLKSEL = 0; // 1/2 speed UPB clock as Baud Clock source
    U1CONbits.STSEL = 0;   // 1 stop bit

    //Use fractional baud rate divider
    U1CONbits.CLKMOD = 1;   
    U1BRG = BRGVAL; // Baud Rate setting for 9600

    U1STATbits.TXWM = 0; // Interrupt when TX buffer is empty (8 empty slots)
    IEC2bits.U1TXIE = 1; // Enable Transmit interrupt
    
    U1CONbits.ON = 1; // Enable UART
    U1CONbits.TXEN = 1; // Enable UART TX. This generates TX interrupt.
    
}

void __attribute__((interrupt)) _U1TXInterrupt(void)
{ 
    IFS2bits.U1TXIF = 0; // Clear TX interrupt flag
    U1TXREG = transmitChar[transmitCount++]; // Transmit one character
    
    if (transmitCount >= TRANSMIT_CHAR_SIZE) {
        transmitCount = 0;
        //Stop transmitting by disabling the TX interrupt.
        IEC2bits.U1TXIE = 0;
    }
}

#endif


#ifdef UART_TRANSMISSION_WITH_INTERRUPTS

//Clock settings for CPU @8MHz, UART1 clocked from 1/2 speed UPB bus @4MHz
#define FP 4000000
#define BAUDRATE 9600
//Baud rate calculation for fractional baud rate divider
#define BRGVAL (FP/BAUDRATE)

#define TRANSMIT_CHAR_SIZE 16
uint8_t transmitChar[TRANSMIT_CHAR_SIZE] = "ABCDEFGHIJKLMNOP";
uint8_t transmitCount = 0;




int main(void) {    
    //clock_CPU_from_FRC(); //added by zell
    //clock_CPU_for_200MIPS_from_PLL1();  //added by zell
    //clock_CPU_from_FRC();
    //enable_POSC_for_8MHz_crystal();
    //Configure I/O
    //_RP41R = 9;     //Assign UART1 TX output functionality to RP41 (RC8)
   // _TRISC8 = 0;    //Set RC8 as output
    
    //Select RC7 as output, UART_TX-->MCP2221, by Zell
    _RP40R = 9;     //Assign UART1 TX output functionality to RP40 (RC7)
    _TRISC7 = 0;

    U1CONbits.MODE = 0;    // Asynchronous 8-bit UART
    U1CONbits.CLKSEL = 0; // 1/2 speed UPB clock as Baud Clock source
    U1CONbits.STSEL = 0;   // 1 stop bit

    //Use fractional baud rate divider
    U1CONbits.CLKMOD = 1;   
    U1BRG = BRGVAL; // Baud Rate setting for 9600

    U1STATbits.TXWM = 0; // Interrupt when TX buffer is empty (8 empty slots)
    IEC2bits.U1TXIE = 1; // Enable Transmit interrupt
    
    U1CONbits.ON = 1; // Enable UART
    U1CONbits.TXEN = 1; // Enable UART TX. This generates TX interrupt.
    //printf("<This is the hello world demo for dsPIC33AK device test!>"); //not working
    
    while(1) {
        //Re-transmit periodically
        if (IEC2bits.U1TXIE == 0) { //Check if TX interrupt was disabled
            //Delay
            for (uint16_t i = 0; i < 0x1234; i++);
            //Re-enable TX interrupt to resume transmission
            IEC2bits.U1TXIE = 1;
        }
    }
    
    return 0;
}

void __attribute__((interrupt)) _U1TXInterrupt(void)
{ 
    IFS2bits.U1TXIF = 0; // Clear TX interrupt flag
    U1TXREG = transmitChar[transmitCount++]; // Transmit one character
    
    if (transmitCount >= TRANSMIT_CHAR_SIZE) {
        transmitCount = 0;
        //Stop transmitting by disabling the TX interrupt.
        IEC2bits.U1TXIE = 0;
    }
}

#endif

#ifdef UART_RECEPTION_WITH_INTERRUPTS

//Clocking based on CPU @8MHz, UART1 running on 1/2 speed UPB bus @4MHz
#define FP 4000000
#define BAUDRATE 9600
//Baud rate calculation for fractional baud rate mode
#define BRGVAL (FP/BAUDRATE)

#define RECEIVED_CHAR_SIZE 16
uint8_t receivedChar[RECEIVED_CHAR_SIZE];
uint8_t receivedCount = 0;

int main(void) {
    
    //Configure I/O
    _U1RXR = 39; //Assign RP39 (RC6) to UART1 RX input function
    _TRISC6 = 1; //Set RC6 as input

    U1CONbits.MODE = 0;    // Asynchronous 8-bit UART
    U1CONbits.CLKSEL = 0; // 1/2 speed UPB clock as Baud Clock source
    U1CONbits.STSEL = 0;   // 1 stop bit

    //Use fractional baud rate divider
    U1CONbits.CLKMOD = 1;   
    U1BRG = BRGVAL; // Baud Rate setting for 9600

    U1STATbits.RXWM = 0; // Interrupt when there is one word or more in the buffer
    IEC2bits.U1RXIE = 1; // Enable Receive interrupt

    U1CONbits.ON = 1; // Enable UART
    U1CONbits.RXEN = 1; // Enable UART RX.

    while(1) {}

    return 0;
}

void __attribute__((interrupt)) _U1RXInterrupt(void)
{ 
    IFS2bits.U1RXIF = 0; // Clear RX interrupt flag
    while(U1STATbits.RXBE == 0) // Check if RX buffer has data to read 
    { 
        receivedChar[receivedCount++] = U1RXREG; // Read a character from RX buffer
        
        //Avoid buffer overrun
        if (receivedCount >= RECEIVED_CHAR_SIZE) {
            receivedCount = 0;
        }
    }
}

#endif

#ifdef UART_ADDRESS_DETECT_TRANSMISSION

//Clocking based on CPU @8MHz, UART1 clocked from 1/2 speed UPB bus @4MHz
#define FP 4000000
#define BAUDRATE 9600
//Baud rate calculation for fractional baud rate mode
#define BRGVAL (FP/BAUDRATE)

#define ADDRESS1 0x45
#define ADDRESS2 0x55

#define TRANSMIT_CHAR_SIZE 4

uint8_t transmitChar1[TRANSMIT_CHAR_SIZE] = "abcd";
uint8_t transmitChar2[TRANSMIT_CHAR_SIZE] = "efgh";


int main(void) {

    //Configure I/O
    _RP41R = 9;     //Assign UART1 TX output functionality to RP41 (RC8)
    _TRISC8 = 0;    //Set RC8 as output

    U1CONbits.MODE = 4;    // Asynchronous 9-bit UART with address detect
    U1CONbits.CLKSEL = 0; // 1/2 speed UPB clock as Baud Clock source
    U1CONbits.STSEL = 0;   // 1 stop bit

    //Use fractional baud rate divider
    U1CONbits.CLKMOD = 1;   
    U1BRG = BRGVAL; // Baud Rate setting for 9600

    U1CONbits.ON = 1; // Enable UART
    U1CONbits.TXEN = 1; // Enable UART TX.
    
    //Transmit using Address1
    U1PAbits.P1 = ADDRESS1; // Write the address1 value to Parameter1
    
    /* Send data bytes for address1 */
    for (int i = 0; i < TRANSMIT_CHAR_SIZE; i++) {
        U1TXREG = transmitChar1[i];
    }
    //Wait for transmission to complete
    while(U1STATbits.TXMTIF == 0);
    
    //Transmit using Address2
    U1PAbits.P1 = ADDRESS2; // Write the address2 value to Parameter1
    
    /* Send data bytes for address2 */
    for (int i = 0; i < TRANSMIT_CHAR_SIZE; i++) {
        U1TXREG = transmitChar2[i];
    }
    //Wait for transmission to complete
    while(U1STATbits.TXMTIF == 0);
    
    //Periodically repeat the transmission
    while(1) {
        
        //Delay before re-transmitting
        for (int i = 0; i < 0x1234; i++);
        
        U1PAbits.P1 = ADDRESS1; // Write the address1 value to Parameter1
    
        /* Send data bytes for address1 */
        for (int i = 0; i < TRANSMIT_CHAR_SIZE; i++) {
            U1TXREG = transmitChar1[i];
        }
        //Wait for transmission to complete
        while(U1STATbits.TXMTIF == 0);

        U1PAbits.P1 = ADDRESS2; // Write the address2 value to Parameter1
        /* Send data bytes for address2 */
        for (int i = 0; i < TRANSMIT_CHAR_SIZE; i++) {
            U1TXREG = transmitChar2[i];
        }
        //Wait for transmission to complete
        while(U1STATbits.TXMTIF == 0);
        
    }
    
    return 0;
}

#endif

#ifdef UART_ADDRESS_DETECT_RECEPTION

//Clocking based on CPU @8MHz, UART1 clocked from 1/2 speed UPB bus @4MHz
#define FP 4000000
#define BAUDRATE 9600
//Baud rate calculation for fractional baud rate mode
#define BRGVAL (FP/BAUDRATE)

#define RECEIVED_CHAR_SIZE 16
uint8_t receivedChar[RECEIVED_CHAR_SIZE];
uint8_t receivedCharCount=0;

int main(void) {

    //Configure I/O
    _U1RXR = 39; //Assign RP39 (RC6) to UART1 RX input function
    _TRISC6 = 1; //Set RC6 as input

    U1CONbits.MODE = 4;    // Asynchronous 9-bit UART with address detect
    U1CONbits.CLKSEL = 0; // 1/2 speed UPB clock as Baud Clock source
    U1CONbits.STSEL = 0;   // 1 stop bit

    //Use fractional baud rate divider
    U1CONbits.CLKMOD = 1;   
    U1BRG = BRGVAL; // Baud Rate setting for 9600

    U1PAbits.P2 = 0x45; // Write parameter 2 with Address to match
    U1PBbits.P3 = 0xFF; // Write parameter 3 register with mask value

    U1STATbits.RXWM = 0; // Interrupt when there is one word or more in the buffer
    IEC2bits.U1RXIE = 1; // Enable Receive interrupt

    U1CONbits.ON = 1; // Enable UART
    U1CONbits.RXEN = 1; // Enable UART RX.
    
    while(1) {}
    
    return 0;
}

void __attribute__((interrupt)) _U1RXInterrupt(void)
    { 
    IFS2bits.U1RXIF = 0; // Clear RX interrupt flag
    while(U1STATbits.RXBE == 0) // Check if RX buffer has data to read 
    { 
        receivedChar[receivedCharCount++] = U1RXREG; // Read a character from RX buffer
        
        //Wrap around array index when array is full
        if (receivedCharCount >= RECEIVED_CHAR_SIZE) {
            receivedCharCount = 0;
        }
    }
}


#endif