/*
 * CompensatingController.c
 *
 * Created: 2017-10-15 11:51:47 AM
 * Author : Jakub Dudarewicz
 */

#define F_CPU 1000000UL

#include "main.h"

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
	appendItem("SET NIGHT TEMP", &setNightGoal);
	appendItem("SET DAY TEMP", &setDayGoal);
	appendItem("SET DAY BRIGHT", &setDayBright);
	appendItem("SET NIGHT BRIGHT", &setNightBright);

	PIDinit(&rPID, 0.04, 0.004, 0, 10000, 0);
	PIDinit(&gPID, 0.01, 0.001, 0, 10000, 0);
	PIDinit(&bPID, 0.01, 0.001, 0, 10000, 0);

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
	int rbuf, gbuf, bbuf, lr, lg, lb, h;
	rbuf = sensorScan(RED, ACCURACY);
	gbuf = sensorScan(GREEN, ACCURACY);
	bbuf = sensorScan(BLUE, ACCURACY);

	drgoal = dayBright * lookup[dayLookupRow][1];
	dggoal = dayBright * lookup[dayLookupRow][2];
	dbgoal = dayBright * lookup[dayLookupRow][3];

	nrgoal = nightBright * lookup[nightLookupRow][1];
	nggoal = nightBright * lookup[nightLookupRow][2];
	nbgoal = nightBright * lookup[nightLookupRow][3];

	if (day)
	{
		lr = drgoal;
		lg = dggoal;
		lb = dbgoal;
	}else{
		lr = nrgoal;
		lg = nggoal;
		lb = nbgoal;
	}
	
	limitAdd16bit(&REDPWM,
				  updatePID(&rPID, lr, rbuf),
				  0x03FF);
	limitAdd8bit(&GREENPWM,
				 updatePID(&gPID, lg, gbuf),
				 0xFF);
	limitAdd8bit(&BLUEPWM,
				 updatePID(&bPID, lb, bbuf),
				 0xFF);
}

void setRGB(){
	REDPWM = getInt("R:", 255);
	GREENPWM = getInt("G:", 255);
	BLUEPWM = getInt("B:", 255);
	lcd_clrscr();
	lcd_puts("PRESS A");
	refreshPending = TRUE;
	while (1)
	{
		buf = keyScan();
		if(buf == AKEY) break;
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
	uint32_t temp = lookup[7][0];
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
	uint32_t temp = lookup[5][0];
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

void blinkLight(){
	for (int i = 0; i < 10; i++){
		DDRD ^= _BV(7) | _BV(6) | _BV(5);
		_delay_ms(200);
	}
}

void displayTime(){
	char string[20];
	lcd_clrscr();
	itoa(getTime(HOURS), string, 10);
	lcd_puts(string);
	lcd_putc(':');
	itoa(getTime(MINUTES), string, 10);
	lcd_puts(string);
	lcd_putc(':');
	itoa(getTime(SECONDS), string, 10);
	lcd_puts(string);
	_delay_ms(2000);
}

void displayLight(){
	int i = 0, rbuf, gbuf, bbuf;
	char string[20], buf;
	while (1)
	{
		buf = keyScan();
		if (buf != NOKEY)
		{
			switch (buf)
			{
				case AKEY:
				return;
				case ONEKEY:
				REDPWM += 8;
				break;
				case FOURKEY:
				REDPWM -= 8;
				break;
				case TWOKEY:
				GREENPWM += 8;
				break;
				case FIVEKEY:
				GREENPWM -= 8;
				break;
				case THREEKEY:
				BLUEPWM += 8;
				break;
				case SIXKEY:
				BLUEPWM -= 8;
				break;
				case BKEY:
				REDPWM = 128;
				GREENPWM = 128;
				BLUEPWM = 128;
				break;
				default:
				break;
			}
		}
		rbuf = sensorScan(RED, ACCURACY);
		gbuf = sensorScan(GREEN, ACCURACY);
		bbuf = sensorScan(BLUE, ACCURACY);
		lcd_clrscr();
		itoa(rbuf, string, 10);
		lcd_puts(string);
		lcd_putc(' ');
		itoa(gbuf, string, 10);
		lcd_puts(string);
		lcd_putc(' ');
		itoa(bbuf, string, 10);
		lcd_puts(string);

		lcd_goto_xy(0,1);
		itoa(REDPWM, string, 16);
		lcd_puts(string);
		lcd_putc(' ');
		itoa(GREENPWM, string, 16);
		lcd_puts(string);
		lcd_putc(' ');
		itoa(BLUEPWM, string, 16);
		lcd_puts(string);
		lcd_puts(" PWM");

		if (i < BUFLENGTH) i++;
		else i = 0;
	}
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

void goalEdit(){
	char string[20], buf;
	while (1)
	{
		LEDControl();
		buf = keyScan();
		if (buf != NOKEY)
		{
			switch (buf)
			{
				case AKEY:
				return;
				case ONEKEY:
				rgoal += 10;
				break;
				case FOURKEY:
				rgoal -= 10;
				break;
				case TWOKEY:
				ggoal += 10;
				break;
				case FIVEKEY:
				ggoal -= 10;
				break;
				case THREEKEY:
				bgoal += 10;
				break;
				case SIXKEY:
				bgoal -= 10;
				break;
				default:
				break;
			}
		}
		lcd_clrscr();
		itoa(rgoal, string, 10);
		lcd_puts(string);
		lcd_putc(' ');
		itoa(ggoal, string, 10);
		lcd_puts(string);
		lcd_putc(' ');
		itoa(bgoal, string, 10);
		lcd_puts(string);
	}
}

void PIDedit(){
	char string[20], buf;
	while (1)
	{
		LEDControl();
		buf = keyScan();
		if (buf != NOKEY)
		{
			switch (buf)
			{
				case AKEY:
				return;
				case ONEKEY:
				rPID.pGain += 0.01;
				break;
				case FOURKEY:
				rPID.pGain -= 0.01;
				break;
				case TWOKEY:
				gPID.pGain += 0.01;
				break;
				case FIVEKEY:
				gPID.pGain -= 0.01;
				break;
				case THREEKEY:
				bPID.pGain += 0.01;
				break;
				case SIXKEY:
				bPID.pGain -= 0.01;
				break;
				case SEVENKEY:
				rPID.dGain += 0.01;
				break;
				case STARKEY:
				rPID.dGain -= 0.01;
				break;
				case EIGHTKEY:
				gPID.dGain += 0.01;
				break;
				case ZEROKEY:
				gPID.dGain -= 0.01;
				break;
				case NINEKEY:
				bPID.dGain += 0.01;
				break;
				case HASHKEY:
				bPID.dGain -= 0.01;
				break;
				default:
				break;
			}
		}
		lcd_clrscr();
		dtostrf(rPID.pGain, 1, 2, string);
		lcd_puts(string);
		lcd_putc(' ');
		dtostrf(bPID.pGain, 1, 2, string);
		lcd_puts(string);
		lcd_putc(' ');
		dtostrf(bPID.pGain, 1, 2, string);
		lcd_puts(string);

		lcd_goto_xy(0, 1);
		dtostrf(rPID.dGain, 1, 2, string);
		lcd_puts(string);
		lcd_putc(' ');
		dtostrf(bPID.dGain, 1, 2, string);
		lcd_puts(string);
		lcd_putc(' ');
		dtostrf(bPID.dGain, 1, 2, string);
		lcd_puts(string);
	}
}