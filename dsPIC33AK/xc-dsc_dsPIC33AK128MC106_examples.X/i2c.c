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
 * File:   i2c.c
 * Author: C68555
 *
 * Created on July 21, 2023, 12:08 PM
 */

#include "xc.h"
#include "examples.h"


#ifdef I2C_HOST_TRANSMISSION_7_BIT_ADDRESS

#pragma config FDEVOPT1_ALTI2C1 = ON //Using RD5 as SCL1 and RD6 as SDA1

#include <xc.h>

#define mCLIENT_ADDRESS 0x4C
#define INIT 0
#define ADDRESS_PHASE 1
#define DATA_WRITE 2

#define PACKET_SIZE 10

unsigned char phase = INIT;

unsigned char hostTransmit[PACKET_SIZE] = {0x11, 0x22, 0x33, 0x44, 0x55, 0x66, 0x77, 0x88, 0x99, 0xAA};
unsigned char transmittedCount = 0;

int main(void) {
    
    /*Configure I2C pins as digital*/
    //On dsPIC33AK128MC106, RD5 and RD7 have no analog functionality and are digital by default.
    
    //I2C module has Host and Client functionality which may both be active at the same time.
    //In this example, Host functionality will be configured and used.
    
    //Configure baud rate for SCL @ 100kHz from 1/2 bus UPB clock @ 4MHz
    I2C1LBRG = 16; //Set low-time baud rate
    I2C1HBRG = 16; //Set high-time baud rate
    
    /* Configure Bus IDle timeout*/
    I2C1CON2bits.BITE = 1;
    I2C1BITObits.BITOPR = 76;
    
    /* Configured interrupt enable bits*/
    I2C1INTCbits.HACKSIE = 1; // Assert HSTIF on ACK seq
    I2C1INTCbits.HDTXIE = 1; // Assert HSTIF on TX
    I2C1INTCbits.HSCIE = 1; // Assert HSTIF on start
    I2C1INTCbits.HSTIE = 1; // Assert I2CxIF when HSTIF is set
    I2C1CON2bits.PSZ = PACKET_SIZE; // Packet size
    I2C1CON2bits.EOPSC = 0b01; // End of packet will be set after data bytes
    I2C1CON1bits.ON = 1; // Enable I2C
    IFS2bits.I2C1IF = 0; // Clear I2C general interrupt 
    IEC2bits.I2C1IE = 1; // Enable I2C general interrupt

    /*wait for bus idle */
    while (!I2C1STAT2bits.BITO);
    phase = ADDRESS_PHASE;
    I2C1CON1bits.SEN = 1; // Send Start bit

    while (1) {
        
        //Once data transmission is complete, repeat the operation.        
        if ((transmittedCount >= PACKET_SIZE) && (I2C1STAT2bits.STOPE)) {
            
            I2C1STAT2bits.STOPE = 0;    //Clear STOPE flag to detect next STOP
            I2C1STAT2bits.STARTE = 0;   //Clear STARTE flag since last START condition
            
            transmittedCount = 0; //Start transmitting from start of data buffer
            
            I2C1CON2bits.NDA = 0; //Will send address first, next byte is not data
            I2C1STAT2bits.EOP = 0; //Clear end of packet bit
            I2C1CON2bits.PSZ = 10; //Re-initialize packet size
            
            //Wait for bus idle
            while (!I2C1STAT2bits.BITO);
            phase = ADDRESS_PHASE;
            I2C1CON1bits.SEN = 1; // Send Start bit
            
        }
    }
}

void __attribute__((interrupt, no_auto_psv)) _I2C1Interrupt(void) {
    IFS2bits.I2C1IF = 0;
    
    switch (phase) {
        case ADDRESS_PHASE: {
            /*Verify if start has been sent*/
            if (I2C1STAT2bits.STARTE) {
                /* Transmit client address with RW =0 , writing to client*/
                I2C1TRN = (mCLIENT_ADDRESS << 1) | 0;
                phase = DATA_WRITE;
            }
            break;
        }
        case DATA_WRITE: {
            /* Set NDA to indicate next byte is data */
            if (I2C1CON2bits.NDA != 1) {
                I2C1CON2bits.NDA = 1;
            }
            /* If Packet size is 0, EOP will be asserted,send STOP on EOP*/
            if (I2C1STAT2bits.EOP) {
                I2C1CON1bits.PEN = 1;
            } 
            else {
                if ((I2C1STAT1bits.ACKSTAT == 0) && (!I2C1STAT1bits.TBF) && (!I2C1STAT1bits.TRSTAT)) {
                    I2C1TRN = hostTransmit[transmittedCount++]; // Send data to client
                }
            }
            break;
        }
    }
}

#endif

#ifdef I2C_HOST_TRANSMISSION_10_BIT_ADDRESS

#pragma config FDEVOPT1_ALTI2C1 = ON    //Using RD5 as SCL1 and RD6 as SDA1

#include <xc.h>

#define INIT 0
#define ADDRESS_PHASE_UPPER_10BIT 1
#define ADDRESS_PHASE_LOWER_10BIT 2
#define DATA_WRITE 3

#define PACKET_SIZE 10

#define mCLIENT_ADDRESS 0x14C

unsigned char hostTransmit[PACKET_SIZE] = {0x11, 0x22, 0x33, 0x44, 0x55, 0x66, 0x77, 0x88, 0x99, 0xAA};
unsigned char transmittedCount = 0;
unsigned char phase = INIT;

int main(void)
{   
    /*Configure I2C pins as digital*/
    //On dsPIC33AK128MC106, RD5 and RD7 have no analog functionality and are digital by default.
 
    //I2C module has Host and Client functionality which may both be active at the same time.
    //In this example, Host functionality will be configured and used.

    //Configure baud rate for SCL @ 100kHz from 1/2 bus UPB clock @ 4MHz
    I2C1LBRG = 16; //Set low-time baud rate
    I2C1HBRG = 16; //Set high-time baud rate

    /* Configure Bus IDle timeout*/
    I2C1CON2bits.BITE = 1;
    I2C1BITObits.BITOPR = 76;

    /* Configured interrupt enable bits*/
    I2C1INTCbits.HACKSIE = 1; // Assert HSTIF on ACK seq
    I2C1INTCbits.HDTXIE = 1; // Assert HSTIF on TX
    I2C1INTCbits.HSCIE = 1; // Assert HSTIF on start
    I2C1INTCbits.HSTIE = 1; // Assert I2CxIF when HSTIF is set
    I2C1CON2bits.PSZ = PACKET_SIZE; // Packet size
    I2C1CON2bits.EOPSC = 0b01; // End of packet will be set after data bytes
    I2C1CON1bits.ON = 1; // Enable I2C
    IFS2bits.I2C1IF = 0; // Clear I2C general interrupt 
    IEC2bits.I2C1IE = 1; // Enable I2C general interrupt 

    /*wait for bus idle */
    while(!I2C1STAT2bits.BITO);

    phase = ADDRESS_PHASE_UPPER_10BIT;
    I2C1CON1bits.SEN= 1; // Send Start bit
 
 
    while(1) {
        
        //If data transmission is complete, repeat the operation.      
        if ((transmittedCount >= PACKET_SIZE) && (I2C1STAT2bits.STOPE)) {
            
            I2C1STAT2bits.STOPE = 0;    //Clear STOPE flag to detect next STOP
            I2C1STAT2bits.STARTE = 0;   //Clear STARTE flag since last START condition
            
            transmittedCount = 0;   //Start transmitting from start of data buffer
            
            I2C1CON2bits.NDA = 0;   //Will send address first, next byte is not data
            I2C1STAT2bits.EOP = 0;  //Clear end of packet bit
            I2C1CON2bits.PSZ = PACKET_SIZE; //Re-initialize packet size
            
            //Wait for bus idle
            while (!I2C1STAT2bits.BITO);
            phase = ADDRESS_PHASE_UPPER_10BIT;
            I2C1CON1bits.SEN = 1; // Send Start bit
            
        }
    }
}
 
void __attribute__((interrupt, no_auto_psv)) _I2C1Interrupt(void)
{
    IFS2bits.I2C1IF = 0;
    switch(phase)
    {
        case ADDRESS_PHASE_UPPER_10BIT: {
            /*Verify if start has been sent*/
            if(I2C1STAT2bits.STARTE)
            {
                /* Transmit client address with RW =0 , writing to client*/
                I2C1TRN = ((((mCLIENT_ADDRESS >> 8) & 0x03)<< 1) | 0 ) + 0b11110000; 
                phase = ADDRESS_PHASE_LOWER_10BIT;
            }
            break;
        }
        case ADDRESS_PHASE_LOWER_10BIT: {
            /*Verify if the upper 10bit address is ACKed*/
            if (I2C1STAT1bits.ACKSTAT ==0)
            {
            I2C1TRN = mCLIENT_ADDRESS & 0xFF ; 
            phase = DATA_WRITE;
            }
            break;
        }
        case DATA_WRITE: {
            /* Set NDA to indicate next byte is data */ 
            if(I2C1CON2bits.NDA !=1)
            {
                I2C1CON2bits.NDA = 1;
            }
            /* If Packet size is 0, EOP will be asserted, send STOP on EOP*/ 
            if(I2C1STAT2bits.EOP)
            {
                I2C1CON1bits.PEN =1; 
            }
            else
            {
                if ((I2C1STAT1bits.ACKSTAT ==0) && (!I2C1STAT1bits.TBF) && (!I2C1STAT1bits.TRSTAT))
                { 
                    I2C1TRN = hostTransmit[transmittedCount++]; // Send data to client
                }
            }
            break;
        }
    }
}

#endif

#ifdef I2C_HOST_RECEPTION_7_BIT_ADDRESS

#pragma config FDEVOPT1_ALTI2C1 = ON    //Using RD5 as SCL1 and RD6 as SDA1

#include <xc.h>

#define mCLIENT_ADDRESS 0x4C

#define INIT 1
#define ADDRESS_PHASE 2
#define DATA_READ 3

#define PACKET_SIZE 10

volatile unsigned char phase = INIT;
volatile unsigned char hostReceived[PACKET_SIZE];
volatile unsigned char receivedCount = 0;

int main(void) {
    
    /*Configure I2C pins as digital*/
    //On dsPIC33AK128MC106, RD5 and RD7 have no analog functionality and are digital by default.

    //I2C module has Host and Client functionality which may both be active at the same time.
    //In this example, Host functionality will be configured and used.

    //Configure baud rate for SCL @ 100kHz from 1/2 bus UPB clock @ 4MHz
    I2C1LBRG = 16; //Set low-time baud rate
    I2C1HBRG = 16; //Set high-time baud rate

    /* Configure Bus IDle timeout*/
    I2C1CON2bits.BITE = 1;
    I2C1BITObits.BITOPR = 76;

    /* Configured interrupt enable bits*/
    I2C1INTCbits.HACKSIE = 1; // Assert HSTIF on ACK seq
    I2C1INTCbits.HDTXIE = 1; // Assert HSTIF on TX
    I2C1INTCbits.HDRXIE = 1; // Assert HSTIF on RX
    I2C1INTCbits.HSCIE = 1; // Assert HSTIF on start
    I2C1INTCbits.HSTIE = 1; // Assert I2CxIF when HSTIF is set
    I2C1CON2bits.SMEN = 1; // Smart mode enabled
    I2C1CON2bits.ACKC = 0b10; // ACK all the bytes expect last byte
    I2C1CON2bits.PSZ = PACKET_SIZE; // Packet size
    I2C1CON2bits.EOPSC = 0b01; // End of packet will be set after data bytes
    I2C1CON1bits.ON = 1; // Enable I2C
    IFS2bits.I2C1IF = 0; // Clear I2C general interrupt 
    IEC2bits.I2C1IE = 1; // Enable I2C general interrupt 

    /*wait for bus idle */
    while (!I2C1STAT2bits.BITO);
    phase = ADDRESS_PHASE;
    I2C1CON1bits.SEN = 1; // Send Start bit
    
    while (1) {
    
        //Once reception is complete, repeat the operation.
        if ((receivedCount >= PACKET_SIZE) && (I2C1STAT2bits.STOPE)) {
            
            I2C1STAT2bits.STOPE = 0;    //Clear STOPE flag to detect next STOP
            I2C1STAT2bits.STARTE = 0;   //Clear STARTE flag since last START condition
            
            receivedCount = 0;  //Reset receive buffer index
            
            I2C1CON1bits.RCEN = 0;          //Not yet receiving data on first transaction
            I2C1CON2bits.NDA = 0;           //Will send address first, next byte is not data
            I2C1CON2bits.PSZ = PACKET_SIZE; //Re-initialize packet size
            I2C1STAT2bits.EOP = 0;          //Clear end of packet bit
            
            /*wait for bus idle */
            while (!I2C1STAT2bits.BITO);
            phase = ADDRESS_PHASE;
            I2C1CON1bits.SEN = 1; // Send Start bit
        }
    
    }
    
 
}

void __attribute__((interrupt, no_auto_psv)) _I2C1Interrupt(void) {
    IFS2bits.I2C1IF = 0;
    switch (phase) {
        case ADDRESS_PHASE: {
            /*Verify if start has been sent*/
            if (I2C1STAT2bits.STARTE) {
                phase = DATA_READ;
                /* Transmit client address with RW =1 , read client*/
                I2C1TRN = (mCLIENT_ADDRESS << 1) | 1;
            }
            break;
        }
        case DATA_READ: {
            /* Set NDA to indicate next byte is data */
            if (I2C1CON2bits.NDA != 1) {
                I2C1CON2bits.NDA = 1;
                /*Enable receive for the 1st time*/
                I2C1CON1bits.RCEN = 1;
            }
            /*Read the received data*/
            if (I2C1STAT1bits.RBF) {
                hostReceived[receivedCount++] = I2C1RCV;
            }
            /* If Packet size is 0, EOP will be asserted,send STOP on EOP*/
            if (I2C1STAT2bits.EOP == 1) {
                I2C1CON1bits.PEN = 1;
            } 
            break;
        }
    }
}

#endif


#ifdef I2C_HOST_RECEPTION_10_BIT_ADDRESS

#pragma config FDEVOPT1_ALTI2C1 = ON    //Using RD5 as SCL1 and RD6 as SDA1

#include <xc.h>

#define mCLIENT_ADDRESS 0x14C

#define INIT 0
#define ADDRESS_PHASE_UPPER_10BIT_WRITE 1
#define ADDRESS_PHASE_LOWER_10BIT 2
#define RESTART 3
#define ADDRESS_PHASE_UPPER_10BIT_READ 4
#define DATA_READ 5

#define PACKET_SIZE 10

unsigned char phase = INIT;
unsigned char hostReceived[PACKET_SIZE];
unsigned char receivedCount = 0;



int main(void) {
    
    /*Configure I2C pins as digital*/
    //On dsPIC33AK128MC106, RD5 and RD7 have no analog functionality and are digital by default.

    //I2C module has Host and Client functionality which may both be active at the same time.
    //In this example, Host functionality will be configured and used.

    //Configure baud rate for SCL @ 100kHz from 1/2 bus UPB clock @ 4MHz
    I2C1LBRG = 16; //Set low-time baud rate
    I2C1HBRG = 16; //Set high-time baud rate
    
    /* Configure Bus IDle timeout*/
    I2C1CON2bits.BITE = 1;
    I2C1BITObits.BITOPR = 76;
    
    /* Configured interrupt enable bits*/
    I2C1INTCbits.HACKSIE = 1; // Assert HSTIF on ACK seq
    I2C1INTCbits.HDTXIE = 1; // Assert HSTIF on TX
    I2C1INTCbits.HDRXIE = 1; // Assert HSTIF on RX
    I2C1INTCbits.HSCIE = 1; // Assert HSTIF on start
    I2C1INTCbits.HSTIE = 1; // Assert I2CxIF when HSTIF is set
    I2C1CON2bits.SMEN = 1; // Smart mode enabled
    I2C1CON2bits.ACKC = 0b10; // ACK all the bytes expect last byte
    I2C1CON2bits.PSZ = PACKET_SIZE; // Packet size
    
    I2C1CON2bits.EOPSC = 0b01; // End of packet will be set after data bytes
    I2C1CON1bits.ON = 1; // Enable I2C
    IFS2bits.I2C1IF = 0; // Clear I2C general interrupt 
    IEC2bits.I2C1IE = 1; // Enable I2C general interrupt 

    /*wait for bus idle */
    while (!I2C1STAT2bits.BITO);
    phase = ADDRESS_PHASE_UPPER_10BIT_WRITE;
    I2C1CON1bits.SEN = 1; // Send Start bit
    
    while (1) {
        
        //Once reception is complete, repeat the operation.
        if ((receivedCount >= PACKET_SIZE) && (I2C1STAT2bits.STOPE)) {
            
            I2C1STAT2bits.STOPE = 0;    //Clear STOPE flag to detect next STOP
            I2C1STAT2bits.STARTE = 0;   //Clear STARTE flag since last START condition
            
            receivedCount = 0;  //Reset receive buffer index
            
            I2C1CON1bits.RCEN = 0;          //Not yet receiving data on first transaction
            I2C1CON2bits.NDA = 0;           //Next byte is address, not data
            I2C1CON2bits.PSZ = PACKET_SIZE; //Re-initialize packet size
            I2C1STAT2bits.EOP = 0;          //Clear end of packet bit
            
            /*wait for bus idle */
            while (!I2C1STAT2bits.BITO);
            phase = ADDRESS_PHASE_UPPER_10BIT_WRITE;
            I2C1CON1bits.SEN = 1; // Send Start bit
        }
        
    }
}

void __attribute__((interrupt, no_auto_psv)) _I2C1Interrupt(void) {
    IFS2bits.I2C1IF = 0;
    switch (phase) {
        case ADDRESS_PHASE_UPPER_10BIT_WRITE: {
            /*Verify if start has been sent*/
            if (I2C1STAT2bits.STARTE) {
                /* Transmit client address with RW =0 , write client*/
                I2C1TRN = ((((mCLIENT_ADDRESS >> 8) & 0x03) << 1) | 0) + 0b11110000;
                phase = ADDRESS_PHASE_LOWER_10BIT;
            }
            break;   
        }
        case ADDRESS_PHASE_LOWER_10BIT: {
            /*Verify if the upper 10bit address is ACKed*/
            if (I2C1STAT1bits.ACKSTAT == 0) {
                I2C1TRN = mCLIENT_ADDRESS & 0xFF;
                phase = RESTART;
            }
            break;
        }
        case RESTART: {
            I2C1CON1bits.RSEN = 1; // Send restart
            phase = ADDRESS_PHASE_UPPER_10BIT_READ;
            break;
        }
        case ADDRESS_PHASE_UPPER_10BIT_READ: {
            /* Transmit client address with RW =1 , read client*/
            I2C1TRN = ((((mCLIENT_ADDRESS >> 8) & 0x03) << 1) | 1) + 0b11110000;
            phase = DATA_READ;
            break;
        }
        case DATA_READ: {
            /* Set NDA to indicate next byte is data */
            if (I2C1CON2bits.NDA != 1) {
                I2C1CON2bits.NDA = 1;
                /*Enable receive for the 1st time*/
                I2C1CON1bits.RCEN = 1;
            }
            /*Read the received data*/
            if (I2C1STAT1bits.RBF) {
                hostReceived[receivedCount++] = I2C1RCV;
            }
            /* If Packet size is 0, EOP will be asserted, send STOP on EOP*/
            if (I2C1STAT2bits.EOP) {
                I2C1CON1bits.PEN = 1;
            }
            break;
        }
    }
}

#endif

#ifdef I2C_CLIENT_RECEPTION_7_BIT_ADDRESS

#pragma config FDEVOPT1_ALTI2C1 = ON    //Using RD5 as SCL1 and RD6 as SDA1

#include <xc.h>

#define mCLIENT_ADDRESS 0x4C

#define PACKET_SIZE 10

volatile unsigned char clientReceived[PACKET_SIZE + 1];
volatile unsigned char recievedCount = 0;

int main(void) {
    
    /*Configure I2C pins as digital*/
    //On dsPIC33AK128MC106, RD5 and RD7 have no analog functionality and are digital by default.

    //I2C module has Host and Client functionality which may both be active at the same time.
    //In this example, Client functionality will be configured and used.
    
    //Set up I2C1 client configuration
    I2C1ADD = mCLIENT_ADDRESS; // Configure Client address
    I2C1INTCbits.CADDRIE = 1; // Assert CLTIF on address detect
    I2C1INTCbits.CDRXIE = 1; // Assert CLTIF on received byte
    I2C1INTCbits.CLTIE = 1; // Assert I2CxIF when CLTIF is asserted
    I2C1CON2bits.SMEN = 1; // Enable smart mode
    I2C1CON2bits.PSZ = PACKET_SIZE; // Packet size 
    I2C1CON1bits.ON = 1; // Enable I2C  
    IFS2bits.I2C1IF = 0; // Clear I2C general interrupt 
    IEC2bits.I2C1IE = 1; // Enable I2C general interrupt 

    while (1) {}
 
}
void __attribute__((__interrupt__, no_auto_psv)) _I2C1Interrupt(void) {
    IFS2bits.I2C1IF = 0;
    /* Read received address*/
    if ((I2C1STAT1bits.RBF)&& (!I2C1STAT1bits.D_A)) {
        recievedCount = 0;
        clientReceived[recievedCount] = I2C1RCV;
        recievedCount++;
    } /* Read received data byte*/
    else if ((I2C1STAT1bits.RBF)&& (I2C1STAT1bits.D_A)) {
        clientReceived[recievedCount] = I2C1RCV;
        recievedCount++; // Read received data
    }
}


#endif

#ifdef I2C_CLIENT_RECEPTION_10_BIT_ADDRESS

#pragma config FDEVOPT1_ALTI2C1 = ON    //Using RD5 as SCL1 and RD6 as SDA1

#include <xc.h>

#define mCLIENT_ADDRESS 0x14C

#define PACKET_SIZE 10

volatile unsigned char clientReceived[PACKET_SIZE + 2];
volatile unsigned char recievedCount = 0;

int main(void) {
 
    /*Configure I2C pins as digital*/
    //On dsPIC33AK128MC106, RD5 and RD7 have no analog functionality and are digital by default.
    
    //I2C module has Host and Client functionality which may both be active at the same time.
    //In this example, Client functionality will be configured and used.
    
    //Set up I2C1 client configuration
    I2C1ADD = mCLIENT_ADDRESS; // Configure Client address
    I2C1INTCbits.CADDRIE = 1; // Assert CLTIF on address detect
    I2C1INTCbits.CDRXIE = 1; // Assert CLTIF on received byte

    I2C1INTCbits.CLTIE = 1; // Assert I2CxIF when CLTIF is asserted
    I2C1CON2bits.SMEN = 1; // Enable smart mode
    I2C1CON2bits.PSZ = PACKET_SIZE; // Packet size 
    I2C1CON1bits.A10M = 1; // 10bit addressing 
    I2C1CON1bits.ON = 1; // Enable I2C 
    IFS2bits.I2C1IF = 0; // Clear I2C general interrupt 
    IEC2bits.I2C1IE = 1; // Enable I2C general interrupt 
    
    while (1) {}
}

void __attribute__((__interrupt__, no_auto_psv)) _I2C1Interrupt(void) {
    IFS2bits.I2C1IF = 0;
    
    /* Read received address*/
    if ((I2C1STAT1bits.RBF)&& (!I2C1STAT1bits.D_A)) {
        recievedCount = 0;
        clientReceived[recievedCount] = I2C1RCV;
        recievedCount++;
    }/* Read received data byte*/
    else if ((I2C1STAT1bits.RBF)&& (I2C1STAT1bits.D_A)) {
        clientReceived[recievedCount] = I2C1RCV;
        recievedCount++; // Read received data
    }
    
}

#endif

#ifdef I2C_CLIENT_TRANSMISSION_7_BIT_ADDRESS

#pragma config FDEVOPT1_ALTI2C1 = ON    //Using RD5 as SCL1 and RD6 as SDA1

#include <xc.h>

#define mCLIENT_ADDRESS 0x4C

#define PACKET_SIZE 10

volatile unsigned char clientReceived[PACKET_SIZE + 1];
volatile unsigned char recievedCount = 0;
volatile unsigned char transmittedCount = 0;
volatile unsigned char clientTransmit[PACKET_SIZE] = {0x11, 0x22, 0x33, 0x44, 0x55, 0x66, 0x77, 0x88, 0x99, 0xAA};

int main(void) {
    
    /*Configure I2C pins as digital*/
    //On dsPIC33AK128MC106, RD5 and RD7 have no analog functionality and are digital by default.
    
    //I2C module has Host and Client functionality which may both be active at the same time.
    //In this example, Client functionality will be configured and used.

    //Set up I2C1 client configuration
    I2C1ADD = mCLIENT_ADDRESS; // Configure Client address
    I2C1INTCbits.CADDRIE = 1; // Assert CLTIF on address detect
    I2C1INTCbits.CDRXIE = 1; // Assert CLTIF on received byte
    I2C1INTCbits.CDTXIE = 1; // Assert CLTIF on transmit byte
    I2C1INTCbits.CLTIE = 1; // Assert I2CxIF when CLTIF is asserted
    I2C1CON2bits.SMEN = 1; // Enable smart mode
    I2C1CON2bits.PSZ = PACKET_SIZE; // Packet size 
    I2C1CON1bits.ON = 1; // Enable I2C
    IFS2bits.I2C1IF = 0; // Clear I2C general interrupt 
    IEC2bits.I2C1IE = 1; // Enable I2C general interrupt 
    
    while (1) {}
}

void __attribute__((__interrupt__, no_auto_psv)) _I2C1Interrupt(void) {
    IFS2bits.I2C1IF = 0;
    
    /* Read received address*/
    if ((I2C1STAT1bits.RBF)&& (!I2C1STAT1bits.D_A)) {
        transmittedCount = 0;
        recievedCount = 0;
        clientReceived[recievedCount] = I2C1RCV;
        recievedCount++;        
    }/* Read received data byte*/
    else if ((I2C1STAT1bits.RBF)&& (I2C1STAT1bits.D_A)) {
        clientReceived[recievedCount] = I2C1RCV;
        recievedCount++; // Read received data (?)
    }
    /* transmit byte if R_W is set */
    if ((I2C1STAT1bits.R_W) && (!I2C1STAT1bits.ACKSTAT)) {
        I2C1TRN = clientTransmit[transmittedCount++]; // transmit data
    }
}


#endif

#ifdef I2C_CLIENT_TRANSMISSION_10_BIT_ADDRESS

#pragma config FDEVOPT1_ALTI2C1 = ON    //Using RD5 as SCL1 and RD6 as SDA1

#include <xc.h>

#define mCLIENT_ADDRESS 0x14C

#define PACKET_SIZE 10

volatile unsigned char clientReceived[PACKET_SIZE + 2];
volatile unsigned char recievedCount = 0;
volatile unsigned char clientTransmit[PACKET_SIZE] = {0x11, 0x22, 0x33, 0x44, 0x55, 0x66, 0x77, 0x88, 0x99, 0xAA};
volatile unsigned char transmittedCount = 0;

int main(void) {
    
    /*Configure I2C pins as digital*/
    //On dsPIC33AK128MC106, RD5 and RD7 have no analog functionality and are digital by default.
    
    //I2C module has Host and Client functionality which may both be active at the same time.
    //In this example, Client functionality will be configured and used.
    
    //Set up I2C1 client configuration
    I2C1ADD = mCLIENT_ADDRESS; // Configure Client address
    I2C1INTCbits.CADDRIE = 1; // Assert CLTIF on address detect
    I2C1INTCbits.CDRXIE = 1; // Assert CLTIF on received byte
    I2C1INTCbits.CDTXIE = 1; // Assert CLTIF on transmit byte
    I2C1INTCbits.CLTIE = 1; // Assert I2CxIF when CLTIF is asserted
    I2C1CON2bits.SMEN = 1; // Enable smart mode
    I2C1CON2bits.PSZ = PACKET_SIZE; // Packet size 
    I2C1CON1bits.A10M = 1; // 10bit addressing 
    I2C1CON1bits.ON = 1; // Enable I2C 
    IFS2bits.I2C1IF = 0; // Clear I2C general interrupt 
    IEC2bits.I2C1IE = 1; // Enable I2C general interrupt 
    
    while (1);
}

void __attribute__((__interrupt__, no_auto_psv)) _I2C1Interrupt(void) {

    IFS2bits.I2C1IF = 0;
    /* Read received address*/
    if ((I2C1STAT1bits.RBF)&& (!I2C1STAT1bits.D_A)) {
        transmittedCount = 0;
        recievedCount = 0;
        clientReceived[recievedCount] = I2C1RCV;
        recievedCount++;
    }/* Read received data byte*/
    else if ((I2C1STAT1bits.RBF)&& (I2C1STAT1bits.D_A)) {
        clientReceived[recievedCount] = I2C1RCV;
        recievedCount++; // Read received data
    }
    /* transmit byte if R_W is set */
    if ((I2C1STAT1bits.R_W) &&(!I2C1STAT1bits.ACKSTAT)) {
        I2C1TRN = clientTransmit[transmittedCount++]; //Transmit data
    }
}

#endif