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
 * File:   flash.c
 * Author: C68555
 *
 * Created on May 22, 2023, 2:47 PM
 */


#include "xc.h"
#include "examples.h"

uint32_t write_flash_word(uint32_t* data_words, uint32_t* write_pointer) {
    
    //Select single word write operation
    NVMCONbits.NVMOP = 1; 
    NVMCONbits.WREN = 1;
    
    //Select address of word to write
    NVMADR = (uint32_t)write_pointer;
    
    //Load NVMDATA0-3 registers with the data to write
    NVMDATA0 = data_words[0]; 
    NVMDATA1 = data_words[1];
    NVMDATA2 = data_words[2];
    NVMDATA3 = data_words[3];
    
    //Start operation
    NVMCONbits.WR = 1;
    //Wait for operation to complete
    while (NVMCONbits.WR == 1);
    
    //Clear WREN bit to avoid accidental writes.
    NVMCONbits.WREN = 0;
    
    //Report error status (1 = error, 0 = no error)
    return NVMCONbits.WRERR;
}

uint32_t erase_flash_page(uint32_t* erase_pointer) {

    //Specify an address within the page to erase; NVM controller will determine the page.
    NVMADR = (uint32_t)erase_pointer;
    
    //Select page erase operation
    NVMCONbits.WREN = 1;
    NVMCONbits.NVMOP = 0b0011;
    
    //Start operation
    NVMCONbits.WR = 1;
    //Wait for operation to complete
    while(NVMCONbits.WR == 1);
    
    //Clear WREN bit to avoid accidental erase operations.
    NVMCONbits.WREN = 0;
    
    //Report status of operation (1 = error, 0 = no error)
    return NVMCONbits.WRERR;
}

uint32_t get_flash_page_base_address(uint32_t flash_address) {
    //When erasing a page, the page to erase will be automatically determined based on NVMADR.
    //However, in order to read the contents of a specific page, its starting address must be known or calculated in software.
    
    //Program space, and therefore the first page, start at 0x800000
    uint32_t start_of_program_space = 0x800000;
    
    uint32_t offset_from_program_start = flash_address - start_of_program_space;
    
    //1 page is 0x1000 (4096) bytes
    uint32_t page_index = offset_from_program_start / 0x1000;
    uint32_t page_offset = offset_from_program_start % 0x1000;
    
    uint32_t page_address = start_of_program_space + (page_index * 0x1000);
    
    return page_address;
}

uint32_t get_flash_row_base_address(uint32_t flash_address) {
    //When writing to a row, the row will be automatically determined by NVMADR.
    //In order to read a row, its starting address must be known or calculated in software.
    
    //Program space, and therefore the first row, starts at 0x800000
    uint32_t start_of_program_space = 0x800000;
    uint32_t offset_from_program_start = flash_address - start_of_program_space;
    
    //1 row is 0x200 (512) bytes
    uint32_t row_index = offset_from_program_start / 0x200;
    uint32_t row_address = start_of_program_space + (row_index * 0x200);
    
    return row_address;
}

//Store the value of an entire page pointed to by an address in flash in a RAM buffer (must be 4KB in size)
void read_flash_page(uint32_t* flash_read_pointer, uint32_t* page_buffer) {
    uint32_t page_base_address = get_flash_page_base_address((uint32_t)flash_read_pointer);
    uint32_t* page_pointer = (uint32_t*)page_base_address;
    
    //A 4KB page contains 1024x 32-bit (4 byte) words
    for (int i = 0; i < 1024; i++) {
        page_buffer[i] = page_pointer[i];
    }
}

//Verify the page pointed to by flash_address is erased (all bits are 1)
uint32_t blank_check_page(uint32_t* flash_pointer) {
    uint32_t page_base_address = get_flash_page_base_address((uint32_t)flash_pointer);
    uint32_t* page_pointer = (uint32_t*)page_base_address;
    
    //A 4KB page contains 1024x 32-bit (4 byte) words
    for (int i = 0; i < 1024; i++) {
        //Verify each bit is 1, in units of 32-bit words
        if (page_pointer[i] != 0xFFFFFFFF) {return 0;} //Report page is not blank
    }
    
    //Report page is blank
    return 1;
}

//Read a flash row into a RAM buffer (must be 512 bytes)
void read_flash_row(uint32_t* flash_read_pointer, uint32_t* row_buffer) {
    
    //Calculate starting address of the row pointed to by flash_address
    uint32_t row_base_address = get_flash_row_base_address((uint32_t)flash_read_pointer);
    uint32_t* row_pointer = (uint32_t*)row_base_address;
    
    //One 512B row contains 128x 32-bit (4 byte) words
    for (int i = 0; i < 128; i++) {
        row_buffer[i] = row_pointer[i];
    }
}

//A row consists of 32 flash words (128x 32-bit words, or 512 bytes)
uint32_t write_flash_row(uint32_t* ram_source_pointer, uint32_t* flash_write_pointer) {
    
    //Load NVMSRCADR with address of data in device RAM.
    NVMSRCADR = (uint32_t)ram_source_pointer;
    //Load NVMADR with the Flash address to be programmed.
    NVMADR = (uint32_t)flash_write_pointer;
    
    //Configure NVMCON to Row program
    NVMCONbits.WREN = 1;
    NVMCONbits.NVMOP = 0b0010;
    
    //Set the WR bit, starting program operation.
    NVMCONbits.WR = 1;
    
    //Wait for sequence to complete, WR bit will be cleared by hardware
    while(NVMCONbits.WR);
    
    //Clear WREN bit to avoid accidental writes.
    NVMCONbits.WREN = 0;
    
    //Test the WRERR bit to ensure the program sequence completed successfully (1 = error, 0 = success)
    return NVMCONbits.WRERR;
}

//A page is 8 rows (4096 bytes, or 256 flash words)
//To write an entire page, need to know the starting address of the page (or technically, some address within the first row of the page)
//and have 4KB of source data to write to the page.
//Because this is fundamentally a row write operation, whether or not the written data will actually be page-aligned must be enforced by the software.
uint32_t write_flash_page(uint32_t* ram_source_buffer, uint32_t* flash_write_pointer) {
    
    //Get the base address of the page to write. This will be the address of the first row,
    //and can be offset to address each row within the page.
    uint32_t page_base_address = get_flash_page_base_address((uint32_t)flash_write_pointer);
    uint32_t* page_base_pointer = (uint32_t*)page_base_address;
    
    //A page has 8 rows
    for (int i = 0; i < 8; i++) {
        
        //Offset between each row will be 512 bytes, 128x 32-bit words
        uint32_t buffer_offset = i * 128;
        uint32_t row_program_error = write_flash_row(&ram_source_buffer[buffer_offset], &page_base_pointer[buffer_offset]);
        
        //Stop programming in case of error
        if (row_program_error) {
            return 1;
        }
    }
    
    //Report success
    return 0;
}