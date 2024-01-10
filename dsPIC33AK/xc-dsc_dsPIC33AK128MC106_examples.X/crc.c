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
 * File:   crc.c
 * Author: C68555
 *
 * Created on August 25, 2023, 4:50 PM
 */


#include "xc.h"
#include "examples.h"

unsigned int CalculateNonDirectSeedInSoftware(
    unsigned int seed,              // direct CRC initial value
    unsigned int polynomial,        // polynomial
    unsigned char polynomialOrder)   // polynomial order
{
    unsigned char lsb;
    unsigned char i;
    unsigned int msbmask;

    msbmask = ((unsigned int)1)<<(polynomialOrder-1);
    for (i=0; i<polynomialOrder; i++) {
        lsb = seed & 1;
        if (lsb) {
            seed ^= polynomial;   
        }
        seed >>= 1;
        if (lsb) {
            seed |= msbmask;
        }
    }
    return seed; // return the non-direct CRC initial value
}



#ifdef CRC_CALCULATING_THE_NON_DIRECT_INITIAL_VALUE_MOD_BIT_0

// WHERE THE FUNCTION TO REVERSE THE BIT ORDER CAN BE
unsigned int ReverseBitOrder(
    unsigned int data, // input data
    unsigned char numberOfBits) // width of the input data, valid values are 8,16,32 bits
{
    unsigned int maskin = 0;
    unsigned int maskout = 0;
    unsigned int result = 0;
    unsigned char i;
    
    switch(numberOfBits) 
    {
        case 8: {
            maskin = 0x80; 
            maskout = 0x01; 
            break;
        }
        case 16:  {
            maskin = 0x8000; 
            maskout = 0x0001; 
            break;
        }
        case 32:  {
            maskin = 0x80000000; 
            maskout = 0x00000001; 
            break;
        }
        default: {}
    }
    for(i=0; i<numberOfBits; i++)
    {
        if(data&maskin){ 
            result |= maskout;
        } 
        maskin >>= 1; 
        maskout <<= 1;
    }
    return result;
}

unsigned int CalculateNonDirectSeed(
    unsigned int seed,              // direct CRC initial value 
    unsigned int polynomial,        // polynomial
    unsigned char polynomialOrder)  // polynomial order (valid values are 8, 16, 32 bits)
{
    CRCCON = 0;
    CRCCONbits.ON = 1; // enable CRC
    
    CRCCONbits.CRCISEL= 0; // interrupt when all shifts are done
    CRCCONbits.DWIDTH= polynomialOrder-1; // data width
    CRCCONbits.PLEN= polynomialOrder-1; // polynomial length
    CRCCONbits.CRCGO= 1; // start CRC calculation
    
    polynomial >>= 1; // shift the polynomial right 
    polynomial = ReverseBitOrder(polynomial, polynomialOrder); // reverse bits order of the polynomial
    CRCXOR = polynomial; // set the reversed polynomial
    seed = ReverseBitOrder(seed, polynomialOrder); // reverse bits order of the seed value
    CRCWDAT = seed;
    _CRCIF = 0; // clear interrupt flag
    
    switch(polynomialOrder) // load dummy data to shift out the seed result
    {
        case 8: {
            *((unsigned char*)&CRCDAT) = 0; // load byte
            while(!_CRCIF); // wait until shifts are done 
            seed = CRCWDAT&0x00ff; // read reversed seed 
        }
        case 16: {
            CRCDAT = 0; // load short 
            while(!_CRCIF); // wait until shifts are done 
            seed = CRCWDAT; // read reversed seed 
            break;
        }
        case 32:  {
            // load long 
            CRCDAT = 0; 
            while(!_CRCIF); // wait for shifts are done 
            seed = CRCWDAT; // read reversed seed 
            break;
        }
        default: {}
    }
    seed = ReverseBitOrder(seed, polynomialOrder); // reverse the bit order to get the non-direct seed 
    return seed; // return the non-direct CRC initial value}
}

#endif


#ifdef CRC_ROUTINE_TO_GET_THE_FINAL_CRC_RESULT_IN_LEGACY_MODE_MOD_BIT_0

unsigned int GetCRC(
    unsigned char polynomialOrder, // valid values are 8,16,32
    unsigned char currentDataWidth) // valid values are 8,16,32
{
    unsigned int crc = 0;
    
    while(!CRCCONbits.CRCMPT); // wait until data FIFO is empty
    asm volatile("repeat %0\n nop" : : "r"(currentDataWidth>>1)); // wait until previous data shifts are done
    
    CRCCONbits.DWIDTH = polynomialOrder-1; // set data width to polynomial length
    CRCCONbits.CRCISEL = 0; // interrupt when all shifts are done
    _CRCIF = 0; // clear interrupt flag
    
    switch(polynomialOrder) 
    {
        case 8: { // polynomial length is 8 bits 
            *((unsigned char*)&CRCDAT) = 0; // load byte 
            while(!_CRCIF); // wait until shifts are done 
            //crc = CRCWDATL&0x00ff; // get crc 
            crc = CRCWDAT&0x00ff; // get crc 
            break;
        }
        case 16: {// polynomial length is 16 bits 
            CRCDAT = 0; // load short 
            while(!_CRCIF); // wait until shifts are done 
            crc = CRCWDAT; // get crc 
            break;
        }
        case 32: {// polynomial length is 32 bits 
            CRCDAT = 0; 
            while(!_CRCIF); // wait until shifts are done 
            crc = CRCWDAT; // get crc 
            break;
        }
        default: {}
    }
    CRCCONbits.DWIDTH = currentDataWidth-1; // restore data width for further calculations
    return crc; // return the final CRC value
}

#endif


#ifdef CRC_SMBUS_8_BIT_POLYNOMIAL_WITH_32_BIT_DATA_BIG_ENDIAN_MOD_BIT_1

// standard CRC-SMBUS
#define CRCSMBUS_POLYNOMIAL ((unsigned int)0x00000007)
#define CRCSMBUS_SEED_VALUE ((unsigned int)0x00000000) // direct initial value

// This macro is used to swap bytes for big endian, using the SWAP instruction
#define Swap(x) __extension__({ \
unsigned int __x = (x), __v; \
__asm__ ("swap.l %0;\n\t" \
: "=d" (__v) \
: "d" (__x)); \
__v; \
})

// ASCII bytes "12345678"
volatile unsigned char __attribute__((aligned(4))) message[] = 
{'1','2','3','4','5','6','7','8'};

volatile unsigned char crcResultCRCSMBUS = 0;

int main (void)
{
    unsigned int* pointer;
    unsigned short length;
    unsigned int data;

    CRCCON = 0;
    CRCCONbits.MOD = 1;     // alternate mode
    CRCCONbits.ON = 1;      // enable CRC
    CRCCONbits.LENDIAN = 0; // big endian
    CRCCONbits.CRCISEL = 0; // interrupt when all shifts are done
    CRCCONbits.DWIDTH = 32-1; // 32-bit data width
    CRCCONbits.PLEN = 8-1; // 8-bit polynomial order

    CRCXOR = CRCSMBUS_POLYNOMIAL; // set polynomial
    CRCWDAT = CRCSMBUS_SEED_VALUE; // set initial value

    CRCCONbits.CRCGO = 1; // start CRC calculation
    pointer = (unsigned int*)message;
    length = sizeof(message)/sizeof(unsigned int);

    while(1)
    {
        while(CRCCONbits.CRCFUL); // wait if FIFO is full
        data = *pointer++; // load from little endian
        data = Swap(data); // swap bytes for big endian
        length--;
        if(length == 0)
        {
            break;
        }
        CRCDAT = data; // 32-bit word access to FIFO
    }

    CRCCONbits.CRCGO = 0; // suspend CRC calculation
    _CRCIF = 0; // clear the interrupt flag
    CRCDAT = data; // write last data into FIFO
    CRCCONbits.CRCGO = 1; // resume CRC calculation
    while(!_CRCIF); // wait until shifts are done

    crcResultCRCSMBUS = (unsigned char)CRCWDAT&0x00ff; // get CRC result (must be 0xC7)

    while(1);

    return 1;
}

#endif

#ifdef CRC_16_16_BIT_DATA_WITH_16_BIT_POLYNOMIAL_LITTLE_ENDIAN_MOD_BIT_1

// standard CRC-16
#define CRC16_POLYNOMIAL ((unsigned int)0x00008005)
#define CRC16_SEED_VALUE ((unsigned int)0x00000000) // direct initial value

// ASCII bytes "87654321"
volatile unsigned short message[] = {0x3738,0x3536,0x3334,0x3132};
volatile unsigned short crcResultCRC16 = 0;

int main (void)
{
    unsigned short* pointer;
    unsigned short length;
    unsigned short data;

    CRCCON = 0;
    CRCCONbits.MOD = 1; // alternate mode
    CRCCONbits.ON = 1; // enable CRC
    CRCCONbits.CRCISEL = 0; // interrupt when all shifts are done
    CRCCONbits.LENDIAN = 1; // little endian
    CRCCONbits.DWIDTH = 16-1; // 16-bit data width
    CRCCONbits.PLEN = 16-1; // 16-bit polynomial order
    CRCXOR = CRC16_POLYNOMIAL; // set polynomial
    CRCWDAT = CRC16_SEED_VALUE; // set initial value
    CRCCONbits.CRCGO = 1; // start CRC calculation

    pointer = (unsigned short*)message;
    length = sizeof(message)/sizeof(unsigned short);

    while(1)
    {
        while(CRCCONbits.CRCFUL); // wait if FIFO is full
        data = *pointer++; // load data
        length--;
        if(length == 0)
        {
            break;
        }

        *((unsigned short*)&CRCDAT) = data; // 16-bit word access to FIFO
    }
    
    CRCCONbits.CRCGO = 0; // suspend CRC calculation
    _CRCIF = 0; // clear the interrupt flag
    
    *((unsigned short*)&CRCDAT) = data; // write last data into FIFO
    CRCCONbits.CRCGO = 1; // resume CRC calculation
    
    while(!_CRCIF); // wait until shifts are done
    crcResultCRC16 = (unsigned short)CRCWDAT; // get CRC result (must be 0xE716)
    
    while(1);
    
    return 1;
}


#endif


#ifdef CRC_32_32_BIT_POLYNOMIAL_WITH_32_BIT_DATA_LITTLE_ENDIAN_MOD_BIT_1

// standard CRC-32
#define CRC32_POLYNOMIAL ((uint32_t)0x04C11DB7)
#define CRC32_SEED_VALUE ((uint32_t)0xFFFFFFFF) // direct initial value

// ASCII bytes "12345678"
volatile unsigned char __attribute__((aligned(4))) message[] = {'1','2','3','4','5','6','7','8'};

// function to reverse the bit order (OPTIONAL)
unsigned int ReverseBitOrder(unsigned int data);

volatile uint32_t crcResultCRC32 = 0;

int main(void)
{
    unsigned int* pointer;
    unsigned short length;

    CRCCON = 0;
    CRCCONbits.MOD = 1; // alternate mode
    CRCCONbits.ON = 1; // enable CRC
    CRCCONbits.CRCISEL = 0; // interrupt when all shifts are done
    CRCCONbits.LENDIAN = 1; // little endian
    CRCCONbits.DWIDTH = 32-1; // 32-bit data width
    CRCCONbits.PLEN = 32-1; // 32-bit polynomial order

    CRCXOR = CRC32_POLYNOMIAL; // set polynomial
    CRCWDAT = CRC32_SEED_VALUE; // set initial value

    CRCCONbits.CRCGO = 1; // start CRC calculation

    pointer = (unsigned int*)message;
    length = sizeof(message)/sizeof(unsigned int);

    while(1)
    {
        while(CRCCONbits.CRCFUL); // wait if FIFO is full
        length--;
        if(length == 0)
        {
            break;
        }
        CRCDAT = *pointer++; // 32-bit word access to FIFO
    }
    CRCCONbits.CRCGO = 0; // suspend CRC calculation
    _CRCIF = 0; // clear the interrupt flag
    CRCDAT = *pointer; // write last data into FIFO

    CRCCONbits.CRCGO = 1; // resume CRC calculation
    while(!_CRCIF); // wait until shifts are done
    crcResultCRC32 = CRCWDAT; // get the final CRC result

    // OPTIONAL reverse CRC value bit order and invert (must be 0x9AE0DAAF)
    crcResultCRC32 = ~ReverseBitOrder(crcResultCRC32);
    
    while(1);

    return 1;
}

unsigned int ReverseBitOrder(unsigned int data)
{
    unsigned int maskin;
    unsigned int maskout;
    unsigned int result = 0;
    unsigned char i;
    
    maskin = 0x80000000;
    maskout = 0x00000001;
    for(i=0; i<32; i++)
    {
        if(data&maskin){
            result |= maskout;
        }
        
        maskin >>= 1;
        maskout <<= 1;
    }
    return result;
}


#endif


#ifdef CRC_DATA_WIDTH_SWITCHING_32_BIT_POLYNOMIAL_LITTLE_ENDIAN_MOD_BIT_1

#define CRC32_POLYNOMIAL ((uint32_t)0x04C11DB7)
#define CRC32_SEED_VALUE ((uint32_t)0xFFFFFFFF) // direct initial value

// ASCII bytes "12345678"
volatile uint32_t message1[] = {0x34333231,0x38373635};

// ASCII bytes "123"
volatile unsigned char message2[] = {'1','2','3'};

volatile uint32_t crcResultCRC32 = 0;

int main(void)
{
    unsigned char* pointer8;
    unsigned int* pointer32;
    unsigned short length;

    CRCCON = 0;
    CRCCONbits.MOD = 1; // alternate mode
    CRCCONbits.ON = 1; // enable CRC
    CRCCONbits.CRCISEL = 0; // interrupt when all shifts are done
    CRCCONbits.LENDIAN = 1; // little endian
    CRCCONbits.DWIDTH = 32-1; // 32-bit data width
    CRCCONbits.PLEN = 32-1; // 32-bit polynomial order
    
    CRCXOR = CRC32_POLYNOMIAL; // set polynomial
    CRCWDAT = CRC32_SEED_VALUE; // set initial value
    
    CRCCONbits.CRCGO = 1; // start CRC calculation
    
    pointer32 = (unsigned int*)message1;
    length = sizeof(message1)/sizeof(unsigned int);
    
    while(1)
    {
        while(CRCCONbits.CRCFUL); // wait if FIFO is full
        length--;
        if(length == 0)
        {
            break;
        }
        CRCDAT = *pointer32++; // 32-bit word access to FIFO
    }
    
    CRCCONbits.CRCGO = 0; // suspend CRC calculation
    _CRCIF = 0; // clear the interrupt flag
   
    CRCDAT = *pointer32; // write last 32-bit data into FIFO
    CRCCONbits.CRCGO = 1; // resume CRC calculation
    
    while(!_CRCIF); // wait until shifts are done
    
    CRCCONbits.DWIDTH = 8-1; // switch the data width to 8-bit
 
    pointer8 = (unsigned char*)message2;
    length = sizeof(message2)/sizeof(unsigned char);
    
    while(1)
    {
        while(CRCCONbits.CRCFUL); // wait if FIFO is full
        length--;
        if(length == 0)
        {
            break;
        }
        *((unsigned char*)&CRCDAT) = *pointer8++; // byte access to FIFO
    }
    
    CRCCONbits.CRCGO = 0; // suspend CRC calculation
    _CRCIF = 0; // clear the interrupt flag
    
    *((unsigned char*)&CRCDAT) = *pointer8; // write last 8-bit data into FIFO
    
    CRCCONbits.CRCGO = 1; // resume CRC calculation
    while(!_CRCIF); // wait until shifts are done
    
    crcResultCRC32 = CRCWDAT; // get the final CRC result (must be 0xE092727E)

    while(1);

    return 1;
}

#endif