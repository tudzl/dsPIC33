/**
  Generated main.c file from MPLAB Code Configurator

  @Company
    Microchip Technology Inc.

  @File Name
    main.c

  @Summary
    This is the generated main.c using PIC24 / dsPIC33 / PIC32MM MCUs.

  @Description
    This source file provides main entry point for system initialization and application code development.
    Generation Information :
        Product Revision  :  PIC24 / dsPIC33 / PIC32MM MCUs - 1.171.4
        Device            :  dsPIC33CK256MP506
    The generated drivers are tested against the following:
        Compiler          :  XC16 v2.10
        MPLAB 	          :  MPLAB X v6.05
 */

/*
    (c) 2020 Microchip Technology Inc. and its subsidiaries. You may use this
    software and any derivatives exclusively with Microchip products.

    THIS SOFTWARE IS SUPPLIED BY MICROCHIP "AS IS". NO WARRANTIES, WHETHER
    EXPRESS, IMPLIED OR STATUTORY, APPLY TO THIS SOFTWARE, INCLUDING ANY IMPLIED
    WARRANTIES OF NON-INFRINGEMENT, MERCHANTABILITY, AND FITNESS FOR A
    PARTICULAR PURPOSE, OR ITS INTERACTION WITH MICROCHIP PRODUCTS, COMBINATION
    WITH ANY OTHER PRODUCTS, OR USE IN ANY APPLICATION.

    IN NO EVENT WILL MICROCHIP BE LIABLE FOR ANY INDIRECT, SPECIAL, PUNITIVE,
    INCIDENTAL OR CONSEQUENTIAL LOSS, DAMAGE, COST OR EXPENSE OF ANY KIND
    WHATSOEVER RELATED TO THE SOFTWARE, HOWEVER CAUSED, EVEN IF MICROCHIP HAS
    BEEN ADVISED OF THE POSSIBILITY OR THE DAMAGES ARE FORESEEABLE. TO THE
    FULLEST EXTENT ALLOWED BY LAW, MICROCHIP'S TOTAL LIABILITY ON ALL CLAIMS IN
    ANY WAY RELATED TO THIS SOFTWARE WILL NOT EXCEED THE AMOUNT OF FEES, IF ANY,
    THAT YOU HAVE PAID DIRECTLY TO MICROCHIP FOR THIS SOFTWARE.

    MICROCHIP PROVIDES THIS SOFTWARE CONDITIONALLY UPON YOUR ACCEPTANCE OF THESE
    TERMS.
 */

/**
  Section: Included Files
 */
#include <stdio.h>
#include "mcc_generated_files/system.h"
#include "mcc_generated_files/pin_manager.h"
#include "mcc_generated_files/tmr1.h"
#include "mcc_generated_files/sccp1_tmr.h"
#include "mcc_generated_files/pwm.h"

/*
 * dsPIC33CK256MP506 PWM demo code V1.1
 * Last edit: 22.May.2024, Zell
 * require base dev board DM330029
 * UART def 115200
 * PWM1,2 : 200KHz, interleaved phase 
 * PWM3,4 : 1Mhz, interleaved phase 
 * TP55  RB13 REFCLK output @ 2Mhz
 * TMR1 5us
                         Main application
 */
//Global define and vars
#define Duty_Cycle_Min  40
//#define Duty_Cycle_Max   0x7D0+0x3E8
#define Duty_Cycle_Max   0xF98+0x3E8
#define PG4_Period 0xF98 //HR mode 1Mhz, 3992
uint8_t TMR_250ms_flag = 0;
uint16_t TMR_100us_flag = 0;
uint16_t TMR_100us_cnt = 0;
uint16_t TMR_10us_flag = 0;
uint16_t sys_cnt = 0;
uint8_t SW1_pressed = 0; 

int main(void) {
    // initialize the device
    SYSTEM_Initialize();
    printf("Hello World! dsPIC33CK256MP506 demo running!\r\n");
    //PWM_GeneratorDisable(PWM_GENERATOR_2);

    TMR1_Start();
    SCCP1_TMR_Start();
    while (1) {
        // Add your application code
        if (SW1_pressed) {
            SW1_pressed = 0;
            PG4DC += 4;

            // PWM_GeneratorEnable(PWM_GENERATOR_2);
            //PG4DC+=1;
            if (PG4DC < Duty_Cycle_Min)
                PG4DC = Duty_Cycle_Min;
            else if (PG4DC > Duty_Cycle_Max)
                PG4DC = Duty_Cycle_Min;
        }

        if (TMR_100us_flag) {

            TMR_100us_flag = 0;
            TMR_100us_cnt++;
            if (9 < TMR_100us_cnt) {
                Trigger_RB5_SetHigh();
                TMR_100us_cnt = 0;
                PG4DC += 1; //not working as planned!
                if (PG4DC < Duty_Cycle_Min)
                    PG4DC = Duty_Cycle_Min;
                else if (PG4DC > Duty_Cycle_Max)
                    PG4DC = Duty_Cycle_Min;
                Trigger_RB5_SetLow();
            }
        }

        if (TMR_250ms_flag) {
            TMR_250ms_flag = 0;
            IO_RD15_Toggle();
            sys_cnt++;
        }


    }
    return 1;
}
/**
 End of File
 */

