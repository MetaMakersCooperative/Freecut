/*
 * main.c
 * 
 * Freecut firmware, main program
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

#include <avr/io.h>
#include <avr/interrupt.h>
#include <avr/wdt.h>
#include <inttypes.h>
#include <stdio.h>
#include "lcd.h"
#include "timer.h"
#include "version.h"

int dummy_var;

int main( void )
{


    timer_init( );
    lcd_init( );

    // short beep to show we're awake
    beeper_on( 1760 );
    msleep( 10 );
    beeper_off( );

    // connect stdout to USB port
    fprintf( &lcd, "Freecut v" VERSION );
    while( 1 )
    {
        dummy_var = 0;
    }
}
