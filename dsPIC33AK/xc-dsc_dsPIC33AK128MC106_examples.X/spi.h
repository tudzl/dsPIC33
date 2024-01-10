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
 * File:    spi.h
 * Author: 
 * Comments:
 * Revision history: 
 */

// This is a guard condition so that contents of this file are not included
// more than once.  
#ifndef SPI_H
#define	SPI_H

#include <xc.h> // include processor files - each processor file is guarded.  

void SPI1_initialize_16_bit_host_mode();
void SPI1_initialize_I2S_host_mode_16_bit_channel_data_32_bit_frame();
void SPI1_initialize_left_justified_host_mode_16_bit_channel_data_32_bit_frame();
void SPI1_initialize_right_justified_host_mode_16_bit_channel_data_32_bit_frame();
void SPI1_initialize_PCM_DSP_host_mode_16_bit_channel_data_32_bit_frame();
void SPI1_initialize_I2S_host_mode_625kbps_bclk_16_bit_channel_data_32_bit_frame_with_REFO();
void SPI1_initialize_16_bit_client_mode();
void SPI1_initialize_I2S_client_mode_16_bit_channel_data_32_bit_frame();
void SPI1_initialize_left_justified_client_mode_16_bit_channel_data_32_bit_frame();
void SPI1_initialize_right_justified_client_mode_16_bit_channel_data_32_bit_frame();
void SPI1_initialize_PCM_DSP_client_mode_16_bit_channel_data_32_bit_frame();

uint32_t SPI1_host_exchange_word(uint32_t write_data);
void SPI1_host_exhange_array(uint32_t* transmit_buffer, uint32_t* receive_buffer, uint32_t len);
void configure_CLKGEN9_for_40MHz_MCLK_from_PLL2();

#ifdef	__cplusplus
extern "C" {
#endif /* __cplusplus */

    // TODO If C++ is being used, regular C code needs function names to have C 
    // linkage so the functions can be used by the c code. 

#ifdef	__cplusplus
}
#endif /* __cplusplus */

#endif	/* XC_HEADER_TEMPLATE_H */

