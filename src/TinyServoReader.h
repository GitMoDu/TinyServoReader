///	TinyServoReader.h
///
///	Created for personal use, use it at your own risk and benefit.
///	https://github.com/GitMoDu/TinyServoReader
///
///	Reads duration of input pulse on any pin, outputs a 16 bit value from the input pulse duration.
///	Input range of pulses is from 1000 to 2000 µs, according to servo specifications.Over and under input values are clamped.
///	Requires millis()/micros() enabled.
/// 

#ifndef _TINY_SERVO_READER_h
#define _TINY_SERVO_READER_h

#if defined (ARDUINO) || defined(ATTINY_CORE)
#include <Arduino.h>
#else
#pragma Only ATmega328 and ATTiny 85 supported.
#endif

#ifndef DISABLEMILLIS


class TinyServoReader
{
public:
	static const uint16_t MinServoMicros = 1000;
	static const uint16_t MaxServoMicros = 2000;
	static const uint16_t RangeServoMicros = MaxServoMicros - MinServoMicros;
	static const uint16_t AbsoluteMinServoMicros = MinServoMicros - 200;
	static const uint16_t AbsoluteMaxServoMicros = MaxServoMicros + 200;

	enum CaptureStateEnum
	{
		WaitingForStart,
		WaitingForEnd,
		WaitingForRestart,
	};

	struct CaptureStruct
	{
		volatile CaptureStateEnum CaptureState = CaptureStateEnum::WaitingForStart;
		volatile uint32_t StartMicros = 0;
		volatile uint32_t EndMicros = 0;

		const bool GetRelativeDelta(uint16_t& delta)
		{
			if (StartMicros < EndMicros)
			{
				delta = EndMicros - StartMicros;
			}
			else
			{
				delta = EndMicros + (UINT32_MAX - StartMicros);
			}

			if ((delta < AbsoluteMinServoMicros) ||
				(delta > AbsoluteMaxServoMicros))
			{
				delta = 0;
				return false;
			}

			delta = max(delta, MinServoMicros);
			delta = min(delta, MaxServoMicros);


			delta = ((uint32_t)(delta - MinServoMicros) * UINT16_MAX) / RangeServoMicros;

			return true;
		}
	};


	const uint8_t PinNumber;

public:
	TinyServoReader(const uint8_t pin);
	void Start();
	void Stop();
	const bool GetServoPulseValue(uint16_t& value);
};
#endif
#endif