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
	MCUCR |= _BV(JTD); // disable JTAG to obtain control of PORTC as general I/O

	TCCR1A = _BV(WGM11) | _BV(COM1A1);
	TCCR1B = _BV(WGM12) | _BV(WGM13) | _BV(CS10);
	ICR1 = 0x03FF;
	TCCR2A = _BV(WGM21) | _BV(WGM20) | _BV(COM2A1) | _BV(COM2B1);
	TCCR2B = _BV(CS20);

	DDRD = (_BV(5) | _BV(6) | _BV(7));

	REDPWM = 0;
	GREENPWM = 0;
	BLUEPWM = 0;

	KEYDDR = 0x00;
	KEYPORT = 0x00;

	char buf;
	unsigned char refreshPending = TRUE;

	i2cInit();
	setTime(12, 0, 0);
	RTCStart();

	lcd_init();
	lcd_clrscr();

	writeCustom(UPARROW, upArrow);
	writeCustom(DOWNARROW, downArrow);
	menuDirection = UP;
	appendItem("RGB", &setRGB);
	appendItem("TOGGLE", &toggleLight);
	appendItem("BLINK", &blinkLight);
	appendItem("TIME", &displayTime);
	appendItem("SENSOR", &displayLight);
	appendItem("GOAL", &goalEdit);
	appendItem("PID", &PIDedit);

	PIDinit(&rPID, 0.05, 0, 0, 10000, 0);
	PIDinit(&gPID, 0, 0, 0, 10000, 0);
	PIDinit(&bPID, 0, 0, 0, 10000, 0);

	while(1)
	{
		LEDControl();

		buf = keyScan();
		if (buf != NOKEY)
		{
			refreshPending = TRUE;
			switch (buf)
			{
				case AKEY:
				prevMenu();
				menuDirection = UP;
				break;
				case BKEY:
				nextMenu();
				menuDirection = DOWN;
				break;
				case CKEY:
				if(selectedItem->menuPressed != NULL) selectedItem->menuPressed();
				break;
			}
		}
		if (refreshPending == TRUE)
		{
			displayMenu();
			refreshPending = FALSE;
		}
    }
}

void LEDControl(){
	int rbuf, gbuf, bbuf;
	rbuf = sensorScan(RED, ACCURACY);
	gbuf = sensorScan(GREEN, ACCURACY);
	bbuf = sensorScan(BLUE, ACCURACY);
	
	limitAdd16bit(&REDPWM, updatePID(&rPID, rgoal, rbuf), 0x03FF);
	limitAdd8bit(&GREENPWM, updatePID(&gPID, ggoal, gbuf), 0xFF);
	limitAdd8bit(&BLUEPWM, updatePID(&bPID, bgoal, bbuf), 0xFF);
}

void setRGB(){
	REDPWM = getInt("R:", 255);
	GREENPWM = getInt("G:", 255);
	BLUEPWM = getInt("B:", 255);
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

void limitAdd8bit(volatile uint8_t *PWMCHANNEL, int value, int limit){
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

void limitAdd16bit(volatile uint16_t *PWMCHANNEL, int value, int limit){
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