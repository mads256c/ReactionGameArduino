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

//Bitmap definitions. Used to draw numbers / letters
constexpr uint8_t None = B00000000;
constexpr uint8_t Zero = B01111110;
constexpr uint8_t One = B00010010;
constexpr uint8_t Two = B10111100;
constexpr uint8_t Three = B10110110;
constexpr uint8_t Four = B11010010;
constexpr uint8_t Five = B11100110;
constexpr uint8_t Six = B11101110;
constexpr uint8_t Seven = B00110010;
constexpr uint8_t Eight = B11111111;
constexpr uint8_t Nine = B11110110;
constexpr uint8_t A = B11111010;
constexpr uint8_t B = B00111110;
constexpr uint8_t C = B10011100;
constexpr uint8_t D = B01111010;
constexpr uint8_t E = B10011110;
constexpr uint8_t F = B11101000;
constexpr uint8_t I = B01001000;
constexpr uint8_t L = B01001100;

//Draws the fail message when the user pressed the wrong button.
void drawFail()
{
	//Reset displays
	for (size_t i = 2; i < 9; i++)
	{
		digitalWrite(i, HIGH);
	}

	for (size_t i = 22; i < BUTTON_MIN; i++)
	{
		digitalWrite(i, HIGH);
	}

	drawBitmap(F, 22); //First 7-seg is starting on pin 2.
	drawBitmap(A, 30); //Etc.
	drawBitmap(I, 38);
	drawBitmap(L, 45);
}

//Draws a number (0-9999).
void drawNumber(const uint16_t number)
{
	//Reset displays
	for (size_t i = 2; i < 9; i++)
	{
		digitalWrite(i, HIGH);
	}

	for (size_t i = 22; i < BUTTON_MIN; i++)
	{
		digitalWrite(i, HIGH);
	}

	//Split the number into digits.
	const auto firstDigit = number / 1000 % 10;
	const auto secondDigit = number / 100 % 10;
	const auto thirdDigit = number / 10 % 10;
	const auto fourthDigit = number % 10;

	drawBitmap(digitToBitmap(firstDigit), 22); //First 7-seg is starting on pin 2.
	drawBitmap(digitToBitmap(secondDigit), 30); //Etc.
	drawBitmap(digitToBitmap(thirdDigit), 38);
	drawBitmap(digitToBitmap(fourthDigit), 45);
}

//Converts a digit (0-9) to a bitmask.
uint8_t digitToBitmap(const uint8_t number)
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
void drawBitmap(const uint8_t bitmap, const uint8_t startPin)
{
	//Go through all the bits and check if they are set in bitmask
	for (uint8_t i = 0; i < 7; i++)
	{
		const uint8_t bitshift = B10000000 >> i; 

		if ((bitshift & bitmap) == bitshift)
		{
			if (i + startPin > 44)
				digitalWrite((i + startPin) - 45  + 2, LOW);
			else
				digitalWrite(i + startPin, LOW);
		}
	}
}

//Runs once at startup
void setup()
{
	//Set up pinmode for the buttons.
	for (uint8_t i = BUTTON_MIN; i < BUTTON_MAX + 1; i++)
	{
		pinMode(i, INPUT);
	}

	//Setup pinmode for leds
	for (uint8_t i = LED_MIN; i < LED_MAX + 1; i++)
	{
		pinMode(i, OUTPUT);
	}

	//Setup pinmodes for the 7-seg displays.
	for (uint8_t i = 2; i < 9; i++)
	{
		pinMode(i, OUTPUT);

		digitalWrite(i, HIGH);
	}

	for (uint8_t i = 22; i < BUTTON_MIN; i++)
	{
		pinMode(i, OUTPUT);

		digitalWrite(i, HIGH);
	}

}

//Runs forever
void loop() {

	delay(random(TIME_MIN, TIME_MAX));
	
	//Find a random button and the corrosponding led.
	const uint8_t randomButton = random(BUTTON_MIN, BUTTON_MAX + 1);
	const uint8_t randomLED = randomButton + (LED_MIN - BUTTON_MIN);

	digitalWrite(randomLED, HIGH); //Turn on the led.

	const unsigned long before = millis(); //Get the time before any button press.

	//While loop, but exitLoop is in the scope.
	for (bool exitLoop = false; !exitLoop;) {

		//For each button.
		for (uint8_t i = BUTTON_MIN; i < BUTTON_MAX + 1; i++)
		{
			//If the button i is pressed.
			if (digitalRead(i) == HIGH)
			{
				//If i is the button chosen.
				if (i == randomButton)
				{
					const unsigned long after = millis(); //Get the time

					unsigned long timeTaken = after - before; //Calculate the difference.

					//If timetaken is over 9999 just print 9999.
					if (timeTaken > 9999)
						timeTaken = 9999;

					drawNumber(timeTaken); //Draw the time taken.
				}
				else
				{
					drawFail(); //Draw fail
				}


				exitLoop = true;
				break;
			}

		}

	}

	digitalWrite(randomLED, LOW); //Turn off the led.

}

