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
 * File:   spi.c
 * Author: C68555
 *
 * Created on August 17, 2023, 11:32 AM
 */

#include <xc.h>
#include "spi.h"
#include "oscillator.h"
#include "examples.h"


void SPI1_initialize_16_bit_host_mode() {

/* The following code example will initialize the SPI1 in Host mode. */
    
    SPI1BRG = 1; //1MHz SCK from 1/2 speed UPB clock @4MHz (MCLKEN = 0) /4
    SPI1STATbits.SPIROV = 0; // clear the Overflow
    
    SPI1CON1bits.MSTEN = 1; //Host mode
    
    //16 bits transfer
    SPI1CON1bits.MODE16 = 1;
    SPI1CON1bits.MODE32 = 0;
    
    SPI1CON1bits.CKP = 0; //Clock is active high
    SPI1CON1bits.CKE = 0; //Transmit happens on idle to active clock transition
    SPI1CON1bits.SMP = 0; //Input data sampled at the middle of data output time
    
    SPI1IMSKbits.SPITBEN = 1; // SPI1 transmit buffer empty generates interrupt event
    _SPI1TXIE = 1; // Enable TX interrupt
    
    SPI1IMSKbits.SPIRBFEN = 1; // SPI1 receive buffer full generates interrupt event
    _SPI1RXIE = 1; // Enable RX interrupt
    
    //Enable SPI1
    SPI1CON1bits.SPIEN = 1;
    // from here, the device is ready to transmit and receive data. 
    //Buffer can be loaded to transmit data, which will initiate a transfer.
    
}
  

void SPI1_initialize_I2S_host_mode_16_bit_channel_data_32_bit_frame() {
    /* The following code example will initialize the SPI1 Module in I2S Host 
    mode. */
    
    SPI1STATbits.SPIROV = 0; // clear the Overflow
    SPI1BRG = 1; //31.25kbps sample rate from 1/2 speed UPB clock @4MHz (MCLKEN = 0)

    SPI1CON1bits.AUDEN = 1; //Enable audio protocol mode
    SPI1CON1bits.AUDMOD = 0b00; //I2S mode
    
    SPI1CON1bits.MSTEN = 1; //Host mode
    
    //16-bit data, 32-bit channel
    SPI1CON1bits.MODE32 = 0;
    SPI1CON1bits.MODE16 = 0;
    
    SPI1CON1bits.CKP = 1; //Clock is active low

    SPI1IMSKbits.SPITBEN = 1; // SPI1 transmit buffer empty generates interrupt event
    _SPI1TXIE = 1; // Enable TX interrupt
    
    SPI1IMSKbits.SPIRBFEN = 1; // SPI1 receive buffer full generates interrupt event
    _SPI1RXIE = 1; // Enable RX interrupt
    
    //Enable SPI1
    SPI1CON1bits.SPIEN = 1;
    
    // from here, the device is ready to receive and transmit data
}



void SPI1_initialize_left_justified_host_mode_16_bit_channel_data_32_bit_frame() {

    /* The following code example will initialize the SPI1 Module in Left 
    Justified Host mode. */
    SPI1STATbits.SPIROV = 0; // clear the Overflow
    SPI1BRG = 1; //31.25kbps sample rate from 1/2 speed UPB clock @4MHz (MCLKEN = 0)
    
    SPI1CON1bits.AUDEN = 1; //Enable audio protocol mode
    SPI1CON1bits.AUDMOD = 0b01; //Left-justified mode
    SPI1CON1bits.AUDMONO = 0; //Audio data is stereo
    
    SPI1CON1bits.MSTEN = 1; //Host mode
    
    //16-bit channel, 32-bit frame
    SPI1CON1bits.MODE32 = 0;
    SPI1CON1bits.MODE16 = 0;
    
    SPI1CON1bits.CKP = 0; //Clock is active high
    SPI1CON1bits.FRMPOL = 1; //Frame pulse/client select is active high
    
    SPI1IMSKbits.SPITBEN = 1; // SPI1 transmit buffer empty generates interrupt event
    _SPI1TXIE = 1; // Enable TX interrupt
    
    SPI1IMSKbits.SPIRBFEN = 1; // SPI1 receive buffer full generates interrupt event
    _SPI1RXIE = 1; // Enable RX interrupt

    //Enable SPI1
    SPI1CON1bits.SPIEN = 1;
    
    // from here, the device is ready to receive and transmit data
}


void SPI1_initialize_right_justified_host_mode_16_bit_channel_data_32_bit_frame() {
    /* The following code example will initialize the SPI1 Module in Right 
    Justified Host mode. */
    SPI1STATbits.SPIROV = 0; // clear the Overflow
    SPI1BRG = 1; //31.25kbps sample rate from 1/2 speed UPB clock @4MHz (MCLKEN = 0)
    
    SPI1CON1bits.AUDEN = 1; //Enable audio protocol mode
    SPI1CON1bits.AUDMOD = 0b10; //Right-justified mode
    SPI1CON1bits.AUDMONO = 0; //Stereo mode

    SPI1CON1bits.MSTEN = 1; //Host mode
    
    //16-bit channel, 32-bit frame
    SPI1CON1bits.MODE32 = 0;
    SPI1CON1bits.MODE16 = 0;
    
    SPI1CON1bits.CKP = 0; //Clock is active high
    SPI1CON1bits.FRMPOL = 1; //Frame pulse/client select is active high
    
    SPI1IMSKbits.SPITBEN = 1; // SPI1 transmit buffer empty generates interrupt event
    _SPI1TXIE = 1; // Enable TX interrupt
    
    SPI1IMSKbits.SPIRBFEN = 1; // SPI1 receive buffer full generates interrupt event
    _SPI1RXIE = 1; // Enable RX interrupt
    
    //Enable SPI1
    SPI1CON1bits.SPIEN = 1;
    
    // from here, the device is ready to receive and transmit data 
}


void SPI1_initialize_PCM_DSP_host_mode_16_bit_channel_data_32_bit_frame() {

    /* The following code example will initialize the SPI1 Module in PCM/DSP Host 
    Mode. */
    SPI1STATbits.SPIROV = 0; // clear the Overflow
    SPI1BRG = 1; //31.25kbps sample rate from 1/2 speed UPB clock @4MHz (MCLKEN = 0)
    
    SPI1CON1bits.AUDEN = 1; //Enable audio protocol mode
    SPI1CON1bits.AUDMOD = 0b11; //PCM/DSP mode
    SPI1CON1bits.AUDMONO = 0; //Stero mode
    
    SPI1CON1bits.MSTEN = 1; //Host mode
    
    //16-bit channel, 32-bit frame
    SPI1CON1bits.MODE32 = 0;
    SPI1CON1bits.MODE16 = 0;
    
    SPI1CON1bits.CKP = 0; //Clock is active high
    SPI1CON1bits.FRMPOL = 1; //Frame pulse/client select is active high
    
    SPI1IMSKbits.SPITBEN = 1; // SPI1 transmit buffer empty generates interrupt event
    _SPI1TXIE = 1; // Enable TX interrupt
    
    SPI1IMSKbits.SPIRBFEN = 1; // SPI1 receive buffer full generates interrupt event
    _SPI1RXIE = 1; // Enable RX interrupt

    //Enable SPI1
    SPI1CON1bits.SPIEN = 1;
    
    // from here, the device is ready to receive and transmit data

}



void SPI1_initialize_I2S_host_mode_625kbps_bclk_16_bit_channel_data_32_bit_frame_with_REFO() {
    
    //Need to increase the CPU rate to keep up with data bandwidth requirements
    clock_CPU_for_200MIPS_from_PLL1();
    
    //Configure CLKGEN9 to output an MCLK of 40MHz.
    configure_CLKGEN9_for_40MHz_MCLK_from_PLL2();
    
    //Configure REFO2, which is controlled by CLKGEN13.
    //CLKGEN13 will be configured with identical settings to CLKGEN9, to
    //provide the same MCLK clock frequency from the same source.
            
    //Enable CLKGEN13, if not already enabled
    CLK13CONbits.ON = 1;
    
    //Set CLKGEN13 fractional divider to divide by 16
    CLK13DIVbits.INTDIV = 4;
    CLK13DIVbits.FRACDIV = 0;
    //Request CLKGEN13 fractional divider switch
    CLK13CONbits.DIVSWEN = 1;
    //Wait for CLKGEN13 fractional divider switch to complete
    while(CLK13CONbits.DIVSWEN);

    //Set PLL2 Fout as new CLKGEN13 clock source
    CLK13CONbits.NOSC = 6;
    //Request CLKGEN13 clock switch
    CLK13CONbits.OSWEN = 1; 
    //Wait for CLKGEN13 clock switch to complete
    while (CLK13CONbits.OSWEN);
    
    //Enable external output of CLKGEN13 clock
    CLK13CONbits.OE = 1;
    
    //Map REFO2 to pin RD9
    _TRISD9 = 0; //RD9 is output
    _RP58R = 23; //RP58 (RD9) is mapped to REFO2 output function
    
    //The following code will initialize the SPI1 Module in I2S Master mode.
    
    SPI1CON1bits.MCLKEN = 1; //Use MCLK from CLKGEN9
    
    SPI1STATbits.SPIROV = 0; // clear the Overflow
    
    SPI1BRG = 0; // to generate 625 kbps sample rate, PBCLK @ 40 MHz (MCLKEN = 1)
    //Sample rate = LRCK frequency = (baud rate / frame size) = (20MHz / 32) = 625kpbs.

    SPI1CON1bits.AUDEN = 1; //Enable audio protocol mode
    SPI1CON1bits.AUDMOD = 0b00; //I2S mode
    
    SPI1CON1bits.MSTEN = 1; //Host mode
    
    //16-bit data, 32-bit channel
    SPI1CON1bits.MODE32 = 0;
    SPI1CON1bits.MODE16 = 0;
    
    SPI1CON1bits.CKP = 1; //Clock is active low

    SPI1IMSKbits.SPITBEN = 1; // SPI1 transmit buffer empty generates interrupt event
    _SPI1TXIE = 1; // Enable TX interrupt
    
    SPI1IMSKbits.SPIRBFEN = 1; // SPI1 receive buffer full generates interrupt event
    _SPI1RXIE = 1; // Enable RX interrupt
    
    //Enable SPI1
    SPI1CON1bits.SPIEN = 1;
    
    // from here, the device is ready to receive and transmit data

}

void SPI1_initialize_16_bit_client_mode() {
    
    /* The following code example will initialize the SPI1 in Client mode. */

    SPI1STATbits.SPIROV = 0; // clear the Overflow
    
    SPI1CON1bits.MSTEN = 0; //Client mode
    
    //16 bits transfer
    SPI1CON1bits.MODE16 = 1;
    SPI1CON1bits.MODE32 = 0;
    
    SPI1CON1bits.CKP = 0; //Clock is active high
    SPI1CON1bits.CKE = 0; //Transmit happens on idle to active clock transition
    SPI1CON1bits.SMP = 0; //Input data sampled at the middle of data output time
    
    SPI1IMSKbits.SPIRBFEN = 1; // SPI1 receive buffer full generates interrupt event
    _SPI1RXIE = 1; // Enable interrupts
    
    //Enable SPI1
    SPI1CON1bits.SPIEN = 1;
    // from here, the device is ready to transmit and receive data. Buffer can be loaded to transmit data.
    //Data will be transmitted when the host initiates a transfer.
    
}


void SPI1_initialize_I2S_client_mode_16_bit_channel_data_32_bit_frame() {
    /* The following code example will initialize the SPI1 Module in I2S Client 
    mode. */
    
    SPI1STATbits.SPIROV = 0; // clear the Overflow
    
    SPI1CON1bits.AUDEN = 1; //Enable audio protocol mode
    SPI1CON1bits.AUDMOD = 0b00; //I2S mode

    SPI1CON1bits.MSTEN = 0; //Client mode
    
    //16-bit data, 32-bit channel
    SPI1CON1bits.MODE32 = 0;
    SPI1CON1bits.MODE16 = 0;
    
    SPI1CON1bits.CKP = 1; //Clock is active low
   
    SPI1IMSKbits.SPIRBFEN = 1; // SPI1 receive buffer full generates interrupt event
    _SPI1RXIE = 1; // Enable interrupts
    
    //Enable SPI1
    SPI1CON1bits.SPIEN = 1;
    
    // from here, the device is ready to receive and transmit data        
}
    

void SPI1_initialize_left_justified_client_mode_16_bit_channel_data_32_bit_frame() {
    /* The following code example will initialize the SPI1 Module in Left 
    Justified Client mode. */
    
    SPI1STATbits.SPIROV = 0; // clear the Overflow
    
    SPI1CON1bits.AUDEN = 1; //Enable audio protocol mode
    SPI1CON1bits.AUDMOD = 0b01; //Left-justified mode
    SPI1CON1bits.AUDMONO = 0; //Audio data is stereo
    
    SPI1CON1bits.MSTEN = 0; //Client mode
    
    //16-bit channel, 32-bit frame
    SPI1CON1bits.MODE32 = 0;
    SPI1CON1bits.MODE16 = 0;
    
    SPI1CON1bits.CKP = 0; //Clock is active high
    SPI1CON1bits.FRMPOL = 1; //Frame pulse/client select is active high
    
    SPI1IMSKbits.SPIRBFEN = 1; // SPI1 receive buffer full generates interrupt event
    _SPI1RXIE = 1; // Enable interrupts
 
    //In audio modes, reception/transmission may be constantly driven by the host.
    //Therefore, ignore transmit underrun.
    SPI1CON1bits.IGNTUR = 1;
    //In case of transmit underrun, present the data stored in SPI1URDT,
    //instead of the last transmitted data.
    SPI1CON1bits.URDTEN = 1;
    SPI1URDT = 0xAA55;
    
    //Enable SPI1
    SPI1CON1bits.SPIEN = 1;
    
    // from here, the device is ready to receive and transmit data
}


void SPI1_initialize_right_justified_client_mode_16_bit_channel_data_32_bit_frame() {
    /* The following code example will initialize the SPI1 Module in Right 
    Justified Client mode. */
    
    SPI1STATbits.SPIROV = 0; // clear the Overflow
    
    SPI1CON1bits.AUDEN = 1; //Enable audio protocol mode
    SPI1CON1bits.AUDMOD = 0b10; //Right-justified mode
    
    SPI1CON1bits.MSTEN = 0; //Client mode
    
    //16-bit channel, 32-bit frame
    SPI1CON1bits.MODE32 = 0;
    SPI1CON1bits.MODE16 = 0;
    
    SPI1CON1bits.CKP = 0; //Clock is active high
    SPI1CON1bits.FRMPOL = 1; //Frame pulse/client select is active high
    
    //In audio modes, reception/transmission may be constantly driven by the host.
    //Therefore, ignore transmit underrun.
    SPI1CON1bits.IGNTUR = 1;
    //In case of transmit underrun, present the data stored in SPI1URDT,
    //instead of the last transmitted data.
    SPI1CON1bits.URDTEN = 1;
    SPI1URDT = 0xAA55;
    
    SPI1IMSKbits.SPIRBFEN = 1; // SPI1 receive buffer full generates interrupt event
    _SPI1RXIE = 1; // Enable interrupts
    
    //Enable SPI1
    SPI1CON1bits.SPIEN = 1;
    // from here, the device is ready to receive and transmit data
}


void SPI1_initialize_PCM_DSP_client_mode_16_bit_channel_data_32_bit_frame() {

    /* The following code example will initialize the SPI1 Module in PCM/DSP 
    Client Mode. */
    
    SPI1STATbits.SPIROV = 0; // clear the Overflow
    
    SPI1CON1bits.AUDEN = 1;     //Enable audio protocol mode
    SPI1CON1bits.AUDMOD = 0b11; //PCM/DSP mode
    SPI1CON1bits.AUDMONO = 0;   //Stereo mode
    SPI1CON1bits.FRMPOL = 1;    //Frame pulse/client select is active high
    
    SPI1CON1bits.MSTEN = 0; //Client mode
    
    //16-bit channel, 32-bit frame
    SPI1CON1bits.MODE32 = 0;
    SPI1CON1bits.MODE16 = 0;
    
    SPI1CON1bits.CKP = 0; //Clock is active high
    
    //In audio modes, reception/transmission may be constantly driven by the host.
    //Therefore, ignore transmit underrun.
    SPI1CON1bits.IGNTUR = 1;
    //In case of transmit underrun, present the data stored in SPI1URDT,
    //instead of the last transmitted data.
    SPI1CON1bits.URDTEN = 1;
    SPI1URDT = 0xAA55;
    
    SPI1IMSKbits.SPIRBFEN = 1; // SPI1 receive buffer full generates interrupt event
    _SPI1RXIE = 1; // Enable interrupts
    
    //Enable SPI1
    SPI1CON1bits.SPIEN = 1;
    // from here, the device is ready to receive and transmit data

}

//The SPIxBUF register is 32 bits wide. For a variably sized N-bit transfer, only the
//lower N number of bits are used, but the procedure is the same regardless of the data size.
uint32_t SPI1_host_exchange_word(uint32_t write_data) {
    //Populate data buffer, initiating transaction
    SPI1BUF = write_data;
    //Wait for SPI operation to complete
    while(!SPI1STATbits.SRMT);
    //Read data shifted into the buffer from client
    return SPI1BUF;
}

//To transfer N number of words, write SPIxBUF, and read back for the client response, N number of times.
void SPI1_host_exhange_array(uint32_t* transmit_buffer, uint32_t* receive_buffer, uint32_t len) {
   
    //Complete len number of transfers
    for (int i = 0; i < len; i++) {
        
        //Wait until there is some space in the transmit buffer
        while(SPI1STATbits.SPITBF);
        
        //Transfer one word from the transmit array, placing read back value in receive array.
        receive_buffer[i] = SPI1_host_exchange_word(transmit_buffer[i]);
    }
}


void configure_CLKGEN9_for_40MHz_MCLK_from_PLL2() {
        
    PLL2CONbits.ON = 1; //Enable PLL generator 2, if not already enabled
    
    //Select FRC as PLL2's clock source
    PLL2CONbits.NOSC = 1;
    //Request PLL2 clock switch
    PLL2CONbits.OSWEN = 1;
    //Wait for PLL2 clock switch to complete
    while(PLL2CONbits.OSWEN);
    
    //Set up PLL2 dividers to output 200MHz
    PLL2DIVbits.PLLPRE = 1; //Reference input will be 8MHz, no division
    PLL2DIVbits.PLLFBDIV = 160; //Fvco = 8MHz * 160 = 1280MHz
    PLL2DIVbits.POSTDIV1 = 4; //Divide Fcvo by 4
    PLL2DIVbits.POSTDIV2 = 1; //Fpllo = Fvco / 4 / 1 = 320 MHz

    //The PLLSWEN bit controls changes to the PLL feedback divider.
    //Request PLL2 feedback divider switch
    PLL2CONbits.PLLSWEN = 1;
    //Wait for PLL2 feedback divider switch to complete
    while(PLL2CONbits.PLLSWEN);

    //The FOUTSWEN bit controls changes to the PLL output dividers.
    //Request PLL2 output divider switch
    PLL2CONbits.FOUTSWEN = 1;
    //Wait for PLL2 output divider switch to complete
    while(PLL2CONbits.FOUTSWEN);

    //Enable CLKGEN9, if not already enabled
    CLK9CONbits.ON = 1;
    
    //Set CLKGEN9 fractional divider to divide by 8
    CLK9DIVbits.INTDIV = 4;
    CLK9DIVbits.FRACDIV = 0;
    //Request CLKGEN9 fractional divider switch
    CLK9CONbits.DIVSWEN = 1;
    //Wait for CLKGEN9 fractional divider switch to complete
    while(CLK9CONbits.DIVSWEN);

    //Set PLL2 Fout as new CLKGEN9 clock source
    CLK9CONbits.NOSC = 6;
    //Request CLKGEN9 clock switch
    CLK9CONbits.OSWEN = 1; 
    //Wait for CLKGEN9 clock switch to complete
    while (CLK9CONbits.OSWEN);
    
    //This yields a 40MHz MCLK source available to the SPI by selecting SPIxCON1bits.MCLKEN.
    //With a BRG value of 0, the fastest SCK that can be achieved is 20MHz.
    //Higher frequencies can be obtained by decreasing the CLKGEN9 divider values.
}

#ifdef SPI_HOST_BLOCKING_OPERATION

int main(void) {
    
    //SCKA is RA8 (RP9)
    //MISOA is RA9 (RP10)
    //MOSIA is RA10 (RP11)
    //CSA is RA2 (RP3)
    
    _RP9R = 14;     //RA8 assigned to SCK1OUT function
    _ANSELA8 = 0;   //RA8 digital
    _TRISA8 = 0;    //RA8 output
    
    _SDI1R = 10;    //SDI1 input function assigned to RA9
    _ANSELA9 = 0;   //RA9 digital
    _TRISA9 = 1;    //RA9 input
    
    _RP11R = 13;    //RA10 assigned to SDO1 output function
    _ANSELA10 = 0;  //RA10 digital
    _TRISA10  = 0;  //RA10 output
    
    _RP3R = 15;     //RA2 assigned to SS1OUT function
    _ANSELA2 = 0;   //RA2 digital
    _TRISA2 = 0;    //RA2 output
    
    //Initialize SPI in Host mode, 16-bit data size. Do not enable SPI interrupts.
    SPI1BRG = 1; //SCK @1MHz; clocked from 1/2 speed UPB clock @4MHz (MCLKEN = 0), /4
    SPI1STATbits.SPIROV = 0; // clear the Overflow
    SPI1CON1bits.MSTEN = 1; //Host mode
    //16 bits transfer
    SPI1CON1bits.MODE16 = 1;
    SPI1CON1bits.MODE32 = 0;
    SPI1CON1bits.CKP = 0; //Clock is active high
    SPI1CON1bits.CKE = 0; //Transmit happens on idle to active clock transition
    SPI1CON1bits.SMP = 0; //Input data sampled at the middle of data output time
    SPI1CON1bits.SPIEN = 1;
    
    //For 16-bit modes, only the lower 16 bits of these 32-bit values will be transmitted.
    uint32_t transmit_data [8] = {0x11111111, 0x22222222, 0x33333333, 0x44444444, 0x55555555, 0x66666666, 0x77777777, 0x88888888};
    uint32_t receive_data [8] = {0, 0, 0, 0, 0, 0, 0, 0};
    
    while (1) {
        
        //Transfer 8 words of data
        SPI1_host_exhange_array(transmit_data, receive_data, 8);
        
        //Some delay in between transactions
        for (int i = 0; i < 0xFF; i++);
    }
}

#endif

#ifdef SPI_HOST_INTERRUPT_DRIVEN_OPERATION

#define TX_BUFFER_LEN 8
#define RX_BUFFER_LEN 128

volatile uint32_t tx_isr_buff [TX_BUFFER_LEN] = {0x11111111, 0x22222222, 0x33333333, 0x44444444, 0x55555555, 0x66666666, 0x77777777, 0x88888888};
volatile uint32_t tx_isr_buff_index = 0;

volatile uint32_t rx_isr_buff[RX_BUFFER_LEN];
volatile uint32_t rx_isr_buff_index = 0;

void __attribute__((__interrupt__)) _SPI1RXInterrupt() {
    
    _SPI1RXIF = 0;
    
    //Store received data into a buffer
    rx_isr_buff[rx_isr_buff_index++] = SPI1BUF;
    if (rx_isr_buff_index >= RX_BUFFER_LEN) {
        rx_isr_buff_index = 0;
    }
    
}

void __attribute__((__interrupt__)) _SPI1TXInterrupt() {
    
    _SPI1TXIF = 0;
    
    //Repeat writes until transmit buffer is full
    while(!SPI1STATbits.SPITBF) {
    
        SPI1BUF = tx_isr_buff[tx_isr_buff_index++];
        if (tx_isr_buff_index >= TX_BUFFER_LEN) {
            tx_isr_buff_index = 0;
        }
    }
    
}


int main(void) {
    
    //SCKA is RA8 (RP9)
    //MISOA is RA9 (RP10)
    //MOSIA is RA10 (RP11)
    //CSA is RA2 (RP3)
    
    _RP9R = 14;     //RA8 assigned to SCK1OUT function
    _ANSELA8 = 0;   //RA8 digital
    _TRISA8 = 0;    //RA8 output
    
    _SDI1R = 10;    //SDI1 input function assigned to RA9
    _ANSELA9 = 0;   //RA9 digital
    _TRISA9 = 1;    //RA9 input
    
    _RP11R = 13;    //RA10 assigned to SDO1 output function
    _ANSELA10 = 0;  //RA10 digital
    _TRISA10  = 0;  //RA10 output
    
    _RP3R = 15;     //RA2 assigned to SS1OUT function
    _ANSELA2 = 0;   //RA2 digital
    _TRISA2 = 0;    //RA2 output
    
    //Set up SPI in host mode, 16-bit data. This function enables the TX/RX interrupts.
    SPI1_initialize_16_bit_host_mode();
    
    while (1);
    
}


#endif

#ifdef SPI_CLIENT_INTERRUPT_DRIVEN_OPERATION

#define RX_BUFFER_LEN 256
volatile uint32_t rx_buffer[RX_BUFFER_LEN];
volatile uint32_t rx_buffer_index = 0;

void __attribute__((__interrupt__)) _SPI1RXInterrupt() {
    
    _SPI1RXIF = 0;
    
    //Read until the RX buffer is empty.
    while(!SPI1STATbits.SPIRBE) {
        
        //Get received data and store in buffer
        uint32_t received = SPI1BUF;
        rx_buffer[rx_buffer_index++] = received;

        //For debugging purpose, transmit back the same data received
        SPI1BUF = received;

        if (rx_buffer_index >= RX_BUFFER_LEN) {
            rx_buffer_index = 0;
        }
    }
    
}

int main(void) {

    //SCKA is RA8 (RP9)
    //MISOA is RA9 (RP10)
    //MOSIA is RA10 (RP11)
    //CSA is RA2 (RP3)
    
    _SCK1R = 9;     //SCK1 input function assigned to RA8
    _ANSELA8 = 0;   //RA8 digital
    _TRISA8 = 1;    //RA8 input
    
    _SDI1R = 10;    //SDI1 input function assigned to RA9
    _ANSELA9 = 0;   //RA9 digital
    _TRISA9 = 1;    //RA9 input
    
    _RP11R = 13;    //RA10 assigned to SDO1 output function
    _ANSELA10 = 0;  //RA10 digital
    _TRISA10  = 0;  //RA10 input
    
    _SS1R = 3;      //SS1 input function assigned to RA2
    _ANSELA2 = 0;   //RA2 digital
    _TRISA2 = 1;    //RA2 input

    //Initialize the SPI in client mode, 16-bit data size. This will enable the RX interrupt.
    SPI1_initialize_16_bit_client_mode();
        
    while (1);
}

#endif
