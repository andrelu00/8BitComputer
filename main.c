/*  
*   8 Bit PC emulator inspired by Ben Eater "Bulding an 8-bit breadboard Computer!" youtube series
*   Made by: André Luiz(andrelu00) 
*/

#include <stdio.h>
#include <stdlib.h>
#include "utils.h"
#include "bus.h"
#include "cpu.h"
#include "alu.h"
#include "ram.h"


#define TRUE  1
#define FALSE 0

//This is a way to print binary numbers that i found on StackOverflow by user William Whyte
#define BYTE_TO_BINARY_PATTERN "%c%c%c%c%c%c%c%c"
#define BYTE_TO_BINARY(byte)  \
  (byte & 0x80 ? '1' : '0'), \
  (byte & 0x40 ? '1' : '0'), \
  (byte & 0x20 ? '1' : '0'), \
  (byte & 0x10 ? '1' : '0'), \
  (byte & 0x08 ? '1' : '0'), \
  (byte & 0x04 ? '1' : '0'), \
  (byte & 0x02 ? '1' : '0'), \
  (byte & 0x01 ? '1' : '0') 

//Can print negative numbers with this function
#define BYTE_TO_BINARY_TWO_COMPLEMENT(byte)  \
  (byte & 0x80 ? '-' : '0'), \
  (byte & 0x40 ? '1' : '0'), \
  (byte & 0x20 ? '1' : '0'), \
  (byte & 0x10 ? '1' : '0'), \
  (byte & 0x08 ? '1' : '0'), \
  (byte & 0x04 ? '1' : '0'), \
  (byte & 0x02 ? '1' : '0'), \
  (byte & 0x01 ? '1' : '0') 

void main()
{
    int on = TRUE;
    int turnOff;
    int showTwoComplements = TRUE;

    int clock = 0;

    /*
    *   In the video theres a huge discussion about the registers and the bus
    *   But i will just use some arrays for the emulator
    */
    int registerA[8]           = {0b1 , 0b0, 0b0, 0b0, 0b0, 0b0, 0b0, 0b1};
    int registerB[8]           = {0b0 , 0b0, 0b0, 0b0, 0b0, 0b0, 0b0, 0b1};
    int displayRegister[8]     = {0b0};
    int bus[8]                 = {0b0 , 0b0, 0b0, 0b0, 0b0, 0b0, 0b0, 0b0};
    int instructionRegister[8] = {0b0}; //This is a special register that will be used later
    int ram[16]                = {0b0};
    int programcounter         = 0;

    while(on==1)//Computer Loop
    {
        if(programcounter > 15){
            programcounter = 0;
        }

        //Commands
        clockCycle(&clock);
        //alu(registerA, registerB, bus, TRUE, FALSE);
        loadToBus(registerA, bus);
        writeMemory(ram, bus, 0);
        //readMemory(ram, bus, 0);
        //loadFromBus(registerA, bus);

        //Visualization of the computer
        printf("\nClock Cycle: %i\n", clock);
        printf("\nRegister A\n");
        for(int i = 0; i < 8; i++)
        {
            printf("%i ",registerA[i]);

            if(i == 7)
            {
                printf("\n");
            }
        }
        printf("\nRegister B\n");
        for(int i = 0; i < 8; i++)
        {
            printf("%i ",registerB[i]);

            if(i == 7)
            {
                printf("\n");
            }
        }
        printf("\nBus\n");
        for(int i = 0; i <8; i++)
        {
            printf("%i ",bus[i]);

            if(i == 7)
            {
                printf("\n");
            }
        }
        printf("\nRAM\n");
        if(showTwoComplements == FALSE){
            for(int i = 0; i<16; i++)
            {
                printf("| %i -", ram[i]);
                printf(" "BYTE_TO_BINARY_PATTERN, BYTE_TO_BINARY(ram[i]));
                printf(" ");
                if(i == 15)
                {
                    printf("\n");
                }
            }
        }
        else{
            for(int i = 0; i<16; i++)
            {
                printf("| %i -", ram[i]);
                printf(" "BYTE_TO_BINARY_PATTERN, BYTE_TO_BINARY_TWO_COMPLEMENT(ram[i]));
                printf(" ");
                if(i == 15)
                {
                    printf("\n");
                }
            }
        }

        printf("\nDisplay\n");
        if(showTwoComplements == FALSE){
            printf(""BYTE_TO_BINARY_PATTERN, BYTE_TO_BINARY(convertToDecimal(displayRegister)));
        }
        else{
            printf(""BYTE_TO_BINARY_PATTERN, BYTE_TO_BINARY_TWO_COMPLEMENT(convertToDecimal(displayRegister)));
        }
        

        printf("\nProgram Counter\n%i\n", programcounter);
        programcounter ++; //Tecnically the Program counter should not go up every cicle, doing this to keep things simple for now

        /*
        * Just to finish the program through terminal
        * This is a janky solution, might rework later
        */
        scanf(" %i", &turnOff);
        if(turnOff == FALSE){
            on = FALSE;
        }
    }
}