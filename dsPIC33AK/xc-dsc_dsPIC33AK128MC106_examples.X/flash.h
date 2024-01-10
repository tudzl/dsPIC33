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
 * File:        flash.h  
 * Author: 
 * Comments:
 * Revision history: 
 */

// This is a guard condition so that contents of this file are not included
// more than once.  
#ifndef FLASH_H
#define	FLASH_H

#include <xc.h> // include processor files - each processor file is guarded.  

//Write 1 flash word (128 bits) into a location in flash.
//uint32_t* data_words:     Pointer to a buffer of 4x 32-bit values containing the data to write to the flash word
//uint32_t* write_pointer:  Pointer to the location in flash to write the word to. Must be 128-bit aligned.
//Return value: 0 if operation was successful, 1 if error was reported (NVMCONbits.WRERR)
uint32_t write_flash_word(uint32_t* data_words, uint32_t* write_pointer);

//Erase the contents of 1 flash page (4kB).
//uint32_t* erase_pointer:  A pointer to a location within the flash page to erase.
//Return value: 0 if operation was successful, 1 if error was reported (NVMCONbits.WRERR)
uint32_t erase_flash_page(uint32_t* erase_pointer);

//Get the starting address of a flash page, given an address within the page.
//uint32_t flash_address: An address within the page to determine the base address of.
//Return value: The base address of the page.
uint32_t get_flash_page_base_address(uint32_t flash_address);

//Get the starting address of a flash row, given an address within the row.
//uint32_t flash_address: An address within the row to determine the base address of.
//Return value: The base address of the row.
uint32_t get_flash_row_base_address(uint32_t flash_address);

//Copy data from a page of flash (4kB) into a RAM buffer.
//uint32_t* flash_read_pointer: An address within the page to read
//uint32_t* page_buffer: Buffer to store page data in. Must have at least 4kB size.
void read_flash_page(uint32_t* flash_read_pointer, uint32_t* page_buffer);

//Determine whether a page of RAM is fully erased (all bits are 1).
//uint32_t* flash_pointer: Pointer to a location within the page to blank check.
//Return value: 0 if page is not blank, 1 if page is blank.
uint32_t blank_check_page(uint32_t* flash_pointer);

//Copy data from a row of flash (512B) into a RAM buffer.
//uint32_t* flash_read_pointer: Pointer to a location within the row to read.
//uint32_t* row_buffer: Buffer to store row data in. Must have at least 512B size.
void read_flash_row(uint32_t* flash_read_pointer, uint32_t* row_buffer);

//Write one row of flash (512B) from a buffer in RAM.
//uint32_t* ram_source_pointer: Buffer in RAM holding data to write to flash. Must have at least 512B size.
//uint32_t* row_flash_write_pointer: Pointer to a location within the flash row to write the data to.
//Return value: 0 if operation was successful, 1 if error was reported (NVMCONbits.WRERR)
uint32_t write_flash_row(uint32_t* ram_source_pointer, uint32_t* flash_write_pointer);

//Write one page of flash (4kB, 8 rows) from a buffer in RAM.
//uint32_t* ram_source_pointer: Buffer in RAM holding data to write to flash. Must have at least 4kB size.
//uint32_t* flash_write_pointer: Pointer to a location within the flash page to write the data to.
//Return value: 0 if operation was successful, 1 if error was reported (NVMCONbits.WRERR)
uint32_t write_flash_page(uint32_t* ram_source_buffer, uint32_t* flash_write_pointer);

//Naming convention:
//pointer = uint32_t*
//address = uint32_t
//buffer = uint32_t* (uint32_t[])

#ifdef	__cplusplus
extern "C" {
#endif /* __cplusplus */

    // TODO If C++ is being used, regular C code needs function names to have C 
    // linkage so the functions can be used by the c code. 

#ifdef	__cplusplus
}
#endif /* __cplusplus */

#endif	/* FLASH_H */

