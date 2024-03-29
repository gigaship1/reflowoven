/*This file has been prepared for Doxygen automatic documentation generation.*/
/*! \file *********************************************************************
 *
 * \brief Timer 
 *
 * Discrete PID controller implementation. Set up by giving P/I/D terms
 * to Init_PID(), and uses a struct PID_DATA to store internal values.
 *
 * - File:               timer.c
 * - Compiler:           AGCC
 * - Supported devices:  All AVR devices
 *
 * \author               Stephan Harms
 *                       Support email: avr@stephanharms.de
 *
 * $Name$
 * $Revision: 1 $
 * $RCSfile$
 * $Date: 2009-03-02$
 *****************************************************************************/

#include "timer.h"
#include "ReflowOven.h"


#include <avr/io.h>
#include <avr/interrupt.h>



extern uint8_t updatePID;

extern uint8_t outputVal[MAX_OUTPUT];
extern uint8_t outputPin[MAX_OUTPUT];


uint8_t timeRamp;

// the values are calculated with AvrWiz for ATmega8 @ 16 mhz
// Timer0 settings: 159744 ticks (= 9.984 ms)

// Timer0 Overflow
ISR(TIMER0_OVF_vect)
{ 
  uint8_t i;
  // reinit counter
  TCNT0 = 100;

  timeRamp++;
  if(timeRamp>=100) // 100 * 10ms = 1s
  {
    timeRamp = 0;
	updatePID = TRUE;
    // set output pin hi
    for(i = 0; i <= MAX_OUTPUT; i++)
    {

	  if(outputVal[i] != 0)
	    OUTPUT_PORT |= (1<<outputPin[i]);
	}
  }

  // set output pin low
  for(i = 0; i <= MAX_OUTPUT; i++)
  {
    if(timeRamp >= outputVal[i])
	  OUTPUT_PORT &= ~(1<<outputPin[i]);
   }
	  


}

void Timer0_init()
{
	// Timer0 settings: ~ 159744 ticks (9.984 ms)
	TCCR0 = (1<<CS02) ; // prescaler = 256
	// init counter
	TCNT0 = 100;
	TIMSK = (1<<TOIE0); // Timer0 Overflow Interrupt Enable
}

void InitTimer()
{
  uint8_t i;
	Timer0_init();
	timeRamp = 0;

	// set output pin low
  for(i = 0; i <= MAX_OUTPUT; i++)
  {
	  OUTPUT_DDR |= (1<<outputPin[i]);
  }
	  
}



// generated by AvrWiz on 3/3/2009 8:27:54 PM

