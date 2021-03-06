
/*
 Copyright (C) 2011 J. Coliz <maniacbug@ymail.com>

 This program is free software; you can redistribute it and/or
 modify it under the terms of the GNU General Public License
 version 2 as published by the Free Software Foundation.

 */
#ifndef __ARCH_CONFIG_H__
#define __ARCH_CONFIG_H__

#define RF24_LINUX

#include <stddef.h>
#include "../bus/SPIDevice.h"
#include "../gpio/GPIO.h"
#include "compatibility.h"
#include <stdint.h>
#include <stdio.h>
#include <time.h>
#include <string.h>
#include <sys/time.h>

#define _BV(x) (1<<(x))
#define _SPI spi

//#undef SERIAL_DEBUG
#ifdef SERIAL_DEBUG
#define IF_SERIAL_DEBUG(x) ({x;})
#else
#define IF_SERIAL_DEBUG(x)
#endif

// Avoid spurious warnings
#if 1
#if ! defined( NATIVE ) && defined( ARDUINO )
#undef PROGMEM
#define PROGMEM __attribute__(( section(".progmem.data") ))
#undef PSTR
#define PSTR(s) (__extension__({static const char __c[] PROGMEM = (s); &__c[0];}))
#endif
#endif

typedef uint16_t prog_uint16_t;
#define PSTR(x) (x)
#define printf_P printf
#define strlen_P strlen
#define PROGMEM
#define pgm_read_word(p) (*(p)) 
#define PRIPSTR "%s"
#define pgm_read_byte(p) (*(p))

// Function, constant map as a result of migrating from Arduino
#define LOW exploringBB::GPIO::LOW
#define HIGH exploringBB::GPIO::HIGH
#define INPUT exploringBB::GPIO::INPUT
#define OUTPUT exploringBB::GPIO::OUTPUT
#define digitalWrite(pin, value) exploringBB::GPIO::write(pin, value)
//#define pinMode(pin, direction) exploringBB::GPIO::open(pin, direction)
#define delay(milisec) __msleep(milisec)
#define delayMicroseconds(usec) __usleep(usec)
#define millis() __millis()

#endif // __ARCH_CONFIG_H__
// vim:ai:cin:sts=2 sw=2 ft=cpp
