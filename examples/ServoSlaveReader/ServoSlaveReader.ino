#include <TinyServoReader.h>

#ifdef ATTINY_CORE
#define INPUT_PWM_PIN PB2
#else
#pragma Only ATTiny 85 supported.
#endif

#define OUTPUT_UPDATE_PERIOD_MILLIS 50

TinyServoReader ServoReader(INPUT_PWM_PIN);


void setup()
{
	pinMode(PB1, OUTPUT);

	ServoReader.Start();
}

void loop()
{
	uint16_t PulseValue;
	if (ServoReader.GetServoPulseValue(PulseValue, OUTPUT_UPDATE_PERIOD_MILLIS))
	{
		analogWrite(PB1, map(PulseValue, 0, UINT16_MAX, 0, UINT8_MAX));
	}
}
