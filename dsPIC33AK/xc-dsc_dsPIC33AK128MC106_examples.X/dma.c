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
 * File:   dma.c
 * Author: C68555
 *
 * Created on May 22, 2023, 2:47 PM
 */


#include "xc.h"
#include "examples.h"


#ifdef DMA_ONE_SHOT_TRANSFER_EXAMPLE

unsigned char Array1[4];
unsigned char Array2[4];

int main()
{
    for (int i=0; i<4; i++)
    {
        Array1[i]=i+1; //fill with i+1
        Array2[i]=0; //fill with 0
    }
    
    DMACONbits.ON=1; //Enable DMA
    
    //Set lower and upper address limit
    DMAHIGH=0x5000;
    DMALOW=0x4000;
    
    DMA0SRC=(unsigned int)& Array1; //load the source address
    DMA0DST=(unsigned int)& Array2; //load destination address
    DMA0CNT=4; //Four Transfer to be done
    DMA0CH=0;
    DMA0CHbits.SAMODE=1; //Source address increment mode
    DMA0CHbits.DAMODE=1; //Destination address increment mode
    DMA0CHbits.TRMODE=0; //One-Shot Transfer mode
    DMA0CHbits.INTOEN=1; //Enable interrupt on DONE being set
    DMA0CHbits.SIZE=0; //One byte transferred at a time
    DMA0CHbits.CHEN=1; //Enable channel
    
    IFS2bits.DMA0IF=0;
    
    DMA0CHbits.CHREQ=1; //First trigger
    while(DMA0CHbits.CHREQ);
    DMA0CHbits.CHREQ=1; //Second trigger
    while(DMA0CHbits.CHREQ);
    DMA0CHbits.CHREQ =1; //Third trigger
    while(DMA0CHbits.CHREQ); //HALF=1 since DMACNT is at halfway point
    DMA0CHbits.CHREQ=1; //Fourth trigger DMACNT=0 and transfer complete
    while(DMA0CHbits.CHREQ);
    while(!IFS2bits.DMA0IF); //Transfer Complete (DMA0IF=1 ,DONE=1, CHAEN=0)
    
    IFS2bits.DMA0IF=0;
    
    while(1);
}

#endif


#ifdef DMA_REPEATED_ONE_SHOT_TRANSFER_EXAMPLE

unsigned char Array1[4];
unsigned char Array2[4];

void test(void);

int main()
{
    for (int i=0; i<4; i++)
    {
        Array1[i]=i+1; //fill with i+1
        Array2[i]=0; //fill with 0
    }
    
    DMACONbits.ON=1; //Enable DMA

    //set lower and upper address limit
    DMAHIGH=0x5000;
    DMALOW=0x4000;
    
    DMA0SRC=(unsigned int)& Array1; //load the source address
    DMA0DST=(unsigned int)& Array2; //load destination address
    DMA0CNT=4;
    
    DMA0CH=0;
    DMA0CHbits.SAMODE=1; //Source address increment mode
    DMA0CHbits.DAMODE=1; //Destination address increment mode
    DMA0CHbits.TRMODE=1; //Transfer mode Repeat One-Shot
    DMA0CHbits.RELOADS=1; //Reload Source Address
    DMA0CHbits.RELOADD=1; //Reload Destination Address
    DMA0CHbits.INTOEN=1; //Enable interrupt on DONE being set
    DMA0CHbits.SIZE=0; //One byte transferred at a time
    DMA0CHbits.CHEN=1; //Channel enable
    IFS2bits.DMA0IF=0;
    
    while(1)
    {
        DMA0CHbits.CHREQ=1; //First trigger
        while(DMA0CHbits.CHREQ);
        DMA0CHbits.CHREQ=1; //Second trigger
        while(DMA0CHbits.CHREQ);
        DMA0CHbits.CHREQ=1; //Third trigger
        while(DMA0CHbits.CHREQ); //HALF=1 since DMACNT is at half way point
        DMA0CHbits.CHREQ=1; //Fourth trigger DMACNT=0
        //Wait for transfer complete
        while(DMA0CHbits.CHREQ); //DMACNT reloaded to 4, DMA0IF=1
        //Since RELOADS=1 and RELOADD=1,DMASRC0/DMADST0 are reloaded.
        while(!IFS2bits.DMA0IF);
        DMA0STATbits.DONE=0; //Clear DONE and HALIF flag
        DMA0STATbits.HALF=0;
        IFS2bits.DMA0IF=0;
    }
}

#endif

#ifdef DMA_CONTINUOUS_TRANSFER_EXAMPLE

unsigned int Array1[100];
unsigned int Array2[100];

int main()
{
    for (int i=0; i<100; i++)
    {
        Array1[i]=i+1; //fill with i+1
        Array2[i]=0; //fill with 0
    }
    
    DMACONbits.ON=1; //Enable DMA
    
    //Set lower and upper address limit
    DMAHIGH=0x5000;
    DMALOW=0x4000;
    
    DMA0SRC=(unsigned int)& Array1; // load the source address
    DMA0DST=(unsigned int)& Array2; // load destination address
    DMA0CNT=100; // 100 Transactions per trigger
    
    DMA0CH=0;
    DMA0CHbits.SAMODE=1; //Source address increment mode
    DMA0CHbits.DAMODE=1; //Destination address increment mode
    DMA0CHbits.TRMODE=2; //Transfer mode Continuous
    DMA0CHbits.INTOEN=1; //Enable interrupt on DONE being set
    DMA0CHbits.SIZE=2; //One 32-bit word transferred at a time
    DMA0CHbits.CHEN=1; //Channel enable
    
    IFS2bits.DMA0IF=0; 
    DMA0CHbits.CHREQ=1; //Enable the transfer by software trigger
    while(!DMA0STATbits.HALF); //HALF=1 is set when DMA0CNT reaches halfway
    while(!IFS2bits.DMA0IF); //DONE=1;CHAEN=0,DMA0IF=1 
    //100 (DMAXCNT=100) transfers complete with one trigger
    
    IFS2bits.DMA0IF=0;
    
    while(1);
}

#endif


#ifdef DMA_REPEATED_CONTINUOUS_TRANSFER_EXAMPLE

unsigned int Array1[100];
unsigned int Array2[100];
int i;

int main()
{
    
    for (i=0;i<100;i++)
    {
        Array1[i]=i+1; //fill with i+1
        Array2[i]=0; //fill with 0
    }
    
    DMACONbits.ON=1; //Enable DMA
     
    DMACONbits.PRIORITY = 1; //Robin is round
    
    //Set lower and upper address limit
    DMAHIGH=0x5000;
    DMALOW=0x4000;
    
    DMA0SRC=(unsigned int)& Array1; // load the source address
    DMA0DST=(unsigned int)& Array2; // load destination address
    DMA0CNT=100; // 100 Transactions per trigger
    
    DMA0CH=0;
    DMA0CHbits.SAMODE=1; //Source address increment mode
    DMA0CHbits.DAMODE=1; //Destination address increment mode
    DMA0CHbits.TRMODE=3; //Transfer mode Repeat continuous
    DMA0CHbits.RELOADS=1; //Reload Source Address
    DMA0CHbits.RELOADD=1; //Reload Destination Address
    DMA0CHbits.INTOEN=1; //Enable interrupt on DONE being set
    DMA0CHbits.SIZE=2; //One 32-bit word transferred at a time
    DMA0CHbits.CHEN=1; //Channel enable
    IFS2bits.DMA0IF=0;
    
    while(1)
    {
        DMA0CHbits.CHREQ=1; //TIGGER
        while(!IFS2bits.DMA0IF); //Wait for transaction to complete
        
        //Clear the Destination Memory
        for (i=0;i<100;i++)
        {
            Array2[i]=0; //fill with 0
        }
        
        //Clear done interrupt flag and status bits prior to re-triggering
        IFS2bits.DMA0IF=0;
        DMA0STATbits.DONE=0;
        DMA0STATbits.HALF=0;
    }
}

#endif

#ifdef DMA_FIXED_TO_BLOCK_CONTINUOUS_TRANSFER_EXAMPLE


void UartInit(void);
unsigned char Array2[32];

int main() 
{
    UartInit();
    
    for (int i = 0; i < 32; i++) 
    {
        Array2[i] = 0; //fill with 0
    }
    
    DMACONbits.ON = 1; //Enable DMA

    DMAHIGH = 0x5000; //set lower and upper address limit
    DMALOW = 0x4000;
    DMA0SRC = (uint32_t) &U1RXREG;
    DMA0DST = (uint32_t) &Array2[0]; // load destination address
    DMA0CNT = 4; //When the UART buffer has 4 bytes, do an interrupt and transfer 4 bytes
    DMA0CH = 0;
    
    DMA0CHbits.SIZE=0; //One byte transferred at a time
    DMA0CHbits.SAMODE = 0; //Source address increment mode: do not increment
    DMA0CHbits.DAMODE = 1; //Destination address increment mode: increment 1
    DMA0CHbits.TRMODE = 2; //Transfer mode: Continuous
    
    DMA0SELbits.CHSEL = 15; //Trigger DMA channel 0 on UART1 Receive
    DMA0CHbits.INTOEN=1; //Enable interrupt on DONE being set
    DMA0CHbits.CHEN = 1; //Channel 0 enable
    
    IFS2bits.DMA0IF = 0;
    //Wait for the bytes to be transferred (triggered by UART reception)
    while (!IFS2bits.DMA0IF); //DONE=1;CHAEN=0,DMA0IF=1 and transfer complete with one trigger
    
    IFS2bits.DMA0IF = 0;
    
    //4 bytes received from UART1 are now be stored in Array2.
    //DMA0DST will point at Array2[4], and further bytes can be transferred by enabling the channel again.
    
    while (1);
}


//Enable UART1 for 115200 baud rate using RB6 as UART1 TX and RB7 as UART1 RX.
void UartInit(void) 
{
    //Enable CLKEN8 to provide clock to UART
    CLK8CONbits.ON = 1;
    
    //Map RB6 (RP23) to UART1 TX
    _RP23R = 9;
    //Map RB7 (RP24) to UART1 RX
    _U1RXR = 24;
       
    //Set RB6/RB7 as digital I/O
    _ANSELB6 = 0;
    _ANSELB7 = 0;
    //Set RB6 as output
    _TRISB6 = 0;
    //Set RB7 as input
    _TRISB7 = 1;
    
    U1CONbits.BCLKMOD = 1; //Use fractional baud rate generation
    U1CONbits.BCLKSEL = 1; //Select CLKGEN8 as clock source
    
    U1CONbits.MODE = 0; //Asynchronous 8-bit UART
    
    //Enable RX and TX functionality
    U1CONbits.URXEN = 1;
    U1CONbits.UTXEN = 1;
    
    //In fractional baud rate mode, UxBRG = Input clock / baud rate, for 8MHz and 115200 baud
    U1BRG = (8000000 / 115200);
    
    U1STAbits.URXISEL = 3; //Interrupt after 4 transfers
    
    //Lastly, enable UART1
    U1CONbits.ON = 1;
    
    _U1RXIF = 0;
}


#endif