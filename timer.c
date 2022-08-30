/*
 * timer.c
 *
 * Timer 0 is used as overall (slow) event timer, as well sleep delay timer.
 * Timer 1 is used as solenoid PWM, through OC1B output
 * Timer 2 is used for stepper timing 
 * Timer 3 is used to generate tones on the speaker through OC3A output, 
 * period is adjusted for tone.
 *
 *
 * Copyright 2010 <freecutfirmware@gmail.com> 
 *
 * This file is part of Freecut.
 *
 * Freecut is free software: you can redistribute it and/or modify it
 * under the terms of the GNU General Public License version 2.
 *
 * Freecut is distributed in the hope that it will be useful, but WITHOUT
 * ANY WARRANTY; without even the implied warranty of MERCHANTABILITY
 * or FITNESS FOR A PARTICULAR PURPOSE. See the GNU General Public
 * License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with Freecut. If not, see http://www.gnu.org/licenses/.
 *
 */

#include <avr/interrupt.h>
#include <avr/io.h>
#include <avr/wdt.h>
#include <string.h>
#include <stdio.h>
#include "timer.h"

static uint8_t count_Hz = 250;
static uint8_t count_25Hz = 10;
volatile uint8_t flag_Hz;
volatile uint8_t flag_25Hz;

/*
 * called @250 Hz, divide further in software for slow events 
 */
ISR( TIMER0_COMP_vect ) 
{
    if( --count_25Hz == 0 )
    {
        count_25Hz = 10;
        flag_25Hz = 1;
    }
    if( --count_Hz == 0 )
    {
	count_Hz = 250;
	flag_Hz = 1;
    }
}        


/*
 * Turn on beeper. Hz specifies frequency of the tone.
 */
void beeper_on( int Hz )
{
    DDRE |= (1 << 3);
    OCR3A = (8000000L + Hz/2) / Hz - 1;
}

void beeper_off( void )
{
    DDRE &= ~(1 << 3);
}

/*
 * usleep: sleep (approximate/minimum) number of microseconds. We use timer0 
 * which runs at 62.50 kHz, or at 16 usec/tick. Maximum delay is about 2 
 * milliseconds . For longer delays, use msleep().
 *
 */
void usleep( int usecs )
{
    signed char end = TCNT0 + usecs / 16;

    while( (signed char) (TCNT0 - end) < 0 )
	continue;
}

void msleep( unsigned msecs )
{
    while( msecs-- != 0 )
	usleep( 1000 );
}


/*
 * Init timers 
 */
void timer_init( void )
{
    // set timer 0 for 250 Hz period
    TCCR0  = (1 << WGM01) | 6;     		// prescaler 1:256 -> 62.50 kHz
    OCR0   = 249; 		  		// 125 kHz / 250 = 250 Hz 
    TIMSK  = (1 << OCIE0); 	    		// enable OVF interrupt 
}
