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
	Serial.begin(57600);

	ServoReader.Start();
}

void loop()
{
	uint16_t PulseValue;
	if (ServoReader.GetServoPulseValue(PulseValue, OUTPUT_UPDATE_PERIOD_MILLIS))
	{
		Serial.print(F("Input Value: "));
		Serial.println(PulseValue);
	}
}
