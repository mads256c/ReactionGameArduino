/*
 Name:		ReactionGameArduino.ino
 Created:	9/6/2018 1:16:02 PM
 Author:	Mads
*/

#define TIME_MIN 500
#define TIME_MAX 3000

#define BUTTON_MIN 47
#define BUTTON_MAX 49
#define LED_MIN 9
#define LED_MAX 11


constexpr uint8_t None = B00000000;
constexpr uint8_t Zero = B11111100;
constexpr uint8_t One = B01100000;
constexpr uint8_t Two = B11011010;
constexpr uint8_t Three = B11110010;
constexpr uint8_t Four = B01100110;
constexpr uint8_t Five = B10110110;
constexpr uint8_t Six = B10111110;
constexpr uint8_t Seven = B11100000;
constexpr uint8_t Eight = B11111111;
constexpr uint8_t Nine = B11110110;
constexpr uint8_t A = B11101110;
constexpr uint8_t B = B00111110;
constexpr uint8_t C = B10011100;
constexpr uint8_t D = B01111010;
constexpr uint8_t E = B10011110;
constexpr uint8_t F = B10001110;
constexpr uint8_t I = B00001100;
constexpr uint8_t L = B00011100;


void drawFail()
{
	//Reset displays
	for (size_t i = 2; i < 9; i++)
	{
		digitalWrite(i, HIGH);
	}

	for (size_t i = 22; i < 43; i++)
	{
		digitalWrite(i, HIGH);
	}

	drawBitmask(F, 2); //First 7-seg is starting on pin 2.
	drawBitmask(A, 22); //Etc.
	drawBitmask(I, 29);
	drawBitmask(L, 36);
}

//Draws a number (0-9999).
void drawNumber(const uint16_t number)
{
	//Reset displays
	for (size_t i = 2; i < 9; i++)
	{
		digitalWrite(i, HIGH);
	}

	for (size_t i = 22; i < 43; i++)
	{
		digitalWrite(i, HIGH);
	}

	//Split the number into digits.
	const auto firstDigit = number / 1000 % 10;
	const auto secondDigit = number / 100 % 10;
	const auto thirdDigit = number / 10 % 10;
	const auto fourthDigit = number % 10;

	drawBitmask(digitToBitmask(firstDigit), 2); //First 7-seg is starting on pin 2.
	drawBitmask(digitToBitmask(secondDigit), 22); //Etc.
	drawBitmask(digitToBitmask(thirdDigit), 29);
	drawBitmask(digitToBitmask(fourthDigit), 36);
}

//Converts a digit (0-9) to a bitmask.
uint8_t digitToBitmask(const uint8_t number)
{
	switch (number)
	{
	case 0: return Zero;
	case 1: return One;
	case 2: return Two;
	case 3: return Three;
	case 4: return Four;
	case 5: return Five;
	case 6: return Six;
	case 7: return Seven;
	case 8: return Eight;
	case 9: return Nine;
	default: return None;
	}
}

//Draws a bitmask
void drawBitmask(const uint8_t bitmask, const uint8_t startPin)
{
	for (uint8_t i = 0; i < 7; i++)
	{
		const uint8_t bitshift = B10000000 >> i;

		if ((bitshift & bitmask) == bitshift)
		{
			digitalWrite(i + startPin, LOW);
		}
	}
}

void setup()
{
	//Set up pinmode for the buttons.
	for (size_t i = BUTTON_MIN; i < BUTTON_MAX + 1; i++)
	{
		pinMode(i, INPUT);
	}

	//Setup pinmode for leds
	for (size_t i = LED_MIN; i < LED_MAX + 1; i++)
	{
		pinMode(i, OUTPUT);
	}

	//Setup pinmodes for the 7-seg displays.
	for (size_t i = 2; i < 9; i++)
	{
		pinMode(i, OUTPUT);

		digitalWrite(i, HIGH);
	}

	for (size_t i = 22; i < 43; i++)
	{
		pinMode(i, OUTPUT);

		digitalWrite(i, HIGH);
	}

}

void loop() {
	delay(random(TIME_MIN, TIME_MAX));

	const uint8_t randomButton = random(BUTTON_MIN, BUTTON_MAX + 1);
	const uint8_t randomLED = randomButton + (LED_MIN - BUTTON_MIN);

	digitalWrite(randomLED, HIGH);
	const unsigned long before = millis();

	//While loop, but exitLoop is in the scope.
	for (bool exitLoop = false; !exitLoop;) {
		for (uint8_t i = BUTTON_MIN; i < BUTTON_MAX + 1; i++)
		{
			if (digitalRead(i) == HIGH)
			{
				if (i == randomButton)
				{
					const unsigned long after = millis();

					unsigned long timeTaken = after - before;

					if (timeTaken > 9999)
						timeTaken = 9999;

					drawNumber(timeTaken);
				}
				else
				{
					drawFail();
				}


				exitLoop = true;
				break;
			}

		}

	}

	digitalWrite(randomLED, LOW);

}

