#include "TinyServoReader.h"

volatile uint32_t LastHigh, LastPulseDuration, LastPeriod, TimeStampGrunt;

uint8_t PinIndex = 0;

void PinChangeInterrupt()
{
	TimeStampGrunt = micros();
	if (PINB & PinIndex)
	{
		if (LastHigh != 0)
		{
			LastPeriod = TimeStampGrunt - LastHigh;
		}
		LastHigh = TimeStampGrunt;
	}
	else
	{
		if (LastHigh != 0)
		{
			//We clamp the value so we never have to deal with off-spec values for servos.
			LastPulseDuration = constrain(TimeStampGrunt - LastHigh, SERVO_READER_PULSE_MIN_DURATION_MICROS, SERVO_READER_PULSE_MAX_DURATION_MICROS);
		}
	}
}

TinyServoReader::TinyServoReader(const uint8_t pin)
{
	PinNumber = pin;

	//Same result.
	//PinIndex = pow(2, INPUT_PWM_PIN);
	PinIndex = 1 << pin;
}

void TinyServoReader::Begin()
{
	// Attach interrupt handler and start measuring.
	attachInterrupt(digitalPinToInterrupt(PinNumber), PinChangeInterrupt, CHANGE);
}

void TinyServoReader::Begin(const uint8_t pin)
{
	PinNumber = pin;
	PinIndex = 1 << pin;
	Begin();
}

bool TinyServoReader::HasPulseDuration()
{
	return LastPulseDuration != 0;
}

bool TinyServoReader::HasPeriod()
{
	return LastPeriod != 0;
}

uint16_t TinyServoReader::GetValue()
{
	return uint16_t((LastPulseDuration*UINT16_MAXVALUE) / SERVO_READER_PULSE_MAX_DURATION_MICROS);
}

uint32_t TinyServoReader::GetPulseDuration()
{
	LastPulseDuration;
}

void TinyServoReader::Invalidate()
{
	if (LastHigh == 0 ||
		((micros() - LastHigh) > INPUT_SIGNAL_TIMEOUT_DURATION_MICROS))
	{
		LastHigh = 0;
		LastPeriod = 0;
		LastPulseDuration = 0;
	}
}
