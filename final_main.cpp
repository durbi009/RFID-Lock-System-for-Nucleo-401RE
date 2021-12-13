/* mbed Microcontroller Library
 * Copyright (c) 2019 ARM Limited
 * SPDX-License-Identifier: Apache-2.0
 */

//Test of cheap 13.56 Mhz RFID-RC522 module from eBay
//This code is based on Martin Olejar's MFRC522 library. Minimal changes
//Adapted for Nucleo STM32 F401RE. Should work on other Nucleos too
 
//Connect as follows:
//RFID pins        ->  Nucleo header CN5 (Arduino-compatible header)
//----------------------------------------
//RFID IRQ=pin5    ->   Not used. Leave open
//RFID MISO=pin4   ->   Nucleo SPI_MISO=PA_6=D12
//RFID MOSI=pin3   ->   Nucleo SPI_MOSI=PA_7=D11
//RFID SCK=pin2    ->   Nucleo SPI_SCK =PA_5=D13
//RFID SDA=pin1    ->   Nucleo SPI_CS  =PB_6=D10
//RFID RST=pin7    ->   Nucleo         =PA_9=D8
//3.3V and Gnd to the respective pins                              
                              
#include "mbed.h"
#include "MFRC522.h"
#include "BufferedSerial.h"
#include "platform/mbed_thread.h"
 
// Nucleo Pin for MFRC522 reset (pick another D pin if you need D8)
#define MF_RESET    D8
#define RELAY D3
DigitalOut Lock(D3);
DigitalOut red(D4);
DigitalOut green(D5);

//volatile int scancount = 0;
volatile bool match = true;
//Serial connection to PC for output
BufferedSerial pc(USBTX, USBRX);
const char master[4] = {0X73,0X4A,0X79,0X1A}; // white card is set to master key by default
//InterruptIn button(D3);
MFRC522    RfChip   (SPI_MOSI, SPI_MISO, SPI_SCK, SPI_CS, MF_RESET);
//MFRC522    MASTER    (SPI_MOSI, SPI_MISO, SPI_SCK, SPI_CS, MF_RESET); 
int main(void) {
  printf("starting...\n");
   
  // Init. RC522 Chip
  RfChip.PCD_Init();
 
  while (true) 
  {
      red = 1;
      green = 0;
    Lock = 1;
   //RELAY = High; 
    // Look for new cards
    if ( ! RfChip.PICC_IsNewCardPresent())
    {
      ThisThread::sleep_for(500ms);
      continue;
    }
          // Select one of the cards
    if ( ! RfChip.PICC_ReadCardSerial())
    {
      ThisThread::sleep_for(500ms);
      continue;
    }  

     // Print Card UID
   
   
    printf("Card UID is: ");
    for (uint8_t i = 0; i < RfChip.uid.size; i++)
    {
        printf(" %X02", RfChip.uid.uidByte[i]);
        if (RfChip.uid.uidByte[i] != master[i])
            match = false; 
    }
    printf("\n\r"); 
    
    if (match)
    {
        printf("Card matches master!");
        Lock = 0;
        red = 0;
        green = 1;
        ThisThread::sleep_for(1000ms);
    }
    else {
    {
        printf("Card does not match!");
        match = true;
    
        //continue;
        
    }
    }
    printf("\n\r"); 
    }// end of while

       

}//end of main

    
  
