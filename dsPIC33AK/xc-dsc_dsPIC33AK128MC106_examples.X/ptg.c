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
 * File:   ptg.c
 * Author: C68555
 *
 * Created on August 23, 2023, 10:41 AM
 */


#include "xc.h"
#include "ptg.h" //Contains Examples 2-1, 2-2, and 2-3
#include "examples.h"

#ifdef PTG_SINGLE_STEP_DEMONSTRATION_PROGRAM

//This code shows a basic example of using PTG Single-Step Mode.
//Program must be run in Debug Mode.
//Once the PTGSSEN bit is set, press the push button (RD5) to set PTGSTRT to execute a single command. 
//An LED (RC4) will toggle on the completion of each command.

void PTG_populate_queue() {
    //Set up commands in the PTG Step Queue
    PTGQUE0bits.STEP0 = 0; //NOP
    PTGQUE0bits.STEP1 = 0b10100000; //PTGJMP(0)
}

int main (void) {
    _TRISD5 = 1; //RD5 input connected to push button switch
    _Bool sw_latch = 0; //Latch button input so one press makes one step

    _TRISC4 = 0; //RE0 output connected to external indicator
    _LATC4 = 0; //RE0 output low initially

    PTG_populate_queue(); //Set up step commands in PTG queue
    PTGCONbits.ON = 1; //Place breakpoint on this line. When execution halts, 
    //set PTGSSEN = 1 using the debugger, then continue.

    _PTGSTEPIE = 1; //Enable PTG Step Interrupt
    while(1) {
        if (!_RD5) { //Check if button is pressed (active low)
            if (!sw_latch) { 
                _PTGSTRT = 1; //Execute a single Step command
                sw_latch = 1;
            }
        }
        else {
            sw_latch = 0; //Reset latch for next button press
        }
    }
 
    return 0;
}

void __attribute__ ((__interrupt__)) _PTGSTEPInterrupt(void) {
    _LATC4 = !_LATC4; //Toggle RC4 to show step has taken place
    _PTGSTEPIF = 0;
}


#endif


#ifdef PTG_GENERATING_PHASE_SHIFTED_WAVEFORMS

void IO_initialize() {
    _PCI12R = 167;  //Connect PCI 12 to PTG trigger 26
    _TRISD2 = 0;    //Set RD2 as output (used by PWM1H)
    _TRISD0 = 0;    //Set RD0 as output (used by PWM2H)
}

void PWM1_initialize() {
 
    PG1CONbits.CLKSEL = 1; //Main PWM clock (undivided/unscaled) used for PWM2
    PG1IOCONbits.PENH = 1; //PWM generator 1 controls PWM1H pin
    PG1EVTbits.ADTR2EN1 = 1; //PGA1TRIGA match controls PWM1 ADC Trigger 2

    PG1PER = 8000 << 4; //Period 1ms for a 8MHz PWM clock
    PG1DC = 4000 << 4; //50% duty cycle
    PG1PHASE = 0; //0 phase offset
    PG1TRIGA = 0; //PWM ADC trigger 2 will happen at start of cycle

    PG1CONbits.ON = 1; //Enable PWM Generator 1
}

void PWM2_initialize() {

    PG2CONbits.CLKSEL = 1; //Main PWM clock (undivided/unscaled) used for PWM2
    PG2CONbits.SOCS = 0b1111; //PCI sync used for start of cycle
    PG2IOCONbits.PENH = 1; //PWM generator 2 controls PWM2H pin

    PG2PER = 8000 << 4; //Period 1ms for a 8MHz PWM clock
    PG2DC = 4000 << 4; //50% duty cycle
    PG2PHASE = 0; //0 phase offset

    PG2SPCIbits.PSS = 0b01100; //PCI12 as PWM2 sync source
    PG2CONbits.ON = 1; //Enable PWM Generator 2
}
void PTG_initialize() {
    
    PTGT0LIM = 1000; //0.125ms T0 delay
    
    PTGQUE0bits.STEP0 = PTGWHI(0); //Wait for high-to-low edge on trigger from PWM1
    PTGQUE0bits.STEP1 = PTGCTRL(t0Wait); //Wait on T0
    PTGQUE0bits.STEP2 = PTGTRIG(26); //Trigger PWM2
    PTGQUE0bits.STEP3 = PTGJMP(0); //Restart sequence
    
    PTGCONbits.ON = 1; //Enable PTG
    PTGCONbits.PTGSTRT = 1; //Start executing commands
}

void clocks_initialize() {
    
    //Configure both CLKGEN5 (PWM) and CLKGEN10 (PTG) to use 8MHz clock from FRC.
    
    //Enable CLKGEN5, if not already enabled
    CLK5CONbits.ON = 1;

    //Reset CLKGEN5 fractional divider for 1:1 ratio
    CLK5DIVbits.INTDIV = 0;
    CLK5DIVbits.FRACDIV = 0;
    //Request CLKGEN10 fractional divider switch
    CLK5CONbits.DIVSWEN = 1;
    //Wait for CLKGEN10 fractional divider switch to complete
    while(CLK5CONbits.DIVSWEN);
    
    //Select FRC as CLKGEN5's new clock source
    CLK5CONbits.NOSC = 1;
    //Request CLKGEN5 clock source switch
    CLK5CONbits.OSWEN = 1;
    //Wait for CLKGEN5 clock source switch to complete
    while(CLK5CONbits.OSWEN);
    
    PCLKCONbits.MCLKSEL = 1; //Use CLKGEN5 to provide PWM MCLK
    
    //Enable CLKGEN10, if not already enabled
    CLK10CONbits.ON = 1; 
    
    //Reset CLKGEN10 fractional divider for 1:1 ratio
    CLK10DIVbits.INTDIV = 0;
    CLK10DIVbits.FRACDIV = 0;
    //Request CLKGEN10 fractional divider switch
    CLK10CONbits.DIVSWEN = 1;
    //Wait for CLKGEN10 fractional divider switch to complete
    while(CLK10CONbits.DIVSWEN);
    
    //Select FRC as CLKGEN10's new clock source
    CLK10CONbits.NOSC = 1;
    //Request CLKGEN10 clock source switch
    CLK10CONbits.OSWEN = 1;
    //Wait for CLKGEN10 clock source switch to complete
    while(CLK10CONbits.OSWEN);
        
}

int main(void) {
    
    clocks_initialize();
    IO_initialize();
    PWM1_initialize();
    PWM2_initialize();
    PTG_initialize();

    while (1);

    return 0;
}

#endif

#ifdef PTG_INTERLEAVED_SAMPLING_STEP_COMMAND_PROGRAM

//Allocate buffers for ADC results. 
//Size of 50 is based on 2 PWM cycles, 25 results per cycle.
#define RESULT_BUFFER_SIZE 50
unsigned int voltage_buffer[RESULT_BUFFER_SIZE];
int voltage_buffer_index = 0;
unsigned int current_buffer[RESULT_BUFFER_SIZE];
int current_buffer_index = 0;
_Bool readings_ready = 0;

void clocks_initialize() {
    //Set up CPU clock (Fcy) to run at 200 MHz. 

    //Start by configuring PLL1
    
    PLL1CONbits.ON = 1; //Enable PLL1, if not already enabled
    
    //PLL1 has input frequency 8MHz (FRC)
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
    
    VCO1DIVbits.INTDIV = 2; //Divide Fvco by 4
    //The DIVSWEN bit controls changes to the VCO divider.
    //Request PLL1 VCO divider switch
    PLL1CONbits.DIVSWEN = 1;
    //Wait for PLL1 VCO divider switch to complete
    while(PLL1CONbits.DIVSWEN);

    //Reset CLKGEN1 dividers for 1:1 ratio
    CLK1DIVbits.INTDIV = 0;
    CLK1DIVbits.FRACDIV = 0;
    //Request CLKGEN1 divider switch
    CLK1CONbits.DIVSWEN = 1;
    //Wait for divider switch to complete
    while(CLK1CONbits.DIVSWEN);
    
    CLK1CONbits.NOSC = 5; //Set PLL1 Fout as CPU clock source
    CLK1CONbits.OSWEN = 1; //Request clock switch
    while (CLK1CONbits.OSWEN); //Wait for switch to complete

    
    //Configure CLKGEN5 to provide a 125MHz PWM MCLK

    CLK5CONbits.ON = 1; //Enable CLKGEN5, if not already enabled
    
    CLK5DIVbits.INTDIV = 1; //Divide input clock by 2
    CLK5DIVbits.FRACDIV = 0;
    //Request CLKGEN5 divider switch
    CLK5CONbits.DIVSWEN = 1;
    //Wait for CLKGEN5 divider switch to complete
    while(CLK5CONbits.DIVSWEN);

    CLK5CONbits.NOSC = 7; //Use divided PLL1 VCO (250MHz)
    CLK5CONbits.OSWEN = 1; //Request clock switch
    while(CLK5CONbits.OSWEN); //Wait for switch to complete
    
    PCLKCONbits.MCLKSEL = 1; //Use CLKGEN5 as PWM MCLK source

    
    //Configure CLKGEN6 to provide a 250MHz input clock to the ADC

    CLK6CONbits.ON = 1; //Enable CLKGEN6, if not already enabled
    
    //Reset CLKGEN6 dividers for 1:1 ratio
    CLK6DIVbits.INTDIV = 0;
    CLK6DIVbits.FRACDIV = 0;
    //Request CLKGEN6 divider switch
    CLK6CONbits.DIVSWEN = 1;
    //Wait for CLKGEN6 divider switch to complete
    while(CLK6CONbits.DIVSWEN);
    
    CLK6CONbits.NOSC = 7; //Use divided PLL1 VCO (250MHz)
    CLK6CONbits.OSWEN = 1; //Request clock switch
    while (CLK6CONbits.OSWEN); //Wait for switch to complete

    
    //Configure CLKGEN10 to provide a 125MHz clock for the PTG.

    CLK10CONbits.ON = 1; //Enable CLKGEN10, if not already enabled
    
    CLK10DIVbits.INTDIV = 1; //Divide input clock by 2
    CLK10DIVbits.FRACDIV = 0;
    //Request CLKGEN10 divider switch
    CLK10CONbits.DIVSWEN = 1;
    //Wait for CLKGEN10 divider switch to complete
    while(CLK10CONbits.DIVSWEN);

    CLK10CONbits.NOSC = 7; //Use divided PLL1 VCO (250MHz)
    CLK10CONbits.OSWEN = 1; //Request clock switch
    while (CLK10CONbits.OSWEN); //Wait for switch to complete
}

void PTG_initialize() {
 
    //Enable PTG interrupts 0 and 1
    _PTG0IE = 1;
    _PTG1IE = 1;

    //Set up control registers
    PTGT0LIM = 625; //5us T0 delay
    PTGT1LIM = 125; //1us T1 delay
    PTGC0LIM = 24; //Repeat C0 loop 25 times
    PTGC1LIM = 1; //Repeat C1 loop once
    PTGHOLD = 625; //5us (used to restore T0 delay)
    PTGADJ = 125; //1us (added to T0 delay)
    PTGQPTR = 0; //Initialize step queue pointer

    //Outer loop
    PTGQUE0bits.STEP0 = PTGWHI(0); // Wait for positive edge trigger 0 (PWM1 ADC Trigger 2)
    PTGQUE0bits.STEP1 = PTGCTRL(t0Wait); // Start PTGT0, wait for time out
    PTGQUE0bits.STEP2 = PTGIRQ(0); // Generate IRQ 0
    // Inner loop
    PTGQUE0bits.STEP3 = PTGTRIG(12); // Generate output trigger 12 (ADC conversion)
    PTGQUE1bits.STEP4 = PTGCTRL(t1Wait); // Start PTGT1, wait for time out
    PTGQUE1bits.STEP5 = PTGJMPC0(3); // Go to STEP3 if PTGC0 != PTGC0LIM, increment PTGC0 (ie. repeat steps 3-5 24 times)
    // End inner loop
    PTGQUE1bits.STEP6 = PTGADD(t0Limit); // Add PTGADJ to PTGT0LIM
    PTGQUE1bits.STEP7 = PTGJMPC1(0); // Jump to 0 PTGC1LIM times (once, making 2 iterations)
    // End outer loop

    PTGQUE2bits.STEP8 = PTGIRQ(1); // Generate IRQ 1
    PTGQUE2bits.STEP9 = PTGCOPY(t0Limit); // Copy PTGHOLD to PTGT0LIM (restore original value)
    PTGQUE2bits.STEP10 = PTGJMP(0); // Jump to start of queue

    //Start PTG
    PTGCONbits.ON = 1;
    PTGCONbits.PTGSTRT = 1;
}

void PWM1_initialize() {
    _TRISD2 = 0; //Set PWM1H pin as output to observe cycle

    PG1CONbits.CLKSEL = 1; // //Main PWM clock (no dividing or scaling) used for PWM1
    PG1IOCONbits.PENH = 1; // PWM generator 1 controls PWM1H pin (RD2)
    PG1EVTbits.ADTR2EN1 = 1; // Enable PG1TRIGA match as PWM1 ADC Trigger 2 source

    PG1PER = 6250 << 4; //Period of 50us
    PG1PHASE = 0; //0 phase offset
    PG1DC = 3125 << 4; //50% duty cycle
    PG1TRIGA = 0x0000; // ADC trigger 2 (PTG input) will happen at start of cycle
    
    PG1CONbits.ON = 1; // Enable PWM Generator 1
}

void ADC_initialize() {

    //Use RA2 as input to be converted by ADC (voltage reading)
    _ANSELA2 = 1;
    _TRISA2 = 1;
    //Use RA4 as input to be converted by ADC (current reading)
    _ANSELA4 = 1;
    _TRISA4 = 1;

    //Initialize ADC
    AD1CONbits.ON = 1;
    while(!AD1CONbits.ADRDY);

    //AD1AN0 input selected by data channel 0, conversion is triggered by ADC trigger 30
    AD1CHCON0bits.MODE = 0; //Single-sample mode
    AD1CHCON0bits.PINSEL = 0; //Positive input is AD1AN0/RA2
    AD1CHCON0bits.NINSEL = 0; //Single-ended mode

    AD1CHCON0bits.TRG1SRC = 30; //Channel 0 triggered by ADC trigger 30 (PTG trigger 12)

    //AD1AN1 input selected by data channel 1, conversion is triggered by ADC trigger 30
    AD1CHCON1bits.MODE = 0; //Single-sample mode
    AD1CHCON1bits.PINSEL = 1; //Positive input is AD1AN1/RA4
    AD1CHCON1bits.NINSEL = 0; //Single-ended mode

    AD1CHCON1bits.TRG1SRC = 30; //Channel 1 ADC trigger 30 (PTG trigger 12)

    //Enable ADC Channel 0 and 1 interrupts
    _AD1CH0IE = 1;
    _AD1CH1IE = 1;
}

void calculate_average_power() {
    //To do: Use the buffered ADC readings to calculate average power over the last 2 PWM cycles.
    //Calculation details are application-specific.
    
    //Below is a simplified calculation for demonstration purpose.
    float power_sum = 0;
    for (int i = 0; i < RESULT_BUFFER_SIZE; i++) {
        //Power = Current * Voltage
        power_sum += ((float)voltage_buffer[i] * (float)current_buffer[i]);
    }
    //Take average
    float average_power = power_sum / RESULT_BUFFER_SIZE;
    
    //Toggle I/O indicator to show completion
    _LATD10 ^= 1;
}

int main(void) {
    
    //Set digital outputs for timing indicators
    _TRISD2 = 0;
    _TRISD5 = 0;
    _TRISD6 = 0;
    _TRISD10 = 0;
    
    clocks_initialize();
    PWM1_initialize();
    ADC_initialize();
    PTG_initialize();

    while(1) {
        //Wait for readings to be ready
        if (readings_ready) {
            //Perform power calculations and reset for next time.
            calculate_average_power();
            readings_ready = 0;
        }
    }
    return 0;
}

void __attribute__((__interrupt__)) _PTG0Interrupt() {
    _PTG0IF = 0;
    //Interrupt indicates a series of 25 ADC readings will now be collected.
}

void __attribute__((__interrupt__)) _PTG1Interrupt() {
    _PTG1IF = 0;
    //2 PWM cycles of ADC results have been collected, average power can be calculated for those 2 cycles.
    readings_ready = 1;
    //Reset buffer indices here to prevent overrun.
    voltage_buffer_index = 0;
    current_buffer_index = 0;
}

void __attribute__((__interrupt__)) _AD1CH0Interrupt() {
    unsigned int voltage_result = AD1DATA0;
    _AD1CH0IF = 0;
    //Buffer ADC result for later use
    voltage_buffer[voltage_buffer_index++] = voltage_result;
    _LATD5 ^= 1; //Toggle indicator I/O
}

void __attribute__((__interrupt__)) _AD1CH1Interrupt() {
    unsigned int current_result = AD1DATA1;
    _AD1CH1IF = 0;
    //Buffer ADC result for later use
    current_buffer[current_buffer_index++] = current_result;
    _LATD6 ^= 1; //Toggle indicator I/O
}

#endif
 
 
#ifdef PTG_RATIOED_SAMPLING_STEP_COMMAND_PROGRAM

void clocks_initialize() {
    
    //Configure CLKGEN5 to provide an 8MHz clock for the PWM
    CLK5CONbits.ON = 1; //Enable CLKGEN5, if not already enabled
    
    //Reset CLKGEN5 dividers for 1:1 ratio
    CLK5DIVbits.INTDIV = 0;
    CLK5DIVbits.FRACDIV = 0;
    CLK5CONbits.DIVSWEN = 1;
    //Wait for divider switch to complete
    while(CLK5CONbits.DIVSWEN);

    CLK5CONbits.NOSC = 1; //Select FRC, 8MHz
    CLK5CONbits.OSWEN = 1; //Request clock switch
    while (CLK5CONbits.OSWEN); //Wait for switch to complete
    
    PCLKCONbits.MCLKSEL = 1; //Use CLKGEN5 for PWM clock
    
    
    PLL1CONbits.ON = 1; //Enable PLL generator 1, if not already enabled

    //Set up PLL1
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

    VCO1DIVbits.INTDIV = 2; //Divide Fvco by 4
    //The DIVSWEN bit controls changes to the VCO divider.
    //Request PLL1 VCO divider switch
    PLL1CONbits.DIVSWEN = 1;
    //Wait for PLL1 VCO divider switch to complete
    while(PLL1CONbits.DIVSWEN);
    
    //Reset CLKGEN1 dividers for 1:1 ratio
    CLK1DIVbits.INTDIV = 0;
    CLK1DIVbits.FRACDIV = 0;
    CLK1CONbits.DIVSWEN = 1;
    //Wait for divider switch to complete
    while(CLK1CONbits.DIVSWEN);
    
    CLK1CONbits.NOSC = 1; //Set FRC as CPU clock source
    CLK1CONbits.OSWEN = 1; //Request clock switch
    while (CLK1CONbits.OSWEN); //Wait for switch to complete

    //Configure CLKGEN6 to provide a 250MHz input clock to the ADC.
    
    CLK6CONbits.ON = 1; //Enable CLKGEN6, if not already enabled
    
    //Reset CLKGEN6 dividers for 1:1 ratio
    CLK6DIVbits.INTDIV = 0;
    CLK6DIVbits.FRACDIV = 0;
    CLK6CONbits.DIVSWEN = 1;
    //Wait for divider switch to complete
    while(CLK6CONbits.DIVSWEN);
    
    CLK6CONbits.NOSC = 7; //Set PLL1 VCODIV as ADC clock source
    CLK6CONbits.OSWEN = 1; //Request clock switch
    while (CLK6CONbits.OSWEN); //Wait for switch to complete

    //Configure CLKGEN10 to provide an 8MHz clock for the PTG
    
    CLK10CONbits.ON = 1; //Enable CLKGEN10, if not already enabled
    
    //Reset CLKGEN10 dividers for 1:1 ratio
    CLK10DIVbits.INTDIV = 0;
    CLK10DIVbits.FRACDIV = 0;
    //Request CLKGEN10 divider switch
    CLK10CONbits.DIVSWEN = 1;
    //Wait for divider switch to complete
    while(CLK10CONbits.DIVSWEN);

    CLK10CONbits.NOSC = 1; //Select FRC, 8MHz
    CLK10CONbits.OSWEN = 1; //Request clock switch
    while (CLK10CONbits.OSWEN); //Wait for switch to complete
}

void PTG_initialize() {
 
    PTGT0LIM = 16000; // 2 ms T0 delay
    PTGT1LIM = 8000; // 1 ms T1 delay
    PTGC0LIM = 2; // 3 iterations of the C0 loop

    PTGQPTR = 0; //Initialize step queue pointer
    
    //Initialize Step registers
    PTGQUE0bits.STEP0 = PTGWHI(15);        // Wait for trigger from INT2 (zero crossing detect)
    PTGQUE0bits.STEP1 = PTGTRIG(12);       // Take 1/8 rate samples using ADC trigger 30
    PTGQUE0bits.STEP2 = PTGCTRL(t0Wait);   // Wait 2ms
    PTGQUE0bits.STEP3 = PTGTRIG(24);       // Trigger PPS output 55 (SCR)
    PTGQUE1bits.STEP4 = PTGCTRL(t1Wait);   // Wait1ms before starting the 1x and 1/2x triggers
    PTGQUE1bits.STEP5 = PTGTRIG(26);       // Trigger PWM1 to trigger conversions at 1x and 1/2x rates
    //Start main loop
    PTGQUE1bits.STEP6 = PTGCTRL(t0Wait);   // Wait2ms (pre-trigger delay) for subsequent triggers
    PTGQUE1bits.STEP7 = PTGTRIG(26);       // Trigger PWM1 to trigger ADC conversions at 1x and 1/2x rates
    PTGQUE2bits.STEP8 = PTGJMPC0(6);       // Jump to step 6 (twice, for 3 iterations total)
    //End main loop
    PTGQUE2bits.STEP9 = PTGJMP(0);

    //Start the PTG
    PTGCONbits.ON = 1;
    PTGCONbits.PTGSTRT = 1;
}

void ADC_initialize() {
 
    //Enable analog inputs AD1AN0 - AD1AN5
    _ANSELA2 = 1;
    _ANSELA4 = 1;

    _ANSELA6 = 1;
    _ANSELA5 = 1;
    _ANSELB1 = 1;
    _ANSELB3 = 1;

    //Enable ADC
    AD1CONbits.ON = 1;
    while(!AD1CONbits.ADRDY);

    //Assign ADC inputs to core 1 channels 0 - 5
    AD1CHCON0bits.MODE = 0; //Single-sample mode
    AD1CHCON0bits.PINSEL = 0; //Positive input is AD1AN0/RA2
    AD1CHCON0bits.NINSEL = 0; //Single-ended mode

    AD1CHCON1bits.MODE = 0; //Single-sample mode
    AD1CHCON1bits.PINSEL = 1; //Positive input is AD1AN1/RA4
    AD1CHCON1bits.NINSEL = 0; //Single-ended mode

    AD1CHCON2bits.MODE = 0; //Single-sample mode
    AD1CHCON2bits.PINSEL = 2; //Positive input is AD1AN2/RA6
    AD1CHCON2bits.NINSEL = 0; //Single-ended mode

    AD1CHCON3bits.MODE = 0; //Single-sample mode
    AD1CHCON3bits.PINSEL = 3; //Positive input is AD1AN3/RA5
    AD1CHCON3bits.NINSEL = 0; //Single-ended mode

    AD1CHCON4bits.MODE = 0; //Single-sample mode
    AD1CHCON4bits.PINSEL = 4; //Positive input is AD1AN4/RB1
    AD1CHCON4bits.NINSEL = 0; //Single-ended mode

    AD1CHCON5bits.MODE = 0; //Single-sample mode
    AD1CHCON5bits.PINSEL = 5; //Positive input is AD1AN5/RB3
    AD1CHCON5bits.NINSEL = 0; //Single-ended mode

    //Configure channel triggers
    AD1CHCON0bits.TRG1SRC = 30; //Channel 0 triggered by ADC trigger 30 (PTG trigger 12)
    AD1CHCON1bits.TRG1SRC = 30; //Channel 1 triggered by ADC trigger 30 (PTG trigger 12)
    AD1CHCON2bits.TRG1SRC = 30; //Channel 2 triggered by ADC trigger 30 (PTG trigger 12)
    AD1CHCON3bits.TRG1SRC = 30; //Channel 3 triggered by ADC trigger 30 (PTG trigger 12)
    AD1CHCON4bits.TRG1SRC = 5; //Channel 4 triggered by PWM1 ADC Trigger 2
    AD1CHCON5bits.TRG1SRC = 7; //Channel 5 triggered by PWM2 ADC Trigger 2

    //Enable interrupts for ADC core 1 channels 0 - 5
    _AD1CH0IE = 1;
    _AD1CH1IE = 1;
    _AD1CH2IE = 1;
    _AD1CH3IE = 1;
    _AD1CH4IE = 1;
    _AD1CH5IE = 1;
 
}
void PWM1_initialize() {
    
    PG1CONbits.CLKSEL = 1; //Main PWM clock (no dividing or scaling) used for PWM1
    PG1CONbits.SOCS = 0b1111; //PCI sync used for start of cycle
    PG1SPCIbits.PSS = 0b01100; //PCI12 as PWM1 sync source
    PG1CONbits.TRGMOD = 1; //PWM generator is re-triggerable
    PG1IOCONbits.PENH = 0; //PWM generator 1 does not control PWM1H pin
    PG1EVTbits.ADTR2EN1 = 1; //Enable PGA1TRIGA match as ADC trigger 2 source
    PG1EVTbits.ADTR1EN1 = 1; //Enable PGA1TRIGA match as ADC trigger 1 source
    PG1EVTbits.PGTRGSEL = 1; //Use TRIGA compare as PWM generator trigger

    PG1PER = 16000 << 4; //PWM1 period is 2ms
    PG1DC = 8000 << 4; //50% duty cycle
    PG1PHASE = 0; //0 phase offset

    PG1TRIGA = 0; //ADC trigger at 0

    PG1CONbits.ON = 1; //Enable PWM Generator 1
}

void PWM2_initialize() {
    
    PG2CONbits.CLKSEL = 1; //Main PWM clock (no dividing or scaling) used for PWM2
    PG2CONbits.TRGCNT = 1; //PWM2 completes two cycles once triggered
    PG2CONbits.TRGMOD = 1; //PWM2 is re-triggerable
    PG2CONbits.SOCS = 0b0001; //PWM start of cycle from PG1, selected by PGTRGSEL
    PG2IOCONbits.PENH = 0; //PWM generator 2 does not control PWM2H pin
    PG2EVTbits.ADTR2EN1 = 1; //Enable PGA1TRIGA match as ADC trigger 2 source
    
    PG2PER = 8000 << 4; //PWM2 period is 1ms
    PG2DC = 4000 << 4; //50% duty cycle
    PG2PHASE = 0; //0 phase offset

    PG1TRIGA = 0; //ADC trigger at 0

    PG2CONbits.ON = 1; //Enable PWM Generator 2
}

void IO_initialize() {

    //Configure INT2 to use RC2 (zero-crossing detect input)
    _TRISC2 = 1;
    _INT2R = 35;
    //Configure PTG trigger 24 to use RC3 (SCR output)
    _TRISC3 = 0;
    _RP36R = 55; 
    //Connect PCI12 to PTG Trigger 26, PPS input #167 (RPI166)
    _PCI12R = 167;

    //Output indicators for ADC interrupts
    _TRISD4 = 0;
    _TRISD5 = 0;
    _TRISD6 = 0;
    _TRISD10 = 0;
    _TRISD11 = 0;
    _TRISD12 = 0;
}

int main(void) {
   
    clocks_initialize();
    IO_initialize();
    ADC_initialize();
    PWM1_initialize();
    PWM2_initialize();
    PTG_initialize();

    while (1);

    return 0;
}

void __attribute__((__interrupt__)) _AD1CH0Interrupt() {
    unsigned int adc_result = AD1DATA0; //Reading the result is required to clear the interrupt
    _AD1CH0IF = 0; //Clear flag
    _LATD4 ^= 1; //Toggle I/O indicator
}

void __attribute__((__interrupt__)) _AD1CH1Interrupt() {
    unsigned int adc_result = AD1DATA1; //Reading the result is required to clear the interrupt
    _AD1CH1IF = 0; //Clear flag
    _LATD5 ^= 1; //Toggle I/O indicator
}

void __attribute__((__interrupt__)) _AD1CH2Interrupt() {
    unsigned int adc_result = AD1DATA2; //Reading the result is required to clear the interrupt
    _AD1CH2IF = 0; //Clear flag
    _LATD6 ^= 1; //Toggle I/O indicator
}

void __attribute__((__interrupt__)) _AD1CH3Interrupt() {
    unsigned int adc_result = AD1DATA3; //Reading the result is required to clear the interrupt
    _AD1CH3IF = 0; //Clear flag
    _LATD10 ^= 1; //Toggle I/O indicator
}

void __attribute__((__interrupt__)) _AD1CH4Interrupt() {
    unsigned int adc_result = AD1DATA4; //Reading the result is required to clear the interrupt
    _AD1CH4IF = 0; //Clear flag
    _LATD11 ^= 1; //Toggle I/O indicator
}

void __attribute__((__interrupt__)) _AD1CH5Interrupt() {
    unsigned int adc_result = AD1DATA5; //Reading the result is required to clear the interrupt
    _AD1CH5IF = 0; //Clear flag
    _LATD12 ^= 1; //Toggle I/O indicator
}

#endif
 
