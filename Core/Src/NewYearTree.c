/** ____________________________________________________________________
 *
 * 	@file		NewYearTree.c
 *
 *	GitHub:		qsivey
 *	Telegram:	@qsivey
 *	Email:		qsivey@gmail.com
 *	____________________________________________________________________
 */

#include "NewYearTree.h"


NewYearTree_t NewYearTree;
NewYearTreeSettings_t NewYearTreeSettings;


/* ‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾
 *													 Interrupt Callbacks
 */
void HAL_TIM_PWM_PulseFinishedCallback(TIM_HandleTypeDef *htim)
{
	HAL_TIM_PWM_Stop_DMA(&htim1, TIM_CHANNEL_2);
}


void HAL_TIM_PeriodElapsedCallback (TIM_HandleTypeDef *htim)
{
	static ui8 counter = 0;
	static ui8 starStatus = 0;  // 0 = off | 1 = on

	if (htim->Instance == TIM3)
	{
		counter++;

		if (counter >= NewYearTree.starBrightnessLevels[NewYearTree.brightness])
		{
			if (starStatus == 1)
			{
				starTurnOFF_;
				starStatus = 0;
			}
		}

		else
			if (starStatus == 0)
			{
				starTurnON_;
				starStatus = 1;
			}


//		melodyTime++;
//
//		if (melodyTime > currentMelody[melodyCurrentNote].time)
//		{
//			melodyTime = 0;
//			melodyCurrentNote++;
//
//			/* Change frequency */
//			if (currentMelody[melodyCurrentNote].note != NOTE_MUTE)
//			{
//				__HAL_TIM_ENABLE(&htim6);
//				htim6.Instance->ARR = 1500000.0 / currentMelody[melodyCurrentNote].note;
//			}
//
//			else
//				__HAL_TIM_DISABLE(&htim6);
//
//			if (melodyCurrentNote >= currentMelodyDuration)
//				melodyCurrentNote = 0;  // or break
//		}


	}
}


/* ‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾
 *													   Adjunct Functions
 */
static void NYT_ConvertBrightness (ui8 *brightness)
{
	if (*brightness == 0) return;
	if (NewYearTree.brightness == 0) { *brightness = 0; return; }

	ui8 brightnessTemp = *brightness / (7.0F / ((float)NewYearTree.brightness));
	*brightness = (ui8)roundf(255.0f * powf((float)brightnessTemp / 255.0f, NYT_BRIGHTNESS_CURVE));
}


void NYT_HSBtoRGB (ui16 hue, ui8 saturation, ui8 brightness, ui8 *red, ui8 *green, ui8 *blue)
{
	NYT_ConvertBrightness(&brightness);

    float h = hue % 360;
    float s = saturation / 255.0f;
    float v = brightness / 255.0f;

    float c = v * s;
    float x = c * (1 - fabs(fmod(h / 60.0f, 2) - 1));
    float m = v - c;

    float r = 0, g = 0, b = 0;

    if (h >= 0 && h < 60) {
        r = c; g = x; b = 0;
    } else if (h >= 60 && h < 120) {
        r = x; g = c; b = 0;
    } else if (h >= 120 && h < 180) {
        r = 0; g = c; b = x;
    } else if (h >= 180 && h < 240) {
        r = 0; g = x; b = c;
    } else if (h >= 240 && h < 300) {
        r = x; g = 0; b = c;
    } else if (h >= 300 && h < 360) {
        r = c; g = 0; b = x;
    }

    *red = (ui8)((r + m) * 255.0f);
    *green = (ui8)((g + m) * 255.0f);
    *blue = (ui8)((b + m) * 255.0f);
}


void NYT_HSB256toRGB (ui8 hue, ui8 saturation, ui8 brightness, ui8 *red, ui8 *green, ui8 *blue)
{
	NYT_ConvertBrightness(&brightness);

    float h = hue * 360.0f / 255.0f;
    float s = saturation / 255.0f;
    float v = brightness / 255.0f;

    float c = v * s;
    float x = c * (1 - fabs(fmod(h / 60.0f, 2) - 1));
    float m = v - c;

    float r = 0, g = 0, b = 0;

    if (h >= 0 && h < 60) {
        r = c; g = x; b = 0;
    } else if (h >= 60 && h < 120) {
        r = x; g = c; b = 0;
    } else if (h >= 120 && h < 180) {
        r = 0; g = c; b = x;
    } else if (h >= 180 && h < 240) {
        r = 0; g = x; b = c;
    } else if (h >= 240 && h < 300) {
        r = x; g = 0; b = c;
    } else if (h >= 300 && h < 360) {
        r = c; g = 0; b = x;
    }

    *red = (ui8)((r + m) * 255.0f);
    *green = (ui8)((g + m) * 255.0f);
    *blue = (ui8)((b + m) * 255.0f);
}


/* ‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾
 *													   Service Functions
 */
void NYT_LED_SetColor (ui16 ledIndex, ui8 red, ui8 green, ui8 blue)
{
    if (ledIndex >= LED_COUNT) return;

    ui32 color = (red << 16) | (green << 8) | blue;

    for (ui8 i = 0; i < 24; i++)
    {
    	if (color & (1 << (23 - i)))
    		NewYearTree.ledData[ledIndex * 24 + i] = NYT_LED_LOGIC_HIGH;  // 1

        else
        	NewYearTree.ledData[ledIndex * 24 + i] = NYT_LED_LOGIC_LOW;  // 0
    }
}


void NYT_LED_Update (void)
{
    memset(&NewYearTree.ledData[LED_COUNT * 24], 0, LED_RESET_LEN);

    HAL_TIM_PWM_Start_DMA(&htim1, TIM_CHANNEL_2, (uint32_t*)NewYearTree.ledData, LED_DATA_LEN);
}


void NYT_LED_ClearAll (void)
{
	for (ui8 i = 0; i < LED_COUNT; i++)
		NYT_LED_SetColor(i, 0, 0, 0);

	NYT_LED_Update();
}


/* ‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾
 *																   Modes
 */
void NYT_LED_ModeStatic (void)
{
	ui8 red, green, blue;

	while (1)
	{
		NYT_HSBtoRGB(0, 0, MAX_BRIGHTNESS, &red, &green, &blue);

		for (ui16 i = 0; i < 16; i++)
			NYT_LED_SetColor(i, red, green, blue);

		NYT_LED_Update();
		HAL_Delay(10);
	}
}


/* All - hue */
void NYT_LED_Mode1 (void)
{
	nytAssert()

	ui8 red, green, blue;

	for (ui16 i = 0; i < 360; i++)
	{
		NYT_HSBtoRGB(i, NewYearTree.saturation, MAX_BRIGHTNESS, &red, &green, &blue);

		for (ui8 i = 0; i < LED_COUNT; i++)
			NYT_LED_SetColor(i, red, green, blue);

		NYT_LED_Update();

		nytDelay_(NewYearTree.delayArray[NewYearTree.speed], NYT_MODE1_CONST_SPEED);
	}
}


/* One by one - hue */
void NYT_LED_Mode2 (void)
{
	nytAssert()

	ui8 red, green, blue;
	ui8 led = 0;

	for (ui16 i = 0; i < 256; i += 4)
	{
		NYT_HSB256toRGB(i, NewYearTree.saturation, MAX_BRIGHTNESS, &red, &green, &blue);

		/* New LED */
		NYT_LED_SetColor(led, red, green, blue);

		/* Old LED */
		NYT_LED_SetColor(led ? (led - 1) : (LED_COUNT - 1), 0, 0, 0);

		led++;

		if (led >= 16)
			led = 0;

		NYT_LED_Update();

		nytDelay_(NewYearTree.delayArray[NewYearTree.speed], NYT_MODE2_CONST_SPEED);
	}
}


/* Z-axis - hue blinking */
void NYT_LED_Mode3 (void)
{
	nytAssert()

	ui8 red, green, blue;
	static ui8 hue = 0;

	for (ui8 i = 0; i < 4; i++)
	{
		NYT_HSB256toRGB(hue, NewYearTree.saturation, MAX_BRIGHTNESS, &red, &green, &blue);
		hue += 8;

		NYT_LED_SetColor(0 + i, red, green, blue);
		NYT_LED_SetColor(7 - i, red, green, blue);
		NYT_LED_SetColor(8 + i, red, green, blue);
		NYT_LED_SetColor(15 - i, red, green, blue);

		NYT_LED_Update();
		nytDelay_(NewYearTree.delayArray[NewYearTree.speed], NYT_MODE3_CONST_SPEED);

		/* Clear old */
		NYT_LED_SetColor(0 + i, 0, 0, 0);
		NYT_LED_SetColor(7 - i, 0, 0, 0);
		NYT_LED_SetColor(8 + i, 0, 0, 0);
		NYT_LED_SetColor(15 - i, 0, 0, 0);

		NYT_LED_Update();

		nytDelay_(50, 20);
	}
}


/* All - hue +60 degree */
void NYT_LED_Mode4 (void)
{
	nytAssert()

	ui8 red, green, blue;
	ui16 hue = 0;
	ui8 brightness = 1;
	ui8 direction = 0;

	while (1)
	{
		NYT_HSBtoRGB(hue, NewYearTree.saturation, brightness, &red, &green, &blue);

		for (ui8 k = 0; k < LED_COUNT; k++)
			NYT_LED_SetColor(k, red, green, blue);

		NYT_LED_Update();
		nytDelay_(NewYearTree.delayArray[NewYearTree.speed], NYT_MODE4_CONST_SPEED);

		if (brightness == 0)
		{
			brightness++;
			direction = 0;
			hue += 60;  // color step

			if (hue >= 360)
				return;
		}

		if (brightness == 255) direction = 1;

		if (direction) brightness--;
		else brightness++;
	}
}


/* Circle - hue */
void NYT_LED_Mode5 (void)
{
	nytAssert()

	ui8 red, green, blue;

	for (ui16 i = 0; i < 360; i++)
	{
		/* 0 - 3 */
		NYT_HSBtoRGB(i, NewYearTree.saturation, MAX_BRIGHTNESS, &red, &green, &blue);

		for (ui8 k = 0; k < (LED_COUNT / 4); k++)
			NYT_LED_SetColor(k, red, green, blue);

		/* 12 - 15 */
		NYT_HSBtoRGB(i + 90, NewYearTree.saturation, MAX_BRIGHTNESS, &red, &green, &blue);

		for (ui8 k = 0; k < (LED_COUNT / 4); k++)
			NYT_LED_SetColor(k + 12, red, green, blue);

		/* 4 - 7 */
		NYT_HSBtoRGB(i + 180, NewYearTree.saturation, MAX_BRIGHTNESS, &red, &green, &blue);

		for (ui8 k = 0; k < (LED_COUNT / 4); k++)
			NYT_LED_SetColor(k + 4, red, green, blue);

		/* 8 - 11 */
		NYT_HSBtoRGB(i + 270, NewYearTree.saturation, MAX_BRIGHTNESS, &red, &green, &blue);

		for (ui8 k = 0; k < (LED_COUNT / 4); k++)
			NYT_LED_SetColor(k + 8, red, green, blue);

		NYT_LED_Update();

		nytDelay_(NewYearTree.delayArray[NewYearTree.speed], NYT_MODE5_CONST_SPEED);
	}
}


/* Z-axis hue */
void NYT_LED_Mode6 (void)
{
	nytAssert()

	ui8 red, green, blue;

	for (ui16 i = 0; i < 360; i++)
	{
		/* 0 - 3 */
		NYT_HSBtoRGB(i, NewYearTree.saturation, MAX_BRIGHTNESS, &red, &green, &blue);

		NYT_LED_SetColor(0, red, green, blue);
		NYT_LED_SetColor(7, red, green, blue);
		NYT_LED_SetColor(8, red, green, blue);
		NYT_LED_SetColor(15, red, green, blue);

		/* 12 - 15 */
		NYT_HSBtoRGB(i + 90, NewYearTree.saturation, MAX_BRIGHTNESS, &red, &green, &blue);

		NYT_LED_SetColor(1, red, green, blue);
		NYT_LED_SetColor(6, red, green, blue);
		NYT_LED_SetColor(9, red, green, blue);
		NYT_LED_SetColor(14, red, green, blue);

		/* 4 - 7 */
		NYT_HSBtoRGB(i + 180, NewYearTree.saturation, MAX_BRIGHTNESS, &red, &green, &blue);

		NYT_LED_SetColor(2, red, green, blue);
		NYT_LED_SetColor(5, red, green, blue);
		NYT_LED_SetColor(10, red, green, blue);
		NYT_LED_SetColor(13, red, green, blue);

		/* 8 - 11 */
		NYT_HSBtoRGB(i + 270, NewYearTree.saturation, MAX_BRIGHTNESS, &red, &green, &blue);

		NYT_LED_SetColor(3, red, green, blue);
		NYT_LED_SetColor(4, red, green, blue);
		NYT_LED_SetColor(11, red, green, blue);
		NYT_LED_SetColor(12, red, green, blue);

		NYT_LED_Update();

		nytDelay_(NewYearTree.delayArray[NewYearTree.speed], NYT_MODE6_CONST_SPEED);
	}
}


/* Random 1 LED */
void NYT_LED_Mode7 (void)
{
	nytAssert()

	ui8 brightness = 1;
	ui8 color = 0;
	ui8 LED_Number;

	LED_Number = rand() % 16;

	for (ui16 i = 0; i < 256; i++)
	{
		brightness = i;

		NYT_ConvertBrightness(&brightness);

		if (color == 0)
			NYT_LED_SetColor(LED_Number, brightness, 0, 0);

		else if (color == 1)
			NYT_LED_SetColor(LED_Number, 0, brightness, 0);

		else
			NYT_LED_SetColor(LED_Number, 0, 0, brightness);

		NYT_LED_Update();

		nytDelay_(NewYearTree.delayArray[NewYearTree.speed], NYT_MODE7_CONST_SPEED);
	}

	for (ui16 i = 255; i > 0; i--)
	{
		brightness = i;

		NYT_ConvertBrightness(&brightness);

		if (color == 0)
			NYT_LED_SetColor(LED_Number, brightness, 0, 0);

		else if (color == 1)
			NYT_LED_SetColor(LED_Number, 0, brightness, 0);

		else
			NYT_LED_SetColor(LED_Number, 0, 0, brightness);

		NYT_LED_Update();

		nytDelay_(NewYearTree.delayArray[NewYearTree.speed], NYT_MODE7_CONST_SPEED);
	}

	color++;

	if (color >= 3)
		color = 0;
}


/* Random absolute */
void NYT_LED_Mode8 (void)
{
	nytAssert()

	ui8 red, green, blue;

	for (ui8 i = 0; i < LED_COUNT; i++)
	{
		red = rand() % 256;
		green = rand() % 256;
		blue = rand() % 256;

		NYT_ConvertBrightness(&red);
		NYT_ConvertBrightness(&green);
		NYT_ConvertBrightness(&blue);

		NYT_LED_SetColor(i, red, green, blue);
	}

	NYT_LED_Update();

	nytDelay_(NewYearTree.delayArray[NewYearTree.speed], NYT_MODE8_CONST_SPEED);
}


/* Turn on one by one and turn off one by one */
void NYT_LED_Mode9 (void)
{
	nytAssert()

	ui8 red, green, blue;

	const ui8 LED_Order [LED_COUNT] = { 3, 12, 4, 11, 13, 5, 10, 2, 6, 9, 1, 14, 8, 0, 15, 7 };
	ui8 brightnessArr [LED_COUNT] = { 0 };
	static ui8 countLED = 0;

	ui16 i;

	while (brightnessArr[LED_COUNT - 1] < 255)
	{
		for (i = 0; i < 360; i++)
		{
			brightnessArr[countLED]++;

			for (ui8 k = 0; k < countLED + 1; k++)
			{
				NYT_HSBtoRGB(i + ((k % 4) * 90), NewYearTree.saturation, brightnessArr[k], &red, &green, &blue);
				NYT_LED_SetColor(LED_Order[k], red, green, blue);
			}

			NYT_LED_Update();

			nytDelay_(NewYearTree.delayArray[NewYearTree.speed], NYT_MODE9_CONST_SPEED);

			if (brightnessArr[countLED] >= MAX_BRIGHTNESS)
			{
				countLED++;

				if (countLED == LED_COUNT)
					break;
			}
		}
	}

	ui8 countLED_Sub = 0;

	while (brightnessArr[LED_COUNT - 1] > 1)
	{
		for (; i < 360; i++)
		{
			brightnessArr[countLED_Sub]--;

			for (ui8 k = (LED_COUNT - countLED); k < LED_COUNT; k++)
			{
				NYT_HSBtoRGB(i + ((k % 4) * 90), NewYearTree.saturation, brightnessArr[k], &red, &green, &blue);
				NYT_LED_SetColor(LED_Order[k], red, green, blue);
			}

			NYT_LED_Update();

			nytDelay_(NewYearTree.delayArray[NewYearTree.speed], NYT_MODE9_CONST_SPEED);

			if (brightnessArr[countLED_Sub] == 0)
			{
				countLED--;
				countLED_Sub++;
			}

			if (!countLED)
				break;
		}

		i = 0;
	}
}


/* Random color refreshing */
void NYT_LED_Mode10 (void)
{
	ui8 LED_Order [LED_COUNT];
	ui8 LED_Number;
	ui8 passFlag = 1;

	ui8 red, green, blue;

	memset(LED_Order, 0xFF, LED_COUNT);

	for (ui16 i = 0; i < LED_COUNT; )
	{
		passFlag = 1;

		LED_Number = rand() % LED_COUNT;

		for (ui16 k = 0; k < LED_COUNT; k++)
		{
			if (LED_Order[k] == LED_Number)
			{
				passFlag = 0;
				break;
			}
		}

		if (passFlag)
		/* Flash new LED */
		{
			LED_Order[i] = LED_Number;

			red = rand() % 256;
			green = rand() % 256;
			blue = rand() % 256;

			NYT_ConvertBrightness(&red);
			NYT_ConvertBrightness(&green);
			NYT_ConvertBrightness(&blue);

			NYT_LED_SetColor(LED_Order[i], red, green, blue);

			NYT_LED_Update();

			nytDelay_(NewYearTree.delayArray[NewYearTree.speed], NYT_MODE11_CONST_SPEED);

			i++;
		}
	}
}


/* Add and sub, random color */
void NYT_LED_Mode11 (void)
{
	ui8 LED_Order [LED_COUNT];
	ui8 LED_Number;
	ui8 passFlag = 1;

	ui8 red, green, blue;

	memset(LED_Order, 0xFF, LED_COUNT);

	for (ui16 i = 0; i < LED_COUNT; )
	{
		passFlag = 1;

		LED_Number = rand() % LED_COUNT;

		for (ui16 k = 0; k < LED_COUNT; k++)
		{
			if (LED_Order[k] == LED_Number)
			{
				passFlag = 0;
				break;
			}
		}

		if (passFlag)
		/* Flash new LED */
		{
			LED_Order[i] = LED_Number;

			red = rand() % 256;
			green = rand() % 256;
			blue = rand() % 256;

			NYT_ConvertBrightness(&red);
			NYT_ConvertBrightness(&green);
			NYT_ConvertBrightness(&blue);

			NYT_LED_SetColor(LED_Order[i], red, green, blue);

			NYT_LED_Update();

			nytDelay_(NewYearTree.delayArray[NewYearTree.speed], NYT_MODE11_CONST_SPEED);

			i++;
		}
	}

	/* Sub */
	memset(LED_Order, 0xFF, LED_COUNT);

	for (ui16 i = 0; i < LED_COUNT; )
	{
		passFlag = 1;

		LED_Number = rand() % LED_COUNT;

		for (ui16 k = 0; k < LED_COUNT; k++)
		{
			if (LED_Order[k] == LED_Number)
			{
				passFlag = 0;
				break;
			}
		}

		if (passFlag)
		/* Flash new LED */
		{
			LED_Order[i] = LED_Number;

			NYT_LED_SetColor(LED_Order[i], 0, 0, 0);

			NYT_LED_Update();

			nytDelay_(NewYearTree.delayArray[NewYearTree.speed], NYT_MODE11_CONST_SPEED);

			i++;
		}
	}
}


/* Circle - hue, Z- axis - saturation */
void NYT_LED_Mode12 (void)
{
	nytAssert()

	// todo
	ui8 red, green, blue;

	for (ui16 i = 0; i < 360; i++)
	{
		/* 0 - 3 */
		for (ui8 k = 0; k < (LED_COUNT / 4); k++)
		{
			NYT_HSBtoRGB(i, (4 - k) * 63, MAX_BRIGHTNESS, &red, &green, &blue);
			NYT_LED_SetColor(k, red, green, blue);
		}

		/* 12 - 15 */
		for (ui8 k = 0; k < (LED_COUNT / 4); k++)
		{
			NYT_HSBtoRGB(i + 90, (k + 1) * 63, MAX_BRIGHTNESS, &red, &green, &blue);
			NYT_LED_SetColor(k + 12, red, green, blue);
		}

		/* 4 - 7 */
		for (ui8 k = 0; k < (LED_COUNT / 4); k++)
		{
			NYT_HSBtoRGB(i + 180, (k + 1) * 63, MAX_BRIGHTNESS, &red, &green, &blue);
			NYT_LED_SetColor(k + 4, red, green, blue);
		}

		/* 8 - 11 */
		for (ui8 k = 0; k < (LED_COUNT / 4); k++)
		{
			NYT_HSBtoRGB(i + 270, (4 - k) * 63, MAX_BRIGHTNESS, &red, &green, &blue);
			NYT_LED_SetColor(k + 8, red, green, blue);
		}

		NYT_LED_Update();

		nytDelay_(NewYearTree.delayArray[NewYearTree.speed], NYT_MODE12_CONST_SPEED);
	}
}


/* Just red */
void NYT_LED_ModeSimpleColor (void)
{
	nytAssert()

	ui8 red, green, blue;

	if (NewYearTree.staticColorState != NYTSCS_WARM_LIGHT)
		NYT_HSBtoRGB((NewYearTree.staticColorState - 1) * NYT_HUE_CIRCLE_STEP, NewYearTree.saturation, MAX_BRIGHTNESS, &red, &green, &blue);

	else
		NYT_HSBtoRGB(17, MAX_SATURATION / 1.06, MAX_BRIGHTNESS, &red, &green, &blue);

	for (ui8 i = 0; i < LED_COUNT; i++)
		NYT_LED_SetColor(i, red, green, blue);

	NYT_LED_Update();
}
