/*******************************************************
Title:			MCP7940M RTC module library
				I2C interface
Filename:		'RTC.h'
Author:			Jakub Dudarewicz
Version:		0.1
Created:		12-2017
Target MCU:		Atmel AVR

This code is distributed under the GNU Public License
which can be found at http://www.gnu.org/licenses/gpl.txt
*******************************************************/

#ifndef FILE
#define FILE

#define TRUE			1
#define FALSE			0

#define LOCAL_ADDR		0xA0
#define RTC_ADDRESS		0xDE
#define BUFFER_SIZE		20

#define RTCSEC			0x00
#define RTCMIN			0x01
#define	RTCHOUR			0x02
#define RTCWKDAY		0x03
#define	RTCDATE			0x04
#define	RTCMTH			0x05
#define	RTCYEAR			0x06
#define	CONTROL			0x07
#define OSCTRIM			0x08
#define ALM0SEC			0x0A
#define	ALM0MIN			0x0B
#define	ALM0HOUR		0x0C
#define	ALM0WKDAY		0x0D
#define ALM0DATE		0x0E
#define	ALM0MTH			0x0F
#define ALM1SEC			0x11
#define	ALM1MIN			0x12
#define	ALM1HOUR		0x13
#define	ALM1WKDAY		0x14
#define ALM1DATE		0x15
#define	ALM1MTH			0x16

#define	RTCSEC_ST			7
#define RTCHOUR_FORMAT		6
#define RTCHOUR_AMPM		5
#define RTCMTH_LPYR			5
#define RTCYEAR_TRIMSIGN	7
#define	RTCWKDAY_OSCRUN		5
#define CONTROL_OUT			7
#define CONTROL_SQWEN		6
#define CONTROL_ALM1EN		5
#define CONTROL_ALM0EN		4
#define CONTROL_EXTOSC		3
#define CONTROL_CRSTRIM		2
#define CONTROL_SQWFS1		1
#define CONTROL_SQWFS0		0
#define	OSCTRIM_SIGN		7

#define	RTCSEC_MASK		0x7F
#define	RTCMIN_MASK		0x7F
#define	RTCHOUR_MASK	0x3F
#define RTCWKDAY_MASK	0x07
#define	RTCDATE_MASK	0x3F
#define	RTCMTH_MASK		0x1F
#define	TRIMVAL_MASK	0x7F

#define BCD2BIN(bcd)	(bcd & 0x0F) + (((bcd & 0xF0) / 16) * 10)
#define	BIN2BCD(bin)	((bin / 10) * 16) | (bin % 10)

typedef enum {
	SECONDS,
	MINUTES,
	HOURS,
	WEEKDAY,
	MONTH,
	YEAR,
	LEAP
}UNIT;

void sendToAdress(uint8_t address, uint8_t data);
uint8_t* getFromAdress(uint8_t address, uint8_t consecutive);
void RTCStart();
void RTCStop();
void set12HFormat();
void set24HFormat();
void isOSCRunning();
void isLeap();
//SQWFS setting
//00 - 1Hz
//01 - 4.096kHz
//10 - 8.192kHz
//11 - 32.768kHz
void RTCConfigure(uint8_t settingByte);
void setTime(uint8_t hours, uint8_t minutes, uint8_t seconds);
void setDate(uint8_t year, uint8_t month, uint8_t day);
uint8_t getTime(UNIT u);

#endif /* INCFILE1_H_ */