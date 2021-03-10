#include "TinyServoReader.h"


uint8_t PinIndex = 0;
TinyServoReader::CaptureStruct Capture;

// Interrupt latency ~[8 ; 18] us.
// Interrupt function duration ~[4.5 ; 5.0] us.
// We use a 2 field time getter to avoid doing multiplications during the interrupt.
void TinyServoReaderPinChangeInterrupt()
{
	if (PINB & PinIndex)
	{
		Capture.StartMicros = micros();
		Capture.CaptureState = TinyServoReader::CaptureStateEnum::WaitingForEnd;
	}
	else
	{
		Capture.EndMicros = micros();
		Capture.CaptureState = TinyServoReader::CaptureStateEnum::WaitingForStart;
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
	Capture.CaptureState = TinyServoReader::CaptureStateEnum::WaitingForEnd;
	Capture.EndMicros = 0;
}

const bool TinyServoReader::GetServoPulseValue(uint16_t& value, const uint32_t timeoutDurationMillis)
{
	if (Capture.CaptureState == TinyServoReader::CaptureStateEnum::WaitingForStart)
	{
		// Copy a timings buffer of the last capture.
		noInterrupts();
		CaptureBuffer.StartMicros = Capture.StartMicros;
		CaptureBuffer.EndMicros = Capture.EndMicros;
		interrupts();

		return CaptureBuffer.GetRelativeDelta(value, timeoutDurationMillis);
	}
	else
	{
		return false;
	}
}