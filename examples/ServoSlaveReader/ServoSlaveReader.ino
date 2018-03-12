#include <TinyServoReader.h>

#ifdef ATTINY_CORE
#define INPUT_PWM_PIN PB2
#define OUTPUT_A1_PIN PB0
#define OUTPUT_A2_PIN PB1
#else
#pragma Only ATTiny 85 supported.
#endif

#define OUTPUT_UPDATE_PERIOD_MILLIS 200

TinyServoReader ServoReader(INPUT_PWM_PIN);

uint32_t LastUpdated = 0;

void setup()
{
	Serial.begin(57600);

	ServoReader.Begin();
}

void loop()
{
	if (millis() - LastUpdated > OUTPUT_UPDATE_PERIOD_MILLIS)
	{
		LastUpdated = millis();
		ServoReader.Invalidate();
		if (ServoReader.HasPulseDuration())
		{
			Serial.print(F("Input Value: "));
			Serial.println(ServoReader.GetValue());
		}
	}
}