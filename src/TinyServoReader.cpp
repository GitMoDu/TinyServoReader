#include "TinyServoReader.h"

volatile uint32_t LastHigh, LastPulseDuration, LastPeriod;

uint8_t PinIndex = 0;

void PinChangeInterrupt()
{
	uint32_t TimeStamp = micros();

	if (PINB & PinIndex)
	{
		if (LastHigh != 0)
		{
			LastPeriod = TimeStamp - LastHigh;
		}
		LastHigh = TimeStamp;
	}
	else
	{
		if (LastHigh != 0)
		{
			TimeStamp = TimeStamp - LastHigh;
			// Clamp the value to servo specification.
			LastPulseDuration = constrain(TimeStamp, SERVO_READER_PULSE_MIN_DURATION_MICROS, SERVO_READER_PULSE_MAX_DURATION_MICROS);
		}
	}
}

TinyServoReader::TinyServoReader(const uint8_t pin)
{
	PinNumber = pin;

	PinIndex = 1 << pin;
}

void TinyServoReader::Begin()
{
	Stop();

	// Attach interrupt handler and start measuring.
	attachInterrupt(digitalPinToInterrupt(PinNumber), PinChangeInterrupt, CHANGE);
}

void TinyServoReader::Stop()
{
	detachInterrupt(digitalPinToInterrupt(PinNumber));

	LastHigh = 0;
	Invalidate();
}

void TinyServoReader::Begin(const uint8_t pin)
{
	PinNumber = pin;
	PinIndex = 1 << pin;

	Begin();
}

bool TinyServoReader::HasPulseDuration()
{
	return LastHigh != 0 && 
		((micros() - LastHigh) < INPUT_SIGNAL_TIMEOUT_DURATION_MICROS);
}

bool TinyServoReader::HasPeriod()
{
	return LastPeriod != 0;
}

uint16_t TinyServoReader::GetValue()
{
	return uint16_t((LastPulseDuration*UINT16_MAX) / SERVO_READER_PULSE_MAX_DURATION_MICROS);
}

uint32_t TinyServoReader::GetPulseDuration()
{
	return LastPulseDuration;
}

void TinyServoReader::Invalidate()
{
	if (!HasPulseDuration())
	{
		LastHigh = 0;
		LastPeriod = 0;
		LastPulseDuration = 0;
	}
}