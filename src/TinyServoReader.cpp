#ifndef DISABLEMILLIS
#include "TinyServoReader.h"


uint8_t PinIndex = 0;
TinyServoReader::CaptureStruct Capture;
uint32_t CounterHelper = 0;


// Interrupt latency ~[8 ; 18] us.
// Interrupt function duration ~[4.5 ; 5.0] us.
// We use a 2 field time getter to avoid doing multiplications during the interrupt.
void TinyServoReaderPinChangeInterrupt()
{
	CounterHelper = micros();

	if (PINB & PinIndex)
	{
		// Rising edge.
		if (Capture.CaptureState == TinyServoReader::CaptureStateEnum::WaitingForStart)
		{
			Capture.StartMicros = CounterHelper;
			Capture.CaptureState = TinyServoReader::CaptureStateEnum::WaitingForEnd;
		}
		else
		{
			// Not expecting an HIGH at this moment, ignore.
		}
	}
	else
	{
		if (Capture.CaptureState == TinyServoReader::CaptureStateEnum::WaitingForEnd)
		{
			Capture.EndMicros = CounterHelper;
			Capture.CaptureState = TinyServoReader::CaptureStateEnum::WaitingForRestart;
		}
		else if (Capture.CaptureState == TinyServoReader::CaptureStateEnum::WaitingForEnd)
		{
			Capture.CaptureState = TinyServoReader::CaptureStateEnum::WaitingForEnd;
			// Not expecting an LOW at this moment, restart.
		}
	}
}

TinyServoReader::TinyServoReader(const uint8_t pin) : PinNumber(pin)
{
	PinIndex = 1 << pin;
}

void TinyServoReader::Start()
{
	// Reset everything.
	Stop();

	// Set up pin.
	pinMode(PinNumber, INPUT);

	// Attach interrupt handler and start measuring.
	attachInterrupt(digitalPinToInterrupt(PinNumber), TinyServoReaderPinChangeInterrupt, CHANGE);
}

void TinyServoReader::Stop()
{
	detachInterrupt(digitalPinToInterrupt(PinNumber));
	Capture.CaptureState = TinyServoReader::CaptureStateEnum::WaitingForStart;
	Capture.EndMicros = 0;
}

const bool TinyServoReader::GetServoPulseValue(uint16_t& value)
{
	if (Capture.CaptureState == TinyServoReader::CaptureStateEnum::WaitingForRestart)
	{
		// With the latest value read out, we are ready for the next one.
		Capture.CaptureState = TinyServoReader::CaptureStateEnum::WaitingForStart;

		return Capture.GetRelativeDelta(value);
	}
	else
	{
		return false;
	}
}
#endif