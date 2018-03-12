///  TinyServoReader.h
/// 
///  Created for personal use, use it at your own risk and benefit.
///  https://github.com/GitMoDu/TinyServoReader
///
///  Reads duration of input pulse on any pin, outputs a 16 bit value from the input pulse duration.
///  Input range of pulses is from 1000 to 2000 µs, according to servo specifications.Over and under input values are clamped.
/// 

#ifndef _TINYSERVOREADER_h
#define _TINYSERVOREADER_h

#if defined (ARDUINO) || defined(ATTINY_CORE)
#include <Arduino.h>
#else
#pragma Only ATmega328 and ATTiny 85 supported.
#endif


#define INPUT_SIGNAL_TIMEOUT_DURATION_MICROS 100000l

#define SERVO_READER_PULSE_MIN_DURATION_MICROS 1000l
#define SERVO_READER_PULSE_MAX_DURATION_MICROS 2000l

#define UINT16_MAXVALUE 65535

class TinyServoReader
{
public:
	TinyServoReader(const uint8_t pin);
	void Invalidate();
	bool HasPeriod();
	bool HasPulseDuration();
	uint32_t GetPulseDuration();
	uint16_t GetValue();

};
#endif

