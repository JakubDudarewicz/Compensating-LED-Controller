/*******************************************************
Title:			Ambient Light Compensating LED Controller
Filename:		'main.c'
Author:			Jakub Dudarewicz
Created:		02-2018
Version:		0.1
Target MCU:		Atmel AVR

This code is distributed under the GNU Public License
which can be found at http://www.gnu.org/licenses/gpl.txt
*******************************************************/

#include "main.h"

PID rPID, gPID, bPID;
float dayBright = 2.7, nightBright = 1; //brightness gain
int nrgoal = 510, nggoal = 386, nbgoal = 282; //night goals
int drgoal = 510, dggoal = 494, dbgoal = 476; //day goals
int dayLookupRow = 6, nightLookupRow = 3; //current selected CCT
const int lookup[LOOKUPLENGTH + 1][4] = {
	{6, 255, 18, 0},
	{10, 255, 68, 0},
	{20, 255, 137, 14},
	{30, 255, 178, 110},
	{40, 255, 206, 167},
	{50, 255, 229, 206},
	{60, 255, 247, 238},
	{65, 255, 255, 255},
	{70, 243, 243, 255},
	{90, 210, 223, 225}
};
char buf, string[16];
int h, m, s = 0, sbuf = -1;
unsigned char refreshPending = TRUE, day = TRUE;
uint32_t time;

int main(void)
{
	MCUCR |= _BV(JTD);
	MCUCR |= _BV(JTD);
	// disable JTAG to obtain control of PORTC as general I/O

	TCCR1A = _BV(WGM11)
	       | _BV(COM1A1);
	TCCR1B = _BV(WGM12)
		   | _BV(WGM13)
		   | _BV(CS10);
	ICR1 = 0x03FF;
	TCCR2A = _BV(WGM21)
		   | _BV(WGM20)
		   | _BV(COM2A1)
		   | _BV(COM2B1);
	TCCR2B = _BV(CS20);

	DDRD = (_BV(5) | _BV(6) | _BV(7));

	REDPWM = 0;
	GREENPWM = 0;
	BLUEPWM = 0;

	KEYDDR = 0x00;
	KEYPORT = 0x00;

	i2cInit();
	setTime(14, 38, 25);
	RTCStart();

	lcd_init();
	lcd_clrscr();

	writeCustom(UPARROW, upArrow);
	writeCustom(DOWNARROW, downArrow);
	menuDirection = UP;
	appendItem("TOGGLE LIGHT", &toggleLight);
	appendItem("MANUAL RGB", &setRGB);
	appendItem("SET NIGHT TEMP", &setNightGoal);
	appendItem("SET DAY TEMP", &setDayGoal);
	appendItem("SET DAY BRIGHT", &setDayBright);
	appendItem("SET NIGHT BRIGHT", &setNightBright);

	PIDinit(&rPID, PGAIN * 5, IGAIN * 5, DGAIN * 4, 10000, -10000);
	PIDinit(&gPID, PGAIN, IGAIN, DGAIN, 10000, -10000);
	PIDinit(&bPID, PGAIN, IGAIN, DGAIN, 10000, -10000);

	while(1)
	{
		h = getTime(HOURS);
		m = getTime(MINUTES);
		s = getTime(SECONDS);

		if(sbuf != s) refreshPending = TRUE;
		sbuf = s;

		if((h < 18) && (h > 6)) day = TRUE;
		
		LEDControl();

		buf = keyScan();
		if (buf != NOKEY)
		{
			refreshPending = TRUE;
			switch (buf)
			{
				case AKEY:
				prevMenu();
				break;
				case BKEY:
				nextMenu();
				break;
				case CKEY:
				if(selectedItem->menuPressed != NULL)
					selectedItem->menuPressed();
				break;
			}
		}
		if (refreshPending == TRUE)
		{
			lcd_clrscr();

			itoa(h, string, 10);
			lcd_puts(string);
			lcd_putc(':');
			itoa(m, string, 10);
			lcd_puts(string);
			lcd_putc(':');
			itoa(s, string, 10);
			lcd_puts(string);

			lcd_goto_xy(12, 0);
			if (day)
			{
				lcd_puts(" DAY");
			}else{
				lcd_puts(" NIGHT");
			}

			lcd_goto_xy(0, 1);

			lcd_puts(selectedItem->itemName);
			refreshPending = FALSE;
		}
    }
}

void LEDControl(){
	int rbuf, gbuf, bbuf, lr, lg, lb;
	rbuf = sensorScan(RED, ACCURACY);
	gbuf = sensorScan(GREEN, ACCURACY);
	bbuf = sensorScan(BLUE, ACCURACY);

	if (day)
	{
		lr = dayBright  * lookup[dayLookupRow][1];
		lg = dayBright  * lookup[dayLookupRow][2];
		lb = dayBright  * lookup[dayLookupRow][3];
	}else{
		lr = nightBright * lookup[nightLookupRow][1];
		lg = nightBright * lookup[nightLookupRow][2];
		lb = nightBright * lookup[nightLookupRow][3];
	}
	
	limitAdd16bit(&REDPWM, updatePID(&rPID, lr, rbuf), 0x03FF);
	limitAdd8bit(&GREENPWM, updatePID(&gPID, lg, gbuf), 0xFF);
	limitAdd8bit(&BLUEPWM, updatePID(&bPID, lb, bbuf), 0xFF);
}

void setRGB(){
	REDPWM = getInt("R:", 255);
	REDPWM *= 4; //10-bit
	GREENPWM = getInt("G:", 255); //8-bit
	BLUEPWM = getInt("B:", 255); //8-bit
	lcd_clrscr();
	lcd_puts("C to quit");
	refreshPending = TRUE;
	while (1)
	{
		buf = keyScan();
		if(buf == CKEY) break;
	}
}

void setNightBright(){
	char string[20], buf, refreshPending = TRUE;
	while (1)
	{
		LEDControl();
		buf = keyScan();
		if (buf != NOKEY)
		{
			switch (buf)
			{
				case CKEY:
				return;
				break;
				case AKEY:
				if(nightBright < 2.9) nightBright += 0.1;
				refreshPending = TRUE;
				break;
				case BKEY:
				if(dayLookupRow > 0.1) nightBright -= 0.1;
				refreshPending = TRUE;
				break;
				default:
				break;
			}
		}
		if (refreshPending == TRUE)
		{
			lcd_clrscr();
			lcd_puts("BRIGHTNESS:");
			dtostrf(nightBright, 1, 2, string);
			lcd_puts(string);
			refreshPending = FALSE;
		}
	}
}

void setDayBright(){
	char string[20], buf, refreshPending = TRUE;
	while (1)
	{
		LEDControl();
		buf = keyScan();
		if (buf != NOKEY)
		{
			switch (buf)
			{
				case CKEY:
				return;
				break;
				case AKEY:
				if(dayBright < 2.9) dayBright += 0.1;
				refreshPending = TRUE;
				break;
				case BKEY:
				if(dayLookupRow > 0.1) dayBright -= 0.1;
				refreshPending = TRUE;
				break;
				default:
				break;
			}
		}
		if (refreshPending == TRUE)
		{
			lcd_clrscr();
			lcd_puts("BRIGHTNESS:");
			dtostrf(dayBright, 1, 2, string);
			lcd_puts(string);
			refreshPending = FALSE;
		}
	}
}

void setDayGoal(){
	dayLookupRow = 7;
	char string[20], buf, refreshPending = TRUE;
	while (1)
	{
		buf = keyScan();
		if (buf != NOKEY)
		{
			switch (buf)
			{
				case CKEY:
					return;
					break;
				case AKEY:
					if(dayLookupRow != LOOKUPLENGTH)
						dayLookupRow++;
					refreshPending = TRUE;
					break;
				case BKEY:
					if(dayLookupRow != 0)
						dayLookupRow--;
					refreshPending = TRUE;
					break;
				default:
				break;
			}
		}
		if (refreshPending == TRUE)
		{
			lcd_clrscr();
			itoa(lookup[dayLookupRow][0], string, 10);
			lcd_puts(string);
			lcd_puts("00 K");
			refreshPending = FALSE;
		}
	}
}

void setNightGoal(){
	nightLookupRow = 5;
	char string[20], buf, refreshPending = TRUE;
	while (1)
	{
		buf = keyScan();
		if (buf != NOKEY)
		{
			switch (buf)
			{
				case CKEY:
					return;
					break;
				case AKEY:
					if(nightLookupRow != LOOKUPLENGTH)
						nightLookupRow++;
					refreshPending = TRUE;
					break;
				case BKEY:
					if(nightLookupRow != 0)
						nightLookupRow--;
					refreshPending = TRUE;
					break;
				default:
					break;
			}
		}
		if (refreshPending == TRUE)
		{
			lcd_clrscr();
			itoa(lookup[nightLookupRow][0], string, 10);
			lcd_puts(string);
			lcd_puts("00 K");
			refreshPending = FALSE;
		}
	}
}

void toggleLight(){
	DDRD ^= _BV(7) | _BV(6) | _BV(5);
}

void limitAdd8bit(volatile uint8_t *PWMCHANNEL,
				  int value,
				  int limit){
	int sum = *PWMCHANNEL + value;
	if(sum > limit){
		*PWMCHANNEL = limit;
		return;
	}
	if(sum < 0){
		*PWMCHANNEL = 0;
		return;
	}
	*PWMCHANNEL = sum;
}

void limitAdd16bit(volatile uint16_t *PWMCHANNEL,
				   int value,
				   int limit){
	int sum = *PWMCHANNEL + value;
	if(sum > limit){
		*PWMCHANNEL = limit;
		return;
	}
	if(sum < 0){
		*PWMCHANNEL = 0;
		return;
	}
	*PWMCHANNEL = sum;
}