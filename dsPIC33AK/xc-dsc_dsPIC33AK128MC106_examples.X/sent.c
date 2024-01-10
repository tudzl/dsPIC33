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
 * File:   sent.c
 * Author: C68555
 *
 * Created on September 18, 2023, 5:03 PM
 */

#include "xc.h"
#include "examples.h"

uint8_t recommended_j22716_crc_implementation(uint8_t* rec_data, uint8_t num_nibbles) {

    //Lookup table
    uint8_t crc_table [] = {0,13,7,10,14,3,9,4,1,12,6,11,15,2,8,5};
    // Initialize checksum to seed value
    uint8_t checksum = 5;
    
    // For each data nibble, bit-wise XOR with lookup value from table
    for(int i=0; i < num_nibbles; i++)
    {
        checksum = rec_data[i] ^ crc_table[checksum];
    }
    // Bit-wise XOR with additional 0 value
    checksum = 0 ^ crc_table[checksum];
    
    return checksum;
}

#ifdef SENT_ASYNCHRONOUS_TRANSMISSION

#define FCLK (4E+6)
#define TICK_TIME (70E-6)
#define FRAME_TIME (25E-3)

int main(void) { 

    _RP44R = 57; //Assign SENT1OUT output function to RP44 (RC11)
    _TRISC11 = 0; //RC11 as output

    SENT1CON1bits.RCVEN = 0; // Module operates as a transmitter
    SENT1CON1bits.TXM = 0; // Asynchronous Transmit
    SENT1CON1bits.NIBCNT = 6; // 6 data nibbles per data packet
    SENT1CON1bits.CRCEN = 1; // CRC is calculated by hardware using the J2716 method
    SENT1CON1bits.PPP = 1; // SENTx messages transmitted with Pause Pulse 
    SENT1CON1bits.PS = 0; // Module clock is TCY
    SENT1CON2 = ( (TICK_TIME * FCLK) - 1); // TICKTIME
    SENT1CON3 = ( FRAME_TIME / TICK_TIME) ; // FRAMETIME

    _SENT1IE = 1; // Enable SENT1 interrupt
    _SENT1IP = 4; // SENT interrupt priority

    SENT1DATbits.STAT = 0; // Status Nibble
    SENT1DATbits.DATA1 = 1; // Data Nibble 1
    SENT1DATbits.DATA2 = 2; // Data Nibble 2
    SENT1DATbits.DATA3 = 3; // Data Nibble 3
    SENT1DATbits.DATA4 = 4; // Data Nibble 4
    SENT1DATbits.DATA5 = 5; // Data Nibble 5
    SENT1DATbits.DATA6 = 6; // Data Nibble 6

    SENT1CON1bits.ON = 1; // Enable SENT module

    while(1);

    return 0;
}

void __attribute__((__interrupt__)) _SENT1Interrupt (void)
{
    _SENT1IF = 0; // Clear interrupt flag 
    // Update SENT1DATA here
}

#endif

#ifdef SENT_SYNCHRONOUS_TRANSMISSION

#define FCLK (4E+6)
#define TICK_TIME (70E-6)
#define FRAME_TIME (25E-3)

int main(void) {
    
    _RP44R = 57; //Assign SENT1OUT output function to RP44 (RC11)
    _TRISC11 = 0; //RC11 as output
 
    SENT1CON1bits.RCVEN = 0; // Module operates as a transmitter
    SENT1CON1bits.TXM = 1; // Synchronous Transmit
    SENT1CON1bits.NIBCNT = 6; // 6 data nibbles per data packet
    SENT1CON1bits.CRCEN = 1; // CRC is calculated by hardware using the J2716 method
    SENT1CON1bits.PPP = 1; // SENTx messages transmitted with Pause Pulse 
    SENT1CON1bits.PS = 0; // Module clock is TCY
    SENT1CON2 = ( (TICK_TIME * FCLK) - 1); // TICKTIME
    SENT1CON3 = ( FRAME_TIME / TICK_TIME) ; // FRAMETIME
 
    _SENT1IE = 1; // Enable SENT1 interrupt
    _SENT1IP = 4; // Set SENT1 interrupt priority
 
    SENT1CON1bits.ON = 1; // Enable SENT module
    
    //Delay before sending first message
    for (int i = 0; i < 0x123; i++);
    
    SENT1DATbits.STAT = 0; // Status Nibble
    SENT1DATbits.DATA1 = 1; // Data Nibble 1
    SENT1DATbits.DATA2 = 2; // Data Nibble 2
    SENT1DATbits.DATA3 = 3; // Data Nibble 3
    SENT1DATbits.DATA4 = 4; // Data Nibble 4
    SENT1DATbits.DATA5 = 5; // Data Nibble 5
    SENT1DATbits.DATA6 = 6; // Data Nibble 6

    SENT1STATbits.SYNCTXEN = 1; // Initiate Synchronous Transmission 
    
    while(1) {
        
        //Delay before sending next message
        for (int i = 0; i < 0x123; i++);
        
        // Update SENT1DATA here
        
        SENT1STATbits.SYNCTXEN = 1; // Initiate Synchronous Transmission 
        while(SENT1STATbits.SYNCTXEN); //Wait
    }
    
    return 0;
}

void __attribute__((__interrupt__)) _SENT1Interrupt (void)
{
    _SENT1IF = 0; // Clear interrupt flag
}


#endif


#ifdef SENT_RECEPTION

#define FCLK (4E+6)
#define TICK_TIME (70E-6)
#define FRAME_TIME (25E-3)
#define SYNC_COUNT (8 * FCLK * TICK_TIME)
#define SYNC_MIN (0.8 * SYNC_COUNT)
#define SYNC_MAX (1.2 * SYNC_COUNT)

uint8_t received_data[6];
uint8_t i;

int main(void) {

    _TRISC10 = 1; //RC10 as input
    _SENT1R = 43; //Assign SENT1 input function to RP43 (RC10)
    
    SENT1CON1bits.RCVEN = 1; // Module operates as a receiver
    SENT1CON1bits.NIBCNT = 6; // 6 data nibbles per data packet
    SENT1CON1bits.CRCEN = 1; // CRC is calculated by hardware using the J2716 method
    SENT1CON1bits.PPP = 1; // SENTx messages transmitted with Pause Pulse 
    SENT1CON1bits.PS = 0; // Module clock is TCY
    
    SENT1CON2 = SYNC_MAX;
    SENT1CON3 = SYNC_MIN;

    _SENT1IE = 1; // Enable SENT1 interrupt
    _SENT1IP = 4; // set SENT interrupt priority

    SENT1CON1bits.ON = 1; // Enable SENT module

    while(1);
    
    return 0;
}

void __attribute__((__interrupt__)) _SENT1Interrupt (void)
{
    _SENT1IF = 0; // Clear interrupt flag 
    
    i=0;
    received_data[i++] = SENT1DATbits.DATA1; // Read Data Nibble 1
    received_data[i++] = SENT1DATbits.DATA2; // Read Data Nibble 2
    received_data[i++] = SENT1DATbits.DATA3; // Read Data Nibble 3
    received_data[i++] = SENT1DATbits.DATA4; // Read Data Nibble 4
    received_data[i++] = SENT1DATbits.DATA5; // Read Data Nibble 5
    received_data[i++] = SENT1DATbits.DATA6; // Read Data Nibble 6
}


#endif

#ifdef SENT_TRANSMISSION_SPC_PULSE_RECEPTION

#include <xc.h>
#define FCY (8E+06)
#include <libpic30.h>

#define FCLK (4E+6)
#define TICK_TIME (50E-6)
#define FRAME_TIME (25E-3)
#define SPC_PULSE_WIDTH (56 * TICK_TIME)

void InputCapture_configure(void);
void SENT_Tx_configure(void);

uint8_t count = 0;
uint16_t falling_edge_capture = 0;
uint16_t raising_edge_capture = 0; 

uint16_t delta = 0;
uint16_t dummy = 0;

float pulse_width;

int main(void) {
    
    SENT_Tx_configure(); // Configure SENT for Transmission
    InputCapture_configure(); // Configure Input Capture 
    
    while (1);
    return 0;
}
 
void InputCapture_configure(void) {
    
    _ICM1R = 44; //Assign Input Capture 1 input function to RP44 (RC11)
    
    CCP1CON1bits.CCSEL = 1; // Input capture mode
    CCP1CON1bits.CLKSEL = 0; // Set the clock source (1/2 speed UPB clock)

    CCP1CON1bits.T32 = 0; // 16-bit Dual Timer mode
    CCP1CON1bits.MOD = 3; // Capture every edge of the event
    CCP1CON2bits.ICS = 0; // Input capture source is IC1 pin
    CCP1CON1bits.OPS = 0; // Interrupt on every input capture event
    CCP1CON1bits.TMRPS = 0; // Set the clock pre-scaler (1:1)
    CCP1CON1bits.ON = 1; // Enable CCP/input capture 
    
    __delay_ms(1);
    
    _CCP1IF = 0; // Clear CCP interrupt flag
    _CCP1IE = 1; // Enable CCP interrupt
}

void SENT_Tx_configure(void) {
    
    _RP44R = 57; //Assign SENT1OUT output function to RP44 (RC11)
    
    SENT1CON1bits.RCVEN = 0; // Module operates as a transmitter
    SENT1CON1bits.TXM = 1; // Synchronous Transmit
    SENT1CON1bits.NIBCNT = 6; // 6 data nibbles per data packet
    SENT1CON1bits.CRCEN = 1; // CRC is calculated by hardware using the J2716 method
    SENT1CON1bits.PPP = 1; // SENTx messages transmitted with Pause Pulse 
    SENT1CON1bits.PS = 0; // Module clock is TCY
    
    SENT1CON2 = ((TICK_TIME * FCLK) - 1); // TICKTIME
    SENT1CON3 = (FRAME_TIME / TICK_TIME); // FRAMETIME
    _SENT1IE = 1; // Enable SENT1 interrupt
    
    SENT1CON1bits.ON = 1; // Enable SENT module
    SENT1DATbits.STAT = 0; // Status Nibble
    SENT1DATbits.DATA1 = 1; // Data Nibble 1
    SENT1DATbits.DATA2 = 2; // Data Nibble 2
    SENT1DATbits.DATA3 = 3; // Data Nibble 3
    SENT1DATbits.DATA4 = 4; // Data Nibble 4
    SENT1DATbits.DATA5 = 5; // Data Nibble 5
    SENT1DATbits.DATA6 = 6; // Data Nibble 6 
}

void __attribute__((__interrupt__)) _CCP1Interrupt(void) {
    _CCP1IF = 0; // Clear interrupt flag
    count++;
    if (count == 1) // Falling edge of the SPC pulse
    { 
        dummy = CCP1BUF; // Dummy read
        falling_edge_capture = CCP1BUF;
    } 
    else if (count == 2) // Raising edge of the SPC pulse
    { 
        count = 0; // Clear count
        raising_edge_capture = CCP1BUF;
        if (raising_edge_capture <= falling_edge_capture) 
        {
            delta = (0xFFFF - falling_edge_capture) + raising_edge_capture; // Rollover case
        } 
        else 
        {
            delta = raising_edge_capture - falling_edge_capture; // Non-rollover case
        }
        pulse_width = delta / FCLK; // Calculate the pulse width
        // Check if pulse width is within the range
//        if((SPC_PULSE_WIDTH * 1.2) >= pulse_width && (SPC_PULSE_WIDTH * 0.8) <= pulse_width ){
            _CCP1IE = 0; // Disable CCP interrupt
            CCP1CON1bits.ON = 0; // Disable CCP/input capture
            
            //Delay before beginning transmit
            __delay_ms(1);
            
            SENT1STATbits.SYNCTXEN = 1; // Initiate Synchronous Transmission
//        }
    }
}

void __attribute__((__interrupt__)) _SENT1Interrupt(void) {
    _SENT1IF = 0; // Clear SENT1 interrupt flag 
    while (SENT1STATbits.PAUSE == 1); // Wait till PAUSE pulse is transmitted
    CCP1CON1bits.ON = 1; // Enable CCP/input capture
    __delay_ms(1);
    _CCP1IF = 0; // Clear CCP interrupt flag
    _CCP1IE = 1; // Enable CCP interrupt
}


#endif

#ifdef SENT_RECEPTION_SPC_PULSE_TRANSMISSION

#include <xc.h>
#define FCY (8E+6)
#include <libpic30.h>

#define FCLK (4E+6)
#define TICK_TIME (50E-6)
#define FRAME_TIME (25E-3)
#define SYNC_COUNT (8 * FCLK * TICK_TIME)
#define SYNC_MIN (0.8 * SYNC_COUNT)
#define SYNC_MAX (1.2 * SYNC_COUNT)

void OutputCompare_configure();
void SENT_RX_configure();
void SendSPCPulse();

uint8_t received_data[7];
uint8_t i;

int main(void) {

    _TRISD5 = 1; //Connect a pull-up switch to RD5
    
    OutputCompare_configure(); // Configure Output Compare
    SENT_RX_configure(); // Configure SENT for Reception
        
    while(1)
    {    
        while(_RD5); // Wait till pull-up switch is pressed
    
        if(SENT1STATbits.RXIDLE == 1) // Check if the line is Idle
        {
            //Receiver can request for SENT data through SendSPCPulse() function
            SendSPCPulse();
        }
    }
    
    return 0;
}

void OutputCompare_configure(){
    
    _CCP3IF = 0; // Clear CCP interrupt flag
    _CCP3IE = 1; // Enable CCP interrupt

    // Set MCCP operating mode
    CCP3CON1bits.CCSEL = 0; // Set MCCP operating mode (OC mode)
    CCP3CON1bits.MOD = 0b0100; // Dual edge compare mode

    //Configure MCCP Timebase
    CCP3CON1bits.T32 = 0; // Set timebase width (16-bit)
    CCP3CON1bits.TMRSYNC = 0; // Set timebase synchronization
    CCP3CON1bits.CLKSEL = 0b000; // Set the clock source (1/2 speed UPB clock)
    CCP3CON1bits.TMRPS = 0b00; // Set the clock pre-scaler (1:1)
    CCP3CON1bits.ONESHOT = 1; //One-shot mode
    CCP3CON1bits.TRIGEN = 1; // Set Sync/Triggered mode (Synchronous)

    //Configure SCCP output for PWM signal
    CCP3CON2bits.OCAEN = 1; // Enable desired output signals (OC1A)
    CCP3CON3bits.POLACE = 1; //OCA output is active low
    CCP3CON3bits.OSCNT = 0;
    CCP3TMR = 0x00000000; //Initialize timer prior to enable module.

    CCP3RA = (56 * TICK_TIME * FCLK); // Set the rising edge compare value
    CCP3RB = (56 * TICK_TIME * FCLK)*2; // Set the falling edge compare value
    
    CCP3PR = CCP3RB; //Configure timebase period
    
    CCP3CON1bits.ON = 1; // Enable SCCP module 
}

void SENT_RX_configure()
{    
    _RP43R = 57; // RP43 (RC10) as SENT1 output
    _SENT1R = 43; // RP43 (RC10) as SENT1 input
    
    SENT1CON1bits.RCVEN = 1; // Module operates as a receiver
    SENT1CON1bits.SPCEN = 1; // Enable SPC
    SENT1CON1bits.NIBCNT = 6; // 6 data nibbles per data packet
    SENT1CON1bits.CRCEN = 1; // CRC is calculated by hardware using the J2716 method
    SENT1CON1bits.PPP = 1; // SENTx messages transmitted with Pause Pulse 
    SENT1CON1bits.PS = 0; // Module clock is TCY
    SENT1CON1bits.SPCEN = 1; // Enable SPC

    SENT1CON2 = SYNC_MAX; 
    SENT1CON3 = SYNC_MIN; 
    _SENT1IE = 1; // Enable SENT1 interrupt
    
    SENT1CON1bits.ON = 1; // Enable SENT module 
}

void SendSPCPulse()
{
    //If a SENT message was not received in response to last SPC pulse, SCCP3 is still disabled.
    //In this case, re-enable SCCP3 and delay before triggering.
    if (!CCP3CON1bits.ON) {
        CCP3CON1bits.ON = 1;
        __delay_ms(1);
    }
    
    CCP3STATbits.TRSET = 1; // Set the Trigger
    while(CCP3STATbits.CCPTRIG);
}

void __attribute__((__interrupt__)) _CCP3Interrupt (void)
{
    _CCP3IF = 0; // Clear interrupt flag
    CCP3STATbits.TRSET = 0; // Set the Trigger
    CCP3CON1bits.ON = 0; // Disable SCCP module 
}

void __attribute__((__interrupt__)) _SENT1Interrupt (void)
{
    _SENT1IF = 0; // Clear interrupt flag 

    i=0;
    received_data[i++] = SENT1DATbits.STAT; // Read STAT
    received_data[i++] = SENT1DATbits.DATA1; // Read DATA1
    received_data[i++] = SENT1DATbits.DATA2; // Read DATA2
    received_data[i++] = SENT1DATbits.DATA3; // Read DATA3
    received_data[i++] = SENT1DATbits.DATA4; // Read DATA4
    received_data[i++] = SENT1DATbits.DATA5; // Read DATA5
    received_data[i++] = SENT1DATbits.DATA6; // Read DATA6
    
    while(SENT1STATbits.PAUSE == 1); // Wait till PAUSE pulse is received
    
    CCP3CON1bits.ON = 1; // Enable SCCP module
}

#endif