/*
 * Main.c
 *
 *  Created on: Sep 3, 2024
 *      Author: Ahmed Nageeb
 */

#include <avr/io.h>
#include <util/delay.h>
#include "STD_Types.h"

#define F_CPU 8000000UL // Define CPU frequency for delay functions

// 7-segment display encoding for digits 0-9
#define ZERO 0b00111111  // 0
#define ONE 0b00000110   // 1
#define TWO 0b01011011   // 2
#define THREE 0b01001111 // 3
#define FOUR 0b01100110  // 4
#define FIVE 0b01101101  // 5
#define SIX 0b01111101   // 6
#define SEVEN 0b00000111 // 7
#define EIGHT 0b01111111 // 8
#define NINE 0b01101111  // 9

#define DELAY_MS 1000 // Delay in milliseconds

// Array holding the binary representations of the digits for the 7-segment display
u8 SSDArr[10] = {ZERO, ONE, TWO, THREE, FOUR, FIVE, SIX, SEVEN, EIGHT, NINE};

// Function Prototypes
void TimerUp(void);
void CountDown(void);
void flash(void);

// Global variables to hold the current digits being displayed
u8 Tens = 0;
u8 Ones = 0;

int main(void)
{
    // Initialize ports
    DDRB = 0x00;  // Set PORTB as input (for buttons)
    DDRA = 0xFF;  // Set PORTA as output (for first 7-segment display)
    DDRC = 0xFF;  // Set PORTC as output (for second 7-segment display)
    PORTB = 0xFF; // Initialize PORTB with pull-up resistors

    while (1)
    {
        TimerUp(); // Continuously call the TimerUp function
    }
}

/*
 * TimerUp
 * 
 * This function counts up on the 7-segment display based on button presses.
 * - Button connected to PB1 or PB2: Increments the ones digit and displays it on the first 7-segment.
 * - Button connected to PB0 or PB2: Increments the tens digit and displays it on the second 7-segment.
 * - Button connected to PB3: Initiates the countdown sequence.
 */
void TimerUp(void)
{
    u8 CountDownResult = 0;
    u8 tenscount= 0;
    u8 onescount= 0;
    // Loop through digits 0-9
   while(1){
        // If button connected to PB1 or PB2 is pressed
        if (PINB == 0b11111110 || PINB == 0b11111100)
        {
            if(PINB == 0b11111000){
            CountDown();
            }
            PORTA = ~SSDArr[tenscount]; // Display digit on the first 7-segment
            Ones = tenscount;            // Update Ones variable
            tenscount++;
            if(tenscount == 10){tenscount =0;}
            _delay_ms(DELAY_MS);         // Delay
        }

        // If button connected to PB0 or PB2 is pressed
        if (PINB == 0b11111101 || PINB == 0b11111100)
        {
            if(PINB == 0b11111000){
            CountDown();
            }
            PORTC = ~SSDArr[onescount]; // Display digit on the second 7-segment
            Tens = onescount;           // Update Tens variable
            onescount++;
            if(onescount == 10){onescount =0;}
            // Adjust delay if specific buttons are pressed
            if (PINB == 0b11111100)
            {
                _delay_ms(DELAY_MS / 100); // Shorter delay
            }
            else
            {
                _delay_ms(DELAY_MS); // Regular delay
            }
        }

        // If button connected to PB3 is pressed, start countdown
        if (PINB == 0b11111000)
        {
            CountDown(); // Call CountDown function
        }
    }
}

/*
 * CountDown
 * 
 * This function initiates a countdown on the 7-segment displays, starting
 * from the current values of Tens and Ones.
 * - It counts down until both digits reach 0.
 * - When both digits reach 0, it calls the flash function.
 */
void CountDown(void)
{
    while (Tens != -1) // Loop until Tens digit is -1
    {
        PORTA = ~SSDArr[Ones]; // Display Ones digit on first 7-segment

        // Decrement Tens digit if Ones digit reaches 0
        if (0 == Ones)
        {
            Ones = 10; // Reset Ones to 10 (which will become 9 after decrement)
            Tens--;    // Decrement Tens digit
        }
        _delay_ms(DELAY_MS); // Delay
        PORTC = ~SSDArr[Tens]; // Display Tens digit on second 7-segment
        Ones--;                // Decrement Ones digit
        
        // If countdown reaches 0-0, call flash function
        if ((Ones == 0) && (Tens == 0))
        {
            flash(); // Flash both 7-segments
        }
    }
}

/*
 * flash
 * 
 * This function flashes both 7-segment displays on and off when the countdown reaches 0-0.
 */
void flash(void)
{
    while (1) // Infinite loop to continuously flash the display
    {
        PORTA = ~SSDArr[0]; // Display 0 on first 7-segment
        PORTC = ~SSDArr[0]; // Display 0 on second 7-segment

        _delay_ms(500);     // Delay
        PORTA = 0xff;       // Turn off first 7-segment
        PORTC = 0xff;       // Turn off second 7-segment
        _delay_ms(500);     // Delay
    }
}
