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
 * File:   oscillator.c
 * Author: C68555
 *
 * Created on May 22, 2023, 2:42 PM
 */


#include "xc.h"
#include "examples.h"

void clock_CPU_for_200MIPS_from_PLL1() {
    
    PLL1CONbits.ON = 1; //Enable PLL generator 1, if not already enabled
    
    //Select FRC as PLL1's clock source
    PLL1CONbits.NOSC = 1;
    //Request PLL1 clock switch
    PLL1CONbits.OSWEN = 1;
    //Wait for PLL1 clock switch to complete
    while(PLL1CONbits.OSWEN);
    
    //Set up PLL1 dividers to output 200MHz
    PLL1DIVbits.PLLPRE = 1; //Reference input will be 8MHz, no division
    PLL1DIVbits.PLLFBDIV = 125; //Fvco = 8MHz * 125 = 1000MHz
    PLL1DIVbits.POSTDIV1 = 5; //Divide Fcvo by 5
    PLL1DIVbits.POSTDIV2 = 1; //Fpllo = Fvco / 5 / 1 = 200 MHz

    //The PLLSWEN bit controls changes to the PLL feedback divider.
    //Request PLL1 feedback divider switch
    PLL1CONbits.PLLSWEN = 1;
    //Wait for PLL1 feedback divider switch to complete
    while(PLL1CONbits.PLLSWEN);

    //The FOUTSWEN bit controls changes to the PLL output dividers.
    //Request PLL1 output divider switch
    PLL1CONbits.FOUTSWEN = 1;
    //Wait for PLL1 output divider switch to complete
    while(PLL1CONbits.FOUTSWEN);

    //Reset CLKGEN1 fractional divider for 1:1 ratio
    CLK1DIVbits.INTDIV = 0;
    CLK1DIVbits.FRACDIV = 0;
    //Request CLKGEN1 fractional divider switch
    CLK1CONbits.DIVSWEN = 1;
    //Wait for CLKGEN1 fractional divider switch to complete
    while(CLK1CONbits.DIVSWEN);

    //Set PLL1 Fout as new CLKGEN1 clock source
    CLK1CONbits.NOSC = 5;
    //Request CLKGEN1 clock switch
    CLK1CONbits.OSWEN = 1; 
    //Wait for CLKGEN1 clock switch to complete
    while (CLK1CONbits.OSWEN);
}

void clock_CPU_from_POSC() {
    
    //Reset CLKGEN1 fractional divider for 1:1 ratio
    CLK1DIVbits.INTDIV = 0;
    CLK1DIVbits.FRACDIV = 0;
    //Request CLKGEN1 fractional divider switch
    CLK1CONbits.DIVSWEN = 1;
    //Wait for CLKGEN1 fractional divider switch to complete
    while(CLK1CONbits.DIVSWEN);

    //Select POSC as new CLKGEN1 clock source
    CLK1CONbits.NOSC = 3;
    //Request CLKGEN1 clock switch
    CLK1CONbits.OSWEN = 1; 
    //Wait for CLKGEN1 clock switch to complete
    while (CLK1CONbits.OSWEN);
}

void clock_CPU_from_FRC() {
    
    //Reset CLKGEN1's dividers to 1:1 ratio
    CLK1DIVbits.INTDIV = 0; //Divide by 1
    CLK1DIVbits.FRACDIV = 0; //Reset fractional part
    //Request CLKGEN1 divider switch
    CLK1CONbits.DIVSWEN = 1;
    //Wait for CLKGEN1 divider switch to complete
    while(CLK1CONbits.DIVSWEN);
    
    //Select FRC as CLKGEN1's new clock source
    CLK1CONbits.NOSC = 1;
    //Request CLKGEN1 clock source switch
    CLK1CONbits.OSWEN = 1;
    //Wait for CLKGEN1 clock source switch to complete
    while(CLK1CONbits.OSWEN);
}

void configure_REFO1_to_output_PLL1_fout_div_10k() {
    
    //REFO1 will output the PLL1 frequency divided by 10,000 on pin RD9.
    
    //Map REFO1 to pin RD9
    _TRISD9 = 0; //RD9 is output
    _RP58R = 22; //RP58 (RD9) is mapped to REFO1 output function
    
    //Enable CLKGEN12, if not already enabled
    CLK12CONbits.ON = 1;
    
    //Set CLKGEN12 divider to divide by 10000
    CLK12DIVbits.INTDIV = 5000;
    //Request clock divider switch for CLKGEN12
    CLK12CONbits.DIVSWEN = 1;
    //Wait for divider switch to complete
    while (CLK12CONbits.DIVSWEN);
    
    //Select PLL1 Fout as CLKGEN12 source
    CLK12CONbits.NOSC = 5;
    //Request source switch for CLKGEN12
    CLK12CONbits.OSWEN = 1;
    //Wait for source switch to complete
    while (CLK12CONbits.OSWEN);
    
    //Enable external output of CLKGEN12 clock
    CLK12CONbits.OE = 1;
}

void configure_PLL2_VCODIV_for_300MHz() {
        
    PLL2CONbits.ON = 1; //Enable PLL generator 2, if not already enabled
    
    //Select FRC as PLL2's clock source
    PLL2CONbits.NOSC = 1;
    //Request PLL2 clock switch
    PLL2CONbits.OSWEN = 1;
    //Wait for PLL2 clock switch to complete
    while(PLL2CONbits.OSWEN);
    
    PLL2DIVbits.PLLPRE = 1; //Reference input will be 8MHz, no division
    PLL2DIVbits.PLLFBDIV = 150; //Fvco = 8MHz * 150 = 1200MHz
    PLL2DIVbits.POSTDIV1 = 4; //Divide Fcvo by 4
    PLL2DIVbits.POSTDIV2 = 1; //Fpllo = Fvco / 4 / 1 = 300 MHz

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
    
    VCO2DIVbits.INTDIV = 2; //Divided VCO clock is 1200MHz / 4 = 300MHz
    //Request VCO divider switch
    PLL2CONbits.DIVSWEN = 1;
    //Wait for VCO divider switch to complete
    while(PLL2CONbits.DIVSWEN);
}

void configure_REFO1_to_output_PLL2_VCODIV_output_div_10k() {
    
    //Map REFO1 to pin RD9
    _TRISD9 = 0; //RD9 is output
    _RP58R = 22; //RP58 (RD9) is mapped to REFO1 output function
    
    //Enable CLKGEN12, if not already enabled
    CLK12CONbits.ON = 1;
    
    //Set CLKGEN12 divider to divide by 10000
    CLK12DIVbits.INTDIV = 5000;
    //Request clock divider switch for CLKGEN12
    CLK12CONbits.DIVSWEN = 1;
    //Wait for divider switch to complete
    while (CLK12CONbits.DIVSWEN);
    
    //Select PLL2 VCO divider output as CLKGEN12 source
    CLK12CONbits.NOSC = 8;
    //Request source switch for CLKGEN12
    CLK12CONbits.OSWEN = 1;
    //Wait for source switch to complete
    while (CLK12CONbits.OSWEN);
    
    //Enable external output of CLKGEN12 clock
    CLK12CONbits.OE = 1;
}

void enable_POSC_for_8MHz_crystal() {    
    
    OSCCFGbits.POSCIOFNC = 0; //Disable CLKO output on RC0
    
    OSCCFGbits.PXTALCFG = 0b000; //Gain setting G0, for 8MHz crystal, don't boost kick start
    OSCCFGbits.POSCMD = 0b01; //MS Oscillator mode, for 8MHz crystal; oscillator mode selected
    
    OSCCTRLbits.POSCEN = 1; //Enable POSC
    while(!OSCCTRLbits.POSCRDY); //Wait for POSC to be ready (maybe)
}

void enable_POSC_EC_mode() {
    
    OSCCFGbits.POSCMD = 0b00; //External clock mode selected
    
    OSCCTRLbits.POSCEN = 1; //Enable POSC
    while(!OSCCTRLbits.POSCRDY); //Wait for POSC to be ready (maybe)
}

void configure_REFO1_to_output_POSC_div_1() {
    
    //Map REFO1 to pin RD9
    _TRISD9 = 0; //RD9 is output
    _RP58R = 22; //RP58 (RD9) is mapped to REFO1 output function
    
    //Enable CLKGEN12, if not already enabled
    CLK12CONbits.ON = 1;
    
    //Set CLKGEN12 divider to divide by 1
    CLK12DIVbits.INTDIV = 0;
    CLK12DIVbits.FRACDIV = 0;
    //Request clock divider switch for CLKGEN12
    CLK12CONbits.DIVSWEN = 1;
    //Wait for divider switch to complete
    while (CLK12CONbits.DIVSWEN);
    
    //Select POSC as CLKGEN12 source
    CLK12CONbits.NOSC = 3;
    //Request source switch for CLKGEN12
    CLK12CONbits.OSWEN = 1;
    //Wait for source switch to complete
    while (CLK12CONbits.OSWEN);
    
    //Enable external output of CLKGEN12 clock
    CLK12CONbits.OE = 1;
}

void enable_CLKO() {
    OSCCFGbits.POSCIOFNC = 1; //Enable CLKO output on RC0 (POSC disabled or EC mode)
}

void disable_CLKO() {
    OSCCFGbits.POSCIOFNC = 0; //Disable CLKO output on RC0 (POSC disabled or EC mode)
}

void setup_REFI1_and_setup_REFO2_to_output_REFI1() {
    
    //Map REFI1 to pin RD0
    _TRISD0 = 1;  //Set RD0 as input
    _REFI1R = 49; //Map REFI1 function to RD0 (RP49)
    
    //Map REFO2 to pin RD10
    _TRISD10 = 0; //RD9 is output
    _RP59R = 23; //RP58 (RD9) is mapped to REFO1 output function
    
    //Enable CLKGEN13, if not already enabled
    CLK13CONbits.ON = 1;
    
    //Set CLKGEN13 divider to divide by 1
    CLK13DIVbits.INTDIV = 0;
    CLK13DIVbits.FRACDIV = 0;
    //Request clock divider switch for CLKGEN13
    CLK13CONbits.DIVSWEN = 1;
    //Wait for divider switch to complete
    while (CLK13CONbits.DIVSWEN);
    
    //Select REFI1 as CLKGEN13 source
    CLK13CONbits.NOSC = 9;
    //Request source switch for CLKGEN13
    CLK13CONbits.OSWEN = 1;
    //Wait for source switch to complete
    while (CLK13CONbits.OSWEN);
    
    //Enable external output of CLKGEN13 clock
    CLK13CONbits.OE = 1;
}
