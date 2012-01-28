/*

RTC command library for the ST Micro's M41T93 Real Time Clock
Authored by Ted Ullrich, http://tomorrow-lab.com
Co-Authored by Justin Downs, http://groundlab.cc
January 2012
Built to work with the RTClib created by JeeLabs http://news.jeelabs.org/code/
Released to the public domain.

*/


#ifndef __RTC_M41T93_H__
#define __RTC_M41T93_H__

#include <RTClib.h>

// RTC based on the DS3234 chip connected via SPI and the SPI library, adapted for the RTC_M41T93
class RTC_M41T93
{
public:
    RTC_M41T93(int _cs_pin): cs_pin(_cs_pin) {}
    uint8_t begin(void);
    void adjust(const DateTime& dt);
    uint8_t isrunning(void);
    DateTime now();

protected:
    void cs(int _value);

private:
    int cs_pin;
};

#endif // __RTC_M41T93_H__
