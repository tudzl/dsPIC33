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
 * File:   pwm.c
 * Author: C68555
 *
 * Created on June 12, 2023, 12:58 PM
 */


#include "xc.h"
#include "examples.h"


void clock_PWM_at_400MHz_from_PLL2_Fout() {
    PLL2CONbits.ON = 1; //Enable PLL generator 2, if not already enabled
    
    //Select FRC as PLL2's clock source
    PLL2CONbits.NOSC = 1;
    //Request PLL2 clock switch
    PLL2CONbits.OSWEN = 1;
    //Wait for PLL2 clock switch to complete
    while(PLL2CONbits.OSWEN);
    
    //Set up PLL2 dividers to output 200MHz
    PLL2DIVbits.PLLPRE = 1; //Reference input will be 8MHz, no division
    PLL2DIVbits.PLLFBDIV = 200; //Fvco = 8MHz * 200 = 1600MHz
    PLL2DIVbits.POSTDIV1 = 4; //Divide Fcvo by 4
    PLL2DIVbits.POSTDIV2 = 1; //Fpllo = Fvco / 4 / 1 = 400 MHz

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
    
    //Enable CLKGEN5
    CLK5CONbits.ON = 1;
    
    //Reset CLKGEN5 fractional divider for 1:1 ratio
    CLK5DIVbits.INTDIV = 0;
    CLK5DIVbits.FRACDIV = 0;
    //Request CLKGEN5 fractional divider switch
    CLK5CONbits.DIVSWEN = 1;
    //Wait for CLKGEN5 fractional divider switch to complete
    while(CLK5CONbits.DIVSWEN);

    //Set PLL2 Fout as new CLKGEN5 clock source
    CLK5CONbits.NOSC = 6;
    //Request CLKGEN5 clock switch
    CLK5CONbits.OSWEN = 1; 
    //Wait for CLKGEN5 clock switch to complete
    while (CLK5CONbits.OSWEN);
    
    //Select CLKGEN5 as PWM master clock source
    PCLKCONbits.MCLKSEL = 1;
}

void configure_PLL2_Fout_200MHz_and_VCODIV_500_MHz() {
    
    PLL2CONbits.ON = 1; //Enable PLL generator 2, if not already enabled
    
    //Select FRC as PLL2's clock source
    PLL2CONbits.NOSC = 1;
    //Request PLL2 clock switch
    PLL2CONbits.OSWEN = 1;
    //Wait for PLL2 clock switch to complete
    while(PLL2CONbits.OSWEN);
    
    //Set up PLL2 dividers to output 200MHz
    PLL2DIVbits.PLLPRE = 1; //Reference input will be 8MHz, no division
    PLL2DIVbits.PLLFBDIV = 125; //Fvco = 8MHz * 125 = 1000MHz
    PLL2DIVbits.POSTDIV1 = 5; //Divide Fcvo by 5
    PLL2DIVbits.POSTDIV2 = 1; //Fpllo = Fvco / 5 / 1 = 200 MHz

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
    
    VCO2DIVbits.INTDIV = 1; //PLL2 VCO FracDiv will divide by 2
    //Request PLL2 VCO divider switch
    PLL2CONbits.DIVSWEN = 1;
    //Wait for VCO divider switch to complete
    while(PLL2CONbits.DIVSWEN);
    //PLL2 VCO FracDiv output == 1000MHz / 2 == 500MHz
    
}

void configure_PLL2_Fout_400MHz() {
    
    PLL2CONbits.ON = 1; //Enable PLL generator 2, if not already enabled
    
    //Select FRC as PLL2's clock source
    PLL2CONbits.NOSC = 1;
    //Request PLL2 clock switch
    PLL2CONbits.OSWEN = 1;
    //Wait for PLL2 clock switch to complete
    while(PLL2CONbits.OSWEN);
    
    //Set up PLL2 dividers to output 200MHz
    PLL2DIVbits.PLLPRE = 1; //Reference input will be 8MHz, no division
    PLL2DIVbits.PLLFBDIV = 200; //Fvco = 8MHz * 200 = 1600MHz
    PLL2DIVbits.POSTDIV1 = 4; //Divide Fcvo by 4
    PLL2DIVbits.POSTDIV2 = 1; //Fpllo = Fvco / 4 / 1 = 400 MHz

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
    
}


void clock_PWM_from_PLL2_Fout() {

    //Enable CLKGEN5
    CLK5CONbits.ON = 1;
    
    //Reset CLKGEN5 fractional divider for 1:1 ratio
    CLK5DIVbits.INTDIV = 0;
    CLK5DIVbits.FRACDIV = 0;
    //Request CLKGEN5 fractional divider switch
    CLK5CONbits.DIVSWEN = 1;
    //Wait for CLKGEN5 fractional divider switch to complete
    while(CLK5CONbits.DIVSWEN);

    //Set PLL2 Fout as new CLKGEN5 clock source
    CLK5CONbits.NOSC = 6;
    //Request CLKGEN5 clock switch
    CLK5CONbits.OSWEN = 1; 
    //Wait for CLKGEN5 clock switch to complete
    while (CLK5CONbits.OSWEN);
    
    //Select CLKGEN5 as PWM master clock source
    PCLKCONbits.MCLKSEL = 1;
}

void clock_PWM_from_UPB_clock() {
    //Select UPB clock (1/2 speed) as PWM master clock source
    PCLKCONbits.MCLKSEL = 0;
}


void initialize_CMP1_and_clock_from_PLL2_VCODIV() {
    
    //DAC needs a 500MHz clock. 
    //In this example, a 500MHz clock is provided from PLL2's VCO dividers.
    //PLL2 needs to be configured for a 500MHz VCO Fracdiv frequency before calling this function.
    
    //Select CMP1A as DAC1 input
    DAC1CONbits.INSEL = 0b000;
    _ANSELA2 = 1; //CMP1A is RA2, set as analog input
    
    //Enable CLKGEN7
    CLK7CONbits.ON = 1;
    
    //Reset CLKGEN6 fractional divider for 1:1 ratio
    CLK7DIVbits.INTDIV = 0;
    CLK7DIVbits.FRACDIV = 0;
    //Request CLKGEN6 fractional divider switch
    CLK7CONbits.DIVSWEN = 1;
    //Wait for CLKGEN6 fractional divider switch to complete
    while(CLK7CONbits.DIVSWEN);

    //Set PLL2 VCO Fracdiv as new CLKGEN6 clock source
    CLK7CONbits.NOSC = 8;
    //Request CLKGEN6 clock switch
    CLK7CONbits.OSWEN = 1; 
    //Wait for CLKGEN6 clock switch to complete
    while (CLK7CONbits.OSWEN);
    
    DACCTRL1bits.ON = 1;
    DAC1CONbits.DACEN = 1;
    
    DAC1DATbits.DACDAT = 2048;
    
}


#ifdef PWM_SIX_STEP_PWM_SCHEME_1_EXAMPLE

//#include <stdint.h>
//For delay function
#define FCY 8000000     //CPU frequency in Hz
#include "libpic30.h"

uint16_t state = 0;

#define H_ACTIVE_L_LOW  0x001C1000
//PGxIOCONbits.PMOD = 0b01 -- Independent output mode
//PGxIOCONbits.PENH = 1 -- PWM Generator x controls PWMxH output pin
//PGxIOCONbits.PENL = 1 -- PWM Generator x controls PWMxL output pin
//PGxIOCONbits.OVRENH = 0 -- PWMxH output not overridden
//PGxIOCONbits.OVRENL = 1 -- PWMxL output overridden
//PGxIOCONbits.OVRDAT = 0b00 -- Override data for PWMxH/L (only L uses it) is 0
//PGxIOCONbits.OSYNC = 0b00 -- User output overrides are synchronized to next start of cycle
#define H_LOW_L_LOW     0x001C3000
//PGxIOCONbits.PMOD = 0b01 -- Independent output mode
//PGxIOCONbits.PENH = 1 -- PWM Generator x controls PWMxH output pin
//PGxIOCONbits.PENL = 1 -- PWM Generator x controls PWMxL output pin
//PGxIOCONbits.OVRENH = 1 -- PWMxH output overridden
//PGxIOCONbits.OVRENL = 1 -- PWMxL output overridden
//PGxIOCONbits.OVRDAT = 0b00 -- Override data for PWMxH/L is 0
//PGxIOCONbits.OSYNC = 0b00 -- User output overrides are synchronized to next start of cycle
#define H_LOW_L_ACTIVE  0x001C2000
//PGxIOCONbits.PMOD = 0b01 -- Independent output mode
//PGxIOCONbits.PENH = 1 -- PWM Generator x controls PWMxH output pin
//PGxIOCONbits.PENL = 1 -- PWM Generator x controls PWMxL output pin
//PGxIOCONbits.OVRENH = 1 -- PWMxH output overridden
//PGxIOCONbits.OVRENL = 0 -- PWMxL output not overridden
//PGxIOCONbits.OVRDAT = 0b00 -- Override data for PWMxH/L (only H uses it) is 0
//PGxIOCONbits.OSYNC = 0b00 -- User output overrides are synchronized to next start of cycle

uint32_t PWM1State[6] = {H_ACTIVE_L_LOW, H_LOW_L_LOW, H_LOW_L_ACTIVE, H_LOW_L_ACTIVE, H_LOW_L_LOW, H_ACTIVE_L_LOW};
uint32_t PWM2State[6] = {H_LOW_L_LOW, H_ACTIVE_L_LOW, H_ACTIVE_L_LOW, H_LOW_L_LOW, H_LOW_L_ACTIVE, H_LOW_L_ACTIVE};
uint32_t PWM3State[6] = {H_LOW_L_ACTIVE, H_LOW_L_ACTIVE, H_LOW_L_LOW, H_ACTIVE_L_LOW, H_ACTIVE_L_LOW, H_LOW_L_LOW};

//Sector 1: 
//PWM1H is active, PWM1L is overridden low.
//PWM2H is overridden low, PWM2L is overridden low.
//PWM3H is overridden low, PWM3L is active.

//Sector 2:
//PWM1H is overridden low, PWM1L is overridden low.
//PWM2H is active, PWM2L is overridden low.
//PWM3H is overridden low, PWM3L is active.

//Sector 3:
//PWM1H is overridden low, PWM1L is active.
//PWM2H is active, PWM2L is overridden low.
//PWM3H is overridden low, PWM3L is overridden low.

//Sector 4:
//PWM1H is overridden low, PWM1L is active.
//PWM2H is overridden low, PWM2L is overridden low.
//PWM3H is active, PWM3L is overridden low.

//Sector 5:
//PWM1H is overridden low, PWM1L is overridden low.
//PWM2H is overridden low, PWM2L is active.
//PWM3H is active, PWM3L is overridden low.

//Sector 6:
//PWM1H is active, PWM1L is overridden low.
//PWM2H is overridden low, PWM2L is active.
//PWM3H is overridden low, PWM3L is overridden low.


void PWMInitialization(void)
{
    //Set PWM master clock to 400MHz from PLL2 through CLKGEN5
    configure_PLL2_Fout_400MHz();
    clock_PWM_from_PLL2_Fout();
    
    //Ensure PWM generators are disabled before initializing
    PG1CONbits.ON = 0;
    PG2CONbits.ON = 0;
    PG3CONbits.ON = 0;
    
    //Set PWM Master Period (frequency 100kHz given a 400MHz master clock)
    MPER = (4000 << 4); //4000 master clocks; time scale units are 1/16 of a clock
    
    //Set Master Duty Cycle - 25%
    MDC = (2000 << 4);
    
    //Set Phase shift - No phase shift
    MPHASE = 0;    
    
    //Configure PWM Generator 1
    
    PG1CONbits.MDCSEL = 1;   //Select MDC as PWM Generator 1's duty cycle register
    PG1CONbits.MPERSEL = 1;  //Select MPER as PWM Generator 1's period register
    PG1CONbits.MPHSEL = 1;   //Select MPHASE as PWM Generator 1's phase register
    PG1CONbits.MSTEN = 1;    //PWM Generator 1 broadcasts software set of UPDREQ control bit and EOC signal to other PWM Generators
    
    
    PG1CONbits.UPDMOD = 0b000;  //PWM buffer update mode is at start of next PWM cycle if UPDREQ = 1
    PG1CONbits.TRGMOD = 0b00;  //PWM generator 1 operates in single trigger mode
    PG1CONbits.SOCS = 0b0000;    //Start of cycle is local EOC
    
        
    PG1CONbits.ON = 0;          //PWM Generator 1 is disabled (do not start yet)
    PG1CONbits.TRGCNT = 0;      //PWM Generator 1 produces 1 PWM cycle when triggered
    PG1CONbits.CLKSEL = 0b01;   //PWM Generator 1 uses PWM Master Clock, undivided and unscaled
    PG1CONbits.MODSEL = 0b000;  //PWM Generator 1 operates in Independent Edge PWM mode
    
    PG1IOCONbits.PMOD = 0b01; //PWM Generator 1 Output Mode is Independent Mode
    PG1IOCONbits.PENH = 1;    //PWM Generator 1 controls the PWM1H output pin
    PG1IOCONbits.PENL = 1;    //PWM Generator 1 controls the PWM1L output pin
    
    //Override is enabled on PWMxH/L with OVRDAT = 0b00, turning OFF PWM outputs
    PG1IOCONbits.OVRENH = 1;
    PG1IOCONbits.OVRENL = 1;
    PG1IOCONbits.OVRDAT = 0b00;
    PG1IOCONbits.OSYNC = 0b00; //User output overrides are synchronized to next start of cycle
    
    //PG1EVTbits.ADTR1EN1 = 1; //PGxTRIGA register compare event is enabled as trigger source for ADC Trigger 1 (why though? Is this used for anything?)
    PG1EVTbits.UPDTRG = 0b01; //A write of the PGxDC register automatically sets the UPDREQ bit
    PG1EVTbits.PGTRGSEL = 0b000; //PWM generator trigger output is EOC
    
    //Configure PWM Generator 2
    
    PG2CONbits.MDCSEL = 1;  //Select MDC as PWM Generator 2's duty cycle register
    PG2CONbits.MPERSEL = 1; //Select MPER as PWM Generator 2's period register
    PG2CONbits.MPHSEL = 1;  //Select MPHASE as PWM Generator 2's phase register
    
    PG2CONbits.MSTEN = 0;       //PWM generator 2 does not broadcast UPDATE status bit or EOC signal to other PWM generators
    PG2CONbits.UPDMOD = 0b011;  //PWM Buffer Update Mode is client immediate (elaborate?)
    PG2CONbits.TRGMOD = 0b00;   //PWM generator operates in Single Trigger Mode
    PG2CONbits.SOCS = 0b0001;   //Start of Cycle is PG1 trigger output selected by PG1EVTbits.PGTRGSEL<2:0> bits 
    
    PG2CONbits.TRGCNT = 0;      //PWM Generator 2 produces 1 PWM cycle when triggered
    PG2CONbits.CLKSEL = 0b01;   //PWM Generator 2 uses PWM Master Clock, undivided and unscaled
    PG2CONbits.MODSEL = 0b000;  //PWM Generator 2 operates in Independent Edge PWM mode
    
    PG2IOCONbits.PMOD = 0b01; //PWM Generator 2 Output Mode is Independent Mode
    PG2IOCONbits.PENH = 1;    //PWM Generator 2 controls the PWM2H output pin
    PG2IOCONbits.PENL = 1;    //PWM Generator 2 controls the PWM2L output pin
    
    //Override is enabled on PWM2H/L with OVRDAT = 0b00, turning OFF PWM outputs
    PG2IOCONbits.OVRENH = 1; 
    PG2IOCONbits.OVRENL = 1;
    PG2IOCONbits.OVRDAT = 0b00;
    PG2IOCONbits.OSYNC = 0b00;  //User output overrides are synchronized to next start of cycle
    
    //Set up PWM Generator 3
        
    PG3CONbits.MDCSEL = 1;  //Select MDC as PWM Generator 3's duty cycle register
    PG3CONbits.MPERSEL = 1; //Select MPER as PWM Generator 3's period register
    PG3CONbits.MPHSEL = 1;  //Select MPHASE as PWM Generator 3's phase register
    
    PG3CONbits.MSTEN = 0;   //PWM generator 3 does not broadcast UPDATE status bit or EOC signal to other PWM generators
    PG3CONbits.UPDMOD = 0b011; //PWM Buffer Update Mode is client immediate
    PG3CONbits.TRGMOD = 0b00;  //PWM generator operates in Single Trigger Mode
    PG3CONbits.SOCS = 0b0001;  //Start of Cycle is PG1 trigger output selected by PG1EVTbits.PGTRGSEL<2:0> bits 
    
    PG3CONbits.TRGCNT = 0;      //PWM Generator 3 produces 1 PWM cycle when triggered
    PG3CONbits.CLKSEL = 0b01;      //PWM Generator 3 uses PWM Master Clock, undivided and unscaled
    PG3CONbits.MODSEL = 0b000;  //PWM Generator 3 operates in Independent Edge PWM mode
    
    PG3IOCONbits.PMOD = 0b01; //PWM Generator 3 Output Mode is Independent Mode
    PG3IOCONbits.PENH = 1;    //PWM Generator 3 controls the PWM3H output pin
    PG3IOCONbits.PENL = 1;    //PWM Generator 3 controls the PWM3L output pin
    
    //Override is enabled on PWM3H/L with OVRDAT = 0b00, turning OFF PWM outputs 
    PG3IOCONbits.OVRENH = 1;
    PG3IOCONbits.OVRENL = 1;
    PG3IOCONbits.OVRDAT = 0b00;
    PG3IOCONbits.OSYNC = 0b00;  //User output overrides are synchronized to next start of cycle
    
    //Enable PWM Generator 2
    PG2CONbits.ON = 1; 
    //Enable PWM Generator 3
    PG3CONbits.ON = 1;
    //Enable PWM generator 1, starting all PWM generators together
    PG1CONbits.ON = 1;
    
}

void Delay() {
    __delay_us(50); //Delay 5 PWM cycles
}

int main()
{
    
    PWMInitialization();
    
    while(1)
    {
        for(state = 0;state < 6;state++)
        {
            Delay();
            
            PG1IOCON = PWM1State[state];
            PG2IOCON = PWM2State[state];
            PG3IOCON = PWM3State[state];
            
        }

    }
}


#endif


#ifdef PWM_SIX_STEP_PWM_SCHEME_2_EXAMPLE

//#include<stdint.h>
//For delay function
#define FCY 8000000     //CPU frequency in Hz
#include "libpic30.h"


void PWMInitialization(void);

#define H_ACTIVE_L_ACTIVE  0x000C0000
//PGxIOCONbits.PMOD = 0b00 -- Complementary output mode
//PGxIOCONbits.PENH = 1 -- PWM generator x controls PWMxH pin
//PGxIOCONbits.PENL = 1 -- PWM generator x controls PWMxL pin
//PGxIOCONbits.OVRENH = 0 -- Override not enabled on PWMxH pin
//PGxIOCONbits.OVRENL = 0 -- Override not enabled on PWMxL pin
#define H_LOW_L_HIGH         0x000C3400
//PGxIOCONbits.PMOD = 0b00 -- Complementary output mode
//PGxIOCONbits.PENH = 1 -- PWM generator x controls PWMxH pin
//PGxIOCONbits.PENL = 1 -- PWM generator x controls PWMxL pin
//PGxIOCONbits.OVRENH = 1 -- Override enabled on PWMxH pin
//PGxIOCONbits.OVRENL = 1 -- Override enabled on PWMxL pin
//PGxIOCONbits.OVRDAT = 0b01 -- PWMxH pin overridden high, PWMxL pin overridden low
#define H_LOW_L_LOW             0x000C3000
//PGxIOCONbits.PMOD = 0b00 -- Complementary output mode
//PGxIOCONbits.PENH = 1 -- PWM generator x controls PWMxH pin
//PGxIOCONbits.PENL = 1 -- PWM generator x controls PWMxL pin
//PGxIOCONbits.OVRENH = 1 -- Override enabled on PWMxH pin
//PGxIOCONbits.OVRENL = 1 -- Override enabled on PWMxL pin
//PGxIOCONbits.OVRDAT = 0b00 -- PWMxH pin overridden low, PWMxL pin overridden low

uint16_t state = 0;

uint32_t PWM1State[6] = {H_ACTIVE_L_ACTIVE, H_LOW_L_LOW, H_LOW_L_HIGH, H_LOW_L_HIGH, H_LOW_L_LOW, H_ACTIVE_L_ACTIVE};
uint32_t PWM2State[6] = {H_LOW_L_LOW, H_ACTIVE_L_ACTIVE, H_ACTIVE_L_ACTIVE, H_LOW_L_LOW, H_LOW_L_HIGH, H_LOW_L_HIGH};
uint32_t PWM3State[6] = {H_LOW_L_HIGH, H_LOW_L_HIGH, H_LOW_L_LOW, H_ACTIVE_L_ACTIVE, H_ACTIVE_L_ACTIVE, H_LOW_L_LOW};

//Sector 1:
//PWM1 has complementary outputs, both controlled by the PWM generator.
//PWM2 has both outputs overridden to low.
//PWM3 has High output overridden to low, Low output overridden to high.
//Sector 2:
//PWM1 has both outputs overridden to low.
//PWM2 has complementary outputs, both controlled by the PWM generator.
//PWM3 has High output overridden to low, Low output overridden to high.
//Sector 3:
//PWM1 has High output overridden to low, Low output overridden to high.
//PWM2 has complementary outputs, both controlled by the PWM generator.
//PWM3 has both outputs overridden to low.
//Sector 4:
//PWM1 has High output overridden to low, Low output overridden to high.
//PWM2 has both outputs overridden to low.
//PWM3 has complementary outputs, both controlled by the PWM generator.
//Sector 5: 
//PWM1 has both outputs overridden to low.
//PWM2 has High output overridden to low, Low output overridden to high.
//PWM2 has complementary outputs, both controlled by the PWM generator.
//Sector 6:
//PWM1 has complementary outputs, both controlled by the PWM generator.
//PWM2 has High output overridden to low, Low output overridden to high.
//PWM3 has both outputs overridden to low.


void Delay() {
    __delay_us(50); //Delay 5 PWM cycles
}

int main()
{
    
    
   //Initialize PWM module
    PWMInitialization();

    while(1)
    {
        for(state = 0; state < 6; state++)
        {
            //Delay is used to simulate BLDC commutation.
            //In practical application, commutation state transition will be based on feedback from Motor.
            Delay();
            
            PG1IOCON = PWM1State[state];
            PG2IOCON = PWM2State[state];
            PG3IOCON = PWM3State[state];
        }
    }
}
void PWMInitialization(void)
{
    //Ensure PWM Generators 1-3 are disabled before configuring
    PG1CONbits.ON = 0;
    PG2CONbits.ON = 0;
    PG3CONbits.ON = 0;
    
    //Set PWM master clock to 400MHz from PLL2 through CLKGEN5
    configure_PLL2_Fout_400MHz();
    clock_PWM_from_PLL2_Fout();
    
    //Set PWM Period -- 100kHz given a 400MHz PWM master clock
    MPERbits.MPER = (4000 << 4); //Time base units are 1/16 of a PWM clock
    //Set Duty Cycle-- 25%
    MDCbits.MDC = (2000 << 4);
    //Set Phase shift - No phase shift
    MPHASEbits.MPHASE = 0;
    
    PG1CONbits.MDCSEL = 1;  //Select MDC as PWM Generator 1's Duty Cycle Register
    PG1CONbits.MPERSEL = 1; //Select MPER as PWM Generator 1's Period Register
    PG1CONbits.MPHSEL = 1;  //Select MPHASE as PWM Generator 1's Phase Register
    
    //PWM Generator broadcasts software set of UPDREQ control bit and EOC signal to other PWM Generators
    PG1CONbits.MSTEN = 1; 

    PG1CONbits.UPDMOD = 0b000;    //PWM Buffer Update Mode is at start of next PWM cycle if UPDREQ = 1
    PG1CONbits.TRGMOD = 0b00;    //PWM generator operates in Single Trigger Mode
    PG1CONbits.SOCS = 0b0000; //Start of Cycle is local EOC 
    
    
    PG1CONbits.CLKSEL = 0b01; //PWM Generator uses PWM Master Clock, undivided and unscaled
    PG1CONbits.MODSEL = 0b000; //PWM Generator operates in Independent Edge PWM mode
    
    
    PG1IOCONbits.PMOD = 0b00; //PWM Generator 1 Output Mode is Complementary Mode
    PG1IOCONbits.PENH = 1; //PWM Generator 1 controls the PWM1H output pin
    PG1IOCONbits.PENL = 1; //PWM Generator 1 controls the PWM1L output pin
    //Override is enabled on PWM1H/L with OVRDAT = 0b00, turning OFF PWM outputs
    PG1IOCONbits.OVRENH = 1;
    PG1IOCONbits.OVRENL = 1;
    PG1IOCONbits.OVRDAT = 0b00;
    PG2IOCONbits.OSYNC = 0b00; //User output overrides are synchronized to next start of cycle
    
    
    //PG1EVTbits.ADTR1EN1 = 1;      // PGxTRIGA register compare event is enabled as trigger source for ADC Trigger 1 (Y tho)
    PG1EVTbits.UPDTRG = 0b01;     //A write of the duty cycle register automatically sets the UPDREQ bit
    PG1EVTbits.PGTRGSEL = 0b000;  //PWM generator 1 trigger output is EOC
    

    PG2CONbits.MDCSEL = 1;      //Select MDC as PWM Generator 2's Duty Cycle Register
    PG2CONbits.MPERSEL = 1;     //Select MPER as PWM Generator 2's Period Register
    PG2CONbits.MPHSEL = 1;      //Select MPHASE as PWM Generator 2's Phase Register
    PG2CONbits.MSTEN = 0;       //PWM generator 2 does not broadcast UPDATE status bit or EOC signal to other PWM generators
    PG2CONbits.UPDMOD = 0b011;  //PWM Buffer Update Mode is client immediate
    PG2CONbits.TRGMOD = 0b00;   //PWM generator operates in Single Trigger Mode
    PG2CONbits.SOCS = 0b0001;   //Start of Cycle is PG1 trigger output selected by PG1EVTbits.PGTRGSEL<2:0> bits
    
    PG2CONbits.CLKSEL = 0b01;    //PWM Generator 2 uses PWM Master Clock, undivided and unscaled
    PG2CONbits.MODSEL = 0b000;   //PWM Generator operates in Independent Edge PWM mode
    
    
    PG2IOCONbits.PMOD = 0b00; //PWM Generator 2 output operates in Complementary Mode
    PG2IOCONbits.PENH = 1;    //PWM Generator 2 controls the PWM2H output pin
    PG2IOCONbits.PENL = 1;    //PWM Generator 2 controls the PWM2L output pin
    
    //Override is enabled on PWMxH/L with OVRDAT = 0b00, turning OFF PWM outputs
    PG2IOCONbits.OVRDAT = 0b00;
    PG2IOCONbits.OVRENH = 1;
    PG2IOCONbits.OVRENL = 1;
    
    PG2IOCONbits.OSYNC = 0b00; //User output overrides are synchronized to next start of cycle
    
    
    PG3CONbits.MDCSEL = 1;      //Select MDC as PWM Generator 3's duty cycle register
    PG3CONbits.MPERSEL = 1;     //Select MPER as PWM Generator 3's period register
    PG3CONbits.MPHSEL = 1;      //Select MPHASE as PWM Generator 3's phase register
    PG3CONbits.MSTEN = 0;       //PWM generator 3 does not broadcast UPDATE status bit or EOC signal to other PWM generators 
    PG3CONbits.UPDMOD = 0b011;  //PWM Buffer Update Mode is client immediate
    PG3CONbits.TRGMOD = 0b00;   //PWM generator operates in Single Trigger Mode
    PG3CONbits.SOCS = 0b001;    //Start of Cycle is PG1 trigger output selected by PG1EVTbits.PGTRGSEL<2:0> bits
    
    
    PG3CONbits.CLKSEL = 0b01;   //PWM Generator 3 uses PWM Master Clock, undivided and unscaled
    PG3CONbits.MODSEL = 0b000;  //PWM Generator 3 operates in Independent Edge PWM mode
    
    PG3IOCONbits.PMOD = 0b00;   //PWM Generator 3 Output Mode is Complementary Mode
    PG3IOCONbits.PENH = 1;      //PWM Generator 3 controls the PWM3H output pin
    PG3IOCONbits.PENL = 1;      //PWM Generator 3 controls the PWMxL output pin
    
    //Override is enabled on PWMxH/L with OVRDAT = 0b00, turning OFF PWM outputs
    PG3IOCONbits.OVRENH = 1;
    PG3IOCONbits.OVRENL = 1;
    PG3IOCONbits.OVRDAT = 0b00;
    
    //User output overrides are synchronized to next start of cycle
    PG3IOCONbits.OSYNC = 0b00;
    
    
    //Enable PWM generator 2
    PG2CONbits.ON = 1;
    //Enable PWM Generator 3
    PG3CONbits.ON = 1;
    //Enable PWM generator 1; starting all PWM generators together
    PG1CONbits.ON = 1;
}


#endif


#ifdef PWM_SIX_STEP_PWM_SCHEME_3_EXAMPLE

//For delay function
#define FCY 8000000 //CPU frequency in Hz
#include "libpic30.h"

void PWMInitialization(void);

unsigned int state = 0;
unsigned int cycleCount = 0;
unsigned int cyclesPerSector = 5;

#define OUTPUTS_ACTIVE 0x000C0000
//PGxIOCONbits.PMOD = 0b00 -- Complementary output mode
//PGxIOCONbits.PENH = 1 -- PWM generator x controls PWMxH pin
//PGxIOCONbits.PENL = 1 -- PWM generator x controls PWMxL pin
//PGxIOCONbits.OVRENH = 0 -- PWMxH output is not overridden
//PGxIOCONbits.OVRENL = 0 -- PWMxL output is not overridden
//PGxIOCONbits.SWAP = 0 -- PWMxH/PWMxL signals are not swapped

#define OUTPUTS_OFF 0x000C3000
//PGxIOCONbits.PMOD = 0b00 -- Complementary output mode
//PGxIOCONbits.PENH = 1 -- PWM generator x controls PWMxH pin
//PGxIOCONbits.PENL = 1 -- PWM generator x controls PWMxL pin
//PGxIOCONbits.OVRENH = 1 -- PWMxH output is overridden
//PGxIOCONbits.OVRENL = 1 -- PWMxL output is overridden
//PGxIOCONbits.OVRDAT = 0b00 --  Override data for PWMxH/L outputs is 0
//PGxIOCONbits.SWAP = 0 -- PWMxH/PWMxL signals are not swapped

#define OUTPUTS_ACTIVE_SWAPPED 0x000C4000
//PGxIOCONbits.PMOD = 0b00 -- Complementary output mode
//PGxIOCONbits.PENH = 1 -- PWM generator x controls PWMxH pin
//PGxIOCONbits.PENL = 1 -- PWM generator x controls PWMxL pin
//PGxIOCONbits.OVRENH = 0 -- PWMxH output is not overridden
//PGxIOCONbits.OVRENL = 0 -- PWMxL output is not overridden
//PGxIOCONbits.SWAP = 1 -- PWMxH/PWMxL signals are swapped

#define OUTPUTS_OFF_SWAPPED 0x000C7000
//PGxIOCONbits.PMOD = 0b00 -- Complementary output mode
//PGxIOCONbits.PENH = 1 -- PWM generator x controls PWMxH pin
//PGxIOCONbits.PENL = 1 -- PWM generator x controls PWMxL pin
//PGxIOCONbits.OVRENH = 1 -- PWMxH output is overridden
//PGxIOCONbits.OVRENL = 1 -- PWMxL output is overridden
//PGxIOCONbits.OVRDAT = 0b00 --  Override data for PWMxH/L outputs is 0
//PGxIOCONbits.SWAP = 1 -- PWMxH/PWMxL signals are swapped

unsigned int PWM1State[6] = {OUTPUTS_ACTIVE, OUTPUTS_ACTIVE, OUTPUTS_OFF, OUTPUTS_ACTIVE_SWAPPED, OUTPUTS_ACTIVE_SWAPPED, OUTPUTS_OFF_SWAPPED};
unsigned int PWM2State[6] = {OUTPUTS_ACTIVE_SWAPPED, OUTPUTS_OFF_SWAPPED, OUTPUTS_ACTIVE, OUTPUTS_ACTIVE, OUTPUTS_OFF, OUTPUTS_ACTIVE_SWAPPED};
unsigned int PWM3State[6] = {OUTPUTS_OFF, OUTPUTS_ACTIVE_SWAPPED, OUTPUTS_ACTIVE_SWAPPED, OUTPUTS_OFF_SWAPPED, OUTPUTS_ACTIVE, OUTPUTS_ACTIVE};

//Sector 1:
//PWM1 is operating with outputs active, non-swapped.
//PWM2 is operating with outputs active, swapped.
//PWM3 has outputs overridden off.

//Sector 2:
//PWM1 is operating with outputs active, non-swapped.
//PWM2 has outputs overridden off, swapped.
//PWM3 is operating with outputs active, swapped.

//Sector 3:
//PWM1 has outputs overridden off.
//PWM2 is operating with outputs active, non-swapped.
//PWM3 is operating with outputs active, swapped.

//Sector 4:
//PWM1 is operating with outputs active, swapped.
//PWM2 is operating with outputs active, non-swapped.
//PWM3 has outputs overridden off, swapped.

//Sector 5:
//PWM1 is operating with outputs active, swapped.
//PWM2 has outputs overridden off.
//PWM3 is operating with outputs active, non-swapped.

//Sector 6:
//PWM1 has outputs overridden off, swapped.
//PWM2 is operating with outputs active, swapped.
//PWM3 is operating with outputs active, non-swapped.


int main(void) 
{

    //Initialize PWM Generators 1 - 3
    PWMInitialization();

    //Between setting PGxCONbits.ON = 1 and assigning to MDC, a delay is needed.
    //Otherwise the write will not be successful (duty cycle will remain 0)
    __delay_us(10);
    
    //To Update Duty cycle values to PG1-PG3:
    //1) Write MDC register
    //2) Set update request bit PG1STATbits.UPDREQ.
    //This will transfer MDC value to all the PWM generators PG1-PG3.
    //Note that Update Mode(UPDMOD) of PG2,PG3 is client EOC and PG1CONbits.MSTEN == 1.
    
    //Set master duty cycle to 50%
    MDC = (1000 << 4);
    //Update duty cycle, etc.
    PG1STATbits.UPDREQ = 1;
    
    //Clear variables used in the _PWM1Interrupt()
    state = 0;
    cycleCount = 0;
    
    //Enable PWM Generator 1 Interrupt
    _PWM1IE = 1;
    
    while (1) 
    { 
        
    }
    
}
void PWMInitialization(void)
{
    //Ensuring PWM Generators 1-3 are disabled prior to configuring module
    PG1CONbits.ON = 0;      
    PG2CONbits.ON = 0;
    PG3CONbits.ON = 0;

    //Set PWM master clock to 400MHz from PLL2 through CLKGEN5
    configure_PLL2_Fout_400MHz();
    clock_PWM_from_PLL2_Fout();
    
    //Set PWM frequency to 100kHz given a PWM master clock of 400MHz
    //Note that center-aligned mode uses 2 timer periods per PWM cycle.
    MPER = (2000 << 4); //Time base units are 1/16 of a clock period
    //Master duty cycle initialized as 0
    MDC = 0;
    //No master phase offset
    MPHASE = 0;

    PG1CONbits.CLKSEL = 0b01;   //PWM Generator 1 uses Master PWM clock, undivided and unscaled
    PG1CONbits.MODSEL = 0b100;  //PWM Generator 1 uses Center-Aligned PWM mode (interrupt/register update once per cycle)
    
    PG1CONbits.MDCSEL = 1;      //PWM Generator 1 uses the MDC register instead of PG1DC
    PG1CONbits.MPERSEL = 1;     //PWM Generator 1 uses the MPER register instead of PG1PER
    PG1CONbits.MPHSEL = 1;      //PWM Generator 1 uses the MPHASE register instead of PG1PHASE
    PG1CONbits.MSTEN = 1;       //PWM Generator 1 broadcasts software set of UPDREQ control bit and EOC signal to other PWM generators
    PG1CONbits.UPDMOD = 0b000;  //Update Data registers at start of next PWM cycle if UPDREQ = 1
    PG1CONbits.TRGMOD = 0b00;   //PWM Generator 1 operates in Single Trigger mode
    PG1CONbits.SOCS = 0b0000;   //Start of Cycle Selection is Local EOC
    
    
    
    PG1IOCONbits.PMOD = 0b00; //PWM Generator 1 outputs operate in Complementary mode
    PG1IOCONbits.PENH = 1;    //PWM Generator 1 controls the PWM1H output pin
    PG1IOCONbits.PENL = 1;    //PWM Generator 1 controls the PWM1L output pin
    PG1IOCONbits.POLH = 0;    //PWM1H Output Polarity is active-high
    PG1IOCONbits.POLL = 0;    //PWM1L Output Polarity is active-high
    
    //Override is enabled on PWM1H/L with OVRDAT = 0b00, turning OFF PWM outputs
    PG1IOCONbits.OVRENH = 1;
    PG1IOCONbits.OVRENL = 1;
    PG1IOCONbits.OVRDAT = 0b00;
   
    PG1IOCONbits.OSYNC = 0b00; //User output overrides via the OVRENL/H and OVRDAT<1:0> bits are synchronized to the local PWM time base (next start of cycle)
    
    
    PG1EVTbits.UPDTRG = 0b00;       //User must set the PG1STATbits.UPDREQ bit (PGxSTAT<3>) manually
    PG1EVTbits.ADTR1EN1 = 1;        //PG1TRIGA register compare event is enabled as trigger source for ADC Trigger 1 (Y tho)
    PG1EVTbits.PGTRGSEL = 0b000;    //EOC event is the PWM Generator trigger
    PG1EVTbits.IEVTSEL = 0b00;      //Interrupts CPU at EOC
    
    PG1DTbits.DTL = (80 << 4); //80 PWM clocks of dead time for PWM1L output
    PG1DTbits.DTH = (80 << 4); //80 PWM clocks of dead time for PWM1H output
    
    PG2CONbits.CLKSEL = 0b01;   //PWM Generator 2 uses Master PWM clock, undivided and unscaled
    PG2CONbits.MODSEL = 0b100;  //PWM Generator 2 uses Center-Aligned PWM mode
    
    PG2CONbits.MDCSEL = 1;      //PWM Generator 2 uses the MDC register instead of PG2DC
    PG2CONbits.MPERSEL = 1;     //PWM Generator 2 uses the MPER register instead of PG2PER 
    PG2CONbits.MPHSEL = 1;      //PWM Generator 2 uses the MPHASE register instead of PG2PHASE
    PG2CONbits.MSTEN = 0;       //PWM Generator 2 does not broadcast UPDATE status bit or EOC signal
    
    PG2CONbits.UPDMOD = 0b010;  //Update mode is client SOC
    PG2CONbits.SOCS = 0b0001;   //Start of Cycle is PG1 trigger output selected by PG1EVTbits.PGTRGSEL<2:0> bits
    
    //Override disabled on PWM2L/H
    PG2IOCONbits.OVRENH = 0;
    PG2IOCONbits.OVRENL = 0;
    
    PG2IOCONbits.PMOD = 0b00; //Complementary output mode
    
    PG2IOCONbits.PENH = 1;  //PWM Generator 2 controls PWM2H pin
    PG2IOCONbits.PENL = 1;  //PWM Generator 2 controls PWM2L pin
    PG2IOCONbits.POLH = 0;  //PWM2H output is active high
    PG2IOCONbits.POLL = 0;  //PWM2L output is active low
    
    PG2IOCONbits.SWAP = 1;  //PWM2H/PWM2L signals are swapped
            
    
    PG2DTbits.DTL = (80 << 4);   //80 PWM clocks of dead time for PWM2L output
    PG2DTbits.DTH = (80 << 4);   //80 PWM clocks of dead time for PWM1H output
    
    
    PG3CONbits.CLKSEL = 0b01;   //PWM Generator 3 uses Master PWM clock, undivided and unscaled
    PG3CONbits.MODSEL = 0b100;  //PWM Generator 3 uses Center-Aligned PWM mode
    
    PG3CONbits.MDCSEL = 1;      //PWM Generator 3 uses the MDC register instead of PG3DC
    PG3CONbits.MPERSEL = 1;     //PWM Generator 3 uses the MPER register instead of PG3PER 
    PG3CONbits.MPHSEL = 1;      //PWM Generator 3 uses the MPHASE register instead of PG3PHASE
    PG3CONbits.MSTEN = 0;       //PWM Generator 3 does not broadcast UPDATE status bit or EOC signal
    
    PG3CONbits.UPDMOD = 0b010;  //Update mode is client SOC
    PG3CONbits.SOCS = 0b0001;   //Start of Cycle is PG1 trigger output selected by PG1EVTbits.PGTRGSEL<2:0> bits
    
    //Override enabled on PWM3L/H, setting both outputs low
    PG3IOCONbits.OVRENH = 1;
    PG3IOCONbits.OVRENL = 1;
    PG3IOCONbits.OVRDAT = 0b00;
    
    PG3IOCONbits.PMOD = 0b00; //Complementary output mode
    
    PG3IOCONbits.PENH = 1;  //PWM Generator 3 controls PWM3H pin
    PG3IOCONbits.PENL = 1;  //PWM Generator 3 controls PWM3L pin 
    PG3IOCONbits.POLH = 0;  //PWM3H output is active high
    PG3IOCONbits.POLL = 0;  //PWM3L output is active high
            
            
    PG3DTbits.DTL = (80 << 4);   //80 PWM clocks of dead time for PWM3L output
    PG3DTbits.DTH = (80 << 4);   //80 PWM clocks of dead time for PWM3H output
    
    
    //Enable PWM generator 2
    PG2CONbits.ON = 1;
    //Enable PWM generator 3
    PG3CONbits.ON = 1;
    //Enable PWM generator 1 last, which will start triggering the other PWMs
    PG1CONbits.ON = 1;
}

void __attribute__((__interrupt__)) _PWM1Interrupt() {
    
    _PWM1IF = 0;
    
    //Count PWM cycles
    cycleCount += 1;
    //If sufficient cycles have passed, switch sectors/states
    if (cycleCount >= cyclesPerSector) {
        cycleCount = 0;
        state += 1;
        if (state >= 6) {
            state = 0;
        }
        PG1IOCON = PWM1State[state];        
        PG2IOCON = PWM2State[state];
        PG3IOCON = PWM3State[state];
    }
    
}

#endif


#ifdef PWM_THREE_PHASE_SINUSOIDAL_PMSM_ACIM_MOTOR_CONTROL_EXAMPLE


void PWMInitialization(void);

int main() {
    
    PWMInitialization();
    
    while(1) {
        //Anything to do here?
    }
    
    return 0;
}

void PWMInitialization(void) {
    
    //Set PWM master clock to 400MHz from PLL2 through CLKGEN5
    configure_PLL2_Fout_400MHz();
    clock_PWM_from_PLL2_Fout();
    
    //Set PWM Master Phase Register - No phase shift
    MPHASE = 0;
    
    //Set PWM Period (frequency 100kHz, given a 400MHz PWM master clock)
    //Note that center-aligned mode takes 2 timer cycles to produce 1 PWM cycle.
    MPER = (2000 << 4); //Time units are 1/16 of PWM period
    
    //Set PWM Duty Cycles
    PG1DC = (500 << 4); //25%
    PG2DC = (1000 << 4); //50%
    PG3DC = (1500 << 4); //75%
    
    //Ensure PWM Generators 1-3 are disabled before initializing
    PG1CONbits.ON = 0;
    PG2CONbits.ON = 0;
    PG3CONbits.ON = 0;
    
    //Set Dead Time Registers
    PG1DTbits.DTL = (200 << 4); //200 PWM clocks of dead time on PWM1L
    PG1DTbits.DTH = (200 << 4); //200 PWM clocks of dead time on PWM1H
    PG2DTbits.DTL = (200 << 4); //200 PWM clocks of dead time on PWM2L
    PG2DTbits.DTH = (200 << 4); //200 PWM clocks of dead time on PWM2H
    PG3DTbits.DTL = (200 << 4); //200 PWM clocks of dead time on PWM3L
    PG3DTbits.DTH = (200 << 4); //200 PWM clocks of dead time on PWM3H

    
    PG1CONbits.MDCSEL = 0;      //Select PG1DC as PWM Generator 1's duty cycle register  
    PG1CONbits.MPERSEL = 1;     //Select MPER as PWM Generator 1's period register
    PG1CONbits.MPHSEL = 1;      //Select MPHASE as PWM Generator 1's phase register
    PG1CONbits.MSTEN = 1;       //PWM Generator 1 broadcasts software set of UPDREQ control bit and EOC signal to other PWM generators.  
    PG1CONbits.UPDMOD = 0b000;  //PWM Buffer Update Mode is at start of next PWM cycle if UPDREQ = 1
    
    PG1CONbits.TRGMOD = 0b00;   //PWM generator 1 operates in Single Trigger Mode 

    PG1CONbits.CLKSEL = 1;      //PWM Generator 1 uses PWM Master Clock, undivided and unscaled
    PG1CONbits.MODSEL = 0b100;  //PWM Generator 1 operates in Center-Aligned mode
    PG1CONbits.SOCS = 0b0000;   //Start of Cycle is Local EOC
    
    PG1IOCONbits.PMOD = 0b00;   //PWM Generator 1 Output operates in Complementary Mode
    PG1IOCONbits.PENH = 1;      //PWM Generator 1 controls the PWM1H output pin
    PG1IOCONbits.PENL = 1;      //PWM Generator 1 controls the PWM1L output pin
    
    
    PG1EVTbits.ADTR1EN1 = 1;       //PGxTRIGA register compare event is enabled as trigger source for ADC Trigger 1
    PG1EVTbits.UPDTRG = 0b01;      //A write of the PGxDC register automatically sets the UPDREQ bit
    PG1EVTbits.PGTRGSEL = 0b000;   //PWM generator trigger output is EOC

    
    PG2CONbits.MDCSEL = 0;      //Select PG2DC as PWM Generator 2's duty cycle register
    PG2CONbits.MPERSEL = 1;     //Select MPER as PWM Generator 2's period register
    PG2CONbits.MPHSEL = 1;      //Select MPHASE as PWM Generator 2's phase register
    PG2CONbits.MSTEN = 0;       //PWM generator 2 does not broadcast UPDATE status bit or EOC signal to other PWM generators
    PG2CONbits.UPDMOD = 0b011;  //PWM Buffer Update Mode is client immediate
    PG2CONbits.TRGMOD = 0b00;   //PWM generator 2 operates in Single Trigger Mode
    PG2CONbits.SOCS = 0b0001;   //Start of Cycle is PG1 trigger output selected by PG1EVTbits.PGTRGSEL<2:0> bits
    
    
    PG2CONbits.CLKSEL = 1;      //PWM Generator 2 uses PWM Master Clock, undivided and unscaled
    PG2CONbits.MODSEL = 0b100;  //PWM Generator 2 operates in Center-Aligned mode
    
    
    PG2IOCONbits.PMOD = 0b00;   //PWM Generator 2 output operates in Complementary Mode
    PG2IOCONbits.PENH = 1;      //PWM Generator 2 controls the PWM2H output pin
    PG2IOCONbits.PENL = 1;      //PWM Generator 2 controls the PWM2L output pin
    
    
    PG3CONbits.MDCSEL = 0;      //Select PG3DC as PWM Generator 3's duty cycle register
    PG3CONbits.MPERSEL = 1;     //Select MPER as PWM Generator 3's period register
    PG3CONbits.MPHSEL = 1;      //Select MPHASE as PWM Generator 3's phase register
    PG3CONbits.MSTEN = 0;       //PWM generator 3 does not broadcast UPDATE status bit or EOC signal to other PWM generators

    PG3CONbits.UPDMOD = 0b011;  //PWM Buffer Update Mode is client immediate
    PG3CONbits.TRGMOD = 0b00;   //PWM generator 3 operates in Single Trigger Mode
    PG3CONbits.SOCS = 0b0001;   //Start of Cycle is PG1 trigger output selected by PG1EVTbits.PGTRGSEL<2:0> bits
    

    PG3CONbits.CLKSEL = 1;      //PWM Generator 3 uses PWM Master Clock, undivided and unscaled
    PG3CONbits.MODSEL = 0b100;  //PWM Generator operates in Center-Aligned mode
    
    PG3IOCONbits.PMOD = 0b00;   //PWM Generator 3 output operates in Complementary Mode
    PG3IOCONbits.PENH = 1;      //PWM Generator 3 controls the PWM3H output pin
    PG3IOCONbits.PENL = 1;      //PWM Generator 3 controls the PWM3L output pin
    
    
    //Enable all PWM generators; PWM generator 1 will start the sequence.
    PG3CONbits.ON = 1;
    PG2CONbits.ON = 1;
    PG1CONbits.ON = 1;
}

#endif


#ifdef PWM_COMPLEMENTARY_PWM_OUTPUT_MODE_EXAMPLE

void PWMInitialization(void);

int main() {
    
    PWMInitialization();
    
    while(1);
    
    return 0;
}

void PWMInitialization(void) {
    
    //clock_PWM_at_400MHz_from_PLL2_Fout();
    clock_PWM_from_UPB_clock();
    
    //PWM Generator 1 uses PG1DC, PG1PER, PG1PHASE registers
    PG1CONbits.MDCSEL = 0;
    PG1CONbits.MPERSEL = 0;
    PG1CONbits.MPHSEL = 0;
    
    PG1CONbits.CLKSEL = 1;      //PWM Generator 1 uses PWM Master Clock, undivided and unscaled
    PG1CONbits.MODSEL = 0b000;  //Independent edge triggered mode
    PG1CONbits.TRGMOD = 0b00;   //PWM Generator 1 operates in Single Trigger mode
    PG1CONbits.SOCS = 0b0000;   //Start of cycle (SOC) = local EOC
    
    PG1IOCONbits.PMOD = 0b00;   //PWM Generator 1 outputs operate in Complementary mode
    
    //PWM Generator controls the PWM1H and PWM1L output pins
    PG1IOCONbits.PENH = 1;      
    PG1IOCONbits.PENL = 1;
    
    //PWM1H and PWM1L output pins are active high
    PG1IOCONbits.POLH = 0;
    PG1IOCONbits.POLL = 0;
    
    //Given the 400MHz input clock from CLKGEN5, this period will result in 100kHz PWM frequency
    PG1PER = (4000 << 4);       //Time base units are 1/16 of a PWM clock
    PG1DC = (1000 << 4);        //25% duty cycle
    PG1PHASE = (400 << 4);      //Rising edge has 400 PWM clocks of phase time
    PG1DTbits.DTH = (80 << 4);  //80 PWM clocks of dead time on PWM1H
    PG1DTbits.DTL = (80 << 4);  //80 PWM clocks of dead time on PWM1L
    
    //Enable PWM Generator 1
    PG1CONbits.ON = 1;

}
#endif


#ifdef PWM_CYCLE_BY_CYCLE_CURRENT_LIMIT_MODE_EXAMPLE

void PWMInitialization(void);
void enable_CMP1();

int main() {
    
    PWMInitialization();
    
    //The CMP1A input will be compared against the DAC1 output to create the CMP1 out signal.
    //If CMP1A > DAC output, PWM1 output will be overridden
    //If CMP1A < DAC output, PWM1 ourput will be active
    
    while(1);
    
    return 0;
}

void PWMInitialization(void) {
    
    configure_PLL2_Fout_200MHz_and_VCODIV_500_MHz();
    clock_PWM_from_PLL2_Fout();
    initialize_CMP1_and_clock_from_PLL2_VCODIV();
    
    PG1CONbits.CLKSEL = 1;      //PWM generator 1 uses the PWM master clock, undivided and unscaled
    PG1CONbits.MODSEL = 0b000;  //PWM generator 1 uses independent edge PWM mode
    PG1CONbits.TRGMOD = 0b00;   //PWM generator 1 uses single trigger mode
    PG1CONbits.UPDMOD = 0b000;  //Update data registers at SOC
    
    //PWM Generator 1 uses PG1DC, PG1PER, PG1PHASE registers
    PG1CONbits.MDCSEL = 0;
    PG1CONbits.MPERSEL = 0;
    PG1CONbits.MPHSEL = 0;
    
    PG1CONbits.MSTEN = 0;       //PWM Generator does not broadcast UPDATE status bit state or EOC signal
    PG1CONbits.SOCS = 0b0000;   //Start of cycle (SOC) = local EOC
       
    PG1IOCONbits.PMOD = 0b00;   //PWM Generator 1 outputs operate in Complementary mode
    
    //PWM Generator 1 controls the PWM1H and PWM1L output pins
    PG1IOCONbits.PENH = 1;
    PG1IOCONbits.PENL = 1;
    //PWM1H and PWM1L output pins are active high
    PG1IOCONbits.POLH = 0;
    PG1IOCONbits.POLL = 0;
    //Current limit data: 1 on PWM1L and 0 on PWM1H
    PG1IOCONbits.CLDAT = 0b01;

    
    //Given the 200MHz PWM clock, this period will result in a PWM frequency of 100kHz
    PG1PER = (2000 << 4);           //Time units are 1/16 of a PWM clock
    PG1DC = (1000 << 4);            //50% duty cycle
    PG1PHASE = (200 << 4);          //200 PWM clocks of phase offset in rising edge of PWM
    PG1DTbits.DTH = (40 << 4);      //40 PWM clocks of dead time on PWM1H
    PG1DTbits.DTL = (40 << 4);      //40 PWM clocks of dead time on PWM1L
    PG1LEBbits.PHR = 1;             //Rising edge of PWM1H will trigger the LEB counter
    PG1LEBbits.LEB = (100 << 4);    //100 PWM clocks of LEB
    
    //PCI logic configuration for current limit cycle by cycle mode, comparator 1 output as PCI source
    PG1CLPCIbits.TERM = 0b001;      //Terminate when PCI source transitions from active to inactive
    PG1CLPCIbits.TSYNCDIS = 0;      //Termination of latched PCI delays till PWM EOC (for Cycle by cycle mode)
    PG1CLPCIbits.AQSS = 0b010;      //LEB active is selected as acceptance qualifier
    PG1CLPCIbits.AQPS = 1;          //LEB active is inverted to accept PCI signal when LEB duration is over
    PG1CLPCIbits.PSYNC = 0;         //PCI source is not synchronized to PWM EOC so that current limit resets PWM immediately
    PG1CLPCIbits.PSS = 0b11011;     //Comparator 1 output is selected as PCI source signal
    PG1CLPCIbits.PPS = 0;           //PCI source signal is not inverted
    PG1CLPCIbits.ACP = 0b011;       //latched PCI is selected as acceptance criteria to work when CMP1 out is active
    PG1CLPCIbits.TQSS = 0b0000;     //No termination qualifier used so terminator will work straight away without any qualifier
    
    //Enable PWM generator 1
    PG1CONbits.ON = 1;
}

#endif


#ifdef PWM_EXTERNAL_PERIOD_RESET_MODE_EXAMPLE

void PWMInitialization(void);

int main() {
    
    PWMInitialization();

    while(1);
    
    return 0;
}

void PWMInitialization(void) {
    
    configure_PLL2_Fout_200MHz_and_VCODIV_500_MHz();
    clock_PWM_from_PLL2_Fout();
    initialize_CMP1_and_clock_from_PLL2_VCODIV();

    
    //PWM generator 1 uses PG1DC, PG1PER, PG1PHASE registers
    PG1CONbits.MDCSEL = 0;
    PG1CONbits.MPERSEL = 0;
    PG1CONbits.MPHSEL = 0;
    
    PG1CONbits.MSTEN = 0;       //PWM Generator does not broadcast UPDATE status bit state or EOC signal
    PG1CONbits.TRGMOD = 0b01;   //PWM Generator operates in Re-Triggerable mode
    PG1CONbits.SOCS = 0b0000;   //Start of cycle (SOC) = local EOC is OR'd with PCI sync
    PG1CONbits.UPDMOD = 0b000;  //Update the data registers at start of next PWM cycle (SOC)
    
    PG1CONbits.MODSEL = 0b000;  //Independent edge triggered mode
    PG1CONbits.CLKSEL = 1;      //PWM Generator 1 uses PWM Master Clock, undivided and unscaled
    
    PG1IOCONbits.PMOD = 0b00;   //PWM Generator outputs operate in Complementary mode
    
    //PWM Generator controls the PWM1H and PWM1L output pins
    PG1IOCONbits.PENH = 1;     
    PG1IOCONbits.PENL = 1;
    
    //PWM1H and PWM1L output pins are active high
    PG1IOCONbits.POLH = 0;
    PG1IOCONbits.POLL = 0;
    
    //For a 200MHz PWM clock, this will result in 100kHz PWM frequency
    PG1PER = (2000 << 4);  //Time units are 1/16 of a PWM clock
    PG1DC = (500 << 4);    //25% duty cycle
    PG1PHASE = 0;          //No Phase offset in rising edge of PWM
    
    PG1DTbits.DTH = (40 << 4);  //40 PWM clocks of dead time on PWM1H
    PG1DTbits.DTL = (40 << 4);  //40 PWM clocks of dead time on PWM1L
    PG1LEBbits.PHR = 1;         //Rising edge of PWM1H will trigger the LEB counter
    PG1LEBbits.LEB = (80 << 4); //80 PWM clocks of LEB

    //PCI logic configuration for current reset (PCI sync mode), 
    //comparator 1 output (current reset signal) as PCI source,
    //and PWM1H falling edge as acceptance qualifier
    
    PG1EVTbits.PWMPCI = 0b000; //PWM Generator #1 output used as PCI signal
    
    PG1SPCIbits.TERM = 0b001;   //Terminate when PCI source transitions from active to inactive
    PG1SPCIbits.TSYNCDIS = 1;   //Termination of latched PCI occurs immediately
    PG1SPCIbits.AQSS = 0b100;   //Inverted PWM1H is selected as acceptance qualifier because PWM should be reset in OFF time
    PG1SPCIbits.AQPS = 1;       //Acceptance qualifier inverted to accept PCI signal when PWM1H on time is over
    PG1SPCIbits.PSYNC = 0;      //PCI source is not synchronized to PWM EOC so that current limit resets PWM immediately
    PG1SPCIbits.PSS = 0b11011;  //CMP1 out is selected as PCI source signal
    PG1SPCIbits.PPS = 1;        //PCI source signal is inverted
    PG1SPCIbits.ACP = 0b011;    //Latched PCI is selected as acceptance criteria to work when CMP1 out is active
    PG1SPCIbits.TQSS = 0b000;   //No termination qualifier used so terminator will work straight away without any qualifier
    
    
    //Enable PWM generator 1
    PG1CONbits.ON = 1;

}

#endif