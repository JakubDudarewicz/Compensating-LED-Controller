/*******************************************************
Title:			MCP7940M RTC module library
				I2C interface
Filename:		'RTC.c'
Author:			Jakub Dudarewicz
Version:		0.1
Created:		12-2017
Target MCU:		Atmel AVR

This code is distributed under the GNU Public License
which can be found at http://www.gnu.org/licenses/gpl.txt
*******************************************************/

#include "i2c.h"
#include "RTC.h"
#include <avr/io.h>
#include <stdlib.h>

void sendToAdress(uint8_t address, uint8_t data)
{
	uint8_t buf[2];
	buf[0] = address;
	buf[1] = data;
	i2cMasterSendNI(RTC_ADDRESS, 2, &buf);
}

uint8_t* getFromAdress(uint8_t address, uint8_t consecutive)
{
	uint8_t *buf = malloc(consecutive);
	buf[0] = address;
	i2cMasterSendNI(RTC_ADDRESS, 1, buf);
	i2cMasterReceiveNI(RTC_ADDRESS, consecutive, buf);
	return buf;
}

void RTCStart()
{
	uint8_t* buf = getFromAdress(RTCSEC, 1);
	sendToAdress(RTCSEC, _BV(RTCSEC_ST) | (*buf & RTCSEC_MASK));
}

void RTCStop()
{
	uint8_t* buf = getFromAdress(RTCSEC, 1);
	sendToAdress(RTCSEC, *buf & RTCSEC_MASK);
}

void setTime(uint8_t hours, uint8_t minutes, uint8_t seconds)
{
	uint8_t* buf = getFromAdress(RTCSEC, 1);
	sendToAdress(RTCSEC, BIN2BCD(seconds) | (*buf & _BV(RTCSEC_ST)));
	sendToAdress(RTCMIN, BIN2BCD(minutes));
	sendToAdress(RTCHOUR, BIN2BCD(hours));
	free(buf);
}

void setDate(uint8_t year, uint8_t month, uint8_t day)
{
	sendToAdress(RTCDATE, BIN2BCD(day) & RTCDATE_MASK);
	sendToAdress(RTCMTH, BIN2BCD(month) & RTCMTH_MASK);
	sendToAdress(RTCYEAR, BIN2BCD(year));
}

uint8_t getTime(UNIT u)
{
	uint8_t address = RTCSEC;
	switch (u)
	{
		case SECONDS:
			address = RTCSEC;
			break;
		case MINUTES:
			address = RTCMIN;
			break;
		case HOURS:
			address = RTCHOUR;
		break;
			case WEEKDAY:
			address = RTCWKDAY;
		break;
			case MONTH:
			address = RTCMTH;
		break;
			case YEAR:
			address = RTCYEAR;
		break;
			case LEAP:
			address = RTCYEAR;
		break;
	}
	uint8_t *data = getFromAdress(address, 1);
	switch (u)
	{
		case SECONDS:
			return BCD2BIN(*data & RTCSEC_MASK);
		case MINUTES:
			return BCD2BIN(*data & RTCMIN_MASK);
		case HOURS:
			return BCD2BIN(*data & RTCHOUR_MASK);
		case WEEKDAY:
			return BCD2BIN(*data & RTCWKDAY_MASK);
		case MONTH:
			return BCD2BIN(*data & RTCMTH_MASK);
		case YEAR:
			return BCD2BIN(*data);
		case LEAP:
			return *data & _BV(RTCMTH_LPYR);
		default:
			return 0;
	}
}

void RTCConfigure(uint8_t settingByte)
{
	sendToAdress(CONTROL, settingByte);
}